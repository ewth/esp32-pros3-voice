/*
    Visit the GitHub repo for project details:
        https://github.com/ewth/esp32-pros3-voice
*/

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_board_init.h"

// ESP-SR
#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "esp_afe_sr_iface.h"
#include "esp_afe_sr_models.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "model_path.h"
#include "esp_process_sdkconfig.h"

#include "speech_actions.h"

static const char         *TAG         = "main";
int                        wakeup_flag = 0;
static esp_afe_sr_iface_t *afe_handle  = NULL;
static volatile int        task_flag   = 0;
srmodel_list_t            *models      = NULL;

void feed_Task(void *arg)
{
    esp_afe_sr_data_t *afe_data        = arg;
    int                audio_chunksize = afe_handle->get_feed_chunksize(afe_data);
    int                nch             = afe_handle->get_feed_channel_num(afe_data);
    int                feed_channel    = esp_get_feed_channel();
    assert(nch == feed_channel);
    int16_t *i2s_buff = malloc(audio_chunksize * sizeof(int16_t) * feed_channel);
    assert(i2s_buff);

    while (task_flag) {
        esp_get_feed_data(true, i2s_buff, audio_chunksize * sizeof(int16_t) * feed_channel);

        afe_handle->feed(afe_data, i2s_buff);
    }
    if (i2s_buff) {
        free(i2s_buff);
        i2s_buff = NULL;
    }
    vTaskDelete(NULL);
}

void detect_Task(void *arg)
{
    esp_afe_sr_data_t *afe_data      = arg;
    int                afe_chunksize = afe_handle->get_fetch_chunksize(afe_data);
    char              *mn_name       = esp_srmodel_filter(models, ESP_MN_PREFIX, ESP_MN_ENGLISH);

    ESP_LOGD(TAG, "Multinet model: '%s'", mn_name);

    esp_mn_iface_t     *multinet     = esp_mn_handle_from_name(mn_name);
    model_iface_data_t *model_data   = multinet->create(mn_name, 6000);
    int                 mu_chunksize = multinet->get_samp_chunksize(model_data);
    esp_mn_commands_update_from_sdkconfig(multinet, model_data); // Add speech commands from sdkconfig
    assert(mu_chunksize == afe_chunksize);

    ESP_LOGD(TAG, "------------DETECT START------------");

    while (task_flag) {
        afe_fetch_result_t *res = afe_handle->fetch(afe_data);
        if (!res || res->ret_value == ESP_FAIL) {
            ESP_LOGE(TAG, "Fetch error");
            break;
        }

        if (res->wakeup_state == WAKENET_DETECTED) {
            ESP_LOGD(TAG, "WAKEWORD DETECTED");
            wake_up_action();
            multinet->clean(model_data);
        }

        if (res->raw_data_channels == 1 && res->wakeup_state == WAKENET_DETECTED) {
            wakeup_flag = 1;
        } else if (res->raw_data_channels > 1 && res->wakeup_state == WAKENET_CHANNEL_VERIFIED) {
            // For a multi-channel AFE, it is necessary to wait for the channel to be verified.
            ESP_LOGD(TAG, "AFE_FETCH_CHANNEL_VERIFIED, channel index: %d", res->trigger_channel_id);
            wakeup_flag = 1;
        }

        if (wakeup_flag == 1) {
            esp_mn_state_t mn_state = multinet->detect(model_data, res->data);

            if (mn_state == ESP_MN_STATE_DETECTING) {
                continue;
            }

            if (mn_state == ESP_MN_STATE_DETECTED) {
                esp_mn_results_t *mn_result = multinet->get_results(model_data);
                for (int i = 0; i < mn_result->num; i++) {
                    ESP_LOGD(TAG, "TOP %d, command_id: %d, phrase_id: %d, string: %s, prob: %f", i + 1, mn_result->command_id[i], mn_result->phrase_id[i], mn_result->string, mn_result->prob[i]);
                    speech_command_action(mn_result->command_id[i]);
                }
                ESP_LOGD(TAG, "-----------LISTENING-----------");
            }

            if (mn_state == ESP_MN_STATE_TIMEOUT) {
                esp_mn_results_t *mn_result = multinet->get_results(model_data);
                ESP_LOGD(TAG, "Timed out waiting for command; detected: %s", mn_result->string);
                afe_handle->enable_wakenet(afe_data);
                wakeup_flag = 0;
                ESP_LOGD(TAG, "-----------WATING FOR WAKE WORD-----------");
                continue;
            }
        }
    }
    if (model_data) {
        multinet->destroy(model_data);
        model_data = NULL;
    }
    vTaskDelete(NULL);
}

void app_main()
{
#if CONFIG_IDF_TARGET_ESP32S3
    ESP_LOGD(TAG, "ESP32-S3 Target: let's go!");
#else
    ESP_LOGE(TAG, "Only ESP32-S3 is supported.");
    return;
#endif

    models = esp_srmodel_init("model"); // partition label defined in partitions.csv
    ESP_ERROR_CHECK(esp_board_init(16000, 2, 16));
    // Removing sdcard interaction... for now
    // ESP_ERROR_CHECK(esp_sdcard_init("/sdcard", 10));

    afe_config_t *afe_config    = afe_config_init(esp_get_input_format(), models, AFE_TYPE_SR, AFE_MODE_LOW_COST);
    afe_handle                  = esp_afe_handle_from_config(afe_config);
    esp_afe_sr_data_t *afe_data = afe_handle->create_from_config(afe_config);
    afe_config_free(afe_config);

    task_flag = 1;
    xTaskCreatePinnedToCore(&detect_Task, "detect", 8 * 1024, (void *)afe_data, 5, NULL, 1);
    xTaskCreatePinnedToCore(&feed_Task, "feed", 8 * 1024, (void *)afe_data, 5, NULL, 0);
}

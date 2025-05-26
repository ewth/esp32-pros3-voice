#pragma once

#include "driver/gpio.h"
#include "esp_idf_version.h"

/**
 * @brief ewth_pros3_board I2C GPIO definition
 *
 */
#define FUNC_I2C_EN (1)
#define I2C_NUM (0)
#define I2C_CLK (100000)
#define GPIO_I2C_SCL (GPIO_NUM_9)
#define GPIO_I2C_SDA (GPIO_NUM_8)

/**
 * @brief ewth_pros3_board SDMMC GPIO definition
 *
 * @note Only avaliable when PMOD connected
 */
#define FUNC_SDMMC_EN (0)
#define SDMMC_BUS_WIDTH (1)
#define GPIO_SDMMC_CLK (GPIO_NUM_NC)
#define GPIO_SDMMC_CMD (GPIO_NUM_NC)
#define GPIO_SDMMC_D0 (GPIO_NUM_NC)
#define GPIO_SDMMC_D1 (GPIO_NUM_NC)
#define GPIO_SDMMC_D2 (GPIO_NUM_NC)
#define GPIO_SDMMC_D3 (GPIO_NUM_NC)
#define GPIO_SDMMC_DET (GPIO_NUM_NC)

/**
 * @brief ewth_pros3_board SDSPI GPIO definition
 *
 */
#define FUNC_SDSPI_EN (1)
#define SDSPI_HOST (SPI2_HOST)
#define GPIO_SDSPI_CS (GPIO_NUM_39)
#define GPIO_SDSPI_SCLK (GPIO_NUM_36)
#define GPIO_SDSPI_MISO (GPIO_NUM_37)
#define GPIO_SDSPI_MOSI (GPIO_NUM_35)

/**
 * @brief ewth_pros3_board I2S GPIO definition
 *
 */
#define FUNC_I2S_EN (1)
#define GPIO_I2S_LRCK (GPIO_NUM_14)
#define GPIO_I2S_MCLK (GPIO_NUM_NC)
#define GPIO_I2S_SCLK (GPIO_NUM_12)
#define GPIO_I2S_SDIN (GPIO_NUM_13)
#define GPIO_I2S_DOUT (GPIO_NUM_NC)

/**
 * @brief ewth_pros3_board I2S GPIO definition
 *
 */
#define FUNC_I2S0_EN (0)
#define GPIO_I2S0_LRCK (GPIO_NUM_NC)
#define GPIO_I2S0_MCLK (GPIO_NUM_NC)
#define GPIO_I2S0_SCLK (GPIO_NUM_NC)
#define GPIO_I2S0_SDIN (GPIO_NUM_NC)
#define GPIO_I2S0_DOUT (GPIO_NUM_NC)

/**
 * @brief ESP32-S3-HMI-DevKit power control IO
 *
 * @note Some power control pins might not be listed yet
 *
 */
#define FUNC_PWR_CTRL (0)
#define GPIO_PWR_CTRL (GPIO_NUM_NC)
#define GPIO_PWR_ON_LEVEL (0)

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)

#define I2S_CONFIG_DEFAULT(sample_rate, channel_fmt, bits_per_chan) {                   \
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate),                                 \
    .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(bits_per_chan, I2S_SLOT_MODE_MONO), \
    .gpio_cfg = {                                                                       \
        .mclk = GPIO_I2S_MCLK,                                                          \
        .bclk = GPIO_I2S_SCLK,                                                          \
        .ws = GPIO_I2S_LRCK,                                                            \
        .dout = GPIO_I2S_DOUT,                                                          \
        .din = GPIO_I2S_SDIN,                                                           \
    },                                                                                  \
}

#endif

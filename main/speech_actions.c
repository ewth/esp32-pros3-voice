#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_board_init.h"
#include "speech_actions.h"

extern int detect_flag;

void wake_up_action(void)
{
    printf("Wake up action\n");
}

void speech_command_action(int command_id)
{
    printf("Speech command action for command_id %d\n", command_id);
}

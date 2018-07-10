/*
main.c (Milestone 1)

Author: Nick Palumbo, Isaac Fisch
* Date Created: March 17, 2017
* Last Modified by: Nick Palumbo
* Date Last Modified: March 24, 2017

THREE windows to debug:
*********************
* WINDOW 1
screen /dev/ttyUSB0 38400
(to exit, ctrl(A) + K, Y) 

* WINDOW 2
make openocd

* WINDOW 3
arm-none-eabi-gdb main.elf

*/
/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include "shell.h"
#include <LSM303AGR_ACC_driver.h>

#define UNUSED(x) (void)(x)
#define BOARD_LED LINE_SAI_SD

/* Defined Global Variables to be used */
int total_count = 0;
int current_count = 0;
int numLogs = 0;
int minutes = 0;
int threshold = 1000;
/* accel_data hold */
int count_data[1000];
int time_data[1000];
/* Defined functions to be used */
int accelAboveThreshold(void);

/* Thread that blinks North LED as an "alive" indicator */
static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetLine(BOARD_LED);   
    chThdSleepMilliseconds(500);
    palClearLine(BOARD_LED);   
    chThdSleepMilliseconds(500);
  }
}

/* Thread to keep track of the time
*************************************
 * Each minute the thread will 
 * sleep for a minute or given time t
 * print the current_count
 * add the current_count to the total_count
 * reset curren_count for the next minute 
 *******************************************/
static THD_WORKING_AREA(waTimerThread,128);
static THD_FUNCTION(timerThread,arg) {
  UNUSED(arg);
  chRegSetThreadName("timer");
  while(TRUE) {
    // Sleep for a Minute
    chThdSleepMilliseconds(1000*60);
    // Save data;
    if (current_count != 0) {
      count_data[numLogs] = current_count;
      time_data[numLogs] = minutes;
      numLogs++;
    }
    // Add the current_count to the total_count
    total_count += current_count;
    // Reset the current_count to be used for the next minute
    current_count = 0;
    // Increment Minutes
    minutes++;
  }
}

static THD_WORKING_AREA(waStepCounterThread,128);
static THD_FUNCTION(stepCounterThread,arg) {
  UNUSED(arg);
  chRegSetThreadName("stepCounter");
  while(TRUE) {
    // Increment Count when Accel above Threshhold th
    if (accelAboveThreshold()) {
      // Increment Count 
      current_count += 1;
      chprintf((BaseSequentialStream*)&SD5,"New Activity Detected!\n\rCurrentCount: %d\n\n\r",current_count);
      chThdSleepMilliseconds(5000);
    }
    chThdSleepMilliseconds(100);
  }
}

/* Display Count functionality */
static void cmd_display_count(BaseSequentialStream *chp, int argc, char *argv[]) {
  UNUSED(argv);
  UNUSED(argc);
  int i = 0;
  // Read & Print Accel Data
  for (i = 0; i < numLogs; i++) {
    // Read & Print Timestamp Data from Global Saved Arrays
    chprintf(chp,"Time: %6dmin | Count: %3d\n\r", time_data[i], count_data[i]);
  }
}

int accelAboveThreshold() {
  // Variables to be used
  int accel[3];
  // Read Accel Data & timestamp
  LSM303AGR_ACC_Get_Acceleration(NULL,&accel);
    // Return above (1) or below (0)
  if (accel[2] > threshold) {
    // Is above Threshold
    return 1;
  } else {
    // Is below Threshold
    return 0;
  }
}

/* Given commands */
static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;

  (void)argv;

  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
}

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static const ShellCommand commands[] = {
  {"myecho", cmd_myecho},
  {"display_count", cmd_display_count},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD5,
  commands
};

/* Application entry point. */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  static thread_t *shelltp = NULL;

  halInit();
  chSysInit();

  LSM303AGR_ACC_Init();
  LSM303AGR_ACC_W_ODR(NULL, LSM303AGR_ACC_ODR_DO_1_25KHz);

  /* Activates the serial driver 5 using the driver default configuration. */
  /* PC12(TX) and PD2(RX). The default baud rate is 38400. */
  sdStart(&SD5, NULL);
  chprintf((BaseSequentialStream*)&SD5, "\n\rUp and Running\n\r");

  /* Initialize the command shell */ 
  shellInit();
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);
  /* Initialize the timer thread and stepCounter thread*/
  chThdCreateStatic(waTimerThread, sizeof(waTimerThread), NORMALPRIO+1, timerThread, NULL);
  chThdCreateStatic(waStepCounterThread, sizeof(waStepCounterThread), NORMALPRIO+1, stepCounterThread, NULL);
  while (TRUE) {
    if (!shelltp) {
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    }
    else if (chThdTerminatedX(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
    chThdSleepMilliseconds(1000);
  }
}

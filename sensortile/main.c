/*
main.c (Milestone 2)

Author: Nick Palumbo, Isaac Fisch
* Date Created: March 17, 2017
* Last Modified by: Nick Palumbo
* Date Last Modified: April 27, 2017

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
#include "time.h"
#include <LSM303AGR_ACC_driver.h>
//#include <stm32_bluenrg_ble.h>

#define UNUSED(x) (void)(x)
#define BOARD_LED LINE_SAI_SD

/* Defined Global Variables to be used */
int current_steps = 0;
int total_steps = 0;
#define threshold 1000
int total_logs = 0;
int step_data[1000];
RTCDateTime time_data[1000];
/* Defined functions to be used */
uint8_t accelAboveThreshold(void);
void setStartingRTCTime(void);
RTCDateTime getTimeStamp(void);
void saveTimeStamp(RTCDateTime timespec);

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

/* This thread is used to print out the accel data.
 * It is easy to check data with it constantly printing out.
 */
static THD_WORKING_AREA(waAccelTestThread,128);
static THD_FUNCTION(accelTestThread,arg) {
  UNUSED(arg);
  int accel[3];
  chRegSetThreadName("test");
  while(TRUE) {
    // Test Accel
    LSM303AGR_ACC_Get_Acceleration(NULL,accel);
    chprintf((BaseSequentialStream*)&SD5,"Accel: %4d,%4d,%4d \n\r",accel[0],accel[1],accel[2]);
    chThdSleepMilliseconds(500);
  }
}

/* This thread keeps track of time by sleeping for time.
 * After the wait time the current_steps are checked and then saved if greater than 0
 */
static THD_WORKING_AREA(waTimerThread,128);
static THD_FUNCTION(timerThread,arg) {
  UNUSED(arg);
  chRegSetThreadName("timer");
  while(TRUE) {
    /* SLEEP for 1 minute */
    chThdSleepMilliseconds(1000*10);
    /* SAVE Data if steps have been detected */
    if (current_steps != 0) {
      /* SAVE timestamp and current steps */
      saveTimeStamp(getTimeStamp());
      step_data[total_logs] = current_steps;
      /* INCREMENT total logs which is the index of the data arrays */
      total_logs += 1;
      /* ADD current steps to the total steps */
      total_steps += current_steps;
      /* RESET current steps */
      current_steps = 0;
    }
  }
}

/* This thread constantly checks if the accel data is above the set threshold.
 * If the accel data is above the accel threshold the current steps are incremented
 */
static THD_WORKING_AREA(waStepCounterThread,512);
static THD_FUNCTION(stepCounterThread,arg) {
  UNUSED(arg);
  chRegSetThreadName("stepCounter");
  while(TRUE) {
    /* CHECK if Above Threshold! */
    if (accelAboveThreshold()) {
      current_steps += 1;
      chThdSleepMilliseconds(3000);
    }
    chThdSleepMilliseconds(100);
  }
}

/* Display Count functionality */
static void cmd_display_count(BaseSequentialStream *chp, int argc, char *argv[]) {
  UNUSED(argv);
  UNUSED(argc);
  UNUSED(chp);
  int i = 0;
  struct tm timp;
  uint32_t tv_msec;
  char buf[26];
  // Read & Print Accel Data
  for (i = 0; i < total_logs; i++) {
    rtcConvertDateTimeToStructTm(&time_data[i],&timp,&tv_msec);
    time_t aTime = mktime(&timp);
    ctime_r(&aTime, buf);
    chprintf((BaseSequentialStream*)&SD5,"Count: %4d | %s \n\r",step_data[i],buf);
  }
}

uint8_t accelAboveThreshold() {
  int accel[3];
  // Read Accel Data
  LSM303AGR_ACC_Get_Acceleration(NULL,accel);
  if (accel[2] > threshold) {
    // Is above Threshold
    return TRUE;
  } else {
    // Is below Threshold
    return FALSE;
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
  rtcInit();
  //setStartingRTCTime();
  
  LSM303AGR_ACC_Init();
  LSM303AGR_ACC_W_ODR(NULL, LSM303AGR_ACC_ODR_DO_1_25KHz);

  /* Activates the serial driver 5 using the driver default configuration. */
  /* PC12(TX) and PD2(RX). The default baud rate is 38400. */
  sdStart(&SD5, NULL);
  chprintf((BaseSequentialStream*)&SD5, "\n\rUp and Running\n\r");

  /* Initialize the command shell */ 
  shellInit();
  //chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);
  /* Initialize the timer thread and stepCounter thread*/
  chThdCreateStatic(waTimerThread, sizeof(waTimerThread), NORMALPRIO+1, timerThread, NULL);
  chThdCreateStatic(waStepCounterThread, sizeof(waStepCounterThread), NORMALPRIO+1, stepCounterThread, NULL);
  //chThdCreateStatic(waAccelTestThread, sizeof(waAccelTestThread), NORMALPRIO+1, accelTestThread, NULL);
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

void setStartingRTCTime() {
  RTCDateTime timespec;
  timespec.year = 37;  /* Years since 1980. */
  timespec.month = 4;
  timespec.dayofweek = 5; 
  timespec.day = 28;
  timespec.millisecond = 1000 * 60 * 60 * 12 + 1000 * 60 * 30;
  rtcSetTime(&RTCD1, &timespec);
}

RTCDateTime getTimeStamp() {
  RTCDateTime timespec;
  rtcGetTime(&RTCD1, &timespec);
  return timespec;
}

void saveTimeStamp(RTCDateTime timespec) {
  time_data[total_logs].year = timespec.year;
  time_data[total_logs].month = timespec.month;
  time_data[total_logs].day = timespec.day;
  time_data[total_logs].dayofweek = timespec.dayofweek;
  time_data[total_logs].millisecond = timespec.millisecond;
}

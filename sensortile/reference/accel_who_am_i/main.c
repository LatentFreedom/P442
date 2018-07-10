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

static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;
  (void)argv;
  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
}

static void cmd_accel_wai(BaseSequentialStream *chp, int argc, char *argv[]) {
  uint8_t wai;
  (void)argv;
  (void)argc;

  LSM303AGR_ACC_R_WHO_AM_I(NULL, &wai);
  chprintf(chp,"lsm303 Who Am I data = 0x%02x\n\r",wai);
}

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static const ShellCommand commands[] = {
  {"myecho", cmd_myecho},
  {"accel_wai",cmd_accel_wai},
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

  /* Initialize the Accelerometer */
  LSM303AGR_ACC_Init();

  /*
   * Activates the serial driver 5 using the driver default configuration.
   * PC12(TX) and PD2(RX). The default baud rate is 38400.
   */
  sdStart(&SD5, NULL);
  chprintf((BaseSequentialStream*)&SD5, "\n\rUp and Running\n\r");

  /* Initialize the command shell */ 
  shellInit();
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);

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

/*
    ChibiOS - Copyright (C) 2006-2014 Giovanni Di Sirio

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

#include "ch.h"
#include "hal.h"
#include "ch_test.h"
#include "shell.h" 
#include "chprintf.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define UNUSED(x) (void)(x)

// set to 1 when timer is on
static int egg_timer_running = 0;
// hold the time for egg timer
static int egg_timer = 0; 
// holds the entered egg time in case of egg timer reset
static int current_egg_timer = 0; 

// Thread that blinks North LED as an "alive" indicator
static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    while (egg_timer_running && egg_timer > 0) {
      chThdSleepMilliseconds(1);
      egg_timer -= 1;
      if (egg_timer <= 0) {
	int i;
	chprintf((BaseSequentialStream*)&SD1, "Times up!\n\r");
	for (i = 0; i < 5; i++) {
	  turnOnLEDs();
	  chThdSleepMilliseconds(200);
	  turnOffLEDs();
	  chThdSleepMilliseconds(200);
	}
      }
    }
    
    egg_timer_running = 0;
    chThdSleepMilliseconds(100);
  }
}

static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;
  
  (void)argv;

  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
}

static void cmd_ledread(BaseSequentialStream *chp, int argc, char *argv[]) {
  
  (void)argv;
  
  if (argc == 1){
    if (strcmp(argv[0], "N") == 0) {
      if (palReadPad(GPIOE, GPIOE_LED3_RED) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    }
    else if (strcmp(argv[0], "NE") == 0){
      if (palReadPad(GPIOE, GPIOE_LED5_ORANGE) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    }
    else if (strcmp(argv[0], "E") == 0){
      if (palReadPad(GPIOE, GPIOE_LED7_GREEN) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    }
    else if (strcmp(argv[0], "SE") == 0){
      if (palReadPad(GPIOE, GPIOE_LED9_BLUE) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    }
    else if (strcmp(argv[0], "S") == 0){
      if (palReadPad(GPIOE, GPIOE_LED10_RED) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    }
    else if (strcmp(argv[0], "SW") == 0){
      if (palReadPad(GPIOE, GPIOE_LED8_ORANGE) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    }
    else if (strcmp(argv[0], "W") == 0){
      if (palReadPad(GPIOE, GPIOE_LED6_GREEN) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    }
    else if (strcmp(argv[0], "NW") == 0){
      if (palReadPad(GPIOE, GPIOE_LED4_BLUE) == 0){
	chprintf(chp, "off\n\r");
      } else {
	chprintf(chp, "on\n\r");
      }
    } else {
      chprintf(chp, "Invalid LED. {N, NE, E, SE, S, SW, W, NW}\n\r");
    }
  }
  else {
    // Exit
    chprintf(chp, "Incorrect number of arguments. ledset {led} {state}\n\r");
    return(0);
  }
}

static void cmd_ledset(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;

  if (argc == 2) {
    // Work
    if (strcmp(argv[0], "N") == 0) {
	if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED3_RED);
	} else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED3_RED);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else if (strcmp(argv[0], "NE") == 0) {
      if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED5_ORANGE);
      } else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED5_ORANGE);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else if (strcmp(argv[0], "E") == 0) {
      if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED7_GREEN);
      } else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED7_GREEN);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else if (strcmp(argv[0], "SE") == 0) {
      if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED9_BLUE);
      } else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED9_BLUE);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else if (strcmp(argv[0], "S") == 0) {
      if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED10_RED);
      } else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED10_RED);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else if (strcmp(argv[0], "SW") == 0) {
      if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED8_ORANGE);
      } else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED8_ORANGE);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else if (strcmp(argv[0], "W") == 0) {
      if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED6_GREEN);
      } else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED6_GREEN);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else if (strcmp(argv[0], "NW") == 0) {
      if (strcmp(argv[1], "on") == 0) {
	 palSetPad(GPIOE, GPIOE_LED4_BLUE);
      } else if (strcmp(argv[1], "off") == 0) {
	 palClearPad(GPIOE, GPIOE_LED4_BLUE);
      } else {
	chprintf(chp, "Invalid state. {on, off}\n\r");
      }
    } else {
      chprintf(chp, "Invalid LED. {N, NE, E, SE, S, SW, W, NW}\n\r");
    }
  } else {
    // Exit
    chprintf(chp, "Incorrect number of arguments. ledset {led} {state}\n\r");
    return(0);
  }
}

static void cmd_timerset(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;

  if (argc == 1) {
    // Set egg timer
    egg_timer = atoi(argv[0]);
    if (egg_timer > 10000 || egg_timer < 0) {
       chprintf(chp, "Invalid time. Time: between 0 and 10000 \n\r");
       egg_timer = 0;
       current_egg_timer = 0;
       return(0);
    }
    // set the current timer in case of reset
    // the current timer will be used to add the value back to the original egg timer
    current_egg_timer = egg_timer;
    chprintf(chp, "Egg timer set: %d ms\n\r", egg_timer);
  } else {
    // Exit
    chprintf(chp, "Incorrect number of arguments. timerset {number of ms} \n\r");
    return(0);
  }
}

static void cmd_timerreset(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  // set egg timer back to the cast set timer
  egg_timer = current_egg_timer;
  chprintf(chp, "Timer reset.\n\r");
}

static void cmd_timerstart(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  // egg_timer_running is 1 when it is running, 0 when it is stopped
  if (egg_timer != 0) {
    egg_timer_running = 1;
    chprintf(chp, "Timer started.\n\r");
  } else {
    chprintf(chp, "You need to set a time first. timerset {ms}\n\r");
  }
}

static void cmd_timerstop(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  // egg_timer_running is 1 when it is running, 0 when it is stopped
  egg_timer_running = 0;
  chprintf(chp, "Timer stopped.\n\r");
}

static void cmd_timergettime(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  //  Report the number of mS remaining until the timer will expire.
  chprintf(chp, "Current time remaining: %d\n\r", egg_timer);
}

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static const ShellCommand commands[] = {
  {"myecho",  cmd_myecho},
  {"ledset",  cmd_ledset},
  {"ledread", cmd_ledread},
  {"timerset", cmd_timerset},
  {"timerreset", cmd_timerreset},
  {"timerstart", cmd_timerstart},
  {"timerstop", cmd_timerstop},
  {"timergettime", cmd_timergettime},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  commands
};

// Application entry point.
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   * PC4(RX) and PC5(TX). The default baud rate is 38400
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));
  chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running\n\r");

  /* Initialize the command shell */ 
  shellInit();
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);

  while (TRUE) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
					    "shell", NORMALPRIO + 1,
					    shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);
    chThdSleepMilliseconds(1000);
  }
 }

void turnOnLEDs() {
  palSetLine(LINE_LED3_RED);
  palSetLine(LINE_LED4_BLUE);
  palSetLine(LINE_LED5_ORANGE);
  palSetLine(LINE_LED6_GREEN);
  palSetLine(LINE_LED7_GREEN);
  palSetLine(LINE_LED8_ORANGE);
  palSetLine(LINE_LED9_BLUE);
  palSetLine(LINE_LED10_RED);
}

void turnOffLEDs() {
  palClearLine(LINE_LED3_RED);
  palClearLine(LINE_LED4_BLUE);
  palClearLine(LINE_LED5_ORANGE);
  palClearLine(LINE_LED6_GREEN);
  palClearLine(LINE_LED7_GREEN);
  palClearLine(LINE_LED8_ORANGE);
  palClearLine(LINE_LED9_BLUE);
  palClearLine(LINE_LED10_RED);	
}

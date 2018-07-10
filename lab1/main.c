/*
main.c (lab1)

Author: Nick Palumbo, Isaac Fisch
* Date Created: January 17, 2017
* Last Modified by: Nick Palumbo
* Date Last Modified: January 19, 2017

* Description: 
This application is an egg timer. 
Using the STM32 board this application will allow a user to set a time to countdown.
The timer can be set by pressing 's'.
The North Red LED will flash until the timer runs out.
When the time runs out all LEDs will turn on.
The user, at any point while the application is running, can reset the timer by typing 's'.

screen /dev/ttyUSB0 38400
(to exit, ctrl(A) + K, Y) 

*/

/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio
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
#include "chprintf.h"

int getEggTime(void);
void toggleLEDs(void);
void turnOnLEDs(void);
void turnOffLEDs(void);

static int egg_timer_running = 0; // set to 1 when timer is on
static int egg_timer = 0;

//  Blinker thread #1.
THD_WORKING_AREA(waThread1, 128);
THD_FUNCTION(Thread1, arg) {

  (void)arg;
  char *eggStart;
  chRegSetThreadName("reader");
  chprintf((BaseSequentialStream*)&SD1, "\n\rPress 's' to start timer:\n\r");
  while (true) {
    //chprintf((BaseSequentialStream*)&SD1, "\n\rPress 's' to start timer:\n\r");
    egg_timer = 0;
    chnRead((BaseSequentialStream*)&SD1, &eggStart, 1);
    chprintf((BaseSequentialStream*)&SD1, "%c", eggStart);
    if (eggStart == 's'){
      if (egg_timer_running) {
  turnOffLEDs();
  egg_timer_running = 0;
      }
      chprintf((BaseSequentialStream*)&SD1, "\n\rEnter a time in milliseconds between 1 and 10000\n\r");
      egg_timer = getEggTime();
      egg_timer_running = 1;
     
    }
  }
}

//  Blinker thread #2.
THD_WORKING_AREA(waThread2, 128);
THD_FUNCTION(Thread2, arg) {
  char *val;
  (void)arg;
  chRegSetThreadName("blinker");
  while(true) {
    while (egg_timer != 0) {
      palSetLine(LINE_LED3_RED);
      chThdSleepMilliseconds(50);
      palClearLine(LINE_LED3_RED);
      chThdSleepMilliseconds(50);
      egg_timer -= 100;
    }
    if (egg_timer == 0 && egg_timer_running == 1) {
      turnOnLEDs();
    }
  }
}


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
   * PC5(TX) and PC4(RX) are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));
  chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running\n\r");
  

  // Creates the example threads.
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO+1, Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched.
   */
 
  while(true){
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      turnOffLEDs();
    chThdSleepMilliseconds(100);
    }
}

int getEggTime() {
  char eggTime[6];
  char val;
  int count = 0;
  while ((count != 6) && (val != '\r')) {
    chnRead((BaseSequentialStream*)&SD1, &val, 1);
    if (val == '\r'){
      chprintf((BaseSequentialStream*)&SD1, "\n\r", 2);
      break;
      }
    chprintf((BaseSequentialStream*)&SD1, "%c", val);
    eggTime[count] = val;
    count++;
  }
  int return_value = atoi(eggTime);
  return return_value;
}

void toggleLEDs() {
  palToggleLine(LINE_LED3_RED);
  palToggleLine(LINE_LED4_BLUE);
  palToggleLine(LINE_LED5_ORANGE);
  palToggleLine(LINE_LED6_GREEN);
  palToggleLine(LINE_LED7_GREEN);
  palToggleLine(LINE_LED8_ORANGE);
  palToggleLine(LINE_LED9_BLUE);
  palToggleLine(LINE_LED10_RED);
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

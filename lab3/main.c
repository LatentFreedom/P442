/*
<<<<<<< HEAD
main.c (lab3)

Author: Nick Palumbo, Isaac Fisch
* Date Created: January 27, 2017
* Last Modified by: Nick Palumbo
* Date Last Modified: February 3, 2017

screen /dev/ttyUSB0 38400
(to exit, ctrl(A) + K, Y) 

*/

/*
=======
>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa
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
#include <math.h>

#define UNUSED(x) (void)(x)

<<<<<<< HEAD
// SPI configuration, sets up PortA Bit 8 as the chip select for the pressure sensor
=======
/* SPI configuration, sets up PortA Bit 8 as the chip select for the pressure sensor */
>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa
static SPIConfig pressure_cfg = {
  NULL,
  GPIOA,
  8,
  SPI_CR1_BR_2 | SPI_CR1_BR_1,
  0
};

<<<<<<< HEAD
// SPI configuration, sets up PortE Bit 3 as the chip select for the gyro
=======
/* SPI configuration, sets up PortE Bit 3 as the chip select for the gyro */
>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa
static SPIConfig gyro_cfg = {
  NULL,
  GPIOE,
  3,
  SPI_CR1_BR_2 | SPI_CR1_BR_1,
  0
};

uint8_t pressure_read_register (uint8_t address) {
  uint8_t receive_data;

  address = address | 0x80;            /* Set the read bit (bit 7)         */
  spiAcquireBus(&SPID1);               /* Acquire ownership of the bus.    */
  spiStart(&SPID1, &pressure_cfg);     /* Setup transfer parameters.       */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte */
  spiReceive(&SPID1, 1,&receive_data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
  spiReleaseBus(&SPID1);               /* Ownership release.               */
  return (receive_data);
}

void pressure_write_register (uint8_t address, uint8_t data) {
  address = address & (~0x80);         /* Clear the write bit (bit 7)      */
  spiAcquireBus(&SPID1);               /* Acquire ownership of the bus.    */
  spiStart(&SPID1, &pressure_cfg);     /* Setup transfer parameters.       */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte */
  spiSend(&SPID1, 1, &data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
  spiReleaseBus(&SPID1);               /* Ownership release.               */
}

uint8_t gyro_read_register (uint8_t address) {
  uint8_t receive_data;

  address = address | 0x80;            /* Set the read bit (bit 7)         */
  spiAcquireBus(&SPID1);               /* Acquire ownership of the bus.    */
  spiStart(&SPID1, &gyro_cfg);         /* Setup transfer parameters.       */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte */
  spiReceive(&SPID1, 1,&receive_data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
  spiReleaseBus(&SPID1);               /* Ownership release.               */
  return (receive_data);
}

void gyro_write_register (uint8_t address, uint8_t data) {
  address = address & (~0x80);         /* Clear the write bit (bit 7)      */
  spiAcquireBus(&SPID1);               /* Acquire ownership of the bus.    */
  spiStart(&SPID1, &gyro_cfg);         /* Setup transfer parameters.       */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte            */
  spiSend(&SPID1, 1, &data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
  spiReleaseBus(&SPID1);               /* Ownership release.               */
}

/* Thread that blinks North LED as an "alive" indicator */
static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    palSetPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
    palClearPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
  }
}

<<<<<<< HEAD
//////////////
//// GYRO ////
//////////////

// Read or write a byte from the gyro. 
// {address} is a byte in hex. 
// {value} is in hex and only needed for write.
static void cmd_gyro(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;

  if (argc == 2 || argc == 3) {
  	// READ the command line inputs
    if (strcmp(argv[0], "r") == 0 && argc == 2) {
      // READ
      // address comes in as hex string and must be converted to hex int
      uint8_t address = (uint8_t)strtol(argv[1],NULL,16);
      int receive_data = gyro_read_register(address);
      chprintf(chp,"0x%02x\n\r",receive_data);
    } else if (strcmp(argv[0],"w") == 0 && argc == 3) {
      // WRITE the value to the given address
      uint8_t address = (uint8_t)strtol(argv[1],NULL,16);
      uint8_t value = (uint8_t)strtol(argv[2],NULL,16);
      gyro_write_register(address, value);
    } else {
      // Exit
      chprintf(chp, "Invalid argument. gyro {r/w} {address} {value}\n\r");
    }
  } else {
    // Exit
    chprintf(chp, "Invalid number of arguments. gyro {r/w} {address} {value}\n\r");
  }
}

// MAKE SURE YOU START THE GYRO SENSOR READING //
// gyro w 20 8B

// Read and print the values from the entire address space from the gyro.
static void cmd_gyrorall(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  
  // Address Space (byte in hex): (pg 29)
  chprintf(chp,"--------------------------\n\r");
  chprintf(chp,"NAME              | DATA |\n\r");
  chprintf(chp,"------------------|-------\n\r");
  chprintf(chp,"WHO_AM_I          | 0x%02x |\n\r",gyro_read_register(0x0F));  
  chprintf(chp,"CTRL_REG1         | 0x%02x |\n\r",gyro_read_register(0x20));
  chprintf(chp,"CTRL_REG2         | 0x%02x |\n\r",gyro_read_register(0x21));
  chprintf(chp,"CTRL_REG3         | 0x%02x |\n\r",gyro_read_register(0x22)); 
  chprintf(chp,"CTRL_REG4         | 0x%02x |\n\r",gyro_read_register(0x23)); 
  chprintf(chp,"CTRL_REG5         | 0x%02x |\n\r",gyro_read_register(0x24));
  chprintf(chp,"REFERENCE         | 0x%02x |\n\r",gyro_read_register(0x25)); 
  chprintf(chp,"OUT_TEMP          | 0x%02x |\n\r",gyro_read_register(0x26)); 
  chprintf(chp,"STATUS_REG        | 0x%02x |\n\r",gyro_read_register(0x27));
  chprintf(chp,"OUT_X_L           | 0x%02x |\n\r",gyro_read_register(0x28)); 
  chprintf(chp,"OUT_X_H           | 0x%02x |\n\r",gyro_read_register(0x29));
  chprintf(chp,"OUT_Y_L           | 0x%02x |\n\r",gyro_read_register(0x2A));
  chprintf(chp,"OUT_Y_H           | 0x%02x |\n\r",gyro_read_register(0x2B));
  chprintf(chp,"OUT_Z_L           | 0x%02x |\n\r",gyro_read_register(0x2C));
  chprintf(chp,"OUT_Z_H           | 0x%02x |\n\r",gyro_read_register(0x2D));
  chprintf(chp,"FIFO_CTRL_REG     | 0x%02x |\n\r",gyro_read_register(0x2E));
  chprintf(chp,"FIFO_SRC_REG      | 0x%02x |\n\r",gyro_read_register(0x2F));
  chprintf(chp,"INT1_CFG          | 0x%02x |\n\r",gyro_read_register(0x30));
  chprintf(chp,"INT1_SRC          | 0x%02x |\n\r",gyro_read_register(0x31));
  chprintf(chp,"INT1_TSH_XH       | 0x%02x |\n\r",gyro_read_register(0x32));
  chprintf(chp,"INT1_TSH_XL       | 0x%02x |\n\r",gyro_read_register(0x33));
  chprintf(chp,"INT1_TSH_YH       | 0x%02x |\n\r",gyro_read_register(0x34));
  chprintf(chp,"INT1_TSH_YL       | 0x%02x |\n\r",gyro_read_register(0x35));
  chprintf(chp,"INT1_TSH_ZH       | 0x%02x |\n\r",gyro_read_register(0x36));
  chprintf(chp,"INT1_TSH_ZL       | 0x%02x |\n\r",gyro_read_register(0x37));
  chprintf(chp,"INT1_DURATION     | 0x%02x |\n\r",gyro_read_register(0x38));
  chprintf(chp,"--------------------------\n\r");
}

//////////////////
//// PRESSURE ////
//////////////////

// Read or write a byte from the pressure sensor. 
// {address} is a byte in hex. 
// {value} is in hex and only needed for write.
static void cmd_press(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc == 2 || argc == 3) {
    // READ the command line inputs
    if (strcmp(argv[0],"r") == 0 && argc == 2) {
      // READ
      // address comes in as hex string and must be converted to hex int
      uint8_t address = (uint8_t)strtol(argv[1],NULL,16);
      uint8_t receive_data = pressure_read_register(address);
      chprintf(chp,"0x%02x\n\r",receive_data);
    } else if (strcmp(argv[0],"w") == 0 && argc == 3) {
      // WRITE the value to the given address
      uint8_t address = (uint8_t)strtol(argv[1],NULL,16);
      uint8_t value = (uint8_t)strtol(argv[2],NULL,16);
      pressure_write_register(address, value);
    } else {
      // Exit
        chprintf(chp, "Invalid argument. gyro {r/w} {address} {value}\n\r");
    }
  } else {
    // Exit
    chprintf(chp, "Invalid number of arguments. gyro {r/w} {address} {value}\n\r");
  }
}

// Read and print the values from the entire address space from the pressure sensor.
static void cmd_pressrall(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  
  // Address Space (byte in hex): (pg 16)
  chprintf(chp,"-------------------------\n\r");
  chprintf(chp,"NAME              | DATA |\n\r");
  chprintf(chp,"------------------|------|\n\r");
  chprintf(chp,"REF_P_XL          | 0x%02x |\n\r",pressure_read_register(0x08));  
  chprintf(chp,"REF_P_L           | 0x%02x |\n\r",pressure_read_register(0x09));
  chprintf(chp,"REF_P_H           | 0x%02x |\n\r",pressure_read_register(0x0A)); 
  chprintf(chp,"WHO_AM_I          | 0x%02x |\n\r",pressure_read_register(0x0F)); 
  chprintf(chp,"RES_CONF          | 0x%02x |\n\r",pressure_read_register(0x10));
  chprintf(chp,"CTRL_REG1         | 0x%02x |\n\r",pressure_read_register(0x20));
  chprintf(chp,"CTRL_REG2         | 0x%02x |\n\r",pressure_read_register(0x21));
  chprintf(chp,"CTRL_REG3         | 0x%02x |\n\r",pressure_read_register(0x22)); 
  chprintf(chp,"INT_CFG_REG       | 0x%02x |\n\r",pressure_read_register(0x23)); 
  chprintf(chp,"INT_SOURCE_REG    | 0x%02x |\n\r",pressure_read_register(0x24));
  chprintf(chp,"THS_P_LOW_REG     | 0x%02x |\n\r",pressure_read_register(0x25)); 
  chprintf(chp,"THS_P_HIGH_REG    | 0x%02x |\n\r",pressure_read_register(0x26)); 
  chprintf(chp,"STATUS_REG        | 0x%02x |\n\r",pressure_read_register(0x27));
  chprintf(chp,"PRESS_POUT_XL_REH | 0x%02x |\n\r",pressure_read_register(0x28)); 
  chprintf(chp,"PRESS_OUT_L       | 0x%02x |\n\r",pressure_read_register(0x29));     
  chprintf(chp,"PRESS_OUT_H       | 0x%02x |\n\r",pressure_read_register(0x2A)); 
  chprintf(chp,"TEMP_OUT_L        | 0x%02x |\n\r",pressure_read_register(0x2B)); 
  chprintf(chp,"TEMP_OUT_H        | 0x%02x |\n\r",pressure_read_register(0x2C));
  chprintf(chp,"AMP_CTRL          | 0x%02x |\n\r",pressure_read_register(0x30));
  chprintf(chp,"--------------------------\n\r");
  
}

//////////////////
//// ALTITUDE ////
//////////////////

double From_Pressure_mb_To_Altitude_US_Std_Atmosphere_1976_ft(double pressure_mb) {
  return (1-pow(pressure_mb/1013.25,0.190284))*145366.45;
}

double From_ft_To_m(double altitude_ft) {
  return altitude_ft/3.280839895;
}

// MAKE SURE YOU START THE PRESSURE SENSOR READING //
// press w 20 C0

// Print the altitude where "f" gives the value in feet and "m" gives the value in meters.
static void cmd_altitude(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  double pressure_mb;
  double altitude_ft;

  int press_out_xl = pressure_read_register(0x28);
  char press_out_xl_s[3];
  snprintf(press_out_xl_s,3, "%02X",press_out_xl);

  int press_out_l = pressure_read_register(0x29);
  char press_out_l_s[3];
  snprintf(press_out_l_s,3, "%02X",press_out_l);
  
  int press_out_h =  pressure_read_register(0x2A);
  char press_out_h_s[3];
  snprintf(press_out_h_s,3, "%02X",press_out_h);

  char together[10];
  together[0] = press_out_h_s[0];
  together[1] = press_out_h_s[1];
  together[2] = press_out_l_s[0];
  together[3] = press_out_l_s[1];
  together[4] = press_out_xl_s[0];
  together[5] = press_out_xl_s[1];
  chprintf(chp, "together: %s\n\r", together);
  int num = (int)strtol(together, NULL, 16);
  chprintf(chp, "together hex: %d\n\r", num);

  // Check if negative pressure
  int negative = false;
  if (atoi(together[0]) > -1 && atoi(together[0]) <= 8) {
    negative = true;
  }
  pressure_mb = num/4096.0;
  chprintf(chp, "pressure_mb: %d\n\r", pressure_mb);
  if (argc == 1) {
    // Get the command line inputs
    if (strcmp(argv[0],"f") == 0) {
      //// ALTITUDE in FEET
      altitude_ft = From_Pressure_mb_To_Altitude_US_Std_Atmosphere_1976_ft(pressure_mb);
      // PRINT
      chprintf(chp, "Alititude: %lf feet\n\r", altitude_ft);
    } else if (strcmp(argv[0],"m") == 0) {
      //// ALTITUDE in METERS
      double altitude_m;
      // CONVERT
      altitude_ft = From_Pressure_mb_To_Altitude_US_Std_Atmosphere_1976_ft(pressure_mb);
      altitude_m = From_ft_To_m(altitude_ft);
      // PRINT
      chprintf(chp, "Alititude: %lf meters\n\r", altitude_m);
    } else {
      // EXIT
      chprintf(chp, "Invalid argument. alititude {f/m}\n\r");	
    }
  } else {
    // EXIT
    chprintf(chp, "Invalid number of arguments. alititude {f/m}\n\r");
  }
}


static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;
  (void)argv;
=======
static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;

  (void)argv;

>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa
  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
}

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static const ShellCommand commands[] = {
<<<<<<< HEAD
  {"myecho",    cmd_myecho},
  {"gyro",      cmd_gyro},
  {"gyrorall",  cmd_gyrorall},
  {"press",     cmd_press},
  {"pressrall", cmd_pressrall},
  {"altitude",  cmd_altitude},
=======
  {"myecho", cmd_myecho},
>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  commands
};

<<<<<<< HEAD
// Application entry point.
int main(void) {
=======
/*
 * Application entry point.
 */

int main(void) {
  /* double foo; */
  /* foo = pow(2.4,3.5); */
>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa

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
   * PC4(RX) and PC5(TX). The default baud rate is 9600.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));

<<<<<<< HEAD
  // Setup the pins for the spi link on the GPIOA. This link connects to the pressure sensor and the gyro.
=======
  /* 
   *  Setup the pins for the spi link on the GPIOA. This link connects to the pressure sensor and the gyro.  
   * 
   */

>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5));     /* SCK. */
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5));     /* MISO.*/
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5));     /* MOSI.*/
  palSetPadMode(GPIOA, 8, PAL_MODE_OUTPUT_PUSHPULL);  /* pressure sensor chip select */
  palSetPadMode(GPIOE, 3, PAL_MODE_OUTPUT_PUSHPULL);  /* gyro chip select */
  palSetPad(GPIOA, 8);                                /* Deassert the pressure sensor chip select */
  palSetPad(GPIOE, 3);                                /* Deassert the gyro chip select */

  chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running\n\r");
<<<<<<< HEAD
  //chprintf((BaseSequentialStream*)&SD1, "Gyro Whoami Byte = 0x%02x\n\r",gyro_read_register(0x0F));

  // Initialize the command shell
=======
  chprintf((BaseSequentialStream*)&SD1, "Gyro Whoami Byte = 0x%02x\n\r",gyro_read_register(0x0F));
  //  chprintf((BaseSequentialStream*)&SD1, "Here is the value %d.%d\n\r",(int) foo, fo

  /* Initialize the command shell */ 
>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa
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
<<<<<<< HEAD
=======


>>>>>>> e63ccc8d4f17ec480f79b32f56a0815df9f46ffa

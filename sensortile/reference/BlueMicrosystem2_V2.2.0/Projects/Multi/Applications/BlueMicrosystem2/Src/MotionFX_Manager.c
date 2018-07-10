/**
******************************************************************************
* @file    MotionFX_Manager.c
* @author  Central LAB
* @version V2.2.0
* @date    23-December-2016
* @brief   Header for MotionFX_Manager.c
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "TargetFeatures.h"
//#include "MotionFX_Manager.h"
#include "../../../../../Middlewares/ST/STM32_OSX_MotionFX_Library/osx_license.h"

/* Private defines -----------------------------------------------------------*/
#define FROM_MDPS_TO_DPS    0.001
#define FROM_MGAUSS_TO_UT50 (0.1f/50.0f)
#define SAMPLETODISCARD 15
#define GBIAS_ACC_TH_SC_6X (2.0f*0.000765f)
#define GBIAS_GYRO_TH_SC_6X (2.0f*0.002f)
#define GBIAS_MAG_TH_SC_6X (2.0f*0.001500f)
#define GBIAS_ACC_TH_SC_9X (2.0f*0.000765f)
#define GBIAS_GYRO_TH_SC_9X (2.0f*0.002f)
#define GBIAS_MAG_TH_SC_9X (2.0f*0.001500f)
/* Delta time mSec for Deltafusion */
#define DELTATIMESENSORFUSION 0.01

/* Exported Variables -------------------------------------------------------------*/
osxMFX_output iDataOUT;
osxMFX_input iDataIN;

/* Imported Variables -------------------------------------------------------------*/
extern float sensitivity_Mul;

extern osxMFX_calibFactor magOffset;

/* Private Variables -------------------------------------------------------------*/
static int discardedCount = 0;

static osxMFX_knobs iKnobs;
static osxMFX_knobs* ipKnobs;

/**
  * @brief  Run sensor fusion algorithm
  * @param SensorAxesRaw_t ACC_Value_Raw Acceleration value (x/y/z)
  * @param SensorAxes_t GYR_Value Gyroscope value (x/y/z)
  * @param SensorAxes_t MAG_Value Magneto value (x/y/z)
  * @retval None
  */
void MotionFX_manager_run( SensorAxesRaw_t ACC_Value_Raw,SensorAxes_t GYR_Value,SensorAxes_t MAG_Value)
{  
  iDataIN.gyro[0] = GYR_Value.AXIS_X  * FROM_MDPS_TO_DPS;
  iDataIN.gyro[1] = GYR_Value.AXIS_Y  * FROM_MDPS_TO_DPS;
  iDataIN.gyro[2] = GYR_Value.AXIS_Z  * FROM_MDPS_TO_DPS; 

  iDataIN.acc[0] = ACC_Value_Raw.AXIS_X * sensitivity_Mul;
  iDataIN.acc[1] = ACC_Value_Raw.AXIS_Y * sensitivity_Mul;
  iDataIN.acc[2] = ACC_Value_Raw.AXIS_Z * sensitivity_Mul;

  iDataIN.mag[0] = (MAG_Value.AXIS_X - magOffset.magOffX) * FROM_MGAUSS_TO_UT50;
  iDataIN.mag[1] = (MAG_Value.AXIS_Y - magOffset.magOffY) * FROM_MGAUSS_TO_UT50;
  iDataIN.mag[2] = (MAG_Value.AXIS_Z - magOffset.magOffZ) * FROM_MGAUSS_TO_UT50;

  if(discardedCount == SAMPLETODISCARD){
    osx_MotionFX_propagate(&iDataOUT, &iDataIN, DELTATIMESENSORFUSION);
    osx_MotionFX_update(&iDataOUT, &iDataIN, DELTATIMESENSORFUSION, NULL);
  } else {
    discardedCount++;
  }  
}

/**
  * @brief  Initialize MotionFX License
  * @param  MDM_PayLoadLic_t *PayLoad Pointer to the osx License MetaData
  * @retval None
  */
void MotionFX_License_init(MDM_PayLoadLic_t *PayLoad)
{
  MCR_OSX_COPY_LICENSE_TO_MDM(osx_mfx_license,PayLoad->osxLicense);
  if(!osx_MotionFX_initialize()) {
    OSX_BMS_PRINTF("Error MotionFX License authentication\n\r");
    while(1) {
      ;
    }
  } else {
    osx_MotionFX_getLibVersion(PayLoad->osxLibVersion);
    OSX_BMS_PRINTF("Enabled %s\n\r",PayLoad->osxLibVersion);
    if(PayLoad->osxLicenseInitialized==0) {
      NecessityToSaveMetaDataManager=1;
      PayLoad->osxLicenseInitialized=1;
    }
  }
}

/**
  * @brief  Initialize MotionFX engine
  * @retval None
  */
void MotionFX_manager_init(void)
{
  //  ST MotionFX Engine Initializations
  ipKnobs = &iKnobs;

  osx_MotionFX_compass_init();

  osx_MotionFX_getKnobs(ipKnobs);

  ipKnobs->gbias_acc_th_sc_6X = GBIAS_ACC_TH_SC_6X;
  ipKnobs->gbias_gyro_th_sc_6X = GBIAS_GYRO_TH_SC_6X;
  ipKnobs->gbias_mag_th_sc_6X = GBIAS_MAG_TH_SC_6X;

  ipKnobs->gbias_acc_th_sc_9X = GBIAS_ACC_TH_SC_9X;
  ipKnobs->gbias_gyro_th_sc_9X = GBIAS_GYRO_TH_SC_9X;
  ipKnobs->gbias_mag_th_sc_9X = GBIAS_MAG_TH_SC_9X;
  
#ifdef STM32_NUCLEO
  #ifdef IKS01A1
  if(TargetBoardFeatures.HWAdvanceFeatures) {    
  #endif /* IKS01A1 */
    /* LSM6DS3/LSM6DSL */
    ipKnobs->acc_orientation[0] ='n';
    ipKnobs->acc_orientation[1] ='w';
    ipKnobs->acc_orientation[2] ='u';

    ipKnobs->gyro_orientation[0] = 'n';
    ipKnobs->gyro_orientation[1] = 'w';
    ipKnobs->gyro_orientation[2] = 'u';   
  #ifdef IKS01A1
  } else {
    /* LSM6DS0 */
    ipKnobs->acc_orientation[0] ='e';
    ipKnobs->acc_orientation[1] ='n';
    ipKnobs->acc_orientation[2] ='u';

    ipKnobs->gyro_orientation[0] = 'e';
    ipKnobs->gyro_orientation[1] = 'n';
    ipKnobs->gyro_orientation[2] = 'u';
  }
  #endif /* IKS01A1 */
  #ifdef IKS01A1
  ipKnobs->mag_orientation[0] = 's';
  ipKnobs->mag_orientation[1] = 'e';
  ipKnobs->mag_orientation[2] = 'u';
  #elif IKS01A2
  ipKnobs->mag_orientation[0] = 'n';
  ipKnobs->mag_orientation[1] = 'e';
  ipKnobs->mag_orientation[2] = 'u';
  #endif /* IKS01A1 */
#elif STM32_SENSORTILE
  ipKnobs->acc_orientation[0] ='w';
  ipKnobs->acc_orientation[1] ='s';
  ipKnobs->acc_orientation[2] ='u';

  ipKnobs->gyro_orientation[0] = 'w';
  ipKnobs->gyro_orientation[1] = 's';
  ipKnobs->gyro_orientation[2] = 'u';
  
  ipKnobs->mag_orientation[0] = 's';
  ipKnobs->mag_orientation[1] = 'w';
  ipKnobs->mag_orientation[2] = 'u';
#endif /* STM32_NUCLEO */

  ipKnobs->output_type = OSXMFX_ENGINE_OUTPUT_ENU;

  ipKnobs->LMode = 1;
  ipKnobs->modx  = 1;

  osx_MotionFX_setKnobs(ipKnobs);

  osx_MotionFX_enable_6X(OSXMFX_ENGINE_DISABLE);

  osx_MotionFX_enable_9X(OSXMFX_ENGINE_DISABLE);

  discardedCount = 0;

  /* Reset MagnetoOffset */
  magOffset.magOffX = magOffset.magOffY= magOffset.magOffZ=0;

  TargetBoardFeatures.osxMotionFXIsInitalized=1;
  OSX_BMS_PRINTF("Initialized osxMotionFX\n\r");
}

/**
 * @brief  Start 6 axes MotionFX engine
 * @retval None
 */
void MotionFX_manager_start_6X(void)
{
  osx_MotionFX_enable_6X(OSXMFX_ENGINE_ENABLE);
}

/**
 * @brief  Stop 6 axes MotionFX engine
 * @retval None
 */
void MotionFX_manager_stop_6X(void)
{
  osx_MotionFX_enable_6X(OSXMFX_ENGINE_DISABLE);
}

/**
 * @brief  Start 9 axes MotionFX engine
 * @retval None
 */
void MotionFX_manager_start_9X(void)
{
  osx_MotionFX_enable_9X(OSXMFX_ENGINE_ENABLE);
}

/**
 * @brief  Stop 9 axes MotionFX engine
 * @retval None
 */
void MotionFX_manager_stop_9X(void)
{
  osx_MotionFX_enable_9X(OSXMFX_ENGINE_DISABLE);
}

/**
* @brief  Get MotionFX Engine data Out
* @param  None
* @retval osxMFX_output *iDataOUT MotionFX Engine data Out
*/
osxMFX_output* MotionFX_manager_getDataOUT(void)
{
  return &iDataOUT;
}

/**
* @brief  Get MotionFX Engine data IN
* @param  None
* @retval osxMFX_input *iDataIN MotionFX Engine data IN
*/
osxMFX_input* MotionFX_manager_getDataIN(void)
{
  return &iDataIN;
}

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

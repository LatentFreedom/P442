/**
  ******************************************************************************
  * @file    osx_motion_ar.h
  * @author  Central Lab
  * @version V1.2.0
  * @date    4-April-2016
  * @brief   Header for osx_motion_ar module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Licensed under AMS-ST OpenSoftwareX Limited License Agreement, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        ..\OpenSoftwareX_LLA_evaluation_5Nov2014.pdf
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
  ********************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OSX_MOTION_AR_H_
#define _OSX_MOTION_AR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup OSX_MOTION_AR OSX_MOTION_AR
  * @{
  */
    

/** @defgroup OSX_MOTION_AR_Exported_Types OSX_MOTION_AR_Exported_Types
 * @{
 */  
/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /*  acc x axes [g]  */
  float AccY;           /*  acc y axes [g]  */
  float AccZ;           /*  acc z axes [g]  */
} osx_MAR_input_t;  

typedef enum 
{
    OSX_MAR_NOACTIVITY          = 0x00,  
    OSX_MAR_STATIONARY          = 0x01,
    OSX_MAR_WALKING             = 0x02,
    OSX_MAR_FASTWALKING         = 0x03,
    OSX_MAR_JOGGING             = 0x04,
    OSX_MAR_BIKING              = 0x05,
    OSX_MAR_DRIVING             = 0x06
} osx_MAR_output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup OSX_MOTION_AR_Exported_Functions OSX_MOTION_AR_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */


/**
 * @brief  Initialize the MotionAR engine
 * @param  None
 * @retval 1 in case of success, 0 otherwise
 */
uint8_t  osx_MotionAR_Initialize(void);


/**
 * @brief  Set the MotionAR Accelerometer data orientation
 * @param  *acc_orientation: reference system of the Accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval  None
 */
void osx_MotionAR_SetOrientation_Acc(const char *acc_orientation);


/**
 * @brief  Run Activity Recognition Algorithm
 * @param  data_in: pointer to the osx_MAR_input_t structure
 * @retval activity index
 */
osx_MAR_output_t osx_MotionAR_Update(osx_MAR_input_t *data_in);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t osx_MotionAR_GetLibVersion(char *version);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _OSX_MOTION_AR_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

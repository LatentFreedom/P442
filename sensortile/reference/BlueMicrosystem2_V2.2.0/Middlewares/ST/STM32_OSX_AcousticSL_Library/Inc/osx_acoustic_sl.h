/**
******************************************************************************
* @file    osx_acoustic_sl.h
* @author  Central Labs
* @version V1.1.0
* @date    01-Sep-2016
* @brief   This file contains OSX Sound Source Localization library definitions.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*
* This code is ST proprietary and confidential. Any use of the code for
*    whatever purpose is subject to specific written permission of
*             STMicroelectronics International N.V.”.
*
* Some of the library code is based on the CMSIS DSP software library by ARM,
* a suite of common signal processing functions for use on Cortex-M processor
* based devices. Licencing terms are available in the attached release_note.html
* file, in the libSoundSourceLoc100 application note, in the next lines of this
* document and it's available on the web at:
* http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html
*
*   ARM licence note:
*
* Copyright (C) 2009-2012 ARM Limited.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*  - Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  - Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of ARM nor the names of its contributors may be used
*   to endorse or promote products derived from this software without
*   specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OSX_ACOUSTIC_SL_H
#define __OSX_ACOUSTIC_SL_H

#include "stdint.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @addtogroup MIDDLEWARES
* @{
*/

/** @defgroup OSX_ACOUSTIC_SL OSX_ACOUSTIC_SL
* @{
*/

/** @defgroup OSX_Acoustic_SL_Exported_Constants osxAcousticSL Exported Constants
* @{
*/  

 /** @defgroup OSX_Acoustic_SL_algorithm_type
* @brief    Source Localization algorithm type 
* @{
*/ 
#define OSX_ACOUSTIC_SL_ALGORITHM_XCORR                0x00000001
#define OSX_ACOUSTIC_SL_ALGORITHM_GCCP                 0x00000002
#define OSX_ACOUSTIC_SL_ALGORITHM_BMPH                 0x00000004
/**
* @}
*/

/** @defgroup OSX_Acoustic_SL_errors
* @brief    Source Localization errors 
* @{
*/ 
#define OSX_ACOUSTIC_SL_ALGORITHM_ERROR                0x00000001
#define OSX_ACOUSTIC_SL_PTR_CHANNELS_ERROR             0x00000002
#define OSX_ACOUSTIC_SL_CHANNEL_NUMBER_ERROR           0x00000004
#define OSX_ACOUSTIC_SL_SAMPLING_FREQ_ERROR            0x00000008
#define OSX_ACOUSTIC_SL_RESOLUTION_ERROR               0x00000010
#define OSX_ACOUSTIC_SL_THRESHOLD_ERROR                0x00000020
#define OSX_ACOUSTIC_SL_DISTANCE_ERROR                 0x00000040
#define OSX_ACOUSTIC_SL_NUM_OF_SAMPLES_ERROR           0x00000080
#define OSX_ACOUSTIC_SL_LOCK_ERROR                     0x80000000
/**
* @}
*/  
#define OSX_ACOUSTIC_SL_NO_AUDIO_DETECTED              -100  
/**
* @}
*/
  
/** @defgroup OSX_Acoustic_SL_Exported_Types osxAcousticSL Exported Types
* @{
*/
/**
* @brief  Library handler. It keeps track of the static parameters
*         and it handles the internal state of the algorithm.
*/
typedef struct
{
  uint32_t algorithm;                           /*!< Specifies the algorithm to be used between XCORR and GCC-PHAT. This parameter can be a value of @ref 
OSX_Acoustic_SL_algorithm_type. Default value is OSX_ACOUSTIC_BF_TYPE_CARDIOID_BASIC */
  
  uint32_t sampling_frequency;                  /*!< Specifies the sampling frequency - for future use */
  
  uint32_t channel_number;                      /*!< Specifies the number of channels, can be 2 for 180° estimation, 4 for 360° estimation. Default value is 
2. */  
  uint8_t ptr_M1_channels;                      /*!< Number of channels in the stream of Microphone 1. Deafult value is 1. */  
  uint8_t ptr_M2_channels;                      /*!< Number of channels in the stream of Microphone 2. Deafult value is 1. */ 
  uint8_t ptr_M3_channels;                      /*!< Number of channels in the stream of Microphone 3. Deafult value is 1. */
  uint8_t ptr_M4_channels;                      /*!< Number of channels in the stream of Microphone 4. Deafult value is 1. */
  
  uint16_t M12_distance;                        /*!< Distance between Mic1 and Mic2 in decimals of a millimeter. Deafult value is 150. */
  uint16_t M34_distance;                        /*!< Distance between Mic3 and Mic4 in decimals of a millimeter. Deafult value is 150. */
  
  uint32_t internal_memory_size;                /*!< Keeps track of the amount of memory required for the current setup.
                                                     It's filled by the osx_AcousticSL_getMemorySize() function and must
                                                     be used to allocate the right amount of RAM */
  uint32_t * pInternalMemory;                   /*!< Pointer to the memory allocated by the user */
  int16_t samples_to_process;                   /*!< Specifies the number of samples to be processed at a time */      
  
} osx_AcousticSL_Handler_t;

/**
 * @brief  Library dynamic configuration handler. It contains dynamic parameters.
 */
typedef struct
{
  uint16_t threshold;                           /*!< Specifies a value related to a voice-activity score. With values below the threshold, the algorithm does not act. The threshold value ranges from 0 to 1000 and the default value is 24. */
  uint32_t resolution;                          /*!< Angle resolution for the algorithms. Ignored if XCORR is used. Deafult value is 4. */
} osx_AcousticSL_Config_t;


/**
* @}
*/
  
    /** @defgroup OSX_Acoustic_SL_Exported_Functions osxAcousticSL Exported Functions
 * @{
 */
/**
 * @brief  Fills the "internal_memory_size" of the pHandler parameter passed as argument with a value representing the
 *         right amount of memory needed by the library, depending on the specific static parameters adopted.
 * @param  pHandler: osx_AcousticSL_Handler_t filled with desired parameters.
 * @retval 0 if everything is fine.
 */
uint32_t osx_AcousticSL_getMemorySize(osx_AcousticSL_Handler_t * pHandler);

/**
 * @brief  Library initialization.
 * @param  pHandler: osx_AcousticSL_Handler_t filled with desired parameters.
 * @retval 0 if everything is fine.
 *         different from 0 if erroneous parameters have been passed to the Init function and the default value has been used.
 *         The specific error can be recognized by checking the relative bit in the returned word.
 */
uint32_t osx_AcousticSL_Init(osx_AcousticSL_Handler_t * pHandler);

/**
 * @brief  Library data input
 * @param  pM1: pointer to an array that contains PCM samples (16 bit signed int)
 *         representing 1 ms of data acquired by the first channel.
 * @param  pM2: pointer to an array that contains PCM samples (16 bit signed int)
 *         representing 1 ms of data acquired by the second channel.
 * @param  pM3: pointer to an array that contains PCM samples (16 bit signed int)
 *         representing 1 ms of data acquired by the third channel.
 * @param  pM4: pointer to an array that contains PCM samples (16 bit signed int)
 *         representing 1 ms of data acquired by the fourth channel.
 * @param  pHandler: pointer to the handler of the curent Source Localization instance running.
 * @retval 1 if data collection is finished and libSoundSourceLoc_Process must be called, 0 otherwise.
 * @note   Input function reads samples skipping the required number of values depending on the Ptr_Mx_Channels configuration.
 * @note   pM3 and pM4 are ignored in the case the library is setup for using 2 channels.
*/
uint32_t osx_AcousticSL_Data_Input(void *pM1, void *pM2, void *pM3, void *pM4, osx_AcousticSL_Handler_t * pHandler);

/**
 * @brief  Library run function, performs audio analysis when all required data has been collected.
 * @param  Estimated_Angle: pointer to the int32_t variable that will contain the computed value.
 * @param  pHandler: pointer to the handler of the current Source Localization instance running.
 * @retval 0 if everything is ok, 1 otherwise
*/
uint32_t osx_AcousticSL_Process(int32_t * Estimated_Angle, osx_AcousticSL_Handler_t * pHandler);

/**
 * @brief  Library setup function, it sets the values for threshold and resolution. It can be called at runtime to change
 *         dynamic parameters.
 * @note   Only the threshold and resolution are evaluated by the SetConfig function.
 * @retval 0 if everything is fine.
 *         different from 0 if erroneous parameters have been passed to the Init function and the default value has been used.
 *         The specific error can be recognized by checking the relative bit in the returned word.
*/
uint32_t osx_AcousticSL_setConfig(osx_AcousticSL_Handler_t * pHandler, osx_AcousticSL_Config_t * pConfig);

/**
 * @brief  Fills the pConfig structure with the actual dynamic parameters as they are currently used inside the library.
 * @param  pHandler: pointer to the handler of the current Source Localization instance running.
 * @param  pConfig: pointer to the dynamic parameters handler that will be filled with the current library configuration
 * @retval 0 if everything is fine.
*/
uint32_t osx_AcousticSL_getConfig(osx_AcousticSL_Handler_t * pHandler, osx_AcousticSL_Config_t * pConfig);

/**
 * @brief  To be used to retrieve version information.
 * @param  version char array to be filled with the current library version
 * @retval 0 if everything is fine.
*/
uint32_t osx_AcousticSL_GetLibVersion(char *version);

/**
 * @brief  Unlock the library.
 * @note the node-locking license header must be filled with the correct values after the Open.Audio license request
 * @retval 0 if everything is fine.
*/
uint32_t osx_AcousticSL_Initialize(void);

/**
  * @}
  */
/**
  * @}
  */

/**
  * @}
  */
#endif /* __OSX_ACOUSTIC_SL_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

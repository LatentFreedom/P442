/**
 ******************************************************************************
 * @file    AudioBV_Manager.c
 * @author  Central LAB
 * @version V2.2.0
 * @date    23-December-2016
 * @brief   This file includes BlueVoice interface functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#include "TargetFeatures.h"

/* Code for BlueVoice integration - Start Section */
#include "../../../../../Middlewares/ST/STM32_OSX_BlueVoice_Library/osx_license.h"

/* Imported Variables -------------------------------------------------------------*/
extern uint16_t PCM_Buffer[];
extern OSX_BLUEVOICE_ProfileHandle_t BLUEVOICE_tx_handle;

/* Private Variables -------------------------------------------------------------*/
volatile uint8_t SendBlueVoice;
static uint32_t led_toggle_count = 0; /*!< Variable used to handle led toggling.*/
OSX_BLUEVOICE_Config_t BLUEVOICE_Config;
OSX_BV_Status bvStat;

/* Private Defines -------------------------------------------------------------*/
#define LED_TOGGLE_STREAMING  100

/**
  * @brief  Initialize BlueVoice License
  * @param  MDM_PayLoadLic_t *PayLoad Pointer to the osx License MetaData
  * @retval None
  */
void AudioBV_License_init(MDM_PayLoadLic_t *PayLoad)
{
  MCR_OSX_COPY_LICENSE_TO_MDM(osx_bv_license,PayLoad->osxLicense);
  
  if(osx_BlueVoice_Initialize()) {
    OSX_BMS_PRINTF("Error BlueVoice License authentication \n\r");
    while(1) {
      ;
    }
  } else {
    osx_BlueVoice_GetLibVersion(PayLoad->osxLibVersion);
    OSX_BMS_PRINTF("Enabled %s\n\r",PayLoad->osxLibVersion);
    if(PayLoad->osxLicenseInitialized==0) {
      NecessityToSaveMetaDataManager=1;
      PayLoad->osxLicenseInitialized=1;
    }   
  }
}

/**
* @brief  Initialises BlueVoice manager
* @param  None
* @retval None
*/
void AudioBV_Manager_init(void)
{
  
 #ifdef STM32_NUCLEO
  BLUEVOICE_Config.sampling_frequency = FR_8000;
  BLUEVOICE_Config.channel_in = 1;
  BLUEVOICE_Config.channel_tot = 2;
  bvStat = osx_BlueVoice_SetConfig(&BLUEVOICE_Config);
  if (bvStat != OSX_BV_SUCCESS) {
    goto fail;
  }
#endif /* STM32_NUCLEO */
  
#ifdef STM32_SENSORTILE
  BLUEVOICE_Config.sampling_frequency = FR_8000;
  BLUEVOICE_Config.channel_in = 1;
  BLUEVOICE_Config.channel_tot = 1;
  bvStat = osx_BlueVoice_SetConfig(&BLUEVOICE_Config);
  if (bvStat != OSX_BV_SUCCESS) {
    goto fail;
  }
#endif /* STM32_SENSORTILE */
  
  bvStat = osx_BlueVoice_SetTxHandle(&BLUEVOICE_tx_handle);
  if (bvStat != OSX_BV_SUCCESS) {
    goto fail;
  }
  
  /* If everything is ok */
  TargetBoardFeatures.osxAudioBVIsInitalized=1;
  OSX_BMS_PRINTF("Initialized osxBlueVoice\r\n");

  return;
  
  fail:
    while(1){}
}

/**
* @brief  User function that is called when the PCM_Buffer is full and ready to send.
* @param  none
* @retval None
*/
void AudioProcess_BV(void)
{
    OSX_BV_Status status;

    if (osx_BlueVoice_IsProfileConfigured()) {
      status = osx_BlueVoice_AudioIn((uint16_t*) PCM_Buffer, BV_PCM_AUDIO_IN_SAMPLES);
      if (led_toggle_count++ >= LED_TOGGLE_STREAMING) {
        led_toggle_count = 0;
        LedToggleTargetPlatform();
      }
      if(status==OSX_BV_OUT_BUF_READY) {
        SendBlueVoice = 1;
      }
    }
}

/* Code for BlueVoice integration - End Section */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

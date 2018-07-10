/**
  ******************************************************************************
  * @file    osx_bluevoice.h
  * @author  Central Labs
  * @version V2.0.0
  * @date    4-July-2016
  * @brief   This file contains definitions for BlueVoice profile.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OSX_BLUEVOICE_H
#define __OSX_BLUEVOICE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "bluenrg_aci_const.h"
#include "bluenrg_gatt_aci.h" 

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup OSX_BLUEVOICE OSX_BLUEVOICE
* @{
*/

/** @defgroup OSX_BLUEVOICE_Exported_Enums OSX_BLUEVOICE_Exported_Enums
  * @{
 */

/** @addtogroup OSX_BLUEVOICE_Frequencies
  * @{
  */

/** 
* @brief Accepted audio in sampling frequencies.
*/
typedef enum
{
  FR_8000 = 8000,                               /*!< Sampling frequency 8000 kHz.*/
  FR_16000 = 16000                              /*!< Sampling frequency 16000 kHz.*/
} Sampling_fr_t;
/**
  * @}
  */ 

/** @addtogroup OSX_BLUEVOICE_Return_States
  * @{
  */

/** 
* @brief Return states values.
*/
typedef enum 
{
  OSX_BV_SUCCESS = 0x00,                        /*!< Success.*/
  OSX_BV_LOCKER_ERROR = 0x11,                   /*!< The library locker authenticate failed.*/
  OSX_BV_ERROR = 0x01,                          /*!< Error.*/
  OSX_BV_DISCONNETED = 0x10,                    /*!< BLE Disconnected Error.*/
  OSX_BV_NOT_CONFIG = 0x20,                     /*!< BLUEVOICE library not configured.*/
  OSX_BV_RECEIVER_DISABLE = 0x30,               /*!< Receiver mode disabled.*/
  OSX_BV_TRANSMITTER_DISABLE = 0x31,            /*!< Transmitter mode disabled.*/
  OSX_BV_RX_HANDLE_NOT_AVAILABLE = 0x32,        /*!< The handle isn't recognized.*/
  OSX_BV_OUT_BUF_READY = 0x40,                  /*!< The audio out buffer is ready to be sent.*/
  OSX_BV_OUT_BUF_NOT_READY = 0x41,              /*!< The audio out buffer is not ready to be sent.*/
  OSX_BV_PCM_SAMPLES_ERR = 0x50,                /*!< The number of PCM samples given as audio input is not correct.*/
  OSX_BV_NOTIF_DISABLE = 0x60,                  /*!< The notifications are disabled.*/
  OSX_BV_TIMEOUT = 0x70                         /*!< A BLE timeout occurred.*/
} OSX_BV_Status;
/**
  * @}
  */ 

/** @addtogroup OSX_BLUEVOICE_Mode
  * @{
  */

/** 
* @brief BlueVoice working modalities.
*/
typedef enum
{
  NOT_READY = 0x00,                                /*!< Device not ready.*/
  TRANSMITTER = 0x01,                              /*!< Device in Transmitter mode.*/
  RECEIVER = 0x02,                                 /*!< Device in Receiver mode.*/
  HALF_DUPLEX = 0x03                               /*!< Device in HalfDuplex mode.*/
} OSX_BV_Mode;
/**
  * @}
  */ 

/** @addtogroup OSX_BLUEVOICE_Profile_States
  * @{
  */

/** 
* @brief BlueVoice profile status.
*/
typedef enum
{
  OSX_BLUEVOICE_STATUS_UNITIALIZED = 0x00,      /*!< BlueVoice Profile is not initialized.*/
  OSX_BLUEVOICE_STATUS_INITIALIZED = 0x10,      /*!< BlueVoice Profile is initialized.*/
  OSX_BLUEVOICE_STATUS_READY = 0x20,            /*!< BlueVoice channel is ready and idle.*/
  OSX_BLUEVOICE_STATUS_STREAMING = 0x30,        /*!< BlueVoice device is streaming data.*/
  OSX_BLUEVOICE_STATUS_RECEIVING = 0x40         /*!< BlueVoice device is receiving data.*/
} OSX_BV_Profile_Status;

/**
  * @}
  */

/**
  * @}
  */ 

/** @defgroup OSX_BLUEVOICE_Exported_Types OSX_BLUEVOICE_Exported_Types
  * @{
 */

/** @addtogroup OSX_BLUEVOICE_Configuration
  * @{
  */

/** 
* @brief BlueVoice profile configuration parameters.
*/
typedef struct
{   
  Sampling_fr_t sampling_frequency;     /*!< Specifies the sampling frequency in kHz - can be 16 kHz or 8 kHz.*/
  
  uint8_t channel_tot;                  /*!< Number of channels contained in the buffer given as Audio Input.*/
  
  uint8_t channel_in;                   /*!< The chosen channel among the available in the input buffer.*/

} OSX_BLUEVOICE_Config_t;
 /**
  * @}
  */ 

/** @addtogroup OSX_BLUEVOICE_UUIDs
  * @{
  */

/** 
* @brief BlueVoice profile configuration parameters.
*/
typedef struct
{
  uint8_t ServiceUUID[16];              /*!< Service UUID.*/
 
  uint8_t CharAudioUUID[16];            /*!< Audio characteristic UUID.*/
  
  uint8_t CharAudioSyncUUID[16];        /*!< Audio sync characteristic UUID.*/

} OSX_BLUEVOICE_uuid_t;
 /**
  * @}
  */ 

/** @addtogroup OSX_BLUEVOICE_Handlers
  * @{
  */

/** 
* @brief BlueVoice profile configuration parameters.
*/
typedef struct
{
  uint16_t ServiceHandle;               /*!< Service handle.*/
 
  uint16_t CharAudioHandle;             /*!< Audio characteristic handle.*/
  
  uint16_t CharAudioSyncHandle;         /*!< Audio Sync characteristic handle.*/

} OSX_BLUEVOICE_ProfileHandle_t;

 /**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup OSX_BLUEVOICE_Exported_Constants OSX_BLUEVOICE_Exported_Constants
  * @{
  */ 

/** @addtogroup OSX_BLUEVOICE_Timeouts
  * @{
  */
#define OSX_BLUEVOICE_TIMEOUT_STATUS                 ((uint16_t)500)      /*!< status timeout (in ms), the function "osx_BlueVoice_IncTick" must be 
                                                                               called every 1 ms. If timeout expires, the device goes from receiving/streaming 
                                                                               to ready mode.*/
                                                                            
/**
  * @}
  */

/** @addtogroup OSX_BLUEVOICE_PCM_samples_packet
* @{
*/
#define OSX_BLUEVOICE_PCM_SAMPLES_PER_PACKET            (40)                 /*!< Number of PCM samples contained in a single BlueVoice audio packet .*/

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup OSX_BLUEVOICE_Exported_Functions OSX_BLUEVOICE_Exported_Functions
  * @{
  */

/**
  * @brief  This function is called to initialize the bluevoice library.
  * @param  None.
  * @retval OSX_BV_Status: OSX_BV_SUCCESS if the configuration is ok, OSX_BV_ERROR otherwise.
  */
  OSX_BV_Status osx_BlueVoice_Initialize(void);   
    
/**
  * @brief  This function is called to set the configuration parameters 
  * @param  OSX_BLUEVOICE_Config: It contains the configuration parameters.
  * @retval OSX_BV_Status: OSX_BV_SUCCESS if the configuration is ok, OSX_BV_ERROR otherwise.
  */
  OSX_BV_Status osx_BlueVoice_SetConfig(OSX_BLUEVOICE_Config_t *OSX_BLUEVOICE_Config);

/**
  * @brief  This function returns if the BlueVoice Profile is configured.
  * @param  None.
  * @retval uint8_t: 1 if the profile is configured 0 otherwise.
  */
  uint8_t osx_BlueVoice_IsProfileConfigured(void);
  
/**
  * @brief  This function is called to add BlueVoice Service.
  * @param  service_uuid: Service uuid value.
  * @param  service_handle: Pointer to a variable in which the service handle will be saved.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_AddService(uint8_t *service_uuid, uint16_t *service_handle);

/**
  * @brief  This function is called to add BlueVoice characteristics.
  * @param  uuid: It contains the uuid values of the Audio and audioSync characteristics.
  * @param  service_handle: Handle of the service to which the characteristic must be added.
  * @param  handle: Pointer to a BLUEVOICE_ProfileHandle_t struct in which the handles will be saved.
  * @retval OSX_BV_Status: OSX_BV_SUCCESS in case of success, OSX_BV_ERROR otherwise.
  */
  OSX_BV_Status osx_BlueVoice_AddChar(OSX_BLUEVOICE_uuid_t uuid, uint16_t service_handle, OSX_BLUEVOICE_ProfileHandle_t *handle);

/**
  * @brief  This function is called to set the handles if the BlueVoice characteristics are added out of the library.
  * @param  tx_handle: Pointer to a OSX_BLUEVOICE_ProfileHandle_t struct in which the handles are stored.
  * @retval OSX_BV_Status: OSX_BV_SUCCESS in case of success, OSX_BV_ERROR otherwise.
  */
  OSX_BV_Status osx_BlueVoice_SetTxHandle(OSX_BLUEVOICE_ProfileHandle_t *tx_handle);  
  
/**
  * @brief  This function is called to set the handles discovered, if an other BlueVoice module is available.
  * @param  rx_handle: Pointer to a OSX_BLUEVOICE_ProfileHandle_t struct in which the handles are stored.
  * @retval OSX_BV_Status: OSX_BV_SUCCESS in case of success, OSX_BV_ERROR otherwise.
  */
  OSX_BV_Status osx_BlueVoice_SetRxHandle(OSX_BLUEVOICE_ProfileHandle_t *rx_handle);

/**
  * @brief  This function returns the BLUEVOICE Profile State Machine status.
  * @param  None.
  * @retval OSX_BV_Profile_Status: BLUEVOICE Profile Status.
  */
  OSX_BV_Profile_Status osx_BlueVoice_GetStatus(void);

/**
  * @brief  This function returns the current modality.
  * @param  None.
  * @retval OSX_BV_Mode: Current working modality: NOT_READY, RECEIVER, TRANSMITTER or HALF_DUPLEX.
  */
  OSX_BV_Mode osx_BlueVoice_GetMode(void);
   
/**
  * @brief  This function increases the the internal counter, used to switch from Receiving/Streaming to Ready status.
  * @param  None.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_IncTick(void);
  
/**
  * @brief  This function is called to enable notifications mechanism.
  * @param  None.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_EnableNotification(void);
  
/**
  * @brief  This function is called to fill audio buffer.
  * @param  buffer: Audio in PCM buffer.
  * @param  Nsamples: Number of PCM 16 bit audio samples.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_AudioIn(uint16_t* buffer, uint8_t Nsamples);
  
/**
  * @brief  This function must be called when the compressed audio data are ready, 
  *         (when the function BLUEVOICE_AudioIn returns OSX_BV_OUT_BUF_READY)
  * @param  NbyteSent: Number of bytes sent.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_SendData(uint16_t *NbyteSent);
  
/**
  * @brief  This function is called to parse received data.
  * @param  buffer_in: 8-bit packed ADPCM samples source buffer.
  * @param  Len: Dimension in Bytes.
  * @param  attr_handle: Handle of the updated characteristic.
  * @param  buffer_out: 16-bit PCM samples destination buffer.
  * @param  samples: Number of 16-bit PCM samples in the destination buffer.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_ParseData(uint8_t* buffer_in, uint32_t Len, uint16_t attr_handle, uint8_t* buffer_out, uint8_t *samples);

/**
  * @}
  */ 
  
/** @defgroup OSX_BLUEVOICE_Exported_Callbacks OSX_BLUEVOICE_Exported_Callbacks
* @{
*/
  
/**
  * @brief  This function must be called when there is a LE Connection Complete event.
  * @param  handle: Connection handle.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_ConnectionComplete_CB(uint16_t handle);

/**
  * @brief  This function must be called when there is a LE disconnection Complete event. 
  * @param  None.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_DisconnectionComplete_CB(void);

/**
  * @brief  This function must be called when there is a LE attribut modified event. 
  * @param  attr_handle: Attribute handle.
  * @param  attr_len: Attribute length.
  * @param  attr_value: Attribute value.
  * @retval OSX_BV_Status: Value indicating success or error code.
  */
  OSX_BV_Status osx_BlueVoice_AttributeModified_CB(uint16_t attr_handle, uint8_t attr_len, uint8_t *attr_value);

/**
  * @brief  This function is called to get library version.
  * @retval string length
  */
  int osx_BlueVoice_GetLibVersion(char *version);
  
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

#endif /* __OSX_BLUEVOICE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

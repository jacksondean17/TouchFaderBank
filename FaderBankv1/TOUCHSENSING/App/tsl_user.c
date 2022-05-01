/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : App/tsl_user.c
  * Description        : User configuration file for TOUCHSENSING
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "tsl_user.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/*============================================================================*/
/* Channels                                                                   */
/*============================================================================*/

/* Source and Configuration (ROM) */
CONST TSL_ChannelSrc_T MyChannels_Src[TSLPRM_TOTAL_CHANNELS] =
{
    { CHANNEL_0_SRC, CHANNEL_0_IO_MSK, CHANNEL_0_GRP_MSK },
    { CHANNEL_1_SRC, CHANNEL_1_IO_MSK, CHANNEL_1_GRP_MSK },
    { CHANNEL_2_SRC, CHANNEL_2_IO_MSK, CHANNEL_2_GRP_MSK },
    { CHANNEL_3_SRC, CHANNEL_3_IO_MSK, CHANNEL_3_GRP_MSK },
    { CHANNEL_4_SRC, CHANNEL_4_IO_MSK, CHANNEL_4_GRP_MSK },
    { CHANNEL_5_SRC, CHANNEL_5_IO_MSK, CHANNEL_5_GRP_MSK },
    { CHANNEL_6_SRC, CHANNEL_6_IO_MSK, CHANNEL_6_GRP_MSK },
    { CHANNEL_7_SRC, CHANNEL_7_IO_MSK, CHANNEL_7_GRP_MSK },
    { CHANNEL_8_SRC, CHANNEL_8_IO_MSK, CHANNEL_8_GRP_MSK },
    { CHANNEL_9_SRC, CHANNEL_9_IO_MSK, CHANNEL_9_GRP_MSK },
    { CHANNEL_10_SRC, CHANNEL_10_IO_MSK, CHANNEL_10_GRP_MSK },
    { CHANNEL_11_SRC, CHANNEL_11_IO_MSK, CHANNEL_11_GRP_MSK },
    { CHANNEL_12_SRC, CHANNEL_12_IO_MSK, CHANNEL_12_GRP_MSK },
};

/* Destination (ROM) */
CONST TSL_ChannelDest_T MyChannels_Dest[TSLPRM_TOTAL_CHANNELS] =
{
    { CHANNEL_0_DEST },
    { CHANNEL_1_DEST },
    { CHANNEL_2_DEST },
    { CHANNEL_3_DEST },
    { CHANNEL_4_DEST },
    { CHANNEL_5_DEST },
    { CHANNEL_6_DEST },
    { CHANNEL_7_DEST },
    { CHANNEL_8_DEST },
    { CHANNEL_9_DEST },
    { CHANNEL_10_DEST },
    { CHANNEL_11_DEST },
    { CHANNEL_12_DEST },
};

/* Data (RAM) */
TSL_ChannelData_T MyChannels_Data[TSLPRM_TOTAL_CHANNELS];

/*============================================================================*/
/* Banks                                                                      */
/*============================================================================*/

/* List (ROM) */
CONST TSL_Bank_T MyBanks[TSLPRM_TOTAL_BANKS] = {
/* 3CH_LIN_H_NBR1 bank definition*/
   {&MyChannels_Src[0], &MyChannels_Dest[0], MyChannels_Data, BANK_0_NBCHANNELS, BANK_0_MSK_CHANNELS, BANK_0_MSK_GROUPS},
/* 3CH_LIN_H_NBR2 bank definition*/
   {&MyChannels_Src[3], &MyChannels_Dest[3], MyChannels_Data, BANK_1_NBCHANNELS, BANK_1_MSK_CHANNELS, BANK_1_MSK_GROUPS},
/* 3CH_LIN_H_NBR3 bank definition*/
   {&MyChannels_Src[6], &MyChannels_Dest[6], MyChannels_Data, BANK_2_NBCHANNELS, BANK_2_MSK_CHANNELS, BANK_2_MSK_GROUPS},
/* 3CH_LIN_H_NBR4 bank definition*/
   {&MyChannels_Src[9], &MyChannels_Dest[9], MyChannels_Data, BANK_3_NBCHANNELS, BANK_3_MSK_CHANNELS, BANK_3_MSK_GROUPS},
/* TOUCHKEYS bank(s) definition*/
   {&MyChannels_Src[12], &MyChannels_Dest[12], MyChannels_Data, BANK_4_NBCHANNELS, BANK_4_MSK_CHANNELS, BANK_4_MSK_GROUPS},
};
/*============================================================================*/
/* Touchkey sensors                                                           */
/*============================================================================*/

/* Data (RAM) */
TSL_TouchKeyData_T MyTKeys_Data[TSLPRM_TOTAL_TKEYS];

/* Parameters (RAM) */
TSL_TouchKeyParam_T MyTKeys_Param[TSLPRM_TOTAL_TKEYS];

/* State Machine (ROM) */

void MyTKeys_ErrorStateProcess(void);
void MyTKeys_OffStateProcess(void);

CONST TSL_State_T MyTKeys_StateMachine[] =
{
  /* Calibration states */
  /*  0 */ { TSL_STATEMASK_CALIB,              TSL_tkey_CalibrationStateProcess },
  /*  1 */ { TSL_STATEMASK_DEB_CALIB,          TSL_tkey_DebCalibrationStateProcess },
  /* Release states */
  /*  2 */ { TSL_STATEMASK_RELEASE,            TSL_tkey_ReleaseStateProcess },
#if TSLPRM_USE_PROX > 0
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   TSL_tkey_DebReleaseProxStateProcess },
#else
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   0 },
#endif
  /*  4 */ { TSL_STATEMASK_DEB_RELEASE_DETECT, TSL_tkey_DebReleaseDetectStateProcess },
  /*  5 */ { TSL_STATEMASK_DEB_RELEASE_TOUCH,  TSL_tkey_DebReleaseTouchStateProcess },
#if TSLPRM_USE_PROX > 0
  /* Proximity states */
  /*  6 */ { TSL_STATEMASK_PROX,               TSL_tkey_ProxStateProcess },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           TSL_tkey_DebProxStateProcess },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    TSL_tkey_DebProxDetectStateProcess },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     TSL_tkey_DebProxTouchStateProcess },
#else
  /*  6 */ { TSL_STATEMASK_PROX,               0 },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           0 },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    0 },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     0 },
#endif
  /* Detect states */
  /* 10 */ { TSL_STATEMASK_DETECT,             TSL_tkey_DetectStateProcess },
  /* 11 */ { TSL_STATEMASK_DEB_DETECT,         TSL_tkey_DebDetectStateProcess },
  /* Touch state */
  /* 12 */ { TSL_STATEMASK_TOUCH,              TSL_tkey_TouchStateProcess },
  /* Error states */
  /* 13 */ { TSL_STATEMASK_ERROR,              MyTKeys_ErrorStateProcess },
  /* 14 */ { TSL_STATEMASK_DEB_ERROR_CALIB,    TSL_tkey_DebErrorStateProcess },
  /* 15 */ { TSL_STATEMASK_DEB_ERROR_RELEASE,  TSL_tkey_DebErrorStateProcess },
  /* 16 */ { TSL_STATEMASK_DEB_ERROR_PROX,     TSL_tkey_DebErrorStateProcess },
  /* 17 */ { TSL_STATEMASK_DEB_ERROR_DETECT,   TSL_tkey_DebErrorStateProcess },
  /* 18 */ { TSL_STATEMASK_DEB_ERROR_TOUCH,    TSL_tkey_DebErrorStateProcess },
  /* Other states */
  /* 19 */ { TSL_STATEMASK_OFF,                MyTKeys_OffStateProcess }
};

/* Methods for "extended" type (ROM) */
CONST TSL_TouchKeyMethods_T MyTKeys_Methods =
{
  TSL_tkey_Init,
  TSL_tkey_Process
};

/* TouchKeys list (ROM) */

CONST TSL_TouchKey_T MyTKeys[TSLPRM_TOTAL_TOUCHKEYS] =
{
  { &MyTKeys_Data[0], &MyTKeys_Param[0], &MyChannels_Data[CHANNEL_12_DEST], MyTKeys_StateMachine, &MyTKeys_Methods }
};

/*============================================================================*/
/* Linear and Rotary sensors                                                  */
/*============================================================================*/

/* Data (RAM) */
TSL_LinRotData_T MyLinRots_Data[TSLPRM_TOTAL_ALL_LINROTS];

/* Parameters (RAM) */
TSL_LinRotParam_T MyLinRots_Param[TSLPRM_TOTAL_ALL_LINROTS];

/* State Machine (ROM) */

void MyLinRots_ErrorStateProcess(void);
void MyLinRots_OffStateProcess(void);

CONST TSL_State_T MyLinRots_StateMachine[] =
{
  /* Calibration states */
  /*  0 */ { TSL_STATEMASK_CALIB,              TSL_linrot_CalibrationStateProcess },
  /*  1 */ { TSL_STATEMASK_DEB_CALIB,          TSL_linrot_DebCalibrationStateProcess },
  /* Release states */
  /*  2 */ { TSL_STATEMASK_RELEASE,            TSL_linrot_ReleaseStateProcess },
#if TSLPRM_USE_PROX > 0
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   TSL_linrot_DebReleaseProxStateProcess },
#else
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   0 },
#endif
  /*  4 */ { TSL_STATEMASK_DEB_RELEASE_DETECT, TSL_linrot_DebReleaseDetectStateProcess },
  /*  5 */ { TSL_STATEMASK_DEB_RELEASE_TOUCH,  TSL_linrot_DebReleaseTouchStateProcess },
#if TSLPRM_USE_PROX > 0
  /* Proximity states */
  /*  6 */ { TSL_STATEMASK_PROX,               TSL_linrot_ProxStateProcess },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           TSL_linrot_DebProxStateProcess },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    TSL_linrot_DebProxDetectStateProcess },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     TSL_linrot_DebProxTouchStateProcess },
#else
  /*  6 */ { TSL_STATEMASK_PROX,               0 },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           0 },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    0 },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     0 },
#endif
  /* Detect states */
  /* 10 */ { TSL_STATEMASK_DETECT,             TSL_linrot_DetectStateProcess },
  /* 11 */ { TSL_STATEMASK_DEB_DETECT,         TSL_linrot_DebDetectStateProcess },
  /* Touch state */
  /* 12 */ { TSL_STATEMASK_TOUCH,              TSL_linrot_TouchStateProcess },
  /* Error states */
  /* 13 */ { TSL_STATEMASK_ERROR,              MyLinRots_ErrorStateProcess },
  /* 14 */ { TSL_STATEMASK_DEB_ERROR_CALIB,    TSL_linrot_DebErrorStateProcess },
  /* 15 */ { TSL_STATEMASK_DEB_ERROR_RELEASE,  TSL_linrot_DebErrorStateProcess },
  /* 16 */ { TSL_STATEMASK_DEB_ERROR_PROX,     TSL_linrot_DebErrorStateProcess },
  /* 17 */ { TSL_STATEMASK_DEB_ERROR_DETECT,   TSL_linrot_DebErrorStateProcess },
  /* 18 */ { TSL_STATEMASK_DEB_ERROR_TOUCH,    TSL_linrot_DebErrorStateProcess },
  /* Other states */
  /* 19 */ { TSL_STATEMASK_OFF,                MyLinRots_OffStateProcess }
};

/* Methods for "extended" type (ROM) */
CONST TSL_LinRotMethods_T MyLinRots_Methods =
{
  TSL_linrot_Init,
  TSL_linrot_Process,
  TSL_linrot_CalcPos
};

/* Delta Normalization Process
   The MSB is the integer part, the LSB is the real part
   Examples:
   - To apply a factor 1.10:
     0x01 to the MSB
     0x1A to the LSB (0.10 x 256 = 25.6 -> rounded to 26 = 0x1A)
   - To apply a factor 0.90:
     0x00 to the MSB
     0xE6 to the LSB (0.90 x 256 = 230.4 -> rounded to 230 = 0xE6)
*/
CONST uint16_t MyLinRot0_DeltaCoeff[3] = {0x0100, 0x0100, 0x0100};
CONST uint16_t MyLinRot1_DeltaCoeff[3] = {0x0100, 0x0100, 0x0100};
CONST uint16_t MyLinRot2_DeltaCoeff[3] = {0x0100, 0x0100, 0x0100};
CONST uint16_t MyLinRot3_DeltaCoeff[3] = {0x0100, 0x0100, 0x0100};

/* LinRots list (ROM)*/
CONST TSL_LinRot_T MyLinRots[TSLPRM_TOTAL_LINROTS] =
{
   /* LinRot sensor 0 = S1 */
   {
   &MyLinRots_Data[0],
   &MyLinRots_Param[0],
   &MyChannels_Data[CHANNEL_0_DEST],
   3, /* Number of channels */
   MyLinRot0_DeltaCoeff,
   (TSL_tsignPosition_T *)TSL_POSOFF_3CH_LIN_H,
   TSL_SCTCOMP_3CH_LIN_H,
   TSL_POSCORR_3CH_LIN_H,
   MyLinRots_StateMachine,
   &MyLinRots_Methods
   },
   /* LinRot sensor 1 = S2 */
   {
   &MyLinRots_Data[1],
   &MyLinRots_Param[1],
   &MyChannels_Data[CHANNEL_3_DEST],
   3, /* Number of channels */
   MyLinRot1_DeltaCoeff,
   (TSL_tsignPosition_T *)TSL_POSOFF_3CH_LIN_H,
   TSL_SCTCOMP_3CH_LIN_H,
   TSL_POSCORR_3CH_LIN_H,
   MyLinRots_StateMachine,
   &MyLinRots_Methods
   },
   /* LinRot sensor 2 = S3 */
   {
   &MyLinRots_Data[2],
   &MyLinRots_Param[2],
   &MyChannels_Data[CHANNEL_6_DEST],
   3, /* Number of channels */
   MyLinRot2_DeltaCoeff,
   (TSL_tsignPosition_T *)TSL_POSOFF_3CH_LIN_H,
   TSL_SCTCOMP_3CH_LIN_H,
   TSL_POSCORR_3CH_LIN_H,
   MyLinRots_StateMachine,
   &MyLinRots_Methods
   },
   /* LinRot sensor 3 = S4 */
   {
   &MyLinRots_Data[3],
   &MyLinRots_Param[3],
   &MyChannels_Data[CHANNEL_9_DEST],
   3, /* Number of channels */
   MyLinRot3_DeltaCoeff,
   (TSL_tsignPosition_T *)TSL_POSOFF_3CH_LIN_H,
   TSL_SCTCOMP_3CH_LIN_H,
   TSL_POSCORR_3CH_LIN_H,
   MyLinRots_StateMachine,
   &MyLinRots_Methods
   }
};

/*============================================================================*/
/* Generic Objects                                                            */
/*============================================================================*/

/* List (ROM) */
CONST TSL_Object_T MyObjects[TSLPRM_TOTAL_OBJECTS] =
{
  { TSL_OBJ_TOUCHKEY, (TSL_TouchKey_T *)&MyTKeys[0] },
  { TSL_OBJ_LINEAR, (TSL_LinRot_T *)&MyLinRots[0] },
  { TSL_OBJ_LINEAR, (TSL_LinRot_T *)&MyLinRots[1] },
  { TSL_OBJ_LINEAR, (TSL_LinRot_T *)&MyLinRots[2] },
  { TSL_OBJ_LINEAR, (TSL_LinRot_T *)&MyLinRots[3] }
};

/* Group (RAM) */
TSL_ObjectGroup_T MyObjGroup =
{
  &MyObjects[0],        /* First object */
  TSLPRM_TOTAL_OBJECTS, /* Number of objects */
  0x00,                 /* State mask reset value */
  TSL_STATE_NOT_CHANGED /* Current state */
};

/*============================================================================*/
/* TSL Common Parameters placed in RAM or ROM                                 */
/* --> external declaration in tsl_conf.h                                     */
/*============================================================================*/

TSL_Params_T TSL_Params =
{
  TSLPRM_ACQ_MIN,
  TSLPRM_ACQ_MAX,
  TSLPRM_CALIB_SAMPLES,
  TSLPRM_DTO,
#if TSLPRM_TOTAL_TKEYS > 0
  MyTKeys_StateMachine,   /* Default state machine for TKeys */
  &MyTKeys_Methods,       /* Default methods for TKeys */
#endif
#if TSLPRM_TOTAL_LNRTS > 0
  MyLinRots_StateMachine, /* Default state machine for LinRots */
  &MyLinRots_Methods      /* Default methods for LinRots */
#endif
};

/* Private functions prototype -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
/* USER CODE BEGIN Global variables */

/* USER CODE END Global variables */

__IO TSL_tTick_ms_T ECSLastTick; /* Hold the last time value for ECS */

/**
  * @brief  Initialize the STMTouch Driver
  * @param  None
  * @retval None
  */
void tsl_user_Init(void)
{
  TSL_obj_GroupInit(&MyObjGroup); /* Init Objects */

  TSL_Init(MyBanks); /* Init acquisition module */

  tsl_user_SetThresholds(); /* Init thresholds for each object individually (optional) */
}

/**
  * @brief  Execute STMTouch Driver main State machine
  * @param  None
  * @retval status Return TSL_STATUS_OK if the acquisition is done
  */
tsl_user_status_t tsl_user_Exec(void)
{
  static uint32_t idx_bank = 0;
  static uint32_t config_done = 0;
  tsl_user_status_t status = TSL_USER_STATUS_BUSY;

  /* Configure and start bank acquisition */
  if (!config_done)
  {
/* USER CODE BEGIN not config_done start*/

/* USER CODE END not config_done start*/
    TSL_acq_BankConfig(idx_bank);
    TSL_acq_BankStartAcq();
    config_done = 1;
/* USER CODE BEGIN not config_done */

/* USER CODE END not config_done */
  }

  /* Check end of acquisition (polling mode) and read result */
  if (TSL_acq_BankWaitEOC() == TSL_STATUS_OK)
  {
/* USER CODE BEGIN end of acquisition start*/

/* USER CODE END end of acquisition start*/
    STMSTUDIO_LOCK;
    TSL_acq_BankGetResult(idx_bank, 0, 0);
    STMSTUDIO_UNLOCK;
    idx_bank++; /* Next bank */
    config_done = 0;
/* USER CODE BEGIN end of acquisition */

/* USER CODE END end of acquisition */
  }

  /* Process objects, DxS and ECS
     Check if all banks have been acquired
  */
  if (idx_bank > TSLPRM_TOTAL_BANKS-1)
  {
/* USER CODE BEGIN before reset*/

/* USER CODE END before reset*/
    /* Reset flags for next banks acquisition */
    idx_bank = 0;
    config_done = 0;

    /* Process Objects */
    TSL_obj_GroupProcess(&MyObjGroup);

    /* DxS processing (if TSLPRM_USE_DXS option is set) */
    TSL_dxs_FirstObj(&MyObjGroup);

    /* ECS every TSLPRM_ECS_DELAY (in ms) */
    if (TSL_tim_CheckDelay_ms(TSLPRM_ECS_DELAY, &ECSLastTick) == TSL_STATUS_OK)
    {
      if (TSL_ecs_Process(&MyObjGroup) == TSL_STATUS_OK)
      {
        status = TSL_USER_STATUS_OK_ECS_ON;
      }
      else
      {
        status = TSL_USER_STATUS_OK_ECS_OFF;
      }
    }
    else
    {
      status = TSL_USER_STATUS_OK_NO_ECS;
    }
/* USER CODE BEGIN Process objects */

/* USER CODE END Process objects */
  }
  else
  {
    status = TSL_USER_STATUS_BUSY;
/* USER CODE BEGIN TSL_USER_STATUS_BUSY */

/* USER CODE END TSL_USER_STATUS_BUSY */
  }

  return status;
}

/**
 * Interrupt TSC management. See below code example for 3 sensors in main.c:
  * while (1)
  {
#define TKEY_DET(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_DETECT)
#define TKEY_PRX(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_PROX)
#define TKEY_REL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_RELEASE)
#define TKEY_CAL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_CALIB)
    if(tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY){
      if(!TKEY_CAL(0) && !TKEY_CAL(1) && !TKEY_CAL(2) ){
        printf("Delta: sensor0 %3d sensor1 %3d sensor2 %3d\n"
               ,MyTKeys[0].p_ChD->Delta
               ,MyTKeys[1].p_ChD->Delta
               ,MyTKeys[2].p_ChD->Delta);
        if(TKEY_DET(0)){
          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        }else if(TKEY_REL(0)){
          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        }
        if(TKEY_DET(1)){
          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        }else if(TKEY_REL(1)){
          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        }
        if(TKEY_DET(2)){
          HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        }else if(TKEY_REL(2)){
          HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
        }
      }
    }else{
      HAL_Delay(1); Can be replace by __WFI()
    }
  }
 */

/**
 * Local variable used for interrupt acquisition mode
 */
uint32_t idx_bank_it = 0; // Bank acquisition number (from 0 to TSLPRM_TOTAL_BANKS-1)
uint32_t config_done_it = 0; // Start first TSC acquisition done using bank 0
uint32_t acq_done_it = 0; // TSLPRM_TOTAL_BANKS banks acquisition done

/**
  * @brief  Acquisition completed callback in non blocking mode
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc)
{
/* USER CODE BEGIN HAL_TSC_ConvCpltCallback start*/

/* USER CODE END HAL_TSC_ConvCpltCallback start*/
  TSL_acq_BankGetResult(idx_bank_it, 0, 0);
  idx_bank_it++;
  if (idx_bank_it > TSLPRM_TOTAL_BANKS-1)
  {
    // End of all banks acquisition, restart bank 0 in while(1) loop (This is a choice)
    idx_bank_it=0;
    acq_done_it++;
  }else{
    // We restart next bank acquisition
    TSL_acq_BankConfig(idx_bank_it);
    TSL_acq_BankStartAcq_IT();
  }
/* USER CODE BEGIN HAL_TSC_ConvCpltCallback*/

/* USER CODE END HAL_TSC_ConvCpltCallback*/
}

/**
  * @brief  Execute STMTouch Driver main State machine using TSC Interrupt
  * @param  None
  * @retval status Return TSL_STATUS_OK if the acquisition is done
  */
tsl_user_status_t tsl_user_Exec_IT(void)
{
  tsl_user_status_t status = TSL_USER_STATUS_BUSY;

  /* Configure and start bank acquisition */
  if (!config_done_it)
  {
/* USER CODE BEGIN not config_done start*/

/* USER CODE END not config_done start*/
    idx_bank_it = 0;
    acq_done_it = 0;
    TSL_acq_BankConfig(idx_bank_it);
    TSL_acq_BankStartAcq_IT();
    config_done_it = 1;
/* USER CODE BEGIN not config_done */

/* USER CODE END not config_done */
  }

  /* Check end of all group acquisitions (interrupt mode) */
  if (acq_done_it)
  {
/* USER CODE BEGIN acq_done_it start*/

/* USER CODE END acq_done_it start*/

    /* Process Objects */
    TSL_obj_GroupProcess(&MyObjGroup);

    /* DxS processing (if TSLPRM_USE_DXS option is set) */
    TSL_dxs_FirstObj(&MyObjGroup);

    /* ECS every TSLPRM_ECS_DELAY (in ms) */
    if (TSL_tim_CheckDelay_ms(TSLPRM_ECS_DELAY, &ECSLastTick) == TSL_STATUS_OK)
    {
      if (TSL_ecs_Process(&MyObjGroup) == TSL_STATUS_OK)
      {
        status = TSL_USER_STATUS_OK_ECS_ON;
      }
      else
      {
        status = TSL_USER_STATUS_OK_ECS_OFF;
      }
    }
    else
    {
      status = TSL_USER_STATUS_OK_NO_ECS;
    }

    // Restart TSLPRM_TOTAL_BANKS banks acquisition
    idx_bank_it = 0;
    acq_done_it = 0;
    TSL_acq_BankConfig(idx_bank_it);
    TSL_acq_BankStartAcq_IT();
/* USER CODE BEGIN acq_done_it */

/* USER CODE END acq_done_it */
  }
  else
  {
    status = TSL_USER_STATUS_BUSY;
/* USER CODE BEGIN TSL_USER_STATUS_BUSY */

/* USER CODE END TSL_USER_STATUS_BUSY */
  }

/* USER CODE BEGIN Process objects */

/* USER CODE END Process objects */

  return status;
}

/**
  * @brief  Set thresholds for each object (optional).
  * @param  None
  * @retval None
  */
void tsl_user_SetThresholds(void)
{
/* USER CODE BEGIN Tsl_user_SetThresholds */
  /* Example: Decrease the Detect thresholds for the TKEY 0
  MyTKeys_Param[0].DetectInTh -= 10;
  MyTKeys_Param[0].DetectOutTh -= 10;
  */
/* USER CODE END Tsl_user_SetThresholds */
  }

/**
  * @brief  Executed when a sensor is in Error state
  * @param  None
  * @retval None
  */
  void MyTKeys_ErrorStateProcess(void)
{
/* USER CODE BEGIN MyTKeys_ErrorStateProcess */
  /* Add here your own processing when a sensor is in Error state */
/* USER CODE END MyTKeys_ErrorStateProcess */
}
void MyLinRots_ErrorStateProcess(void)
{
/* USER CODE BEGIN MyLinRots_ErrorStateProcess */
  /* Add here your own processing when a sensor is in Error state */
/* USER CODE END MyLinRots_ErrorStateProcess */
}

/**
  * @brief  Executed when a sensor is in Off state
  * @param  None
  * @retval None
  */
void MyTKeys_OffStateProcess(void)
{
/* USER CODE BEGIN MyTKeys_OffStateProcess */
  /* Add here your own processing when a sensor is in Off state */
/* USER CODE END MyTKeys_OffStateProcess */
}

void MyLinRots_OffStateProcess(void)
{
/* USER CODE BEGIN MyLinRots_OffStateProcess */
  /* Add here your own processing when a sensor is in Off state */
/* USER CODE END MyLinRots_OffStateProcess */
}


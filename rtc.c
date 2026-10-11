/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
extern RTC_TimeTypeDef rtc_time;
extern RTC_DateTypeDef rtc_date;

uint32_t calculate_day_number(uint8_t date, uint8_t month, uint8_t year); /* DD.MM.YY */
void calculate_date_from_day_number(uint32_t day_number, uint8_t *date, uint8_t *month, uint8_t *year);

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  RTC_DateTypeDef backup_date;

  rtc_date.Date = (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) >> 8);
  rtc_date.Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
  rtc_date.Year = (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3) >> 8);
  rtc_date.WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);

  HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN); /* There is also an internal date update based on HW RTC time elapsed!! */
  HAL_RTC_GetDate(&hrtc, &backup_date, RTC_FORMAT_BIN); /* Days elapsed since MCU power down */

  uint32_t backup_date_days = calculate_day_number(backup_date.Date, backup_date.Month, backup_date.Year);
  uint32_t rtc_date_days = calculate_day_number(rtc_date.Date, rtc_date.Month, rtc_date.Year);

  rtc_date_days += (backup_date_days - calculate_day_number(1, 1, 0));

  calculate_date_from_day_number(rtc_date_days, &rtc_date.Date, &rtc_date.Month, &rtc_date.Year);

  HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

  /*
   * You have to do not let Init code to reinitialize Time and Date in RTC.
   * It's "a bug" in HAL widely described and complain on forums.
   * That causes every MCU restart the time and date will be same as you configured in CubeMX.
   * All you have to do is just return before init time/date in this user section.
   */
  return;

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint32_t
calculate_day_number(uint8_t date, uint8_t month, uint8_t year) {
    /* Format: */
    /* DD.MM.YY */
    uint32_t _year = year + 20;
    month = (month + 9) % 12;
    _year = _year - (month / 10);
    return ((365 * _year) + (_year / 4) - (_year / 100) + (_year / 400) + (((month * 306) + 5) / 10) + (date - 1));
}

void
calculate_date_from_day_number(uint32_t day_number, uint8_t *date, uint8_t *month, uint8_t *year) {
    uint32_t _date, _month, _year;

    _year = ((10000 * day_number) + 14780) / 3652425;

    int32_t ddd = day_number - ((365 * _year) + (_year / 4) - (_year / 100) + (_year / 400));

    if (ddd < 0) {
        _year -= 1;
        ddd = day_number - ((365 * _year) + (_year / 4) - (_year / 100) + (_year / 400));
    }

    int32_t mi = ((100 * ddd) + 52) / 3060;

    _month = (mi + 2) % 12 + 1;
    _year = _year + (mi + 2)/12;
    _date = ddd - ((mi * 306) + 5)/10 + 1;

    *date = _date;
    *month = _month;
    *year = _year - 20;
}

/* USER CODE END 1 */

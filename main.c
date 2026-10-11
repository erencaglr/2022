/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : AKS v2.0 Main program body
  ******************************************************************************

  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "canbus.h"
#include "fatfs_sd.h"
#include "general.h"
#include "lora.h"
#include "nextion.h"
#include "pca9535.h"
#include "sd_card.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bms_t mert_bms = {0};
esc_t enes_esc = {0};
eys_t eren_eys = {0};
iso_t bugra_iso = {0};
uint8_t bidiribizzik[1];
uint8_t operating_mode = MOD_NORMAL;

uint8_t send_lora_main_flag = 0;
uint8_t send_lora_cell_flag = 0;
uint8_t send_lora_fault_flag = 0;
uint8_t send_nextion_flag = 0;
uint8_t read_pca_flag = 0;

extern uint8_t lora_rx_byte;
extern uint8_t lora_tx_data[10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_RTC_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  //lora_init();
  pca9535_init();

  HAL_TIM_Base_Start_IT(&htim4); /* Start 0,1 second timer interrupt */
  HAL_TIM_Base_Start_IT(&htim2);

  pca9535_read_input();

  HAL_UART_Receive_IT(MERT_BMS, &mert_bms_rx_byte, 1);
  //set_fparam();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//    fault_control();
//    switch (operating_mode) {
//        case MOD_ALARM: /* Alarm mode, if there is a fault this mode will be active */
//            DO_S1_SWITCH = 0;
//            DO_S2_SWITCH = 0;
//
//            DO_ALARM = 1;
//
//            DO_HORN = 0;
//            DO_LIGHTS = 0;
//            break;
//        case MOD_CHARGE:    /* Charge mode, if the car starts charging this mode will be active */
//            DO_S1_SWITCH = 1;
//            DO_S2_SWITCH = 0;
//
//            DO_ALARM = 0;
//
//            DO_HORN = 0;
//            DO_LIGHTS = 0;
//            break;
//        case MOD_NORMAL: /* Normal drive mode */
//            DO_S1_SWITCH = 0;
//            DO_S2_SWITCH = 1;
//
//            DO_ALARM = 0;
//
//            DO_HORN = 1;
//            DO_LIGHTS = 1;
//            break;
//        case MOD_RACE: /* Race drive mode */
//            DO_S1_SWITCH = 0;
//            DO_S2_SWITCH = 1;
//
//            DO_ALARM = 0;
//
//            DO_HORN = 0;
//            DO_LIGHTS = 0;
//            break;
//        default:
//            break;
//    }


    /* Sending things */
    if (send_lora_main_flag) {
//        lora_send_main();
//        sd_write_main();
        sd_write_test();
        send_lora_main_flag = 0;
    }
//
//    if (send_lora_cell_flag) {
//        lora_send_cell();
//        sd_write_cell();
//        send_lora_cell_flag = 0;
//        }
//
    if (send_nextion_flag) {
        send_nextion();
        send_nextion_flag = 0;
    }
//
//    if (read_pca_flag) {
//        pca9535_read_input();
//        read_pca_flag = 0;
//    }

    if(p07_flag) {
    	p17_flag = 1;
    } else {
    	p17_flag = 0;
    }

    if(p06_flag) {
    	p16_flag = 1;
        } else {
        	p16_flag = 0;
        }

    if(p05_flag) {
        	p15_flag = 1;
        } else {
        	p15_flag = 0;
        }

    if(p04_flag) {
        	p14_flag = 1;
        } else {
        	p14_flag = 0;
        }

    if(p03_flag) {           /* selenoid vana */
        	p10_flag = 1;
        } else {
        	p10_flag = 0;
        }

    if(p02_flag) {            /* flasör */
        	p11_flag = 1;
        } else {
        	p11_flag = 0;
        }

    if(p01_flag) {             /* korna */
        	p12_flag = 1;
        } else {
        	p12_flag = 0;
        }

    if(p00_flag) {              /* far */
        	p13_flag = 1;
        } else {
        	p13_flag = 0;
        }

    /* PCA Write Action */
    pca9535_write_output();

    /* Canbus send aks messages */
    can_init();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    lora_normal_mode();
    HAL_CAN_RxFifo1FullCallback(&hcan);

    HAL_UART_Transmit(LORA, lora_tx_data, 10, 100);
    lora_send_cell();
    //Max6675_Read_Temp();


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

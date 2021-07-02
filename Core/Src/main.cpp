/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Silelis_code/si468xFM.h"
#define Si4684_ADDRESS			0x64
si468xFM* pDAB_PL;
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


#define FM_reception
//#define AM_reception
//#define DAB_reception







#define CHAN_5C 178352


char* itoa(int i, char b[]) {
	char const digit[] = "0123456789";
	char* p = b;
	if (i < 0) {
		*p++ = '-';
		i *= -1;
	}
	int shifter = i;
	do {
		 //Move to where representation ends
	    ++p;
		shifter = shifter / 10;
	} while (shifter);
	*p = '\0';
	do {
		 //Move back, inserting digits as u go
	    * --p = digit[i % 10];
		i = i / 10;
	} while (i);
	return b;
}




enum Si468x_MODE
{
	Si468x_MODE_patch_016	=	0x00002000,
	Si468x_MODE_FM			=	0x00006000, //0x000063EC,
	Si468x_MODE_DAB			=	0x00086000, //0x00092000,
	Si468x_MODE_AM			=	0x00106000
}
;




static uint8_t update_interrupts = 0;

enum Interrupt
{
	STCINT,
	ACFINT,
	RDSINT,
	RSQINT,
	DSRVINT,
	DACQINT,
	ERR_CMD,
	CTS
};



void si46xx_fm_tune_freq(uint32_t khz, uint16_t antcap)
{
	/*uint8_t data[5];
	//char buf[4];

	//data[0] = (1<<4) | (1<<0); // force_wb, low side injection
	//data[0] = (1<<4)| (1<<3); // force_wb, tune_mode=2
	data[0] = 0;
	data[1] = ((khz / 10) & 0xFF);
	data[2] = ((khz / 10) >> 8) & 0xFF;
	data[3] = antcap & 0xFF;
	data[4] = 0;*/

	uint8_t data[6] =
	{
			0x30  /*FM_TUNE_FREQ*/,
		0, // (1 << 4) | (1 << 3);  // force_wb, tune_mode=2		lub		(1<<4) | (1<<0); // force_wb, low side injection
		((khz / 10) & 0xFF),
		((khz / 10) >> 8) & 0xFF,
		antcap & 0xFF,
		(antcap>> 8) & 0xFF,
	};
	//I2C_write(Si4684_ADDRESS, data, 6);
}

void si46xx_am_tune_freq(uint32_t khz, uint16_t antcap)
{
	/*uint8_t data[5];
	//char buf[4];

	//data[0] = (1<<4) | (1<<0); // force_wb, low side injection
	//data[0] = (1<<4)| (1<<3); // force_wb, tune_mode=2
	data[0] = 0;
	data[1] = ((khz / 10) & 0xFF);
	data[2] = ((khz / 10) >> 8) & 0xFF;
	data[3] = antcap & 0xFF;
	data[4] = 0;*/

	uint8_t data[6] =
	{
			0x40 /*AM_TUNE_FREQ*/,
		0,
		  // (1 << 4) | (1 << 3);  // force_wb, tune_mode=2		lub		(1<<4) | (1<<0); // force_wb, low side injection
		((khz) & 0xFF),
		((khz) >> 8) & 0xFF,
		antcap & 0xFF,
		(antcap >> 8) & 0xFF,
	};
	//I2C_write(Si4684_ADDRESS, data, 6);
}



struct dab_digrad_status_t {
	uint8_t hard_mute_int;
	uint8_t fic_error_int;
	uint8_t acq_int;
	uint8_t rssi_h_int;
	uint8_t rssi_l_int;
	uint8_t hardmute;
	uint8_t fic_error;
	uint8_t acq;
	uint8_t valid;
	int8_t rssi;  // -128-63
	int8_t snr;  // 0-20
	uint8_t fic_quality;  // 0-100
	uint8_t cnr;  // 0-54
	uint16_t fib_error_count;
	uint32_t frequency;
	uint8_t tuned_index;
	uint8_t fft_offset;
	uint16_t read_ant_cap;
	uint16_t cu_level;  // 0-470
};


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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
 // MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//  HAL_GPIO_WritePin(si468x_SPI_ENAB_GPIO_Port, si468x_SPI_ENAB_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart2, (uint8_t *)"Hello World!\r\n", 14, 200);

	uint8_t si_rec_buff[40];

	for (int i; i < 40; i++)
		si_rec_buff[i] = 0xff;


	//uint8_t resp_=0;

	if (HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 1, 50) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "i2c: EEPROM OK\r\n", 27, 200);
	}

	 si468xFM DAB_PL = si468xFM(Si468X_Reset_GPIO_Port, Si468X_Reset_Pin, &hi2c1, Si4684_ADDRESS<<1, _Si468x_MODE_FM, fmTuningRegionEU);
	 //si468xFM DAB_PL = si468xFM(Si468X_Reset_GPIO_Port, Si468X_Reset_Pin, &hspi1, si468x_SPI_ENAB_GPIO_Port, si468x_SPI_ENAB_Pin, _Si468x_MODE_FM);


	 pDAB_PL = &DAB_PL;

	 si468xPartInfo int1 = DAB_PL.getPartInfo();

	 si468xPowerUpArgs retval = DAB_PL.getPowerUpArgs();

	 si468xBootedFirmware resp_  = DAB_PL.getSystemState(); //GET_SYS_STATE

		//resp_ = si468x_get_system_state(si_rec_buff, 6);

		HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n\r\n\r\n", 6, 200);
		switch(resp_)
		{
		case FMHD_active:
			HAL_UART_Transmit(&huart2, (uint8_t *) "FMHD is active\r\n", 16, 200);
			break;
		case DAB_active:
			HAL_UART_Transmit(&huart2, (uint8_t *) "DAB is active\r\n", 15, 200);
			break;
		case AMHD_active	:
			HAL_UART_Transmit(&huart2, (uint8_t *) "AMHD is active\r\n", 16, 200);
			break;
		default:
			HAL_UART_Transmit(&huart2, (uint8_t *) "Si468x ROM not booted\r\n", 23, 200);
		}
		HAL_Delay(1000);
		// BOOT Si468x

		//DAB_PL.fmTuneFreq(10000);
//		DAB_PL.fmTuneFreq(10790);
//		DAB_PL.fmSeekStart(si468xSeekUp, 0);
//		DAB_PL.getFmRsqStatus(0, 0, 0, 0);
//		DAB_PL.fmSeekStart(si468xSeekUp, 0);
//		DAB_PL.getFmRsqStatus(0, 0, 0, 0);
//		DAB_PL.fmSeekStart(si468xSeekUp, 0);;
//		DAB_PL.getFmRsqStatus(0, 0, 0, 0);
//		DAB_PL.fmSeekStart(si468xSeekUp, 0);
//		DAB_PL.getFmRsqStatus(0, 0, 0, 0);


		uint16_t retFreq;
		retFreq = DAB_PL.fmBandScan(10010, si468xSeekUp);
		si468xFmRdsBlockcount blockCount = DAB_PL.getFmRdsBlockcount(0);
		si468xFmRdsStatus RDSStat;

		GPIO_PinState pinstate;
		while(1){
			//RDSStat =DAB_PL.getFmRdsStatus();
			RDSStat =DAB_PL.getFmRdsStatus(1);
			#ifdef useRds
				#ifdef useRdsParser
					DAB_PL.decodeRDSGroup(RDSStat.BLOCkx);
				#endif

				#ifndef useAdvRdsParser
					DAB_PL.rds_parse((uint16_t*)&RDSStat.BLOCkx);
				#endif
					blockCount = DAB_PL.getFmRdsBlockcount(0);
					pinstate = HAL_GPIO_ReadPin(GPIOB, 4);
			#endif
//			if (HAL_GPIO_ReadPin(GPIOB, 4)==GPIO_PIN_RESET)
//				1;
		}


		retFreq = DAB_PL.fmBandScan(retFreq+5, si468xSeekUp, false);
		retFreq = DAB_PL.fmBandScan(retFreq+5, si468xSeekUp, false);

		retFreq = DAB_PL.fmBandScan(retFreq+5, si468xSeekUp, false);
		retFreq = DAB_PL.fmBandScan(retFreq+5, si468xSeekUp, false);
//		DAB_PL.fmBandScan(10000, si468xSeekUp, false);

#ifdef AM_reception
		//si46xx_set_property(SI46XX_AM_VALID_RSSI_THRESHOLD, 0x0000);
		//si46xx_set_property(SI46XX_AM_VALID_SNR_THRESHOLD, 0x0000);
		//si46xx_set_property(SI46XX_AM_SOFTMUTE_SNR_LIMITS, 0x0000);
		//si46xx_set_property(SI46XX_AM_RDS_CONFIG, 0x0001);
		si46xx_set_property(0x4100, 225);
		//si46xx_set_property(SI46XX_AM_TUNE_FE_CFG, 0x0000);   // switch open
		//si46xx_am_tune_freq(1584, 0);  			// Radio AM Bia�ystok
		//si46xx_am_tune_freq(1062, 0);    		// Radio AM Zamo��
		si46xx_am_tune_freq(756, 0);        		// Radio AM Gi�ycko
		//si46xx_am_tune_freq(225, 0);    		// Polskie Radio Program 1  !!!trzeba zmieni� zakres przeszukiwania w si4689

#endif

#ifdef DAB_reception
	//si46xx_set_property();
		si46xx_set_property(SI46XX_DAB_CTRL_DAB_MUTE_SIGNAL_LEVEL_THRESHOLD, 0);
		si46xx_set_property(SI46XX_DAB_CTRL_DAB_MUTE_SIGLOW_THRESHOLD, 0);
		si46xx_set_property(SI46XX_DAB_CTRL_DAB_MUTE_ENABLE, 0);
		si46xx_set_property(SI46XX_DIGITAL_SERVICE_INT_SOURCE, 1);
		si46xx_set_property(SI46XX_DAB_TUNE_FE_CFG, 0x0001);  // switch closed
		/* si46xx_set_property(SI46XX_DAB_TUNE_FE_CFG,0x0000); // switch open */
		si46xx_set_property(SI46XX_DAB_TUNE_FE_VARM, 10);
		si46xx_set_property(SI46XX_DAB_TUNE_FE_VARB, 10);




		/* STROJENIE STACJI DAB  */
		si468x_dab_set_frequency_list();


		uint8_t valid = 1;
		uint32_t counter;
		char counte_char[20];
		//while (!valid)
		while (valid)
		{
			counter = 0;
		for (int i; i < 40; i++)
			si_rec_buff[i] = 0x00;

		HAL_Delay(500);
		si468x_DAB_tune(0, 0); // 0, bo 178,352 jest w tabeli jako 0 na li�cie

		//resp_ = si468x_read_response(si_rec_buff, 6);
		while (!(si_rec_buff[0] & 0x80))
		{
			resp_ = si468x_read_response(si_rec_buff, 6);
			HAL_Delay(5);
			HAL_UART_Transmit(&huart2, (uint8_t *) "DAB tuning\r\n", 12, 200);
		}
		//HAL_Delay(12000);

		HAL_UART_Transmit(&huart2, (uint8_t *) "DAB tuned\r\n", 11, 200);

		/*		is stcint set?		*/
		si_rec_buff[0] = 0;
		while (!(si_rec_buff[0] & 0x01))
		{
			resp_ = si468x_read_response(si_rec_buff, 6); 		//is stcint set???
			HAL_Delay(10);
			switch ((si_rec_buff[0] & 0x01))
			{
			case 0:

				itoa((int) counter, counte_char);
				//itoa(counte_char, counte_char);
				HAL_UART_Transmit(&huart2, (uint8_t *) counte_char, sizeof(counte_char), 200);
				HAL_UART_Transmit(&huart2, (uint8_t *) " stcint NOT set \r\n", 18, 200);
				break;
			case 1:
				HAL_UART_Transmit(&huart2, (uint8_t *) "stcint set \r\n", 13, 200);
				break;
			}
			counter++;
		}
		/*		is stcint set?		*/

		/*		DAB_DIGRAD_STATUS		*/

		struct dab_digrad_status_t status;
		HAL_Delay(500);
		si468x_DAB_digrad_status();
		HAL_Delay(100);
		resp_ = si468x_read_response(si_rec_buff, 23);

		status.acq = (si_rec_buff[5] & 0x04) ? 1 : 0;
		status.valid = si_rec_buff[5] & 0x01;
		status.rssi = (int8_t)si_rec_buff[6];
		status.snr = (int8_t)si_rec_buff[7];
		status.fic_quality = si_rec_buff[8];
		status.cnr = si_rec_buff[9];
		status.fft_offset = (int8_t) si_rec_buff[17];
		status.frequency = si_rec_buff[12] |
			si_rec_buff[13] << 8 |
			si_rec_buff[14] << 16 |
			si_rec_buff[15] << 24;
		status.tuned_index = si_rec_buff[16];
		status.read_ant_cap = si_rec_buff[18] | si_rec_buff[19] << 8;

		char *stringa[30];
		sprintf(stringa, "%lu \0", status.frequency);
		HAL_UART_Transmit(&huart2, (uint8_t *) "FREQ: ", 6, 200);
		HAL_UART_Transmit(&huart2, (uint8_t *) stringa, 14, 200);
		HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 200);

		char *stringb[30];
		sprintf(stringb, "%x \0", status.rssi);
		HAL_UART_Transmit(&huart2, (uint8_t *) "RSSI: ", 6, 200);
		HAL_UART_Transmit(&huart2, (uint8_t *) stringb, 14, 200);
		HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 200);
		/*		DAB_DIGRAD_STATUS		*/




		/*		is VALID SET		*/
		if (status.valid)
		{
			HAL_UART_Transmit(&huart2, (uint8_t *) "Status!??VALID\r\n", 16, 200);
			valid = 1;
		}
		else
		{
			//HAL_UART_Transmit(&huart2, (uint8_t *) "\033[2J", 4, 200);    //clear screen
			HAL_UART_Transmit(&huart2, (uint8_t *) "Status.??VALID\r\n", 16, 200);
			valid = 0;


			/*DAB_GET_EVENT_STATUS*/
			HAL_UART_Transmit(&huart2, (uint8_t *) "DAB_GET_EVENT_STATUS\r\n", 22, 200);
			si468X_DAB_GET_EVENT_STATUS();
			HAL_Delay(1500);
			resp_ = si468x_read_response(si_rec_buff, 8);




			/*DAB_GET_EVENT_STATUS*/


			/*GET_DIGITAL_SERVICE_LIST*/
			HAL_UART_Transmit(&huart2, (uint8_t *) "GET_DIGITAL_SERVICE_LIST\r\n", 26, 200);
			si468X_GET_DIGITAL_SERVICE_LIST(0);
			HAL_Delay(1500);
			resp_ = si468x_read_response(si_rec_buff, 8);
			HAL_Delay(10);

			/*GET_DIGITAL_SERVICE_LIST*/
		}

		}
		/*		is VALID SET		*/


		/* STROJENIE STACJI DAB  */
#endif




		HAL_UART_Transmit(&huart2, (uint8_t *) "\033[2J", 4, 200);  //clear screen

		HAL_UART_Transmit(&huart2, si_rec_buff, 6, 200);

		HAL_Delay(10);
	//}
	//else
	//{
		HAL_UART_Transmit(&huart2, (uint8_t *) "i2c: Si468 DAB+/FM/AM NOT OK\r\n", 31, 200);
	//}


	HAL_UART_Transmit(&huart2, (uint8_t *) "PROGRAM END\r\n", 13, 200);
	HAL_Delay(500);

  while (1)
  {




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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
1;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

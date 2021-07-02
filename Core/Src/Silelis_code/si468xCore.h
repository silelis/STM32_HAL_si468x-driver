/**
  ******************************************************************************
  * @file    si468xCore.h
  * @author  Dawid "SileliS" Bańkowski	d.bankowski(at)gmail.com
  * @brief   si468x fm/dab+/am radio receiver classes.
  *          This file provides code to manage si468x fm/dab+/am radio receiver.
  *          It is written in C++ and SMT32 HAL.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 Dawid Bańkowski.
  * All rights reserved.</center></h2>
  *
  *For education and private projects:
  * This software component is licensed by GNU Public Licence, * the "License" *;
  * You to copy this scope of file to give informations about copyright to future
  * commercial / open source projects.
  *
  *To use this library for commercial and open source projects please contact with
  *To  author to agree the terms of use.
  ******************************************************************************
*/


#ifndef _SI468X_H_
#define _SI468X_H_

#include "si468x_defines.h"

#include "stm32f4xx.h"


/*---------------------------------------------------*/
//													 //
//			HARDWARE CONFIGURATION					 //
//													 //
//   This section should be configured according     //
//			to hadrware design						 //
//													 //
//													 //
/*---------------------------------------------------*/
//RDS parser
#define useRds				//comment if you dont want to use RDS
#define useAdvRdsParser		//use advanced RDS parser. If not defined You will use simple rds_parser function
//RDS parser

//SET it according to Your hardware design
//reset pin:
#define resetLineLOW	GPIO_PIN_SET					//set it depend on Your reset line hardware design, eg.: direst pin to pin, NPN, PNP, Logic level shifter, other
#define resetLineHIGH	GPIO_PIN_RESET					//set it depend on Your reset line hardware design, eg.: direst pin to pin, NPN, PNP, Logic level shifter, other
//reset pin:

//SPI NVFLASH firmware addreses:
#define		_Si468x_MODE_mini_patch		0x00000100						//configure according to Your hardware design
#define		_Si468x_MODE_patch_016		0x00002000						//configure according to Your hardware design
#define		_Si468x_MODE_FM				0x00006000 //0x000063EC,		//configure according to Your hardware design
#define		_Si468x_MODE_DAB			0x00086000 //0x00092000,		//configure according to Your hardware design
#define		_Si468x_MODE_AM				0x00106000						//configure according to Your hardware design
//SPI NVFLASH firmware addreses:
//SET it according to Your hardware design
/*---------------------------------------------------*/
//													 //
//			HARDWARE CONFIGURATION					 //
//													 //
//   This section should be configured according     //
//			to hadrware design						 //
//													 //
//													 //
/*---------------------------------------------------*/

//do not change this section
#ifdef useRds
	#ifdef useAdvRdsParser
		#include "RDSDecoder.h"
		#warning "RDSDecoder is not implemented with all features"		//todo: I have to learn this class more or find more user friendly
	#endif

	#ifndef useAdvRdsParser
		struct fm_rds_data_t{
			uint8_t sync;
			uint16_t pi;
			uint8_t pty;
			char ps_name[9];
			char radiotext[129];
			uint16_t group_0a_flags;
			uint32_t group_2a_flags;
		};
	#endif
#endif
//do not change this section

//dynamically allocated data structure
struct si468xComStruct{
	uint16_t size=0;				//allocated data size
	uint8_t *data=nullptr;			//pointer to allocated data
};

//for more details please take a look at Si468X PROGRAMMING GUIDE AN649
//GET_PART_INFO command response
struct si468xPartInfo{
	uint8_t chipRev;
	uint8_t romID;
	uint16_t partNo;
};

//for more details please take a look at Si468X PROGRAMMING GUIDE AN649
//POWER_UP command response
struct si468xPowerUpArgs{
	uint8_t	CLK_MODE;
	uint8_t	TR_SIZE;
	uint8_t	IBIAS;
	uint32_t XTAL_FREQ;
	uint8_t	CTUN;
};

//for more details please take a look at Si468X PROGRAMMING GUIDE AN649
//GET_FUNC_INFO command response
struct si468xFuncInfo{
	uint8_t	REVEXT;
	uint8_t	REVBRANCH;
	uint8_t	REVINT;
	bool NOSVN;
	uint8_t	LOCATION;
	bool	MIXEDREV;
	bool	LOCALMOD;
	uint32_t	SVNID;

};

//for more details please take a look at Si468X PROGRAMMING GUIDE AN649
//GET_AGC_STATUS command response
struct si468xAGCStatus{
	uint8_t VHFLNA;
	uint8_t	VHFCATT;
	uint16_t VHFRATT;
	uint8_t	RFINDEX;
};

//enumerate with values according to IMAGE at GET_SYS_STATE command response
enum si468xBootedFirmware{
	//This field indicates which firmware image processed this command.
	Bootloader_active			=0,		//0
	FMHD_active					=1,		//1
	DAB_active					=2,		//2
	TDMB_or_dataOnlyDAB_active	=3,		//3
	FMHD_Demod_active			=4,		//4
	AMHD_active					=5,		//5
	AMHD_Demod_active			=6,		//6
	DAB_Demod_active			=7,		//7
	RESERVED					=16,	//16
	si468xBootERROR				=0xFF
};


/*
 * si468xCore  is class to manage hardware.
 * This is parent class for si468xDAB, si468xFM, si468xAM.
 *
 */
class si468xCore {
public:
	si468xCore(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress);
	//si468xCore(GPIO_TypeDef* _resetPort, uint16_t _resetPin, SPI_HandleTypeDef *_hspi, GPIO_TypeDef* _spiEnablePort, uint16_t _spiEnablePin);	//todo: napisac obsługę po SPI
	~si468xCore();
	si468xBootedFirmware getSystemState();
	si468xPartInfo getPartInfo();
	si468xPowerUpArgs getPowerUpArgs();
	si468xFuncInfo getFuncInfo();
	void setProperty(uint16_t propertyId, uint16_t propertyVal);
	uint16_t getProperty(uint8_t count, uint16_t propertyID);
	si468xAGCStatus getAGCstatus();
	uint16_t testGetRSSI();
	void wait4STCint(void);

protected:
	si468xBootedFirmware init(uint32_t imageAddress);
	void reset_si468x();
	bool build_CommandArray(uint8_t command_id, uint8_t *args, uint16_t num_args, uint8_t *data = nullptr, uint16_t data_size=0);
	bool build_RespondArray(uint16_t data_size);
	void free_CommandArray();
	void free_RespondArray();
	uint8_t read_response(uint16_t sizeOfResp);								//todo: spi communication
	bool readDataFromsi468xDynamic_1stBatch(uint16_t _1stBatchSize);		//todo: spi communication
	bool readDataFromsi468xDynamic_2ndBatch(uint16_t _2ndBatchSize);		//todo: spi communication
	void execute_Command(uint8_t command_id, uint8_t *args, uint16_t num_args, uint8_t *data = nullptr, uint16_t data_size=0);
	void wait4ready(uint32_t delay=0);
	void wait4interrupt(volatile uint8_t* intType, uint8_t bitPosition);
	void setInterruptAsHandled(volatile uint8_t* intType, uint8_t bitPosition);
	void setInterruptAsHandled_STC(void);

	struct si468xComunicationArrays{
		si468xComStruct Command, Respond;
	}ComunicationArrays;

	struct si468xStatuses{
		si468xBootedFirmware UploadedImage = RESERVED;
		union
			{
				struct
				{
					volatile uint8_t STCINT	: 1;
					volatile uint8_t ACFINT	: 1;
					volatile uint8_t RDSINT	: 1;
					volatile uint8_t RSQINT	: 1;
					volatile uint8_t DSRVINT	: 1;
					volatile uint8_t DACQINT	: 1;
					volatile uint8_t ERR_CMD : 1;
					volatile uint8_t CTS		: 1;
				};
				volatile uint8_t STATUS0;
			} uStatus0;

		union
		{
				struct
				{
					volatile uint8_t DACFINT:1;
					volatile uint8_t notAssigned1 :4;
					volatile uint8_t DEVNTINT :1;
					volatile uint8_t notAssigned2 :2;
				};
				volatile uint8_t STATUS1;
		}uStatus1;

		union
		{
			volatile uint8_t STATUS3;
		}uStatus3;

	}DeviceState;

private:
	void _constructorCoreCommon(GPIO_TypeDef* _resetPort, uint16_t _resetPin);
	void resetLOW();
	void resetHIGH();
	//void CS_high();
	//void CS_low();
	bool build_Array(si468xComStruct* array, uint16_t arraySize);
	void free_Array(si468xComStruct array);
//protected:
	bool writeData2si468x(uint8_t *pData, uint16_t Size);			//todo: spi communication
	bool readDataFromsi468x(uint16_t sizeOfResp);					//todo: spi communication
//private:
	void _power_up();				//todo:  dzialanie piny INT
	void _load_init();
	void _host_load_minipatch();
	void _load_NVspiFlashROM(uint32_t flashAddress);
	void _boot(void);

	friend HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
	friend HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
	friend HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
	friend HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
//protected:
	struct si468xHardware{
		 struct{
			 GPIO_TypeDef* si468xResetPort= nullptr;
			 uint16_t si468xResetPin= NULL;
		 }ResetPin;

		//I2C communication
			struct{
				I2C_HandleTypeDef* si468x_hi2c = nullptr;
				uint16_t si468x_DevAddress = 0;
			 }I2CStruct;
		//I2C communication

		 //SPI communication
			 struct{													//TODO: W moim przypadku spi nie działało - do poprawy
				 SPI_HandleTypeDef* si468x_hspi  = nullptr;
				 GPIO_TypeDef* si468xSpiEnablePort= nullptr;
				 uint16_t si468xSpiEnablePin= NULL;
			 }SPIStruct;												//TODO: W moim przypadku spi nie działało - do poprawy
			 //SPI communication
	}Hardware;
};

#endif /* SRC_SILELIS_CODE_SI468X_H_ */

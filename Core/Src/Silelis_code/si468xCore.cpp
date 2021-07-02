/*
 * si468x.cpp
 *
 *  Created on: Dec 21, 2020
 *      Author: dbank
 */

#include "string.h"
#include "si468xCore.h"


/**
  * @brief	Hardware constructor with i2c communication
  * @note
  *
  * @param 	_resetPort	- STM32 reset pin port address
  * 		_resetPin	- STM32 reset pin number
  * 		*_hi2c		- pointer to STM32 i2c bus where
  * 					si468x is connected
  * 		_DevAddress	- si468x i2c bus address
  * @retval n/a
  */
si468xCore::si468xCore(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress) {
	Hardware.I2CStruct.si468x_DevAddress	=	_DevAddress;
	Hardware.I2CStruct.si468x_hi2c			=	_hi2c;
	_constructorCoreCommon(_resetPort, _resetPin);
}

/**
  * @brief	Set values required to manage reset pin
  * @note	Private function which required to construct
  * 		object. It is outside of constructor because
  * 		is also required by SPI constructor.
  *
  * @param 	_resetPort	- STM32 reset pin port address
  * 		_resetPin	- STM32 reset pin number
  * @retval n/a
  */
void si468xCore::_constructorCoreCommon(GPIO_TypeDef* _resetPort, uint16_t _resetPin){
	this->Hardware.ResetPin.si468xResetPort = _resetPort;
	Hardware.ResetPin.si468xResetPin	=	_resetPin;
	resetLOW();

	reset_si468x();
	free_CommandArray();
	free_RespondArray();
}
																																						//TODO: W moim przypadku spi nie działało - do poprawy
/**
  * @brief	Hardware destructor
  * @note	Clear all object data and destroy object.
  *
  * @param 	n/a
  * @retval n/a
*/
si468xCore::~si468xCore() {

	//clear I2C and SPI structures
	if (Hardware.I2CStruct.si468x_hi2c != nullptr)
	{
		Hardware.I2CStruct.si468x_hi2c			= nullptr;
		Hardware.I2CStruct.si468x_DevAddress	= 0;
	}
//	else if (Hardware.SPIStruct.si468x_hspi  != nullptr)			//TODO: W moim przypadku spi nie działało - do poprawy
//	{
//		CS_high();
//		Hardware.SPIStruct.si468x_hspi			= nullptr;
//		Hardware.SPIStruct.si468xSpiEnablePort	= nullptr;
//		Hardware.SPIStruct.si468xSpiEnablePin	= 0;
//	}																//TODO: W moim przypadku spi nie działało - do poprawy

	// reset pin line
	resetLOW();
	Hardware.ResetPin.si468xResetPort= nullptr;
	Hardware.ResetPin.si468xResetPin= NULL;
	//clead comunication buffers
	free_CommandArray();
	free_RespondArray();
}


/**
  * @brief	Wait till si468x is ready to next command
  * @note	if "delay=0" than wait till CTS bit is set.
  * 		if "delay!=x" wait specified period of time
  *
  * @param 	delay - delay time in ms
  * @retval n/a
*/
void si468xCore::wait4ready(uint32_t delay){
	if (delay==0)
	{
		do{
			read_response(6);
		}
		while((DeviceState.uStatus0.CTS !=1)&&(DeviceState.uStatus0.ERR_CMD!=1));
	}
	else
	{

		#ifdef INC_FREERTOS_H
			#error  TODO: FreeRTOS OS_delay
		#else
			HAL_Delay(delay);
		#endif
	}
}

/**
  * @brief	Wait for si468x information about interrupt
  * @note	Check if software interrupt bit is set.
  *
  * @param 	*intType	- pointer to DeviceState.uStatusX (STATUS0, STATUS1) where
  * 					information about si468x interrupts required
  * 					to manage is stored
  * 		bitPosition	- interrupt bit position (which value should be checked)
  * 					 for more details pls take a look at RD_REPLY command response
  * @retval n/a
*/
void si468xCore::wait4interrupt(volatile uint8_t* intType, uint8_t bitPosition){
	do{
		read_response(4);
	}while(((*intType)&(0b1<<bitPosition))!=1);
}


/**
  * @brief	Clear interrupt bit at software site
  * @note	Clear interrupt bit in STM32 not in su468x.
  *
  * @param 	*intType	- pointer to DeviceState.uStatusX (STATUS0, STATUS1) where
  * 					information about si468x interrupts required
  * 					to manage is stored
  * 		bitPosition	- interrupt bit position (which value should be checked)
  * 					 for more details pls take a look at RD_REPLY command response
  * @retval n/a
*/
void si468xCore::setInterruptAsHandled(volatile uint8_t* intType, uint8_t bitPosition){
	*intType &=~(1U<<bitPosition);
}

/**
  * @brief	Clear STC interrupt bit at software site
  * @note	Clear STC interrupt bit in STM32 not in su468x.
  *
  * @param 	n/a
  * @retval n/a
*/
void si468xCore::setInterruptAsHandled_STC(void){
	setInterruptAsHandled(&DeviceState.uStatus0.STATUS0, 0);
}


/**
  * @brief	Wait for si468x information about STC interrupt
  * @note	Check if software interrupt bit is set.
  *
  * @param 	n/a
  * @retval n/a
*/
void si468xCore::wait4STCint(void){
	wait4interrupt(&DeviceState.uStatus0.STATUS0, 0);
}


/**
  * @brief	Set reset pin to low 0V
  * @note	resetLineLOW / resetLineHIGH should be defined
  * 		(in si468xCore.h) according to Your hardware design
  * 		(i.e. if You use Bss138, npn, pnp, etc).
  *
  * @param 	n/a
  * @retval n/a
*/
void si468xCore::resetLOW(){
	HAL_GPIO_WritePin(Hardware.ResetPin.si468xResetPort, Hardware.ResetPin.si468xResetPin, resetLineLOW);
	wait4ready(250);
}


/**
  * @brief	Set reset pin to high ~=3V3
  * @note	resetLineLOW / resetLineHIGH should be defined
  * 		(in si468xCore.h) according to Your hardware design
  * 		(i.e. if You use Bss138, npn, pnp, etc).
  *
  * @param 	n/a
  * @retval n/a
*/
void si468xCore::resetHIGH(){
	HAL_GPIO_WritePin(Hardware.ResetPin.si468xResetPort, Hardware.ResetPin.si468xResetPin, resetLineHIGH);
	wait4ready(250);
}



/**
  * @brief	Reset si468x
  * @note	n/a
  *
  * @param 	n/a
  * @retval n/a
*/
void si468xCore::reset_si468x(){
	resetLOW();
	resetHIGH();
}



/**
  * @brief	Build data array required to store command / received data.
  * 		Those arrays are: ComunicationArrays.Command, ComunicationArrays.Respond.
  * @note	Build communication array. Data about array are stored in structure which
  * 		contains pointer and array size.
  *
  * @param 	* array - array pointer which stores array location in RAM
  * 		arraySize - array size
  * @retval bool value depend of results
*/
bool si468xCore::build_Array(si468xComStruct* array, uint16_t arraySize){
	array->size = arraySize;
	array->data = new uint8_t [array->size];
	if (array->data == nullptr)
	{
		array->size = 0;
		return false;
	}
	return true;
}


/**
  * @brief	Free data array required to store command / received data.
  * 		Those arrays are: ComunicationArrays.Command, ComunicationArrays.Respond.
  * @note	To prevent RAM leak every time new data array is build first it is
  * 		required to destroy old array (free RAM space). Also it is executed at
  * 		the object destructor.
  * 		Set array pointer to nullprt.
  *
  * @param 	array - array struct (pointer + data size)
  * @retval n/a
*/
void si468xCore::free_Array(si468xComStruct array){

	if (array.size>0){
		delete [] array.data;
		array.data = nullptr;
		array.size = 0;
	}
}


/**
  * @brief	Build command array which will be sent to si468x.
  * @note	1st destroy old command array.
  * 		2nd build new array
  * 		3rd build command (copy data to array in right order)
  *
  * @param 	command_id - command ID according to si468x datasheet
  * 		*args - pointer to array where list of arguments is stored
  * 		num_args - arguments quantity (size of args array)
  * 		*data - pointer to array where list of extra arguments is stored (i.e. used in properties commands)
  * 		data_size - extra arguments quantity (size of extra args array)
  * @retval bool value depend of results
*/
bool si468xCore::build_CommandArray(uint8_t command_id, uint8_t *args, uint16_t num_args, uint8_t *data, uint16_t data_size){

	if (build_Array(&ComunicationArrays.Command, 1+ num_args+ data_size)==true){
		ComunicationArrays.Command.data[0] = command_id;
		memcpy(ComunicationArrays.Command.data+1, args, num_args);
		memcpy(ComunicationArrays.Command.data+1 + num_args, data, data_size);
		return true;
	}
	return false;
}

/**
  * @brief	Build array where si468x response will be stored.
  * @note	Response array size is different and depends to command. For more
  * 		details pls take a look at si468x datasheet.
  *
  * @param 	data_size - size of data which will be received.
  * @retval bool value depend of results
*/
bool si468xCore::build_RespondArray(uint16_t data_size){
	return build_Array(&ComunicationArrays.Respond, data_size);
}

/**
  * @brief	Free/ destroy data array required to store command data.
  * @note	Those array is: ComunicationArrays.Command.
  *
  * @param 	n/a
  * @retval n/a
*/
void si468xCore::free_CommandArray()
{
	free_Array(ComunicationArrays.Command);
}

/**
  * @brief	Free/ destroy data array required to store response data.
  * @note	Those array is: ComunicationArrays.Respond.
  *
  * @param 	n/a
  * @retval n/a
*/
void si468xCore::free_RespondArray(){
	free_Array(ComunicationArrays.Respond);
}


/**
  * @brief	This command send data to si468x via i2c or spi
  * @note	if Hardware.I2CStruct.si468x_hi2c != nullptr use i2c as communication protocol
  * 		if Hardware.SPIStruct.si468x_hspi  != nullptr use spi as communication protocol
  * 						!!!!but SPI in my case is not implemented yet!!!!
  * 		For i2c interrupt communication is enabled so developer should consider it while set STM32 i2c port.
  * 		DMA / interrupt / blocking mode can be changed in function body
  *
  * @param 	*pData - pointer to array where command is stored (in this class &ComunicationArrays.Command)
  * 		Size - amount of data that should be sent
  * @retval bool value depend of results
*/
bool si468xCore::writeData2si468x(uint8_t *pData, uint16_t Size){
		HAL_StatusTypeDef retval;


//		if (Hardware.SPIStruct.si468x_hspi  != nullptr)							//TODO: W moim przypadku spi nie działało - do poprawy
//		{
//			CS_high();
//			CS_low();
//			retval = HAL_SPI_Transmit(Hardware.SPIStruct.si468x_hspi, pData, Size, 200);
//			CS_high();
//			//SPI BUS COMMUNICATION
//		}
//		else																	//TODO: W moim przypadku spi nie działało - do poprawy
		if (Hardware.I2CStruct.si468x_hi2c != nullptr)
		{
			//I2C BUS COMMUNICATION
			//retval = HAL_I2C_Master_Transmit(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, pData, Size, 200);
			//retval = HAL_I2C_Master_Transmit_DMA(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, pData, Size);		//TODO: not working
			retval = HAL_I2C_Master_Transmit_IT(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, pData, Size);
			while(HAL_I2C_GetState(Hardware.I2CStruct.si468x_hi2c)!=HAL_I2C_STATE_READY);
			//I2C BUS COMMUNICATION
		}
		wait4ready(4);		//4ms delay between write and read is a must!!!
		if (retval==HAL_OK)
			return true;
		else
			return false;
}


/**
  * @brief	This command read data from si468x via i2c or spi
  * @note	if Hardware.I2CStruct.si468x_hi2c != nullptr use i2c as communication protocol
  * 		if Hardware.SPIStruct.si468x_hspi  != nullptr use spi as communication protocol
  * 						!!!!but SPI in my case is not implemented yet!!!!
  * 		For i2c interrupt communication is enabled so developer should consider it while set STM32 i2c port.
  * 		DMA / interrupt / blocking mode can be changed in function body
  *
  * @param 	sizeOfResp - amount of data that should be read
  * @retval bool value depend of results
*/
bool si468xCore::readDataFromsi468x(uint16_t sizeOfResp){
	HAL_StatusTypeDef retval;

//	if (Hardware.SPIStruct.si468x_hspi  != nullptr)								//TODO: W moim przypadku spi nie działało - do poprawy
//	{
//		//SPI BUS COMMUNICATION
//		CS_high();
//		CS_low();
//		retval = HAL_SPI_Receive(Hardware.SPIStruct.si468x_hspi, ComunicationArrays.Respond.data, ComunicationArrays.Respond.size, 200);
//		CS_high();
//		//spiDisabled();
//		//SPI BUS COMMUNICATION
//	}
//	else																		//TODO: W moim przypadku spi nie działało - do poprawy
	if (Hardware.I2CStruct.si468x_hi2c != nullptr)
	{
		//I2C BUS COMMUNICATION
		//retval = HAL_I2C_Master_Receive(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, ComunicationArrays.Respond.data, ComunicationArrays.Respond.size, 200);
		retval = HAL_I2C_Master_Receive_DMA(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, ComunicationArrays.Respond.data, ComunicationArrays.Respond.size);
		//retval = HAL_I2C_Master_Receive_IT(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, ComunicationArrays.Respond.data, ComunicationArrays.Respond.size);
		while(HAL_I2C_GetState(Hardware.I2CStruct.si468x_hi2c)!=HAL_I2C_STATE_READY);
		//I2C BUS COMMUNICATION
	}

	if (retval==HAL_OK)
		return true;
	else
		return false;
}

/**
  * @brief	This command read data from si468x (1st batch) via i2c or spi in 2 parts because some
  * 		command answer size is not fixed. Usually 1st batch of data ends with
  * 		uint16_t variable which is a lenght/ or total lenght of answer array.
  * 		As usually received data is stored in ComunicationArrays.Respond
  * @note	if Hardware.I2CStruct.si468x_hi2c != nullptr use i2c as communication protocol
  * 		if Hardware.SPIStruct.si468x_hspi  != nullptr use spi as communication protocol
  * 						!!!!but SPI in my case is not implemented yet!!!!
  * 		For i2c interrupt communication is enabled so developer should consider it while set STM32 i2c port.
  * 		DMA / interrupt / blocking mode can be changed in function body
  *
  * @param 	_1stBatchSize - amount of data that should be read to receive information about whole respond size.
  * @retval bool value depend of results
*/
bool si468xCore::readDataFromsi468xDynamic_1stBatch(uint16_t _1stBatchSize){
	HAL_StatusTypeDef retval = HAL_ERROR;
	free_RespondArray();
	if (build_RespondArray(_1stBatchSize)== true)
	{
		writeData2si468x(RD_REPLY, 1);
		/*	if (Hardware.SPIStruct.si468x_hspi  != nullptr)								//TODO: W moim przypadku spi nie działało - do poprawy
			{
				//SPI BUS COMMUNICATION
				while(1);			//todo: better no SPI error detection
				//SPI BUS COMMUNICATION
			}
			else*/																		//TODO: W moim przypadku spi nie działało - do poprawy
			if (Hardware.I2CStruct.si468x_hi2c != nullptr)
			{
				//I2C BUS COMMUNICATION
				HAL_I2C_Master_Seq_Receive_DMA(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, ComunicationArrays.Respond.data, _1stBatchSize, I2C_NEXT_FRAME);
				while(HAL_I2C_GetState(Hardware.I2CStruct.si468x_hi2c)!=HAL_I2C_STATE_READY);
				//I2C BUS COMMUNICATION
			}
	}
	if (retval==HAL_OK)
		return true;
	else
		return false;
}

/**
  * @brief	This command read data from si468x (2nd batch) via i2c or spi in 2 parts because some
  * 		command answer size is not fixed. Usually 1st batch of data ends with
  * 		uint16_t variable which is a lenght/ or total lenght of answer array.
  * 		As usually received data is stored in ComunicationArrays.Respond
  * @note	if Hardware.I2CStruct.si468x_hi2c != nullptr use i2c as communication protocol
  * 		if Hardware.SPIStruct.si468x_hspi  != nullptr use spi as communication protocol
  * 						!!!!but SPI in my case is not implemented yet!!!!
  * 		For i2c interrupt communication is enabled so developer should consider it while set STM32 i2c port.
  * 		DMA / interrupt / blocking mode can be changed in function body
  *
  * @param 	_2ndBatchSize - amount of data that should be read to receive whole response data.
  * @retval bool value depend of results
*/
bool si468xCore::readDataFromsi468xDynamic_2ndBatch(uint16_t _2ndBatchSize){
	HAL_StatusTypeDef retval = HAL_ERROR;

	free_CommandArray();
	if ((build_Array(&ComunicationArrays.Command,ComunicationArrays.Respond.size))==true)
	{
		ComunicationArrays.Command.size=ComunicationArrays.Respond.size;
		memcpy(ComunicationArrays.Command.data, ComunicationArrays.Respond.data, ComunicationArrays.Respond.size);		//temporary copy data to Command array
		free_RespondArray();

		if (build_RespondArray(ComunicationArrays.Command.size+ _2ndBatchSize)== true)
		{
			memcpy(ComunicationArrays.Respond.data, ComunicationArrays.Command.data, ComunicationArrays.Command.size);

			/*	if (Hardware.SPIStruct.si468x_hspi  != nullptr)								//TODO: W moim przypadku spi nie działało - do poprawy
				{
					//SPI BUS COMMUNICATION
					while(1);			//todo: better no SPI error detection
					//SPI BUS COMMUNICATION
				}
				else*/																		//TODO: W moim przypadku spi nie działało - do poprawy
				if (Hardware.I2CStruct.si468x_hi2c != nullptr)
				{
					//I2C BUS COMMUNICATION
					HAL_I2C_Master_Seq_Receive_DMA(Hardware.I2CStruct.si468x_hi2c, Hardware.I2CStruct.si468x_DevAddress, ComunicationArrays.Respond.data+ComunicationArrays.Command.size, _2ndBatchSize, I2C_LAST_FRAME);
					while(HAL_I2C_GetState(Hardware.I2CStruct.si468x_hi2c)!=HAL_I2C_STATE_READY);
					//I2C BUS COMMUNICATION
				}
		}
	}
	if (retval==HAL_OK)
		return true;
	else
		return false;
}

/**
  * @brief	This command read data from si468x via i2c or spi when command answer size is fixed.
  * 		As usually received data is stored in ComunicationArrays.Respond
  * @note	if Hardware.I2CStruct.si468x_hi2c != nullptr use i2c as communication protocol
  * 		if Hardware.SPIStruct.si468x_hspi  != nullptr use spi as communication protocol
  * 						!!!!but SPI in my case is not implemented yet!!!!
  * 		For i2c interrupt communication is enabled so developer should consider it while set STM32 i2c port.
  * 		DMA / interrupt / blocking mode can be changed in function body
  *
  * @param 	sizeOfResp - amount of data that should be read to received.
  * @retval bool value depend of results
*/
uint8_t si468xCore::read_response(uint16_t sizeOfResp){
	free_RespondArray();

	if (build_RespondArray(sizeOfResp)== true)
	{
		writeData2si468x(RD_REPLY, 1);
		readDataFromsi468x(sizeOfResp);
		DeviceState.uStatus0.STATUS0 = ComunicationArrays.Respond.data[0];
		DeviceState.uStatus1.STATUS1 = ComunicationArrays.Respond.data[1]& 0b00100001;
		DeviceState.uStatus3.STATUS3 = ComunicationArrays.Respond.data[3];

		//RD_REPLY for those modes is little different
		switch(DeviceState.UploadedImage)
		{
			//case FMHD_active:
			//	break;
			case DAB_active:
			case DAB_Demod_active:
				DeviceState.uStatus0.STATUS0 = DeviceState.uStatus0.STATUS0& 0b11110001;
				break;
			case AMHD_active:
			case AMHD_Demod_active:
				DeviceState.uStatus0.STATUS0 = DeviceState.uStatus0.STATUS0& 0b11111011;
				//DeviceState.uStatus3.STATUS3 = DeviceState.uStatus3.STATUS3& 0b11011111;		//have different ingidients in different commands
		};

		return DeviceState.uStatus0.ERR_CMD;	//Respond.data[0] & 0x40 ? 1 : 0;		//return 0 if no error 1 if there is error
	}
	else
		while(1);							//infinit loop if table allocation error
											//todo: better error handling
}

/**
  * @brief	This command build command and send it to si468x.
  * @note	n/a
  *
  * @param 	command_id - command ID according to si468x datasheet
  * 		*args - pointer to array where list of arguments is stored
  * 		num_args - arguments quantity (size of args array)
  * 		*data - pointer to array where list of extra arguments is stored (i.e. used in properties commands)
  * 		data_size - extra arguments quantity (size of extra args array)
  * @retval bool value depend of results
*/
void si468xCore::execute_Command(uint8_t command_id, uint8_t *args, uint16_t num_args, uint8_t *data, uint16_t data_size){
	free_CommandArray();

	if (build_CommandArray(command_id, args, num_args, data, data_size)==true)
	{
		writeData2si468x(ComunicationArrays.Command.data, ComunicationArrays.Command.size);
	}
	else
		while(1);							//infinit loop if table allocation error
											//todo: better error handling
}

/**
  * @brief	si468x POWER_UP command
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param n/a
  * @retval n/a
*/
void si468xCore::_power_up(){

	//TODO: działanie zależne od tego czy INTERRUPT PIN JEST ZDEFINIOWANY
	uint8_t args[] = {
		//POWER_UP,
		0x80,   							//ARG1	- CTSIEN 0 : Disable toggling host interrupt line.
											//				 1 : Enable toggling host interrupt line.
		0b010111,//(1 << 4) | (7 << 0),   	//ARG2 	CLK_MODE -	1 : Reference clock generator is in crystal mode.
											//		TR_SIZE  -	7: 19.2 MHz, for more details take a look at "TR_SIZE and IBIAS settings" section on Si468X PROGRAMMING GUIDE (an649)
		0x48,   							//ARG3 	IBIAS=0x28  400uA,		 for more details take a look at "TR_SIZE and IBIAS settings" section on Si468X PROGRAMMING GUIDE (an649)
		0x00,   							//ARG4 	XTAL	19 200 000 in hex is: 0x00	01	24	F8	__00__
		0xF8,   							//ARG5	XTAL	19 200 000 in hex is: 0x00	01	24	__F8__	00
		0x24,   							//ARG6 	XTAL	19 200 000 in hex is: 0x00	01	__24__	F8	00
		0x01,   							//ARG7 	XTAL 	19 200 000 in hex is: 0x00	__01__	24	F8	00
		0x34,    							//ARG8	CTUN - This parameter is only required if using the crystal oscillator, for more details take a look at
											//										"TR_SIZE and IBIAS settings" section on Si468X PROGRAMMING GUIDE (an649)
		0x00 | (1 << 4),   					//ARG9
		0x00,   							//ARG10
		0x00,   							//ARG11
		0x00,   							//ARG12
		0x00,   							//ARG13 IBIAS_RUN
		0x00,   							//ARG14
		0x00  								//ARG15
	};
	execute_Command(POWER_UP, args, 15);
}

/**
  * @brief	si468x LOAD_INIT command
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param n/a
  * @retval n/a
*/
void si468xCore::_load_init(){
	execute_Command(LOAD_INIT, 0, 1);
}


/**
  * @brief	si468x HOST_LOAD minipath to si468x.
  * @note	In ths particular function minipath is stored in STM32 flashmemory.
  * 		For more details pls take a look at si468x datasheet.
  *
  * @param n/a
  * @retval n/a
*/
void si468xCore::_host_load_minipatch()	{
	uint8_t args[] = {
		0x0,
		0x0,
		0x0
	};

	const uint8_t miniPath[940] = {
	0x03, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x5F, 0xBE, 0x2F, 0x01, 0x00, 0x00, 0x00,
	0xEF, 0xBE, 0xAD, 0xDE, 0x50, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x20, 0x50, 0x01, 0x00, 0x00, 0x00, 0x6A, 0x21, 0xD6, 0x4F,
	0x89, 0xA5, 0x4C, 0xD3, 0xD4, 0x6E, 0x92, 0x9F, 0xA9, 0x31, 0x42, 0x2E, 0x20, 0x0F, 0xE2, 0x2B,
	0x7F, 0x72, 0xC2, 0xD7, 0xF0, 0x52, 0x7E, 0xD1, 0x99, 0x89, 0x86, 0x4F, 0x55, 0xE7, 0x6D, 0xDE,
	0x98, 0xFF, 0xD8, 0xCC, 0x71, 0xCF, 0xD2, 0x78, 0x62, 0x58, 0xA4, 0x01, 0xA7, 0x34, 0x6D, 0x73,
	0x91, 0x31, 0xFA, 0x15, 0x50, 0x22, 0x9B, 0x78, 0xB5, 0x49, 0x2A, 0xD1, 0x5A, 0xDE, 0x36, 0x53,
	0x85, 0xDD, 0x6D, 0x46, 0xB8, 0x06, 0xDE, 0xEC, 0x8F, 0x25, 0xDC, 0x6A, 0x16, 0x4B, 0xD8, 0x66,
	0x3D, 0x9A, 0xA2, 0x8D, 0x68, 0xED, 0x67, 0xCB, 0xD1, 0x12, 0xAE, 0x75, 0x85, 0x44, 0x5C, 0x9B,
	0x83, 0xC8, 0xD6, 0x37, 0x96, 0x92, 0x85, 0x9F, 0x12, 0xB3, 0x7D, 0x4F, 0x7E, 0x59, 0xCB, 0x7D,
	0x0B, 0x99, 0x97, 0x7B, 0xC7, 0x60, 0x50, 0x26, 0x10, 0xFE, 0xC1, 0xF3, 0xE0, 0xD1, 0xE0, 0x16,
	0xD3, 0x0D, 0xA8, 0x1C, 0xA6, 0xF7, 0x58, 0x39, 0x4F, 0x42, 0xD3, 0x23, 0xBF, 0x6A, 0xC1, 0x14,
	0xAD, 0x7B, 0x80, 0x01, 0x1B, 0xCB, 0x01, 0xE3, 0x45, 0xEA, 0x62, 0x36, 0x8A, 0x06, 0x04, 0xF5,
	0x97, 0xCD, 0x95, 0x84, 0xE1, 0x56, 0xB9, 0x29, 0xD4, 0xC1, 0x8E, 0x2E, 0xEC, 0x93, 0x1D, 0xF9,
	0x63, 0xF4, 0x58, 0xE1, 0xC6, 0x13, 0xB0, 0x22, 0x3E, 0x79, 0x02, 0x54, 0xA2, 0x2E, 0x09, 0xC8,
	0xA4, 0x7E, 0xD2, 0xD0, 0xAA, 0xEE, 0x27, 0xA0, 0x97, 0x32, 0x4C, 0xE0, 0xA6, 0xE9, 0xFE, 0x3F,
	0xB2, 0xAD, 0x93, 0x2C, 0x89, 0x52, 0x58, 0x78, 0x82, 0x89, 0xB1, 0xF0, 0xD4, 0x7E, 0x60, 0x01,
	0x7A, 0xBC, 0xC1, 0x62, 0xE5, 0x54, 0xE4, 0x32, 0x88, 0xC5, 0xCB, 0x05, 0xA1, 0xE7, 0x98, 0x6A,
	0xE2, 0x56, 0x57, 0xF4, 0x5E, 0x47, 0xAF, 0xC8, 0x8A, 0xB3, 0x1E, 0xD0, 0x30, 0x5E, 0x1D, 0xAE,
	0x0F, 0x2B, 0x19, 0x3D, 0x30, 0x06, 0x13, 0xD8, 0x0E, 0xD4, 0xBE, 0x8B, 0xE2, 0x25, 0x1E, 0x12,
	0xF5, 0xA7, 0xBE, 0xCA, 0x1A, 0x29, 0x8E, 0x07, 0xA1, 0xD0, 0x09, 0x10, 0x45, 0xA2, 0xB7, 0x06,
	0x78, 0x23, 0xB1, 0x99, 0x74, 0xDF, 0x11, 0x25, 0xBC, 0xE5, 0xE0, 0x17, 0x6C, 0x4F, 0x3A, 0x2D,
	0x7A, 0xF7, 0x56, 0x9B, 0xD6, 0x9D, 0xAA, 0x24, 0xDB, 0x5B, 0xBD, 0xCD, 0xD2, 0xDF, 0x58, 0x6C,
	0x78, 0xA2, 0x06, 0x1F, 0xC1, 0x18, 0xA8, 0x2C, 0x26, 0xC6, 0x77, 0x16, 0x01, 0x8B, 0x3E, 0x7A,
	0x44, 0x4A, 0xDD, 0x06, 0xB0, 0x0E, 0xFB, 0x3D, 0xBB, 0x74, 0x39, 0xCD, 0x57, 0xBB, 0xFE, 0xE3,
	0x36, 0x6D, 0x24, 0x95, 0x59, 0x6B, 0x6F, 0x93, 0xFA, 0x38, 0xF0, 0xC9, 0xD9, 0xE4, 0xC4, 0xE1,
	0x02, 0x81, 0xA3, 0xBD, 0x18, 0x72, 0x49, 0xF2, 0x88, 0x09, 0xF1, 0x5A, 0x59, 0x3D, 0x75, 0xCA,
	0xA6, 0x15, 0xB0, 0x5E, 0x7A, 0x7C, 0xE6, 0x87, 0x9C, 0x27, 0xA7, 0x1F, 0x10, 0x4D, 0x68, 0x8C,
	0x4D, 0xD7, 0x7C, 0x38, 0x68, 0xA9, 0x63, 0xC4, 0xDA, 0xA9, 0x89, 0x7D, 0xA2, 0xE4, 0xED, 0x21,
	0x0F, 0x82, 0x9C, 0x4E, 0xD9, 0x4B, 0x9F, 0xD0, 0x61, 0x36, 0x94, 0x2E, 0x61, 0xD2, 0x1A, 0x6A,
	0x31, 0x3B, 0x2B, 0x7B, 0x14, 0xB5, 0x4C, 0x3D, 0x89, 0x8E, 0x7B, 0xA7, 0x3D, 0xD8, 0x18, 0x65,
	0x7D, 0xBD, 0xEC, 0xD2, 0x3F, 0x00, 0xC1, 0xF1, 0x8C, 0xBD, 0xEE, 0x01, 0xEB, 0x27, 0xF5, 0xDA,
	0x34, 0xAE, 0x15, 0x0E, 0x49, 0xF1, 0x54, 0x9F, 0xD3, 0xCD, 0xBC, 0x9F, 0x11, 0x55, 0x5C, 0xFF,
	0xEA, 0xF9, 0x9A, 0x3A, 0xB3, 0xA3, 0x2B, 0xC2, 0x1C, 0xE2, 0x0B, 0x37, 0xFB, 0x78, 0x27, 0x8E,
	0xB0, 0xE0, 0xE6, 0x6C, 0xA1, 0x29, 0x32, 0x6F, 0xE4, 0x07, 0xD3, 0x09, 0xF6, 0x71, 0xA8, 0x8B,
	0xD6, 0x96, 0x8E, 0xB7, 0x3D, 0xA2, 0x3E, 0x40, 0xD8, 0x13, 0x69, 0x7E, 0xA1, 0xE9, 0xD2, 0x55,
	0xFB, 0x22, 0xBB, 0x1A, 0xBE, 0x53, 0xC5, 0xCE, 0x9B, 0x9C, 0x04, 0x2E, 0xE5, 0x45, 0x72, 0x8A,
	0x80, 0x87, 0x8A, 0xFE, 0xF6, 0x52, 0xC2, 0x6E, 0x4E, 0x0E, 0x62, 0x8B, 0x86, 0xFC, 0x9F, 0x23,
	0x11, 0x44, 0x19, 0xDF, 0x8A, 0xDF, 0x4A, 0x01, 0x17, 0xA8, 0x37, 0xAB, 0xA3, 0x7B, 0xA4, 0x0D,
	0x1E, 0xD8, 0x92, 0xE9, 0x86, 0x3D, 0x22, 0x79, 0xB6, 0x3E, 0xAB, 0x97, 0xF7, 0x8E, 0x32, 0x12,
	0xDA, 0x4E, 0x8B, 0x3F, 0x21, 0x80, 0x62, 0x1F, 0x30, 0x16, 0x2B, 0x9E, 0x08, 0xBB, 0xE1, 0x7B,
	0x32, 0xC6, 0x01, 0x8E, 0x8C, 0x04, 0x22, 0xCF, 0xDA, 0x2A, 0xF5, 0xD9, 0x9C, 0xE5, 0x48, 0x5B,
	0xFD, 0x6F, 0xDE, 0x72, 0xB0, 0xFF, 0x6D, 0x53, 0x46, 0x4F, 0x19, 0xDF, 0x1B, 0x3D, 0x11, 0xEB,
	0xFB, 0x78, 0xFD, 0x9E, 0xC0, 0x41, 0x58, 0xF6, 0xD7, 0xE0, 0xF4, 0xAE, 0xD7, 0xEE, 0x38, 0xCF,
	0xC9, 0x02, 0x53, 0x53, 0x72, 0xDE, 0xE1, 0xF5, 0x0D, 0x91, 0x72, 0xA3, 0x57, 0xB5, 0x8C, 0xD3,
	0x5C, 0x84, 0x57, 0x52, 0xA3, 0x61, 0x3A, 0xFB, 0x55, 0x22, 0x13, 0x0F, 0x5F, 0x20, 0x72, 0xDC,
	0xD6, 0x6C, 0x33, 0x32, 0xB8, 0x66, 0x6B, 0x7D, 0x5F, 0x84, 0x91, 0x04, 0x23, 0x75, 0x5D, 0xA1,
	0xB9, 0xDC, 0xD6, 0x44, 0x33, 0xC7, 0xD0, 0x18, 0x7E, 0x3C, 0x6F, 0x3F, 0x5D, 0x7F, 0xE0, 0xCF,
	0xD0, 0x88, 0xBE, 0xF1, 0x87, 0x65, 0x65, 0xDA, 0xCC, 0xD2, 0x75, 0x5F, 0x8F, 0xF7, 0xB3, 0x08,
	0x68, 0xF7, 0xDE, 0xFE, 0xE2, 0x5F, 0x95, 0xA2, 0x92, 0xF6, 0x41, 0xA7, 0x45, 0xF6, 0x34, 0x20,
	0xE5, 0x5F, 0x46, 0x31, 0xDD, 0xD5, 0xDA, 0xB1, 0x5E, 0xB6, 0x06, 0x34, 0xD9, 0x22, 0xF3, 0x95,
	0xC3, 0xD1, 0xBC, 0xCF, 0xAB, 0x83, 0x6E, 0x5D, 0x38, 0x3A, 0xC8, 0xBD, 0xE8, 0x11, 0xAF, 0xC9,
	0x01, 0x0C, 0x20, 0x7B, 0x18, 0xFB, 0x38, 0xBC, 0xFD, 0xE7, 0xCE, 0x96, 0xA2, 0x50, 0xF4, 0x4D,
	0x93, 0x22, 0x93, 0x5B, 0x4E, 0x93, 0xF2, 0x93, 0xC6, 0xBA, 0x45, 0xE2, 0x08, 0xF1, 0xAA, 0x0B,
	0x06, 0x35, 0x84, 0x64, 0x0C, 0x38, 0x1E, 0x9C, 0xE8, 0x68, 0x79, 0x93, 0x11, 0x7D, 0xAB, 0x17,
	0x1A, 0x44, 0x24, 0xD8, 0xB5, 0xDB, 0xE0, 0x0F, 0x9C, 0x51, 0x99, 0x0A, 0x08, 0xDA, 0x90, 0x71,
	0xA5, 0x12, 0xAF, 0x12, 0xF3, 0x3C, 0xBC, 0x8E, 0x17, 0x27, 0x11, 0x16, 0xF8, 0x22, 0x40, 0xFD,
	0xBC, 0x2B, 0xBB, 0x78, 0x25, 0xA7, 0x04, 0xF5, 0x00, 0x00, 0x00, 0x00
	};

	execute_Command(HOST_LOAD, args, 3, (uint8_t*) miniPath, 940);
}

/**
  * @brief	si468x FLASH_LOAD command.
  * @note	Uploads image from nv spi flash connected to si468x
  * 		serial flash interface.
  * 		For more details pls take a look at si468x datasheet.
  *
  * @param	flashAddress - uploaded image begging address in spi flash.
  * @retval n/a
*/
void si468xCore::_load_NVspiFlashROM(uint32_t flashAddress)
{
		uint8_t args[11] = {
		FLASH_LOAD_IMG,
		0x00,
		0x00,							// command prefix
		flashAddress & 0xFF,			// patch/ rom address
		(flashAddress >> 8) & 0xFF,		// patch/ rom address										//todo: sprawdzić czy będzie dobrzxe działać z /*& 0xFF*/
		(flashAddress >> 16) & 0xFF,	// patch/ rom address										//todo: sprawdzić czy będzie dobrzxe działać z /*& 0xFF*/
		flashAddress >> 24,				// patch/ rom address										//todo: sprawdzić czy będzie dobrzxe działać z /*& 0xFF*/
		0x00,							// w manualu programowania "si468x programming guide"
										//			(strona 406: Command 0x05 0x00 – FLASH_LOAD_IMG – (FLASH_LOAD)
										// 			ta komenta nie ma tej i poniższych linijek, ALE JEST TO BLAD
		0x00,
		0x00,
		0x00																					// command post fix
	};

		execute_Command(FLASH_LOAD, args, 11);
}

/**
  * @brief	si468x BOOT command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval n/a
*/
void si468xCore::_boot(void)
{
	execute_Command(BOOT, 0x00, 1);
}

/**
  * @brief	si468x GET_PART_INFO command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval si468xPartInfo - structure which ingredients are
  * 				 similar as in GET_PART_INFO command respond.
*/
si468xPartInfo si468xCore::getPartInfo(){
	execute_Command(GET_PART_INFO, 0x00, 1);
//	wait4ready(5);
	read_response(23);
	si468xPartInfo retVAL;

	retVAL.chipRev=ComunicationArrays.Respond.data[4];
	retVAL.romID =ComunicationArrays.Respond.data[5];
	retVAL.partNo = (uint16_t) (ComunicationArrays.Respond.data[9]<<8) + (uint16_t)ComunicationArrays.Respond.data[8];
	//uint16_t* pPart = (uint16_t*) Respond.data[8];		//same as retVAL.partNo = (uint16_t) (Respond.data[9]<<8) + (uint16_t)Respond.data[8];
	return retVAL;
}

/**
  * @brief	si468x GET_SYS_STATE command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval si468xBootedFirmware - enumerate which points which image had
  * 				been booted.
*/
si468xBootedFirmware si468xCore::getSystemState(){
	execute_Command(GET_SYS_STATE, 0x00, 1);
	read_response(6);
	switch(ComunicationArrays.Respond.data[4]){
		case 0:
			return Bootloader_active;
			break;
		case 1:
			return FMHD_active;
			break;
		case 2:
			return DAB_active;
			break;
		case 3:
			return TDMB_or_dataOnlyDAB_active;
			break;
		case 4:
			return FMHD_Demod_active;
			break;
		case 5:
			return AMHD_active;
			break;
		case 6:
			return AMHD_Demod_active;
			break;
		case 7:
			return DAB_Demod_active;
			break;
		case 17:
			return RESERVED;
			break;
		default:
			return si468xBootERROR;
	}
}

/**
  * @brief	si468x GET_POWER_UP_ARGS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval si468xPowerUpArgs - structure which ingredients are
  * 				 similar as in GET_POWER_UP_ARGS command respond.
*/
si468xPowerUpArgs si468xCore::getPowerUpArgs(){
	execute_Command(GET_POWER_UP_ARGS, 0x00, 1);
	read_response(18);
	si468xPowerUpArgs retVal;
	retVal.CLK_MODE = (ComunicationArrays.Respond.data[6]>>4)&0b11;
	retVal.TR_SIZE	= ComunicationArrays.Respond.data[6]& 0b1111;
	retVal.IBIAS	= ComunicationArrays.Respond.data[7];
	retVal.XTAL_FREQ	= ComunicationArrays.Respond.data[11];
	retVal.XTAL_FREQ	= (retVal.XTAL_FREQ<<8)+ComunicationArrays.Respond.data[10];
	retVal.XTAL_FREQ	= (retVal.XTAL_FREQ<<8)+ComunicationArrays.Respond.data[9];
	retVal.XTAL_FREQ	= (retVal.XTAL_FREQ<<8)+ComunicationArrays.Respond.data[8];
	retVal.CTUN		= ComunicationArrays.Respond.data[12];
	return retVal;
}

/**
  * @brief	si468x GET_FUNC_INFO command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval si468xFuncInfo - structure which ingredients are
  * 				 similar as in GET_FUNC_INFO command respond.
*/
si468xFuncInfo /*void*/ si468xCore::getFuncInfo(){
	execute_Command(GET_FUNC_INFO, 0x00, 1);
	read_response(12);
	si468xFuncInfo retVal;
	retVal.REVEXT 		= ComunicationArrays.Respond.data[4];
	retVal.REVBRANCH	= ComunicationArrays.Respond.data[5];
	retVal.REVINT		= ComunicationArrays.Respond.data[6];
	retVal.NOSVN		= (ComunicationArrays.Respond.data[7]>>7)&0b1;
	retVal.LOCATION		= (ComunicationArrays.Respond.data[7]>>4)&0b11;
	retVal.MIXEDREV		= (ComunicationArrays.Respond.data[7]>>1)&0b1;
	retVal.LOCALMOD		= ComunicationArrays.Respond.data[7]&0b1;
	retVal.SVNID		= ComunicationArrays.Respond.data[11];
	retVal.SVNID		= (retVal.SVNID<<8) +ComunicationArrays.Respond.data[10];
	retVal.SVNID		= (retVal.SVNID<<8) +ComunicationArrays.Respond.data[9];
	retVal.SVNID		= (retVal.SVNID<<8) +ComunicationArrays.Respond.data[8];
	return retVal;
}

/**
  * @brief	si468x SET_PROPERTY command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	propertyId - propertyId according to si468x datasheet
  * 		propertyVal - property value according to si468x datasheet
  * @retval n/a
*/
void si468xCore::setProperty(uint16_t propertyId, uint16_t propertyVal){

	uint8_t propArgs [] ={
		(propertyId & 0xFF),
		(propertyId >> 8) & 0xFF,									//todo: sprawdzić czy będzie dobrzxe działać z /*& 0xFF*/
		propertyVal & 0xFF,
		(propertyVal >> 8) & 0xFF									//todo: sprawdzić czy będzie dobrzxe działać z /*& 0xFF*/
	};

	execute_Command(SET_PROPERTY , 0x00, 1, propArgs, 4);
	read_response(4);
}

/**
  * @brief	si468x loading flash routine.
  * @note	For more details pls take a look at si468x datasheet:
  * 		5. Flowcharts, 5.1. Loading Firmware.
  *
  * @param	imageAddress - uploaded image begging address in spi flash.
  * @retval si468xBootedFirmware - enumerate which points which image had
  * 				been booted.
*/
si468xBootedFirmware si468xCore::init(uint32_t imageAddress){
	wait4ready(3);

	//POWER_UP
	_power_up();
	//DEVICE STATE: BOOTLOADERs
	if (read_response(4)!=0)
		return si468xBootERROR;
	//POWER_UP


	//LOAD_INT
	_load_init();
	read_response(4);
	if (DeviceState.uStatus0.ERR_CMD==1||DeviceState.uStatus0.CTS==0)
		return si468xBootERROR;
	//LOAD_INT

	//LOAD MINIPATH FROM MCU
	_host_load_minipatch();
	wait4ready();	//or wait4ready(11);
	if (DeviceState.uStatus0.ERR_CMD==1||DeviceState.uStatus0.CTS==0)
		return si468xBootERROR;
	//LOAD MINIPATH FROM MCU


	//LOAD_INT
	_load_init();
	read_response(4);
	if (DeviceState.uStatus0.ERR_CMD==1||DeviceState.uStatus0.CTS==0)
		return si468xBootERROR;
	//LOAD_INT


	//images are stored at NVSPI FLASH so for other method You shold modifie this section of init method:

	//load minipath
	_load_NVspiFlashROM(_Si468x_MODE_patch_016);
	wait4ready();
	if (DeviceState.uStatus0.ERR_CMD==1||DeviceState.uStatus0.CTS==0)
		return si468xBootERROR;
	//load minipath


	//set NVspi flash property (max speed)
	//TODO: wylutować spi flash na dobry i w tym scopie wprowadzić set property zmieniające prędkośc odczytu flasha

/*	void si468x_flash_set_property_PROP_FLASH_SPI_CLOCK_FREQ_KHZ()
	{
		uint8_t data[8] =
		{
			FLASH_LOAD, 0x10, 0x00, 0x00,
			PROP_FLASH_SPI_CLOCK_FREQ_KHZ & 0xFF, PROP_FLASH_SPI_CLOCK_FREQ_KHZ >> 8, 0x9C40 & 0xFF, 0x9C40 >>8
		};

		I2C_write(Si4684_ADDRESS, data, 8);
	}*/

	// Set flash speed to 40MHz	//datasheet FLASH_SET_PROP_LIST
	//		si468x_flash_set_property_PROP_FLASH_SPI_CLOCK_FREQ_KHZ();
	//		resp_ = si468x_read_response(si_rec_buff, 6);
	//		HAL_Delay(20);
		// Set flash speed to 40MHz

/*	void si468x_flash_set_property_PROP_HIGH_SPEED_READ_MAX_FREQ_MHZ()
	{
			uint8_t data[8] =
		{
			FLASH_LOAD, 0x10, 0x00, 0x00,
			PROP_HIGH_SPEED_READ_MAX_FREQ_MHZ & 0xFF, PROP_HIGH_SPEED_READ_MAX_FREQ_MHZ >> 8, 0x00FF & 0xFF, 0x00FF >> 8
		};

		I2C_write(Si4684_ADDRESS, data, 8);
	}*/

	// Set flash high speed read speed to 127MHz
	//		si468x_flash_set_property_PROP_HIGH_SPEED_READ_MAX_FREQ_MHZ();
	//		resp_ = si468x_read_response(si_rec_buff, 6);
	//		HAL_Delay(20);
	// Set flash high speed read speed to 127MHz

	//set NVspi flash property (max speed)


	//load final image
	//LOAD_INT
	_load_init();
	read_response(4);
	if (DeviceState.uStatus0.ERR_CMD==1||DeviceState.uStatus0.CTS==0)
		return si468xBootERROR;
	//LOAD_INT

	//load image
	_load_NVspiFlashROM(imageAddress);
	wait4ready();
	if (DeviceState.uStatus0.ERR_CMD==1||DeviceState.uStatus0.CTS==0)
		return si468xBootERROR;
	//load image
	//load final image


	//BOOT
	_boot();
	wait4ready();
	if (DeviceState.uStatus0.ERR_CMD==1||DeviceState.uStatus0.CTS==0)
		DeviceState.UploadedImage = si468xBootERROR;
	else {
		DeviceState.UploadedImage = getSystemState();
	}
	//BOOT

	//SET COMMON PROPERTIES FOR ALL RADIO TYPES
	//THOSE ARE APPROPRIATER TO MINE DESIGN PLEASE CHOOSE YOUR OWN
	setProperty(PROP_INT_CTL_ENABLE, 0b0010000000111111);
	setProperty(PROP_INT_CTL_REPEAT, 0b0010000000111111);
//	setProperty(PROP_DIGITAL_IO_OUTPUT_SELECT, 0x8000); // I2S set master
//	setProperty(PROP_DIGITAL_IO_OUTPUT_SAMPLE_RATE, 0xAC44); // I2S set sample rate 44.1kHz
	setProperty(PROP_PIN_CONFIG_ENABLE, 0x0003); // I2S enable, DACOUTEN enabled, INTB enabled
	setProperty(PROP_DIGITAL_SERVICE_INT_SOURCE, 0x0001); // Enable DSRVPCKTINT

//	setProperty(SI46XX_DIGITAL_IO_OUTPUT_FORMAT,0x1000); // SAMPL_SIZE = 16		//I am not using i2s in my design
//	setProperty(SI46XX_DIGITAL_IO_OUTPUT_SAMPLE_RATE,46877);					//I am not using i2s in my design



	//choose YOUR OWN common properties
	return DeviceState.UploadedImage;
}


/**
  * @brief	si468x GET_PROPERTY command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	count - the number of properties to read,
  * 		propertyID - The id of the property to retrieve.
  * @retval uint16_t - Value of the first property.
*/
uint16_t si468xCore::getProperty(uint8_t count, uint16_t propertyID){

	uint8_t args[3] = {
		count,
		propertyID & 0xFF,
		(propertyID >> 8) & 0xFF,		//todo: sprawdzić czy będzie dobrzxe działać z /*& 0xFF*/
	};

	execute_Command(GET_PROPERTY, args, 3);
	read_response(6);
	return (uint16_t) (ComunicationArrays.Respond.data[5]<<8)+(uint16_t) ComunicationArrays.Respond.data[4];
}


/**
  * @brief	si468x GET_AGC_STATUS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval si468xAGCStatus - structure which ingredients are
  * 				 similar as in GET_AGC_STATUS command respond.
*/
si468xAGCStatus si468xCore::getAGCstatus(){
	execute_Command(GET_AGC_STATUS, 0, 1);
	read_response(23);
	si468xAGCStatus retVal;
	retVal.VHFLNA =	ComunicationArrays.Respond.data[14];
	retVal.VHFCATT = ComunicationArrays.Respond.data[15];
	retVal.VHFRATT= ComunicationArrays.Respond.data[17];
	retVal.VHFRATT = (retVal.VHFRATT<<8) + ComunicationArrays.Respond.data[16];
	retVal.RFINDEX = ComunicationArrays.Respond.data[21];
	return retVal;
}


/**
  * @brief	si468x TEST_GET_RSSI command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval uint16_t - The RSSI measurement in dBuV to 1/256 of
  * 				a dB.
*/
uint16_t si468xCore::testGetRSSI(){
	execute_Command(TEST_GET_RSSI, 0, 1);
	read_response(6);
	return (uint16_t) (ComunicationArrays.Respond.data[5]<<8)+(uint16_t) ComunicationArrays.Respond.data[4];
}

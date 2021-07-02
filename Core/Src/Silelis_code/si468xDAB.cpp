/*
 * si468xDAB.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: dbank
 */

#include "si468xDAB.h"
#include "string.h"

/**
  * @brief	DAB object constructor with i2c communication
  * @note
  *
  * @param 	_resetPort	- STM32 reset pin port address
  * 		_resetPin	- STM32 reset pin number
  * 		*_hi2c		- pointer to STM32 i2c bus where
  * 					si468x is connected
  * 		_DevAddress	- si468x i2c bus address
  * 		imgAddress - uploaded image begging address in spi flash
  * 		*pFreqList - pointer to array (list) which contains DAB frequencies appropriate to geographical region (i.e. Europe is DAB freqs, USA is
  * 		itemsOnFerqList - size of pFreqList (number of frequences)
  * @retval n/a
  */
si468xDAB::si468xDAB(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress, uint32_t imgAddress, uint32_t* pFreqList, uint8_t itemsOnFerqList)
:si468xCore(_resetPort, _resetPin, _hi2c, _DevAddress)
{
	_constructorDABCommon(imgAddress, pFreqList, itemsOnFerqList);
}

//si468xDAB::si468xDAB(GPIO_TypeDef* _resetPort, uint16_t _resetPin, SPI_HandleTypeDef *_hspi, GPIO_TypeDef* _spiEnablePort,
//											uint16_t _spiEnablePin, uint32_t imgAddress, uint32_t* pFreqList, uint8_t itemsOnFerqList)
//:si468xCore(_resetPort, _resetPin, _hspi, _spiEnablePort, _spiEnablePin)
//{
//	_constructorDABCommon(imgAddress, pFreqList, itemsOnFerqList);
//}


/**
  * @brief	Private function part of constructor.
  * @note   Private function used in constructor which can be executed in i2s and spi constructor.
  * 		Auxiliary function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	imgAddress - uploaded image begging address in spi flash
  * 		*pFreqList - pointer to array (list) which contains DAB frequencies appropriate to
  * 				geographical region (i.e. Europe is DAB freqs, USA is
  * 		itemsOnFerqList - size of pFreqList (number of frequences)
  * @retval bool value depend of results
  */
bool si468xDAB::_constructorDABCommon(uint32_t imgAddress, uint32_t* pFreqList, uint8_t itemsOnFerqList){
	init(imgAddress);

		if (DeviceState.UploadedImage != DAB_active){
			DeviceState.UploadedImage = si468xBootERROR;		//TODO: wymyśleć coś lepszego
			return false;
		}
		else{
			_setDABProperties();

			_clearData_DabFreqListOnChip();
			_clearData_RawDigitalServiceList();

			setDabFrequencyList(pFreqList, itemsOnFerqList);
			getDabFrequencyList(); //musi być, bo dzięki tej komendzie DabScan, wie jak szeroki zakres skanowania ma
			//READY to DAB__SCAN
			memset((&dynamicalyAlocatedData.dabServiceListParsed), 0, sizeof(dab_service_list_t));
			dynamicalyAlocatedData.service_label = nullptr;
			return true;
		}
}

/**
  * @brief	DAB object destructor.
  * @note	Clear all object data and destroy object.
  *
  * @param 	n/a
  * @retval n/a
  */
si468xDAB::~si468xDAB() {
	// TODO Auto-generated destructor stub

	_destroyArray_DabFreqListOnChip();
	_destroyArray_RawDigitalServiceList();
	dynamicalyAlocatedData.service_label = nullptr;
}

/**
  * @brief	Private function part of constructor. Sets properties appropriate
  * 		to DAB receiver.
  * 		You can change this function body according to Your hardware design,
  * 		region and requirements.
  * @note   Private function used in constructor which can be executed in i2s
  * 		and spi constructor.
  * 		Auxiliary function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	n/a
  * @retval n/a
  */
void si468xDAB::_setDABProperties(void){
	setProperty(PROP_DAB_ACF_MUTE_SIGLOSS_THRESHOLD, 0);
	setProperty(PROP_DAB_ACF_SOFTMUTE_ATTACK_TIME,0);
	setProperty(PROP_DAB_EVENT_INTERRUPT_SOURCE, 0b11111111);
//	si46xx_dab_set_freq_list(ARRAY_SIZE(frequency_list_nrw),frequency_list_nrw);		//TODO: chceck if otger areas have same frequences
	setProperty(PROP_DAB_XPAD_ENABLE,0);
	setProperty(PROP_DAB_TUNE_FE_CFG,0x0001); // switch closed //,0x0000); // switch open */
	setProperty(PROP_DAB_TUNE_FE_VARM,10);
	setProperty(PROP_DAB_TUNE_FE_VARB,10);
}

/**
  * @brief	Creates "dynamicalyAlocatedData.si468xDabFreqListOnChip" array which is designated to store frequency list
  * 		operated by si468x.
  * @note   Private function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	allocationSize - frequency array size
  * @retval bool value depend of results
  */
bool si468xDAB::_createArray_DabFreqListOnChip(uint8_t allocationSize){
	return createArray((uint32_t**)&dynamicalyAlocatedData.si468xDabFreqListOnChip.pFrequTable, (uint8_t*)&dynamicalyAlocatedData.si468xDabFreqListOnChip.NUM_FREQS, allocationSize);
}

/**
  * @brief	Function set pointer array of "dynamicalyAlocatedData.si468xDabFreqListOnChip"
  * 		to nullptr and array size to 0.
  * @note   Private function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	n/a
  * @retval n/a
  */
void si468xDAB::_clearData_DabFreqListOnChip(void){
	clearData((uint32_t**)&dynamicalyAlocatedData.si468xDabFreqListOnChip.pFrequTable, (uint8_t*)&dynamicalyAlocatedData.si468xDabFreqListOnChip.NUM_FREQS);
}

/**
  * @brief	Destroy "dynamicalyAlocatedData.si468xDabFreqListOnChip" which is
  * 		designated to store frequency list operated by si468x.
  * 		Frees up RAM, sets pointer to nullptr and size to 0.
  * @note   Private function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	n/a
  * @retval n/a
  */
void si468xDAB::_destroyArray_DabFreqListOnChip(void){
	destroyArray((uint32_t**)&dynamicalyAlocatedData.si468xDabFreqListOnChip.pFrequTable, (uint8_t*)&dynamicalyAlocatedData.si468xDabFreqListOnChip.NUM_FREQS);
}

/**
  * @brief	Creates "dynamicalyAlocatedData.si468xDigitalServiceListRAW" array
  * 		which is designated to store raw digital services list received
  * 		from si468x.
  * @note   Private function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	allocationSize - frequency array size
  * @retval bool value depend of results
  */
bool si468xDAB::_createArray_RawDigitalServiceList(uint16_t allocationSize){
	return createArray((uint8_t**)&dynamicalyAlocatedData.si468xDigitalServiceListRAW.pServiceListData, (uint16_t*)&dynamicalyAlocatedData.si468xDigitalServiceListRAW.sizeOfServiceList, allocationSize);
}

/**
  * @brief	Function set pointer array of "dynamicalyAlocatedData.si468xDigitalServiceListRAW"
  * 		to nullptr and array size to 0.
  * @note   Private function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	n/a
  * @retval n/a
  */
void si468xDAB::_clearData_RawDigitalServiceList(void){
	clearData((uint8_t**)&dynamicalyAlocatedData.si468xDigitalServiceListRAW.pServiceListData, (uint16_t*)&dynamicalyAlocatedData.si468xDigitalServiceListRAW.sizeOfServiceList);
}

/**
  * @brief	Destroy "dynamicalyAlocatedData.si468xDigitalServiceListRAW" which is
  * 		designated to store raw digital services list received from si468x.
  * 		Frees up RAM, sets pointer to nullptr and size to 0.
  * @note   Private function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	n/a
  * @retval n/a
  */
void si468xDAB::_destroyArray_RawDigitalServiceList(void){
	destroyArray((uint8_t**)&dynamicalyAlocatedData.si468xDigitalServiceListRAW.pServiceListData, (uint16_t*)&dynamicalyAlocatedData.si468xDigitalServiceListRAW.sizeOfServiceList);
}


/**
  * @brief	si468x DAB_GET_FREQ_LIST command. Freq list is stored in
  * 		"dynamicalyAlocatedData.si468xDabFreqListOnChip" structure.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval n/a
*/
void si468xDAB::getDabFrequencyList(void){
	uint8_t args[] ={
			0
	};
	execute_Command(DAB_GET_FREQ_LIST, args, 1);

	readDataFromsi468xDynamic_1stBatch(8);
	uint16_t NUM_FREQS = (uint16_t) ComunicationArrays.Respond.data[4];
	readDataFromsi468xDynamic_2ndBatch((uint16_t)((NUM_FREQS*sizeof(uint32_t))));


		_createArray_DabFreqListOnChip(NUM_FREQS);
		//_allocateDabFreqListArray(NUM_FREQS);
		uint32_t* convertionPointer;
		for (uint8_t freqNum= 0; freqNum < NUM_FREQS; freqNum++)
		{

			convertionPointer = (uint32_t*)&ComunicationArrays.Respond.data[(7 +(freqNum*4)+1)];
			dynamicalyAlocatedData.si468xDabFreqListOnChip.pFrequTable [freqNum] = *convertionPointer;
		}
}

/**
  * @brief	si468x DAB_SET_FREQ_LIST command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	*pFreqList - pointer to array (list) which contains DAB frequencies appropriate to
  * 				geographical region (i.e. Europe is DAB freqs, USA is
  * 		itemsOnFerqList - size of pFreqList (number of frequences)
  * @retval n/a
*/
void si468xDAB::setDabFrequencyList(uint32_t* pFreqList, uint8_t itemsOnFerqList){
	uint8_t args[] ={
			itemsOnFerqList/sizeof(pFreqList),
			0x0,
			0x0
	};
	uint8_t* convertionPointer;
	convertionPointer=(uint8_t*) pFreqList;
	execute_Command(DAB_SET_FREQ_LIST, args, 3, (uint8_t*) convertionPointer, sizeof(pFreqList)*itemsOnFerqList);
	read_response(4);
}

/**
  * @brief	si468x DAB_ACF_STATUS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a*
  * @retval DabAcfStatus - structure which ingredients are
  * 				 similar as in DAB_ACF_STATUS command respond.
*/
DabAcfStatus si468xDAB::getDabAcfStatus(void){
	uint8_t args[] ={0};
	DabAcfStatus retVal;
	execute_Command(DAB_ACF_STATUS, args, 1);
	read_response(12);
	retVal.RFU1				= ComunicationArrays.Respond.data[4];
	retVal.RFU2				= ComunicationArrays.Respond.data[5];
	retVal.AUDIO_LEVEL		= (uint16_t) (ComunicationArrays.Respond.data[6] + ((ComunicationArrays.Respond.data[7]<<8)&0xff00));
	retVal.CMFT_NOISE_LEVEL	= (uint16_t) (ComunicationArrays.Respond.data[8] + ((ComunicationArrays.Respond.data[9]<<8)&0xff00));
	return retVal;
}

/**
  * @brief	si468x DAB_GET_TIME command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	TIME_TYPE - "Indicate the type of the time"
  *						0 : local time
  *						1 : UTC
  * @retval DabTime - structure which ingredients are
  * 				 similar as in DAB_GET_TIME command respond.
 */
DabTime si468xDAB::getDabTime(bool TIME_TYPE){
	uint8_t args [] = {
			(uint8_t) TIME_TYPE
	};
	DabTime retVal;

	execute_Command(DAB_GET_TIME, args, 1);
	read_response(11);

	retVal.YEAR 	= (uint16_t) (ComunicationArrays.Respond.data[4] + ((ComunicationArrays.Respond.data[5]<<8)&0xff00));
	retVal.MONTHS	= ComunicationArrays.Respond.data[6];
	retVal.DAYS		= ComunicationArrays.Respond.data[7];
	retVal.HOURS	= ComunicationArrays.Respond.data[8];
	retVal.MINUTES	= ComunicationArrays.Respond.data[9];
	retVal.SECONDS	= ComunicationArrays.Respond.data[10];

	return retVal;
}

/**
  * @brief	si468x DAB_GET_AUDIO_INFO command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval DabAudioInfo - structure which ingredients are
  * 				 similar as in DAB_GET_AUDIO_INFO command respond.
 */
DabAudioInfo si468xDAB::getDabAudioInfo(void){
	uint8_t args [] = {
			0
	};
	execute_Command(DAB_GET_AUDIO_INFO, 0, 1);
	DabAudioInfo retVal;
	read_response(20);

	retVal.AUDIO_BIT_RATE		= (uint16_t) (ComunicationArrays.Respond.data[4] + ((ComunicationArrays.Respond.data[5]<<8)&0xff00));
	retVal.AUDIO_SAMPLE_RATE	= (uint16_t) (ComunicationArrays.Respond.data[6] + ((ComunicationArrays.Respond.data[7]<<8)&0xff00));
	retVal.AUDIO_PS_FLAG		= (ComunicationArrays.Respond.data[8]>>3)&0b1;
	retVal.AUDIO_SBR_FLAG		= (ComunicationArrays.Respond.data[8]>>2)&0b1;
	retVal.AUDIO_MODE			= (ComunicationArrays.Respond.data[8])&0b11;
	retVal.AUDIO_DRC_GAIN		= ComunicationArrays.Respond.data[9];

	return retVal;
}

/**
  * @brief	si468x DAB_GET_ANNOUNCEMENT_INFO command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval DabAnnouncementInfo - structure which ingredients are
  * 				 similar as in DAB_GET_ANNOUNCEMENT_INFO command respond.
 */
DabAnnouncementInfo si468xDAB::getDabAnnouncementInfo(void){
	DabAnnouncementInfo retVal;
	execute_Command(DAB_GET_ANNOUNCEMENT_INFO, nullptr,0);
	read_response(16);

	retVal.ANNO_Q_OVFL	= ComunicationArrays.Respond.data[4]&0b1;
	retVal.ANNO_Q_SIZE	= ComunicationArrays.Respond.data[5]&0b1111;
	retVal.CLUSTER_ID	= ComunicationArrays.Respond.data[6];
	retVal.ANNO_STAT	= (ComunicationArrays.Respond.data[7]>>3)&0b1;
	retVal.REGION_FLAG	= (ComunicationArrays.Respond.data[7]>>2)&0b1;
	retVal.SRC			= ComunicationArrays.Respond.data[7]&0b11;
	retVal.ASW			= (uint16_t) (ComunicationArrays.Respond.data[8] + ((ComunicationArrays.Respond.data[9]<<8)&0xff00));
	retVal.ID1			= (uint16_t) (ComunicationArrays.Respond.data[10] + ((ComunicationArrays.Respond.data[11]<<8)&0xff00));
	retVal.ID2			= (uint16_t) (ComunicationArrays.Respond.data[12] + ((ComunicationArrays.Respond.data[13]<<8)&0xff00));
	retVal.REGIONID1	= ComunicationArrays.Respond.data[14];
	retVal.REGIONID2	= ComunicationArrays.Respond.data[15];

	return retVal;
}

/**
  * @brief	si468x DAB_GET_EVENT_STATUS_INFO command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	clearEVENT_ACK - Clears all pending digital radio event interrupts
  * @retval DabEventStatus - structure which ingredients are
  * 				 similar as in DAB_GET_EVENT_STATUS command respond.
*/
DabEventStatus	si468xDAB::getDabEvebtStatus(bool clearEVENT_ACK){
	uint8_t args [] = {
			(uint8_t) clearEVENT_ACK
	};
	DabEventStatus retVal;

	execute_Command(DAB_GET_EVENT_STATUS, args, 1);
	read_response(8);

	retVal.RECFGINT		= (ComunicationArrays.Respond.data[4]>>7)&0b1;
	retVal.RECFGWRNINT	= (ComunicationArrays.Respond.data[4]>>6)&0b1;
	retVal.ANNOINT		= (ComunicationArrays.Respond.data[4]>>4)&0b1;
	retVal.OESERVINT	= (ComunicationArrays.Respond.data[4]>>3)&0b1;
	retVal.SERVLINKINT	= (ComunicationArrays.Respond.data[4]>>2)&0b1;
	retVal.FREQINFOINT	= (ComunicationArrays.Respond.data[4]>>1)&0b1;
	retVal.SVRLISTINT	= (ComunicationArrays.Respond.data[4])&0b1;
	retVal.ANNO			= (ComunicationArrays.Respond.data[5]>>4)&0b1;
	retVal.OESERV		= (ComunicationArrays.Respond.data[5]>>3)&0b1;
	retVal.SERVLINK		= (ComunicationArrays.Respond.data[5]>>2)&0b1;
	retVal.FREQ_INFO	= (ComunicationArrays.Respond.data[5]>>1)&0b1;
	retVal.SVRLIST		= (ComunicationArrays.Respond.data[5])&0b1;
	retVal.SVRLISTVER	= (uint16_t) (ComunicationArrays.Respond.data[6] + ((ComunicationArrays.Respond.data[7]<<8)&0xff00));
	return retVal;
}


/**
  * @brief	si468x DAB_DIGRAD_STATUS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	clearDIGRAD_ACK - Clears all pending digital radio interrupts
  * 		ATTUNE - Return the values as of DAB_VALID_RSSI_TIME after tune. Only
  * 				 the signal quality metric RSSI is affected by setting this bit.
  * 				 0 : Return the current status
  * 				 1 : Return the snapshot taken at DAB_VALID_RSSI_TIME
  * 		clearFIBERR_ACK - Clears the Fast Information Blocks error counter
  * 				(FIB_ERROR_COUNT) when set. The FIB error counter will reset
  * 				to 0 and continue counting.
  * 		clearSTC_ACK - Clears the STC interrupt status when set.
  * @retval DabDigitalStatus - structure which ingredients are
  * 				 similar as in DAB_DIGRAD_STATUS command respond.
*/
DabDigitalStatus si468xDAB::getDabDigradStatus(bool clearDIGRAD_ACK, bool ATTUNE, bool clearFIBERR_ACK, bool clearSTC_ACK){
	uint8_t args [] = {
				((clearDIGRAD_ACK<<3)&0b1000)
			+ 	((ATTUNE<<2)&0b100)
			+ 	((clearFIBERR_ACK<<1)&0b10)
			+	(clearSTC_ACK&0b1)
	};
	execute_Command(DAB_DIGRAD_STATUS, args, 1);
	read_response(23);

	DabDigitalStatus retVal;

	retVal.FICERRINT	= (ComunicationArrays.Respond.data[4]>>3)&0b1;
	retVal.ACQINT		= (ComunicationArrays.Respond.data[4]>>2)&0b1;
	retVal.RSSIHINT		= (ComunicationArrays.Respond.data[4]>>1)&0b1;
	retVal.RSSILINT		= (ComunicationArrays.Respond.data[4])&0b1;
	retVal.FICERR		= (ComunicationArrays.Respond.data[5]>>3)&0b1;
	retVal.ACQ			= (ComunicationArrays.Respond.data[5]>>2)&0b1;
	retVal.VALID		= (ComunicationArrays.Respond.data[5])&0b1;
	retVal.RSSI			= ComunicationArrays.Respond.data[6];
	retVal.SNR			= ComunicationArrays.Respond.data[7];
	retVal.FIC_QUALITY	= ComunicationArrays.Respond.data[8];
	retVal.CNR			= ComunicationArrays.Respond.data[9];
	retVal.FIB_ERROR_COUNT	= (uint16_t) (ComunicationArrays.Respond.data[10] + ((ComunicationArrays.Respond.data[11]<<8)&0xff00));
	retVal.TUNE_FREQ		= (uint32_t) (ComunicationArrays.Respond.data[12] + ((ComunicationArrays.Respond.data[13]<<8)&0xff00) +  ((ComunicationArrays.Respond.data[14]<<16)&0xff0000) + ((ComunicationArrays.Respond.data[15]<<24)&0xff000000));
	retVal.TUNE_INDEX		= ComunicationArrays.Respond.data[16];
	retVal.FFT_OFFSET		= ComunicationArrays.Respond.data[17];
	retVal.READANTCAP		= (uint16_t) (ComunicationArrays.Respond.data[18] + ((ComunicationArrays.Respond.data[19]<<8)&0xff00));
	retVal.CU_LEVEL			= (uint16_t) (ComunicationArrays.Respond.data[20] + ((ComunicationArrays.Respond.data[21]<<8)&0xff00));
	retVal.FAST_DECT		= ComunicationArrays.Respond.data[22];

	return retVal;
}


/**
  * @brief	si468x DAB_TUNE_FREQ command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	FREQ_INDEX - Frequency index for the tuned frequency, see
  * 					 the DAB_SET_FREQ_LIST command that sets the
  * 					 frequency table.
  * 		ANTCAP - Antenna tuning capacitor value in 250 fF units
  * 					 (31.75 pF Max) Range: 0-128
  * 					 0 : Automatically determines the cap setting.
  * 		INJECTION - Injection selection
  * 					0 : Automatic injection selection.
  * 					1 : Low-side injection.
  * 					2 : High-side injection.
  * @retval n/a
*/
void si468xDAB::dabTuneFreq(uint8_t FREQ_INDEX, uint16_t ANTCAP, uint8_t INJECTION){
	uint8_t args [] ={
			INJECTION & 0b11,
			FREQ_INDEX,
			0x0,
			ANTCAP & 0b11111111,
			((ANTCAP>>8)& 0b11111111)
	};
	execute_Command(DAB_TUNE_FREQ,  args, 5);
	read_response(4);
}


/**
  * @brief	si468x GET_DIGITAL_SERVICE_LIST command. Service list is stored
  * 		in "dynamicalyAlocatedData.si468xDigitalServiceListRAW" structure.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	SERTYPE - Sets the type of service list to retrieve.
  * 				  0 : Get the complete(DAB, DMB) service list.
  * 				  1 : Get the data(HD) service list.
  * 				  2 : Get the HD audio info.
  * 				  3 : Get the HD data info.
  * @retval bool depend on results
*/
bool si468xDAB::GetDigitalSerivceList(uint8_t SERTYPE){
	uint8_t args[]={
			SERTYPE&0b11
	};
	execute_Command(GET_DIGITAL_SERVICE_LIST, args, 1);
	readDataFromsi468xDynamic_1stBatch(6);
	uint16_t SIZE = (uint16_t)(ComunicationArrays.Respond.data[4]+((ComunicationArrays.Respond.data[5]<<8)&0xff00));
	readDataFromsi468xDynamic_2ndBatch(SIZE);

	uint16_t sizeOfRawServiceList = SIZE+4; 	//SIZE + 4  because parser takes all respon
	//create struct DigitalServiceListRaw and copy databout servicelist from ComunicationArrays.Respond.data to struct
	bool retVal = _createArray_RawDigitalServiceList(sizeOfRawServiceList);
	if (retVal == true){
		memcpy(dynamicalyAlocatedData.si468xDigitalServiceListRAW.pServiceListData, ComunicationArrays.Respond.data, sizeOfRawServiceList);
	}
	return retVal;
}

/**
  * @brief	parse RAW service list received from si468x
  * 		(stored in "dynamicalyAlocatedData.si468xDigitalServiceListRAW")
  * 		and save it in "dynamicalyAlocatedData.dabServiceListParsed".
  * @note	For more details pls take a look at si468x datasheet,
  * 		7. Digital Services User’s Guide, 7.1. Reference Material
  *
  * @param	n/a
  * @retval bool depend on results
*/
bool si468xDAB::dabParseDigitalServiceListRAW(void){
	uint8_t *data;
	data = dynamicalyAlocatedData.si468xDigitalServiceListRAW.pServiceListData;
	uint16_t len;
	len = dynamicalyAlocatedData.si468xDigitalServiceListRAW.sizeOfServiceList;
	uint16_t pos;
	uint8_t component_num;
	uint8_t i, p;

	if (len < 6){
		_destroyArray_RawDigitalServiceList();
		return false; // no list available? exit
	}
	if (len >= 9) {
		dynamicalyAlocatedData.dabServiceListParsed.list_size = data[5] << 8 | data[4];
		dynamicalyAlocatedData.dabServiceListParsed.version = data[7] << 8 | data[6];
		dynamicalyAlocatedData.dabServiceListParsed.num_services = data[8];
	}
	// 9,10,11 are align pad
	pos = 12;
	if (len <= pos){
		_destroyArray_RawDigitalServiceList();
		return false; // no services? exit
	}


	// size of one service with zero component: 24 byte
	// every component + 4 byte
	for (i = 0; i < dynamicalyAlocatedData.dabServiceListParsed.num_services; i++) {
		dynamicalyAlocatedData.dabServiceListParsed.services[i].service_id = data[pos + 3] << 24 | data[pos + 2] << 16 | data[pos + 1] << 8 | data[pos];
		component_num = data[pos + 5] & 0x0F;
		dynamicalyAlocatedData.dabServiceListParsed.services[i].num_components = component_num;
		memcpy(dynamicalyAlocatedData.dabServiceListParsed.services[i].service_label, &data[pos + 8], 16);
		dynamicalyAlocatedData.dabServiceListParsed.services[i].service_label[16] = '\0';
		for (p = 0; p < component_num; p++) {
			dynamicalyAlocatedData.dabServiceListParsed.services[i].component_id[p] = data[pos + 25] << 8 | data[pos + 24];
			pos += 4;
		}
		pos += 24;
	}

	_destroyArray_RawDigitalServiceList();
	return true;

}

/**
  * @brief	Sort list of services stored in "dynamicalyAlocatedData.dabServiceListParsed".
  * @note	Sorts based on service_id parameter.
  *
  * @param	n/a
  * @retval n/a
*/
void si468xDAB::sortParsedServiceList(void){
	uint8_t i,p,swapped;

	swapped = 0;
	for(i=dynamicalyAlocatedData.dabServiceListParsed.num_services;i>1;i--){
		for(p=0;p<i-1;p++){
			if(dynamicalyAlocatedData.dabServiceListParsed.services[p].service_id >
				dynamicalyAlocatedData.dabServiceListParsed.services[p+1].service_id){
				_swap_services(p,p+1);
				swapped = 1;
			}
		}
		if(!swapped)
			break;
	}
}

/**
  * @brief	Count how many correct services is stored in
  * 		"dynamicalyAlocatedData.dabServiceListParsed".
  * @note	Number of services is determined based on "service_id!=0"
  *
  * @param	n/a
  * @retval uint16_t - number of servises
*/
uint16_t si468xDAB::dabParsedServiceListSize(){
	uint16_t retVal = 0;
	for (uint16_t i =0;i<MAX_SERVICES;i++){
		if(dynamicalyAlocatedData.dabServiceListParsed.services[i].service_id!=0)
			retVal++;
	}
	return retVal;
}


/**
  * @brief	Swap position of services stored in "dynamicalyAlocatedData.dabServiceListParsed".
  * @note	Private Auxiliary function executes by sortParsedServiceList.
  *
  * @param	n/a
  * @retval n/a
*/
void si468xDAB::_swap_services(uint8_t first, uint8_t second){
	struct dab_service_t tmp;

	memcpy(&tmp,&dynamicalyAlocatedData.dabServiceListParsed.services[first],sizeof(tmp));
	memcpy(&dynamicalyAlocatedData.dabServiceListParsed.services[first],
			&dynamicalyAlocatedData.dabServiceListParsed.services[second] ,sizeof(tmp));
	memcpy(&dynamicalyAlocatedData.dabServiceListParsed.services[second],&tmp ,sizeof(tmp));
}



/**
  * @brief	si468x DAB band scan routine.
  * @note	For more details pls take a look at si468x datasheet:
  * 		5. Flowcharts, 5.6. Data Service Receiver—DAB–Band Scan
  *
  * @param	FREQ_INDEX - Frequency index for the tuned frequency, see
  * 					 the DAB_SET_FREQ_LIST command that sets the
  * 					 frequency table.
  * 		serviceType -  SERTYPE - Sets the type of service list to retrieve.
  * 				  		0 : Get the complete(DAB, DMB) service list.
  * 				  		1 : Get the data(HD) service list.
  * 				  		2 : Get the HD audio info.
  * 				  		3 : Get the HD data info.
  * 		seekDirection - 0 - FREQ_INDEX--
  * 						1 - FREQ_INDEX++
  * 		ANTCAP - Antenna tuning capacitor value in 250 fF units
  * 				 	(31.75 pF Max) Range: 0-128
  * 				 	0 : Automatically determines the cap setting.
  * 		INJECTION - Injection selection
  * 					0 : Automatic injection selection.
  * 					1 : Low-side injection.
  * 					2 : High-side injection.
  * @retval uint8_t - Indicates the currently tuned frequency index.
*/
uint8_t si468xDAB::dabBandScan(uint8_t FREQ_INDEX, uint8_t serviceType, bool seekDirection, uint16_t ANTCAP, uint8_t INJECTION){

	//int16_t _FREQ_INDEX = (int16_t)FREQ_INDEX;
	DabDigitalStatus _DigradStatusVal;
	uint16_t temporarySVRLISTVER;
	int8_t timeoutServiceListStabilised;
	int8_t timeoutServiceListReady;
	/*const*/ int8_t timoutVal = 10;
	/*bool retValGetDigitalSerivceList;*/
	bool bandReached = false;

	if (FREQ_INDEX>(dynamicalyAlocatedData.si468xDabFreqListOnChip.NUM_FREQS-1)){
		FREQ_INDEX =(dynamicalyAlocatedData.si468xDabFreqListOnChip.NUM_FREQS-1);
	}

	memset(&_DigradStatusVal, 0, sizeof(_DigradStatusVal));				//todo: wykasować to jak bedzie juz działać
	do{
		do{

			if ((FREQ_INDEX==0)&(seekDirection==si468xSeekDown)){
				bandReached = true;
			}
			if ((FREQ_INDEX ==(dynamicalyAlocatedData.si468xDabFreqListOnChip.NUM_FREQS-1))&(seekDirection==si468xSeekUp)){
				bandReached = true;
			}

			dabTuneFreq(FREQ_INDEX, ANTCAP, INJECTION);
			wait4STCint();
			setInterruptAsHandled_STC();
			_DigradStatusVal = getDabDigradStatus(1, 0, 0, 1);

			//if (!_DigradStatusVal.VALID){
				if (!bandReached)
				{
					if (seekDirection == si468xSeekUp)
						FREQ_INDEX++;
					else
						FREQ_INDEX--;
				}
			//}
		}while((_DigradStatusVal.VALID !=1)&&(!bandReached));		//Is Valid set??
	}while((_DigradStatusVal.ACQ!=1)&&(!bandReached)); //FIC Quality & Sig Level ok??


	DabEventStatus _DabEventStatus;
	memset(&_DabEventStatus, 0, sizeof(_DabEventStatus));

	temporarySVRLISTVER = _DabEventStatus.SVRLISTVER;
	timeoutServiceListReady = timoutVal;
	timeoutServiceListStabilised = timoutVal;

	do{
		do{
			_DabEventStatus =getDabEvebtStatus(1);
			if (timeoutServiceListReady>0){
				timeoutServiceListReady--;
			}
//			if (timeoutServiceListReady==0)
//				break;
		}while(!_DabEventStatus.SVRLIST&& timeoutServiceListReady);		//Is Service list ready?


		/*retValGetDigitalSerivceList = */GetDigitalSerivceList(serviceType);



		if (temporarySVRLISTVER!=_DabEventStatus.SVRLISTVER){			//Has service list stabilised??
			temporarySVRLISTVER = _DabEventStatus.SVRLISTVER;
			timeoutServiceListStabilised = timoutVal;
		}
	}while((timeoutServiceListStabilised --)&&(timeoutServiceListReady));												//Has service list stabilised??


	dabParseDigitalServiceListRAW();
	//_destroyArray_RawDigitalServiceList();		//jeśli nie ma tej komendy w dabParseDigitalServiceListRAW();
	sortParsedServiceList();
	return _DigradStatusVal.TUNE_INDEX;
}


/**
  * @brief	Function STARTS/ STOPS digital service.
  * @note   Private function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	StopStart - 0x81 - START_DIGITAL_SERVICE
  * 					0x82 - STOP_DIGITAL_SERVICE
  * 		ServiceNo - number of position in dynamicalyAlocatedData.dabServiceListParsed.services[x] structure
  *			SERTYPE - Sets the type of service list to retrieve.
  * 				  0 : Get the complete(DAB, DMB) service list.
  * 				  1 : Get the data(HD) service list.
  * 				  2 : Get the HD audio info.
  * 				  3 : Get the HD data info.
  * @retval n/a
  */
void si468xDAB::_dabStartStopDigitalService(uint8_t StopStart, uint8_t ServiceNo, bool SERTYPE){
	uint32_t service_id = dynamicalyAlocatedData.dabServiceListParsed.services[ServiceNo].service_id;
	uint16_t component_id = dynamicalyAlocatedData.dabServiceListParsed.services[ServiceNo].component_id[0];
	//dynamicalyAlocatedData.service_label = &dynamicalyAlocatedData.dabServiceListParsed.services[ServiceNo].service_label;

	uint8_t args [] ={
			SERTYPE,
			0x0,
			0x0,
			service_id & 0xFF,
			(service_id >> 8) & 0xFF,
			(service_id >> 16) & 0xFF,
			(service_id >> 24) & 0xFF,
			component_id & 0xFF,
			(component_id >> 8) & 0xFF,
			(component_id >> 16) & 0xFF,
			(component_id >> 24) & 0xFF,
	};
	execute_Command(StopStart, args, 11);
}

/**
  * @brief	Function STARTS digital service.
  * @note   For more details pls take a look at START_DIGITAL_SERVICE
  * 		command in si468x datasheet.
  *
  * @param 	ServiceNo - number of position in dynamicalyAlocatedData.dabServiceListParsed.services[x] structure
  *			SERTYPE - Sets the type of service list to retrieve.
  * 				  0 : Get the complete(DAB, DMB) service list.
  * 				  1 : Get the data(HD) service list.
  * 				  2 : Get the HD audio info.
  * 				  3 : Get the HD data info.
  * @retval bool depend on results
  */
bool si468xDAB::dabStartDigitalService(uint8_t ServiceNo, bool SERTYPE){  // ServiceNo must be in range of <0, dabParsedServiceListSize()>
	_dabStartStopDigitalService(START_DIGITAL_SERVICE, ServiceNo, SERTYPE);
	read_response(4);
	if (DeviceState.uStatus0.ERR_CMD==0){
		dynamicalyAlocatedData.service_label = dynamicalyAlocatedData.dabServiceListParsed.services[ServiceNo].service_label;
		return true;
	}
	else {
		dynamicalyAlocatedData.service_label = nullptr;
		return false;
	}
}

/**
  * @brief	Function STOPS digital service.
  * @note   For more details pls take a look at STOP_DIGITAL_SERVICE
  * 		command in si468x datasheet.
  *
  * @param 	ServiceNo - number of position in dynamicalyAlocatedData.dabServiceListParsed.services[x] structure
  *			SERTYPE - Sets the type of service list to retrieve.
  * 				  0 : Get the complete(DAB, DMB) service list.
  * 				  1 : Get the data(HD) service list.
  * 				  2 : Get the HD audio info.
  * 				  3 : Get the HD data info.
  * @retval bool depend on results
  */
bool si468xDAB::dabStopDigitalService(uint8_t ServiceNo, bool SERTYPE){  // ServiceNo must be in range of <0, dabParsedServiceListSize()>
	_dabStartStopDigitalService(STOP_DIGITAL_SERVICE, ServiceNo, SERTYPE);
	read_response(4);
	if (DeviceState.uStatus0.ERR_CMD==1){		//if error not stopped
		dynamicalyAlocatedData.service_label = dynamicalyAlocatedData.dabServiceListParsed.services[ServiceNo].service_label;
		return false;
	}
	else {
		dynamicalyAlocatedData.service_label = nullptr;
		return true;
	}
}

/**
  * @brief	si468x DAB_GET_FREQ_INFO command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	n/a
  * @retval DabGetTunedFreqInfo - structure which ingredients are
  * 				 similar as in DAB_GET_FREQ_INFO command respond.
*/
DabGetTunedFreqInfo si468xDAB::dabGetFreqInfo(void){
	uint8_t args [] ={
			0x0
	};
	DabGetTunedFreqInfo retVal;
	execute_Command(DAB_GET_FREQ_INFO, args, 1);
	read_response(20);

	retVal.LENGTH_FI_LIST		= (uint32_t) ComunicationArrays.Respond.data[4]	+ ((ComunicationArrays.Respond.data[5]<<8)&0xff00) + ((ComunicationArrays.Respond.data[6]<<16)&0xff0000) + ((ComunicationArrays.Respond.data[7]<<24)&0xff000000);
	retVal.FI_ID_0				= (uint32_t) ComunicationArrays.Respond.data[8]	+ ((ComunicationArrays.Respond.data[9]<<8)&0xff00) + ((ComunicationArrays.Respond.data[10]<<16)&0xff0000) + ((ComunicationArrays.Respond.data[11]<<24)&0xff000000);
	retVal.FI_FREQ_0			= (uint32_t) ComunicationArrays.Respond.data[12] + ((ComunicationArrays.Respond.data[13]<<8)&0xff00) + ((ComunicationArrays.Respond.data[14]<<16)&0xff0000) + ((ComunicationArrays.Respond.data[15]<<24)&0xff000000);
	retVal.FI_FREQ_INDEX_0		= ComunicationArrays.Respond.data[16];
	retVal.FI_RNM_0				= ComunicationArrays.Respond.data[17];
	retVal.FI_CONTINUITY_0		= ComunicationArrays.Respond.data[18];
	retVal.FI_CONTROL_0			= ComunicationArrays.Respond.data[19];

	return retVal;
}

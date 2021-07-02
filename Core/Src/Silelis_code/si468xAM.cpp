/*
 * si468xAM.cpp
 *
 *  Created on: Feb 1, 2021
 *      Author: dbank
 */


#include "si468xAM.h"
#include "string.h"


/**
  * @brief	AM object constructor with i2c communication
  * @note
  *
  * @param 	_resetPort	- STM32 reset pin port address
  * 		_resetPin	- STM32 reset pin number
  * 		*_hi2c		- pointer to STM32 i2c bus where
  * 					si468x is connected
  * 		_DevAddress	- si468x i2c bus address
  * 		imgAddress - uploaded image begging address in spi flash
  * 		freqSpacing - AM Seek Frequency Spacing.
  * 						Range: 1-31
  * 						9 : AM in Asia (9kHz)
  * 						10 : AM in US (10kHz)
  * @retval n/a
  */
si468xAM::si468xAM(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress, uint32_t imgAddress, uint8_t freqSpacing)
:si468xCore(_resetPort, _resetPin, _hi2c, _DevAddress)
{
	_setAmTuningParam(freqSpacing);
	_constructorAMCommon(imgAddress);
}

/**
  * @brief	AM object destructor.
  * @note	Clear all object data and destroy object.
  *
  * @param 	n/a
  * @retval n/a
  */
si468xAM::~si468xAM(){

}

/**
  * @brief	Function sets AM tuning parameters like:
  * 		AM_SEEK_BAND_BOTTOM, AM_SEEK_BAND_TOP,
  * 		AM_SEEK_FREQUENCY_SPACING
  * @note	n/a
  *
  * @param 	freqSpacing - AM Seek Frequency Spacing.
  * 						Range: 1-31
  * 						9 : AM in Asia (9kHz)
  * 						10 : AM in US (10kHz)
  * @retval n/a
  */
void si468xAM::_setAmTuningParam(uint8_t freqSpacing){
	tuningParam.AM_SEEK_BAND_BOTTOM = 520;
	tuningParam.AM_SEEK_BAND_TOP = 1710;
	tuningParam.AM_SEEK_FREQUENCY_SPACING= freqSpacing;
}

/**
  * @brief	Private function part of constructor.
  * @note   Private function used in constructor which can be executed in i2s and spi constructor.
  * 		Auxiliary function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	imgAddress - uploaded image begging address in spi flash
  * @retval bool value depend of results
  */
bool si468xAM::_constructorAMCommon(uint32_t  imgAddress){
	init(imgAddress);
	if (DeviceState.UploadedImage != AMHD_active){
		DeviceState.UploadedImage = si468xBootERROR;		//TODO: wymyśleć coś lepszego
		return false;
	}
	else{
		_setAMProperties();
		return true;
	}
}

/**
  * @brief	Private function part of constructor. Sets properties appropriate
  * 		to AM receiver.
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
void si468xAM::_setAMProperties(){
	setProperty(PROP_AM_SEEK_BAND_BOTTOM, tuningParam.AM_SEEK_BAND_BOTTOM);
	setProperty(PROP_AM_SEEK_BAND_TOP, tuningParam.AM_SEEK_BAND_TOP);
	setProperty(PROP_AM_SEEK_FREQUENCY_SPACING, tuningParam.AM_SEEK_FREQUENCY_SPACING);
	//setProperty(PROP_AM_VALID_RSSI_THRESHOLD,
	//setProperty(PROP_AM_VALID_SNR_THRESHOLD,
	//setProperty(PROP_AM_SOFTMUTE_SQ_LIMITS,
	//TODO: set Your properties
}


/**
  * @brief	Tune AM frequency
  * @note   For more details pls take a look at AM_TUNE_FREQ in
  * 		si468x datasheet.
  *
  * @param 	kHzFreq - Frequency in multiples of 1.0 kHz added to
  * 				a starting frequency of 0 Hz
  * 		ANTCAP - When non-zero this parameter sets the antenna
  * 				tuning capacitor value to (ANTCAP-1)*142 fF
  * 				(581.49 pF Max).
  * 				Range: 0-4096
  *					0 : Automatically determines the cap setting.
  * 		TUNE_MODE - Set the desired tuning mode.
  * 					0 : Tune and render analog audio as fast as
  * 					possible, do not attempt to acquire HD.
  * 					1 : Reserved
  * 					2 : Tune and render analog audio as fast as
  * 					possible, try to acquire HD and crossfade to
  * 					HD if acquired. Always selects the Main Program
  * 					Service. To select an SPS use the START_DIGITAL_SERVICE
  * 					command after the HD has been acquired.
  * 					3 : Tune and attempt to acquire HD and render audio if
  * 					successful. The Main Program Service (MPS) will be
  * 					rendered. To select an SPS use the START_DIGITAL_SERVICE
  * 					command after the HD has been acquired.
  * 		INJECTION - Injection selection
  * 					0 : Automatic injection selection.
  * 					1 : Low-side injection.
  * 					2 : High-side injection.
  * @retval n/a
  */
void si468xAM::amTuneFreq(uint32_t kHzFreq,  uint16_t ANTCAP, uint8_t TUNE_MODE, uint8_t INJECTION){
	uint8_t args[]={
			((TUNE_MODE<<2)&0b1100) + (INJECTION & 0b11),
			kHzFreq & 0xff,
			((kHzFreq >>8)& 0xff),
			ANTCAP & 0xFF,
			(ANTCAP >> 8) & 0xFF
	};
	execute_Command(AM_TUNE_FREQ, args, 5);
}

/**
  * @brief	Start seek valid AM station from current frequency.
  * @note   For more details pls take a look at AM_SEEK_START in
  * 		si468x datasheet.
  *
  * @param 	seekDirection - determines direction of seek and band limit.
  * 					0 : Seek down. Band limit is AM_SEEK_BAND_BOTTOM.
  * 					1 : Seek up. Band limit is AM_SEEK_BAND_TOP.
  * 		WRAP - determines seek behavior upon reaching a band limit.
  * 					0 : Halt seek at band limit.
  * 					1 : When band limit is hit, continue seek from opposite band limit.
  * 		FORCE_WB - Forces the DFE into wideband mode.
  * 					0 : Normal operation.
  * 					1 : Force Wide bandwidth
  * 		TUNE_MODE - Set the desired tuning mode.
  * 					0 : Tune and render analog audio as fast as
  * 					possible, do not attempt to acquire HD.
  * 					1 : Reserved
  * 					2 : Tune and render analog audio as fast as
  * 					possible, try to acquire HD and crossfade to
  * 					HD if acquired. Always selects the Main Program
  * 					Service. To select an SPS use the START_DIGITAL_SERVICE
  * 					command after the HD has been acquired.
  * 					3 : Tune and attempt to acquire HD and render audio if
  * 					successful. The Main Program Service (MPS) will be
  * 					rendered. To select an SPS use the START_DIGITAL_SERVICE
  * 					command after the HD has been acquired.
  * 		ANTCAP - When non-zero this parameter sets the antenna
  * 				tuning capacitor value to (ANTCAP-1)*142 fF
  * 				(581.49 pF Max).
  * 				Range: 0-4096
					0 : Automatically determines the cap setting.
  * 		INJECTION - Injection selection
  * 					0 : Automatic injection selection.
  * 					1 : Low-side injection.
  * 					2 : High-side injection.
  * @retval n/a
  */
void si468xAM::amSeekStart(bool seekDirection, bool WRAP, bool FORCE_WB, uint8_t TUNE_MODE, uint8_t INJECTION, uint16_t ANTCAP){
	uint8_t args[]={
			((FORCE_WB<<4) & 0b10000) + ((TUNE_MODE<<2) & 0b1100) + (INJECTION & 0b11),
			((seekDirection <<1 ) & 0b10) + (WRAP & 0b1),
			0x0,
			ANTCAP & 0xFF,
			(ANTCAP >> 8) & 0xFF,
	};
	execute_Command(AM_SEEK_START, args, 5);
}


/**
  * @brief	si468x AM_RSQ_STATUS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	clearRSQACK - Clears RSQINT, SNRHINT, SNRLINT, RSSIHINT,
  * 					  RSSILINT if set.
  * 		ATTUNE - Return the values as of AM_VALID_SNR_TIME after tune.
  * 					Only the signal quality metrics RSSI, SNR, ISSI,
  * 					ASSI and the status bits INJECT, AFCL, and VALID
  * 					are affected by setting this bit.
  * 					0 : Return the current status
  * 					1 : Return the snapshot taken at AM_VALID_SNR_TIME
  * 		cancelSeek - Aborts a seek currently in progress.
  * 					0 : Don't abort
  * 					1 : Abort
  * 		clearSTCACK - Clears the STC interrupt status indicator if set.
  * @retval si468xFmAmRsqStatus - structure which ingredients are
  * 				 similar as in AM_RSQ_STATUS and FM_RSQ_STATUS command respond.
*/
si468xFmAmRsqStatus si468xAM::getAmRsqStatus(bool clearRSQACK, bool ATTUNE,  bool cancelSeek, bool clearSTCACK){
	uint8_t args[] ={
			((clearRSQACK << 3) & 0b1000) + ((ATTUNE<<2) & 0b100) + ((cancelSeek<<1) & 0b10) + (clearSTCACK & 0b1)
	};
	execute_Command(AM_RSQ_STATUS, args, 1);
	read_response(17);

	si468xFmAmRsqStatus retVal;
	retVal.HDLEVELHINT		=(ComunicationArrays.Respond.data[4]>>5)& 0b1;
	retVal.HDLEVELLINT		=(ComunicationArrays.Respond.data[4]>>4)& 0b1;
	retVal.SNRHINT			=(ComunicationArrays.Respond.data[4]>>3)& 0b1;
	retVal.SNRLINT			=(ComunicationArrays.Respond.data[4]>>2)& 0b1;
	retVal.RSSIHINT			=(ComunicationArrays.Respond.data[4]>>1)& 0b1;
	retVal.RSSILIN			=ComunicationArrays.Respond.data[4]& 0b1;
	retVal.BLTFT			=(ComunicationArrays.Respond.data[5]>>7)& 0b1;
	retVal.HDDETECTED		=(ComunicationArrays.Respond.data[5]>>5)& 0b1;
	retVal.FLT_HDDETECTED	=(ComunicationArrays.Respond.data[5]>>4)& 0b1;
	retVal.AFCRL			=(ComunicationArrays.Respond.data[5]>>1)& 0b1;
	retVal.VALID			=ComunicationArrays.Respond.data[5]& 0b1;
	retVal.READFREQ			= (uint16_t) ((ComunicationArrays.Respond.data[7]<<8)&0xff00) + (uint16_t) (ComunicationArrays.Respond.data[6]);
	retVal.FREQOFF			=ComunicationArrays.Respond.data[8];
	retVal.RSSI				=ComunicationArrays.Respond.data[9];
	retVal.SNR				=ComunicationArrays.Respond.data[10];
	retVal.fmMULT_amMOD		=ComunicationArrays.Respond.data[11];
	retVal.READANTCAP		= (uint16_t) ((ComunicationArrays.Respond.data[13]<<8)&0xff00) + (uint16_t) (ComunicationArrays.Respond.data[12]);
	retVal.HDLEVEL			=ComunicationArrays.Respond.data[15];
	retVal.FILTERED_HDLEVEL	=ComunicationArrays.Respond.data[16];
	return retVal;
}

/**
  * @brief	si468x AM_ACF_STATUS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	clearACFACK - Clears ACFINT and any ACF interrupt bits
  * 						if set.
  * @retval si468xFmAmAcfStatus - structure which ingredients are
  * 				 similar as in AM_ACF_STATUS and FM_ACF_STATUS
  * 				 command respond.
*/
si468xFmAmAcfStatus si468xAM::getAmAcfStatus(bool clearACFACK){
	uint8_t args[] ={
			(clearACFACK&0b1)
		};
	execute_Command(AM_ACF_STATUS, args, 1);
	read_response(9);

	si468xFmAmAcfStatus retVal;
	memset(&retVal, 0, sizeof(si468xFmAmAcfStatus));

	retVal.HIGHCUT_INT 		=(ComunicationArrays.Respond.data[4]>>1)&0b1;
	retVal.SMUTE_INT		=ComunicationArrays.Respond.data[4]&0b1;
	retVal.HIGHCUT_CONV		=(ComunicationArrays.Respond.data[5]>>5)&0b1;
	retVal.SMUTE_CONV		=(ComunicationArrays.Respond.data[5]>>4)&0b1;
	retVal.HIGHCUT_STATE	=(ComunicationArrays.Respond.data[5]>>1)&0b1;
	retVal.SMUTE_STATE		=ComunicationArrays.Respond.data[5]&0b1;
	retVal.ATTN				=ComunicationArrays.Respond.data[6]&0b11111;
	retVal.HIGHCUT			=ComunicationArrays.Respond.data[7];
	retVal.fmSTBLEND_amLOWCUT	=ComunicationArrays.Respond.data[8];

	return retVal;
}

/**
  * @brief	si468x AM band scan routine.
  * @note	For more details pls take a look at si468x datasheet:
  * 		5. Flowcharts, 5.4. Data Service Receiver—FMHD–Band Scan
  * 		!!!There is no flowchart fot AM band scan so this routine
  * 		was written based on FM band scan.
  *
  * @param	kHzFreq - Frequency in multiples of 1.0 kHz added to
  * 				a starting frequency of 0 Hz
  * 		seekDirection - determines direction of seek and band limit.
  * 					0 : Seek down. Band limit is AM_SEEK_BAND_BOTTOM.
  * 					1 : Seek up. Band limit is AM_SEEK_BAND_TOP.
  * 		WRAP - determines seek behavior upon reaching a band limit.
  * 					0 : Halt seek at band limit.
  * 					1 : When band limit is hit, continue seek from opposite band limit.
  * 		FORCE_WB - Forces the DFE into wideband mode.
  * 					0 : Normal operation.
  * 					1 : Force Wide bandwidth
  * 		TUNE_MODE - Set the desired tuning mode.
  * 					0 : Tune and render analog audio as fast as
  * 					possible, do not attempt to acquire HD.
  * 					1 : Reserved
  * 					2 : Tune and render analog audio as fast as
  * 					possible, try to acquire HD and crossfade to
  * 					HD if acquired. Always selects the Main Program
  * 					Service. To select an SPS use the START_DIGITAL_SERVICE
  * 					command after the HD has been acquired.
  * 					3 : Tune and attempt to acquire HD and render audio if
  * 					successful. The Main Program Service (MPS) will be
  * 					rendered. To select an SPS use the START_DIGITAL_SERVICE
  * 					command after the HD has been acquired.
  * 		ANTCAP - When non-zero this parameter sets the antenna
  * 				tuning capacitor value to (ANTCAP-1)*142 fF
  * 				(581.49 pF Max).
  * 				Range: 0-4096
					0 : Automatically determines the cap setting.
  * 		INJECTION - Injection selection
  * 					0 : Automatic injection selection.
  * 					1 : Low-side injection.
  * 					2 : High-side injection.
  * @retval uint16_t - AM tuned frequency or frequency where scan stops
*/
uint16_t si468xAM::amBandScan(uint32_t kHzFreq,  bool seekDirection, bool WRAP, uint16_t ANTCAP, uint8_t TUNE_MODE, uint8_t INJECTION, bool FORCE_WB){
	//amTuneFreq(kHzFreq, ANTCAP, TUNE_MODE, INJECTION);
	si468xFmAmRsqStatus AmRsqStatus;

	if (kHzFreq>tuningParam.AM_SEEK_BAND_TOP)
		AmRsqStatus.READFREQ = kHzFreq>tuningParam.AM_SEEK_BAND_TOP;
	else
		AmRsqStatus.READFREQ = kHzFreq;

	amTuneFreq(AmRsqStatus.READFREQ,  ANTCAP, TUNE_MODE, INJECTION);

	do{
		do{
			wait4STCint();
			//setInterruptAsHandled(&DeviceState.uStatus0.STATUS0, 1);
			setInterruptAsHandled_STC();
			AmRsqStatus = getAmRsqStatus(true, 0, 0, 1);

			if (!AmRsqStatus.VALID)
				amSeekStart(seekDirection, WRAP, FORCE_WB, TUNE_MODE, INJECTION, ANTCAP);

		}while((AmRsqStatus.VALID!=1)&&(AmRsqStatus.BLTFT!=1));   //Nie wiem czy nie lepiej sprawdzać to po FmRsqStatus.SNRHINT i FmRsqStatus.SNRLINT
		//SNR OK?
	}while((AmRsqStatus.VALID!=1)&&(AmRsqStatus.BLTFT!=1));

	return AmRsqStatus.READFREQ;
}

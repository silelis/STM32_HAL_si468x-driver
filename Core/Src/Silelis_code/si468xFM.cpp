/*
 * si468xFM.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: dbank
 */

#include "string.h"
#include "si468xFM.h"

/**
  * @brief	FM object constructor with i2c communication
  * @note
  *
  * @param 	_resetPort	- STM32 reset pin port address
  * 		_resetPin	- STM32 reset pin number
  * 		*_hi2c		- pointer to STM32 i2c bus where
  * 					si468x is connected
  * 		_DevAddress	- si468x i2c bus address
  * 		imgAddress - uploaded image begging address in spi flash
  * 		region - Sets tuner parameters like FM_SEEK_BAND_BOTTOM,
  * 				FM_SEEK_BAND_TOP, FM_SEEK_FREQUENCY_SPACING appropriate for:
  * 				0x01 - fmTuningRegionEU;
  * 				0x02 - fmTuningRegionUS;
  * 				0x03 - fmTuningRegionJP;
  * @retval n/a
  */
si468xFM::si468xFM(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress, uint32_t imgAddress, uint8_t region)
	:si468xCore(_resetPort, _resetPin, _hi2c, _DevAddress)
	#ifdef useRdsParser
		,RDSDecoder(RDS_LOCALE_EU)			//TODO: chcnge this depend on MASTER CONFIGURATION
		//,RDSTranslator(RDS_LOCALE_EU)		//TODO: chcnge this depend on MASTER CONFIGURATION
	#endif
{
	_constructorFMCommon(imgAddress, region);
}

/**
  * @brief	Private function part of constructor.
  * @note   Private function used in constructor which can be executed in i2s and spi constructor.
  * 		Auxiliary function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	imgAddress - uploaded image begging address in spi flash
  * 		region - Sets tuner parameters like FM_SEEK_BAND_BOTTOM,
  * 				FM_SEEK_BAND_TOP, FM_SEEK_FREQUENCY_SPACING appropriate for:
  * 				0x01 - fmTuningRegionEU;
  * 				0x02 - fmTuningRegionUS;
  * 				0x03 - fmTuningRegionJP;
  * @retval bool value depend of results
  */
bool si468xFM::_constructorFMCommon(uint32_t imgAddress, uint8_t region){
	#ifdef useRds
		#ifdef useRdsParser
			//registerCallback(RDS_CALLBACK_AF, decodeAFFrequency);
			//unregisterCallbacks();		//clear RDS class _callbacks (I don't need theme)
		#endif
		#ifndef useAdvRdsParser
			memset(&fm_rds_data, 0, sizeof(fm_rds_data_t));
		#endif
	#endif


	init(imgAddress);

	if (DeviceState.UploadedImage != FMHD_active){
		DeviceState.UploadedImage = si468xBootERROR;		//TODO: wymyśleć coś lepszego
		return false;
	}

	else{
		_setFmTuningParam(region);
		_setFMProperties();
		return true;
	}

}



/*																																	//TODO: W moim przypadku spi nie działało - do poprawy
si468xFM::si468xFM(GPIO_TypeDef* _resetPort, uint16_t _resetPin, SPI_HandleTypeDef *_hspi, GPIO_TypeDef* _spiEnablePort, uint16_t _spiEnablePin, uint32_t imgAddress)
	:si468xCore(_resetPort, _resetPin, _hspi, _spiEnablePort, _spiEnablePin)
	#ifdef useRdsParser
		,RDSDecoder(RDS_LOCALE_EU)			//TODO: chcnge this depend on MASTER CONFIGURATION
		//,RDSTranslator(RDS_LOCALE_EU)		//TODO: chcnge this depend on MASTER CONFIGURATION
	#endif
{
	#ifdef useRdsParser
		//registerCallback(RDS_CALLBACK_AF, decodeAFFrequency);
		//unregisterCallbacks();		//clear RDS class _callbacks (I don't need theme)
	#else
		memset(&fm_rds_data, 0, sizeof(fm_rds_data_t));
	#endif
	init(imgAddress);

	if (DeviceState.UploadedImage != FMHD_active)
		DeviceState.UploadedImage = si468xBootERROR;		//TODO: wymyśleć coś lepszego
	else
		_setFMProperties();
}*/																																	//TODO: W moim przypadku spi nie działało - do poprawy


/**
  * @brief	FM object destructor.
  * @note	Clear all object data and destroy object.
  *
  * @param 	n/a
  * @retval n/a
  */
si468xFM::~si468xFM(){
	//TODO: Destroy si468xFMHD data
}


/**
  * @brief	Private function part of constructor. Sets tunning parameters
  * 		like FM_SEEK_BAND_BOTTOM, FM_SEEK_BAND_TOP,
  * 		FM_SEEK_FREQUENCY_SPACING.
  * @note   Private function used in constructor which can be executed in
  * 		i2s and spi constructor.
  * 		Auxiliary function written to shorten development period and
  * 		takes less space in flash.
  *
  * @param 	region - Sets tuner parameters like FM_SEEK_BAND_BOTTOM,
  * 				FM_SEEK_BAND_TOP, FM_SEEK_FREQUENCY_SPACING appropriate for:
  * 				0x01 - fmTuningRegionEU;
  * 				0x02 - fmTuningRegionUS;
  * 				0x03 - fmTuningRegionJP;
  * @retval bool value depend of results
  */
void si468xFM::_setFmTuningParam(uint8_t region){
	switch(region){
	case fmTuningRegionEU:
		tuningParam.FM_SEEK_BAND_BOTTOM			= 8750;
		tuningParam.FM_SEEK_BAND_TOP			= 10800;
		tuningParam.FM_SEEK_FREQUENCY_SPACING	= 5;
		break;
	case fmTuningRegionUS:
		tuningParam.FM_SEEK_BAND_BOTTOM			= 8750;
		tuningParam.FM_SEEK_BAND_TOP			= 10800;
		tuningParam.FM_SEEK_FREQUENCY_SPACING	= 5;
		break;
	case fmTuningRegionJP:
		tuningParam.FM_SEEK_BAND_BOTTOM			= 7600;
		tuningParam.FM_SEEK_BAND_TOP			= 9500;
		tuningParam.FM_SEEK_FREQUENCY_SPACING	= 5;
		break;
	};
}

/**
  * @brief	Private function part of constructor. Sets properties appropriate
  * 		to FM receiver.
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
void si468xFM::_setFMProperties(){
	setProperty(PROP_FM_AUDIO_DE_EMPHASIS, 0x1); 					//TODO: change it depend on radio MASTER CONFIGURATION
	//wait4ready();											//TODO: nie wiem czy potrzebne

	//TODO: my own RDS props: FM_RDS_INTERRUPT_SOURCE, FM_RDS_INTERRUPT_FIFO_COUNT, FM_RDS_CONFIDENCE


	#ifdef useRds
		setProperty(PROP_FM_RDS_CONFIG, 0x0001); 						// Enable RDS processor
		//setProperty(PROP_FM_RDS_CONFIDENCE,0xAAAA);
		setProperty(PROP_FM_RDS_INTERRUPT_SOURCE,0b11111);
		setProperty(PROP_FM_RDS_INTERRUPT_FIFO_COUNT,0x5);
	#endif


	setProperty(PROP_FM_AUDIO_DE_EMPHASIS, 0x1);					//TODO: chcnge this depend on MASTER CONFIGURATION (REGION)
	setProperty(PROP_FM_SEEK_BAND_BOTTOM, tuningParam.FM_SEEK_BAND_BOTTOM);
	setProperty(PROP_FM_SEEK_BAND_TOP, tuningParam.FM_SEEK_BAND_TOP);
	setProperty(PROP_FM_SEEK_FREQUENCY_SPACING, tuningParam.FM_SEEK_FREQUENCY_SPACING);
	setProperty(PROP_FM_VALID_RSSI_THRESHOLD ,0x0000);
	setProperty(PROP_FM_VALID_SNR_THRESHOLD,0x0000);
	setProperty(PROP_FM_SOFTMUTE_SNR_LIMITS , 0x0000); // set the SNR limits for soft mute attenuation
	setProperty(PROP_FM_TUNE_FE_CFG, 0x0000); // front end switch open1
	//choose Your own list of properties to set "FM" and "AM_FM"
}


/**
  * @brief	Tune FM frequency
  * @note   For more details pls take a look at FM_TUNE_FREQ in
  * 		si468x datasheet.
  *
  * @param 	kHzFreq - Frequency in multiples of 10.0 kHz added to
  * 			a starting frequency of 0 Hz
  * 		ANTCAP - When non-zero this parameter sets the antenna
  * 			tuning capacitor value to (ANTCAP-1)*250 fF
  * 			(31.75 pF Max).
  * 			Range: 0-128
  * 			0 : Automatically determines the cap setting.
  * 		TUNE_MODE - Set the desired tuning mode.
  * 			0 : Tune and render analog audio as fast as possible,
  * 			do not attempt to acquire HD.
  * 			1 : Reserved
  * 			2 : Tune and render analog audio as fast as possible,
  * 			try to acquire HD and crossfade to HD if acquired.
  * 			Always selects the Main Program Service. To select an
  * 			SPS use the START_DIGITAL_SERVICE command after the HD
  * 			has been acquired.
  * 			3 : Tune and attempt to acquire HD and render audio if
  * 			successful. The Main Program Service (MPS) will be
  * 			rendered. To select an SPS use the START_DIGITAL_SERVICE
  * 			command after the HD has been acquired.
  * 		INJECTION - Injection selection
  * 			0 : Automatic injection selection.
  * 			1 : Low-side injection.
  * 			2 : High-side injection.
  * 		DIR_TUNE - Enables the direct tune feature. When this bit is
  * 			set, the service specified in the PROG_ID field will be
  * 			rendered automatically. When this bit is 0 the MPS will be
  * 			rendered.
  * 			0 : MPS is selected.
  * 			1 : The program ID specified in PROG_ID will be rendered.
  * 		PROG_ID - This optional parameter sets the rendered program ID.
  * 			This field is required when the DIR_TUNE options bit is set
  * 			otherwise this field is ignored and can be omitted. Setting
  * 			this field to 0 when DIR_TUNE = 1 has the same effect as
  * 			a normal tune.
  * 			Range: 0-7
  * @retval n/a
  */
void si468xFM::fmTuneFreq(uint32_t kHzFreq,  uint16_t ANTCAP, uint8_t TUNE_MODE, uint8_t INJECTION, bool DIR_TUNE,  uint8_t PROG_ID){
	uint8_t args[]={
			((DIR_TUNE<<5) & 0b10000) + ((TUNE_MODE<<2) & 0b1100) + (INJECTION & 0b11),
			kHzFreq & 0xFF,
			(kHzFreq >> 8) & 0xFF,
			ANTCAP & 0xFF,
			(ANTCAP >> 8) & 0xFF,
			PROG_ID
	};
	execute_Command(FM_TUNE_FREQ, args, 6);
}


/**
  * @brief	si468x FM_RSQ_STATUS command.
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
si468xFmAmRsqStatus si468xFM::getFmRsqStatus(bool clearRSQACK, bool ATTUNE,  bool cancelSeek, bool clearSTCACK){
	uint8_t args[] ={
			((clearRSQACK << 3) & 0b1000) + ((ATTUNE<<2) & 0b100) + ((cancelSeek<<1) & 0b10) + (clearSTCACK & 0b1)
	};
	execute_Command(FM_RSQ_STATUS, args, 1);
	read_response(22);

	si468xFmAmRsqStatus retVal;
	retVal.HDLEVELHINT  =(ComunicationArrays.Respond.data[4]>>5)& 0b1;
	retVal.HDLEVELLINT	=(ComunicationArrays.Respond.data[4]>>4)& 0b1;
	retVal.SNRHINT		=(ComunicationArrays.Respond.data[4]>>3)& 0b1;
	retVal.SNRLINT		=(ComunicationArrays.Respond.data[4]>>2)& 0b1;
	retVal.RSSIHINT		=(ComunicationArrays.Respond.data[4]>>1)& 0b1;
	retVal.RSSILIN		=ComunicationArrays.Respond.data[4] & 0b1;
	retVal.BLTFT		=(ComunicationArrays.Respond.data[5]>>7)& 0b1;
	retVal.HDDETECTED	=(ComunicationArrays.Respond.data[5]>>5)& 0b1;
	retVal.FLT_HDDETECTED	=(ComunicationArrays.Respond.data[5]>>4)& 0b1;
	retVal.AFCRL		=(ComunicationArrays.Respond.data[5]>>1)& 0b1;
	retVal.VALID		=ComunicationArrays.Respond.data[5] & 0b1;
	retVal.READFREQ		=ComunicationArrays.Respond.data[7];
	retVal.READFREQ		= (retVal.READFREQ<<8) + ComunicationArrays.Respond.data[6];
	retVal.FREQOFF		=ComunicationArrays.Respond.data[8];
	retVal.RSSI			=ComunicationArrays.Respond.data[9];
	retVal.SNR			=ComunicationArrays.Respond.data[10];
	retVal.fmMULT_amMOD	=ComunicationArrays.Respond.data[11];
	retVal.READANTCAP	=ComunicationArrays.Respond.data[13];
	retVal.READANTCAP	= (retVal.READANTCAP<<8) + ComunicationArrays.Respond.data[12];
	retVal.HDLEVEL		=ComunicationArrays.Respond.data[15];
	retVal.FILTERED_HDLEVEL	=ComunicationArrays.Respond.data[16];

	return retVal;
}

/**
  * @brief	Start seek valid FM station from current frequency.
  * @note   For more details pls take a look at FM_SEEK_START in
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
void si468xFM::fmSeekStart(bool seekDirection, bool WRAP, bool FORCE_WB, uint8_t TUNE_MODE, uint8_t INJECTION, uint16_t ANTCAP){
	uint8_t args[]={
			((FORCE_WB<<4) & 0b10000) + ((TUNE_MODE<<2) & 0b1100) + (INJECTION & 0b11),
			((seekDirection <<1 ) & 0b10) + (WRAP & 0b1),
			0x0,
			ANTCAP & 0xFF,
			(ANTCAP >> 8) & 0xFF,
	};
	execute_Command(FM_SEEK_START, args, 5);
}

/**
  * @brief	si468x FM band scan routine.
  * @note	For more details pls take a look at si468x datasheet:
  * 		5. Flowcharts, 5.4. Data Service Receiver—FMHD–Band Scan
  *
  * @param	kHzFreq - Frequency in multiples of 10.0 kHz added to
  * 			a starting frequency of 0 Hz

  *			seekDirection - determines direction of seek and band limit.
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
  * @retval uint16_t - FM tuned frequency or frequency where scan stops
*/
uint16_t si468xFM::fmBandScan(uint32_t kHzFreq, bool seekDirection, bool WRAP, uint16_t ANTCAP, /*uint8_t TUNE_MODE,*/ uint8_t INJECTION, bool DIR_TUNE,  uint8_t PROG_ID,  bool FORCE_WB){
	si468xFmAmRsqStatus FmRsqStatus;
	si468xFmRdsStatus RDSStatus;

	if (kHzFreq>tuningParam.FM_SEEK_BAND_TOP)
		FmRsqStatus.READFREQ = kHzFreq>tuningParam.FM_SEEK_BAND_TOP;
	else
		FmRsqStatus.READFREQ = kHzFreq;

	fmTuneFreq(FmRsqStatus.READFREQ /*kHzFreq*/, ANTCAP, 3, INJECTION, DIR_TUNE, PROG_ID);
	//fmSeekStart(seekDirection, WRAP, FORCE_WB, DIR_TUNE, INJECTION, ANTCAP);		//TODO: pokombinować z tą linijką. generalnie jest tak, że jeżeli np. na 100.2 Mhz jest RMF i w fmTuneFreq
																					//		wstawimy ta wartość, to bez tej linijki fmBandScan nie pójdzie w górę / dół bez tej linijki.
																					//		może pokombinować z kHzFreq +/- FM_steep wtedy zawsze ruszy syę zakres (ale co z granizami


		do
		{
			//Is STCINT set??
			do{
				wait4STCint();

				//setInterruptAsHandled(&DeviceState.STATUS0, 1);
				setInterruptAsHandled_STC();
				FmRsqStatus = getFmRsqStatus(true, 0, 0, 1);

				if (!FmRsqStatus.VALID)
					fmSeekStart(seekDirection, WRAP, FORCE_WB, DIR_TUNE, INJECTION, ANTCAP);

			}while ((FmRsqStatus.VALID!=1)&&(FmRsqStatus.BLTFT!=1));   //Nie wiem czy nie lepiej sprawdzać to po FmRsqStatus.SNRHINT i FmRsqStatus.SNRLINT
			//SNR OK?

			#ifdef useRds
				if (FmRsqStatus.VALID==1){

						//Is RDSSYNC set?
						if (_isRDSSyncSet()==true)
						{
							break;
						}
					}
			#endif

			} while((FmRsqStatus.VALID!=1)&&(FmRsqStatus.BLTFT!=1));

			#ifdef useRds
				if (FmRsqStatus.VALID==1){

						//read & parse RDSData
						uint16_t	RDSCounter = 500;
						while(RDSCounter--)
						{
							RDSStatus = getFmRdsStatus(0, 0, 0);
							//				if ((RDSStatus.blockError.BLEA !=3)
							//						&&(RDSStatus.blockError.BLEB !=3)
							//						&&(RDSStatus.blockError.BLEC !=3)
							//						&&(RDSStatus.blockError.BLED !=3))
							#ifdef useAdvRdsParser
									decodeRDSGroup(RDSStatus.BLOCkx);
							#else
									rds_parse((uint16_t*)&RDSStatus.BLOCkx);
							#endif
						};
						//read & parse RDSData

				}
			#endif
	return FmRsqStatus.READFREQ;
}

/**
  * @brief	Private function check in countdown loop (max. 500
  * 		times) if RDSSYNC bit is set.
  * @note   n/a
  *
  * @param 	n/a
  * @retval bool depend on results
  */
bool si468xFM::_isRDSSyncSet(){
	uint16_t	RDSCounter = 500;
	while(RDSCounter--)
	{
		if (getFmRdsStatus(1, 0, 0).RDSSYNC== true)
		{
			return true;
		}
	};
	return false;
}

/**
  * @brief	si468x FM_RDS_BLOCKCOUNT command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	clearsBlockcount - Clears the block counts if set.
  * 			The current block counts will be reported before
  * 			they are cleared.
  * @retval si468xFmRdsBlockcount - structure which ingredients are
  * 				 similar as in FM_RDS_BLOCKCOUNT command respond.
*/
si468xFmRdsBlockcount si468xFM::getFmRdsBlockcount(bool clearsBlockcount){
	si468xFmRdsBlockcount retVal;
	uint8_t args[] ={
			(clearsBlockcount & 0b1)
	};

	execute_Command(FM_RDS_BLOCKCOUNT, args, 1);
	read_response(10);
	retVal.EXPECTED = (ComunicationArrays.Respond.data[5]<<8) + (ComunicationArrays.Respond.data[4]);
	retVal.RECEIVED = (ComunicationArrays.Respond.data[7]<<8) + (ComunicationArrays.Respond.data[6]);
	retVal.UNCORRECTABLE= (ComunicationArrays.Respond.data[9]<<8) + (ComunicationArrays.Respond.data[8]);
	return retVal;
}

#ifdef useRds
	#ifndef useAdvRdsParser
		uint8_t si468xFM::rds_parse(uint16_t *block)
		{
				uint8_t addr;
				fm_rds_data.pi = block[0];
				if((block[1] & 0xF800) == 0x00){ // group 0A
						addr = block[1] & 0x03;
						fm_rds_data.ps_name[addr*2] = (block[3] & 0xFF00)>>8;
						fm_rds_data.ps_name[addr*2+1] = block[3] & 0xFF;
						fm_rds_data.group_0a_flags |= (1<<addr);
				}else if((block[1] & 0xF800)>>11 == 0x04){ // group 2A
						addr = block[1] & 0x0F;
						if((block[1] & 0x10) == 0x00){ // parse only string A
								fm_rds_data.radiotext[addr*4] = (block[2] & 0xFF00)>>8;
								fm_rds_data.radiotext[addr*4+1] = (block[2] & 0xFF);
								fm_rds_data.radiotext[addr*4+2] = (block[3] & 0xFF00)>>8;
								fm_rds_data.radiotext[addr*4+3] = (block[3] & 0xFF);

								if(fm_rds_data.radiotext[addr*4] == '\r'){
										fm_rds_data.radiotext[addr*4] = 0;
										fm_rds_data.group_2a_flags = 0xFFFF;
								}
								if(fm_rds_data.radiotext[addr*4+1] == '\r'){
										fm_rds_data.radiotext[addr*4+1] = 0;
										fm_rds_data.group_2a_flags = 0xFFFF;
								}
								if(fm_rds_data.radiotext[addr*4+2] == '\r'){
										fm_rds_data.radiotext[addr*4+2] = 0;
										fm_rds_data.group_2a_flags = 0xFFFF;
								}
								if(fm_rds_data.radiotext[addr*4+3] == '\r'){
										fm_rds_data.radiotext[addr*4+3] = 0;
										fm_rds_data.group_2a_flags = 0xFFFF;
								}
								fm_rds_data.group_2a_flags |= (1<<addr);
						}
				}
				if(fm_rds_data.group_0a_flags == 0x0F &&
								fm_rds_data.group_2a_flags == 0xFFFF){
						fm_rds_data.ps_name[8] = 0;
						fm_rds_data.radiotext[128] = 0;
						return 1;
				}
				return 0;
		}
	#endif
#endif

/**
  * @brief	si468x FMACF_STATUS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	clearACFACK - Clears ACFINT and any ACF interrupt bits
  * 						if set.
  * @retval si468xFmAmAcfStatus - structure which ingredients are
  * 				 similar as in AM_ACF_STATUS and FM_ACF_STATUS
  * 				 command respond.
*/
si468xFmAmAcfStatus si468xFM::getFmAcfStatus(bool clearACFACK){
	uint8_t args[] ={
			(clearACFACK&0b1)
		};
	execute_Command(FM_ACF_STATUS, args, 1);
	read_response(11);
	si468xFmAmAcfStatus retVal;

	retVal.BLEND_INT_fmOnly		=	((ComunicationArrays.Respond.data[4]>>2)&0b1);
	retVal.HIGHCUT_INT			=	((ComunicationArrays.Respond.data[4]>>1)&0b1);
	retVal.SMUTE_INT			=	(ComunicationArrays.Respond.data[4]&0b1);
	retVal.BLEND_CONV_fmOnly	=	((ComunicationArrays.Respond.data[5]>>6)&0b1);
	retVal.HIGHCUT_CONV			=	((ComunicationArrays.Respond.data[5]>>5)&0b1);
	retVal.SMUTE_CONV			=	((ComunicationArrays.Respond.data[5]>>4)&0b1);
	retVal.BLEND_STATE_fmOnly	=	((ComunicationArrays.Respond.data[5]>>2)&0b1);
	retVal.HIGHCUT_STATE		=	((ComunicationArrays.Respond.data[5]>>1)&0b1);
	retVal.SMUTE_STATE			=	(ComunicationArrays.Respond.data[5]&0b1);
	retVal.ATTN					=	(ComunicationArrays.Respond.data[6]&0b11111);
	retVal.HIGHCUT				=	(ComunicationArrays.Respond.data[7]);
	retVal.PILOT_fmOnly			=	((ComunicationArrays.Respond.data[8]>>7)&0b1);
	retVal.fmSTBLEND_amLOWCUT	=	(ComunicationArrays.Respond.data[8]&0b1111111);

	return retVal;
}

/**
  * @brief	si468x FM_RDS_STATUS command.
  * @note	For more details pls take a look at si468x datasheet.
  *
  * @param	STATUSONLY - Determines if data should be removed from the RDS receive FIFO.
  * 			0 : Data in BLOCKA, BLOCKB, BLOCKC, BLOCKD and BLE contain the oldest
  * 			data in the RDS FIFO and are removed from the FIFO.
  * 			1 : Data in BLOCKA, BLOCKB, BLOCKC, BLOCKD and BLE will contain the last
  * 			valid block A data received for the current station. Data in BLOCKB will
  * 			contain the last valid block B data received for the current station.
  * 			Data in BLE will describe the bit errors for the data in BLOCKA and BLOCKB.
  * 		clearMTFIFO - Clears the FIFO counts of the RDS Receive FIFO to zero if set.
  * 			The FIFO counts will always be cleared during FM_TUNE_FREQ and FM_SEEK_START.
  * 		clearsINTACK - Clears the STATUS:RDSINT bit if set.
  * @retval si468xFmRdsStatus - structure which ingredients are
  * 				 similar as in FM_RDS_STATUS command respond.
*/
si468xFmRdsStatus si468xFM::getFmRdsStatus(bool STATUSONLY, bool clearMTFIFO, bool clearsINTACK){

	uint8_t args[] ={
			((STATUSONLY<<2)&0b100)+((clearMTFIFO<<1)&0b10)+(clearsINTACK&0b1)
	};
	execute_Command(FM_RDS_STATUS, args, 1);
	read_response(20);
	si468xFmRdsStatus retVal;

	retVal.RDSTPPTYINT	= (ComunicationArrays.Respond.data[4]>>4)&0b1;
	retVal.RDSPIINT		= (ComunicationArrays.Respond.data[4]>>3)&0b1;
	retVal.RDSSYNCINT	= (ComunicationArrays.Respond.data[4]>>1)&0b1;
	retVal.RDSFIFOINT	= (ComunicationArrays.Respond.data[4])&0b1;
	retVal.TPPTYVALID	= (ComunicationArrays.Respond.data[5]>>4)&0b1;
	retVal.PIVALID		= (ComunicationArrays.Respond.data[5]>>3)&0b1;
	retVal.RDSSYNC		= (ComunicationArrays.Respond.data[5]>>1)&0b1;
	retVal.RDSFIFOLOST	= (ComunicationArrays.Respond.data[5])&0b1;
	retVal.TP			= (ComunicationArrays.Respond.data[6]>>5)&0b1;
	retVal.PTY			= (ComunicationArrays.Respond.data[6])&0b11111;
	retVal.PI			= (ComunicationArrays.Respond.data[9]<<8) + (ComunicationArrays.Respond.data[8]);
	retVal.RDSFIFOUSED	= (ComunicationArrays.Respond.data[10]);
	retVal.blockError.BLEx		= (ComunicationArrays.Respond.data[11]);
	retVal.BLOCkx[0] 	= (ComunicationArrays.Respond.data[13]<<8) + (ComunicationArrays.Respond.data[12]);
	retVal.BLOCkx[1]	= (ComunicationArrays.Respond.data[15]<<8) + (ComunicationArrays.Respond.data[14]);
	retVal.BLOCkx[2] 	= (ComunicationArrays.Respond.data[17]<<8) + (ComunicationArrays.Respond.data[16]);
	retVal.BLOCkx[3] 	= (ComunicationArrays.Respond.data[19]<<8) + (ComunicationArrays.Respond.data[18]);
	return retVal;
}



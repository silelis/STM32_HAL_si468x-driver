/*
 * si468xFM.h
 *
 *  Created on: Jan 19, 2021
 *      Author: dbank
 */

#ifndef SRC_SILELIS_CODE_SI468XFM_H_
#define SRC_SILELIS_CODE_SI468XFM_H_

#include "si468xCore.h"

#define fmTuningRegionEU	0x1
#define fmTuningRegionUS	0x2
#define fmTuningRegionJP	0x3

struct si468xFmAmRsqStatus{
	bool HDLEVELHINT;
	bool HDLEVELLINT;
	bool SNRHINT;
	bool SNRLINT;
	bool RSSIHINT;
	bool RSSILIN;
	bool BLTFT;
	bool HDDETECTED;
	bool FLT_HDDETECTED;
	bool AFCRL;
	bool VALID;
	uint16_t READFREQ;
	uint8_t	FREQOFF;
	uint8_t	RSSI;
	uint8_t SNR;
	uint8_t fmMULT_amMOD;
	uint16_t READANTCAP;
	uint8_t HDLEVEL;
	uint8_t FILTERED_HDLEVEL;
};


struct si468xFmRdsStatus{
	bool RDSTPPTYINT;
	bool RDSPIINT;
	bool RDSSYNCINT;
	bool RDSFIFOINT;
	bool TPPTYVALID;
	bool PIVALID;
	bool RDSSYNC;
	bool RDSFIFOLOST;
	bool TP;
	uint8_t PTY;
	uint16_t PI;
	uint8_t RDSFIFOUSED;

	union{
		struct{
			uint8_t BLEA : 2;
			uint8_t BLEB : 2;
			uint8_t BLEC : 2;
			uint8_t BLED : 2;
		};
		uint8_t BLEx;
	} blockError;
	uint16_t BLOCkx[4];
};

struct si468xFmRdsBlockcount{
	uint16_t EXPECTED;
	uint16_t RECEIVED;
	uint16_t UNCORRECTABLE;
};


struct si468xFmAmAcfStatus{
	bool BLEND_INT_fmOnly;		//fm only
	bool HIGHCUT_INT;
	bool SMUTE_INT;
	bool BLEND_CONV_fmOnly;		//fm only
	bool HIGHCUT_CONV;
	bool SMUTE_CONV;
	bool BLEND_STATE_fmOnly;	//fm only
	bool HIGHCUT_STATE;
	bool SMUTE_STATE;
	uint8_t ATTN;
	uint8_t HIGHCUT;
	bool PILOT_fmOnly;	//fm only
	uint8_t fmSTBLEND_amLOWCUT;
};

/*
 * si468xCore  is class to manage FM tuner.
 *
 */
class si468xFM: public si468xCore
	#ifdef useRds
		#ifdef useAdvRdsParser
			, public	RDSDecoder
			, public	RDSTranslator
		#endif
	#endif
{

public:
	si468xFM(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress, uint32_t imgAddress, uint8_t region);
	//si468xFM(GPIO_TypeDef* _resetPort, uint16_t _resetPin, SPI_HandleTypeDef*_hspi, GPIO_TypeDef* _spiEnablePort, uint16_t _spiEnablePin, uint32_t imgAddress);	//TODO: W moim przypadku spi nie działało - do poprawy


	//TODO: konstruktor dla SPI i INTERRUPT PIN
	~si468xFM();
	void fmTuneFreq(uint32_t kHzFreq,  uint16_t ANTCAP=0, uint8_t TUNE_MODE=0, uint8_t INJECTION=0, bool DIR_TUNE=0,  uint8_t PROG_ID=0);
	si468xFmAmRsqStatus getFmRsqStatus(bool clearRSQACK=0, bool ATTUNE=0,  bool cancelSeek=0, bool clearSTCACK=1);
	void fmSeekStart(bool seekDirection, bool WRAP=0, bool FORCE_WB=0, uint8_t TUNE_MODE=0, uint8_t INJECTION=0, uint16_t ANTCAP=0);
	uint16_t fmBandScan(uint32_t kHzFreq, bool seekDirection, bool WRAP= 0, uint16_t ANTCAP=0, /*uint8_t TUNE_MODE=0,*/ uint8_t INJECTION=0, bool DIR_TUNE=0,  uint8_t PROG_ID=0,  bool FORCE_WB=0);
	si468xFmRdsStatus getFmRdsStatus(bool STATUSONLY=0, bool clearMTFIFO=0, bool clearsINTACK=0);
	si468xFmRdsBlockcount getFmRdsBlockcount(bool clearsBlockcount);
	si468xFmAmAcfStatus getFmAcfStatus(bool clearACFACK);
	#ifdef useRds
		#ifndef useAdvRdsParser
			uint8_t rds_parse(uint16_t *block);
		#endif
	#endif
protected:

private:
	bool _constructorFMCommon(uint32_t imgAddress, uint8_t region);
	void _setFMProperties(void);
	bool _isRDSSyncSet();
	void _setFmTuningParam(uint8_t region);

	#ifdef useRds
		#ifndef useAdvRdsParser
			fm_rds_data_t fm_rds_data;
		#endif
	#endif
	struct {
				uint16_t FM_SEEK_BAND_BOTTOM;
				uint16_t FM_SEEK_BAND_TOP;
				uint16_t FM_SEEK_FREQUENCY_SPACING;
			}tuningParam;

};


#endif /* SRC_SILELIS_CODE_SI468XFM_H_ */

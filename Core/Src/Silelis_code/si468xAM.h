/*
 * si468xAM.h
 *
 *  Created on: Feb 1, 2021
 *      Author: dbank
 */

#ifndef SRC_SILELIS_CODE_SI468XAM_H_
#define SRC_SILELIS_CODE_SI468XAM_H_

#include "si468xCore.h"
#include "si468xFM.h"

#define amSpacingRegionEU_JP	9
#define amTuningRegionUS		10

/*
 * si468xCore  is class to manage AM tuner.
 *
 */
class si468xAM: public si468xCore{
public:
	si468xAM(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress, uint32_t imgAddress, uint8_t freqSpacing);
	~si468xAM();
	uint16_t amBandScan(uint32_t kHzFreq,  bool seekDirection, bool WRAP=0, uint16_t ANTCAP=0, uint8_t TUNE_MODE=0, uint8_t INJECTION=0, bool FORCE_WB=0);
	void amTuneFreq(uint32_t kHzFreq,  uint16_t ANTCAP=0, uint8_t TUNE_MODE=0, uint8_t INJECTION=0);
	void amSeekStart(bool seekDirection, bool WRAP=0, bool FORCE_WB=0, uint8_t TUNE_MODE=0, uint8_t INJECTION=0, uint16_t ANTCAP=0);
	si468xFmAmRsqStatus getAmRsqStatus(bool clearRSQACK=0, bool ATTUNE=0,  bool cancelSeek=0, bool clearSTCACK=1);
	si468xFmAmAcfStatus getAmAcfStatus(bool clearACFACK);

private:
	bool _constructorAMCommon(uint32_t  imgAddress);
	void _setAMProperties();
	void _setAmTuningParam(uint8_t spacing);

	struct{
		uint16_t AM_SEEK_BAND_BOTTOM;
		uint16_t AM_SEEK_BAND_TOP;
		uint8_t AM_SEEK_FREQUENCY_SPACING;
	}tuningParam;
};

#endif /* SRC_SILELIS_CODE_SI468XAM_H_ */

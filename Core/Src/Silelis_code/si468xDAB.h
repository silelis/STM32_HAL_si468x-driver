/*
 * si468xDAB.h
 *
 *  Created on: Jan 19, 2021
 *      Author: dbank
 */

#ifndef SRC_SILELIS_CODE_SI468XDAB_H_
#define SRC_SILELIS_CODE_SI468XDAB_H_

#include "si468xCore.h"
#include "SileliSMemDynamicAlocator.h"

	const uint32_t dabFreqListOnFlash_DAB[38] = {				//TODO: chceck if it is common or only European ???
			CHAN_5A, CHAN_5B, CHAN_5C, CHAN_5D, CHAN_6A, CHAN_6B, CHAN_6C, CHAN_6D, CHAN_7A, CHAN_7B,
			CHAN_7C, CHAN_7D, CHAN_8A, CHAN_8B, CHAN_8C, CHAN_8D, CHAN_9A, CHAN_9B, CHAN_9C, CHAN_9D,
			CHAN_10A, CHAN_10B, CHAN_10C, CHAN_10D, CHAN_11A, CHAN_11B, CHAN_11C, CHAN_11D, CHAN_12A, CHAN_12B,
			CHAN_12C, CHAN_12D, CHAN_13A, CHAN_13B, CHAN_13C, CHAN_13D, CHAN_13E, CHAN_13F
/*			174928, 176640, 178352, 180064, 181936, 183648, 185360, 187072, 188928, 190640,
			192352, 194064, 195936, 197648, 199360, 201072, 202928, 204640, 206352, 208064,
			209936, 211648, 213360, 215072, 216928, 218640, 220352, 222064, 223936, 225648,
			227360, 229072, 230748, 232496, 234208, 235776, 237448, 239200*/
	};
	//const uint32_t dabFreqListOnFlash_tDMB [] = {};			//todo: set this frequency list

struct DabComponentInfo{
	uint8_t GLOBAL_ID;
	uint8_t LANG;
	uint8_t CHARSETID;
	uint8_t LABEL0;
	uint8_t LABEL1;
	uint8_t LABEL2;
	uint8_t LABEL3;
	uint8_t LABEL4;
	uint8_t LABEL5;
	uint8_t LABEL6;
	uint8_t LABEL7;
	uint8_t LABEL8;
	uint8_t LABEL9;
	uint8_t LABEL10;
	uint8_t LABEL11;
	uint8_t LABEL12;
	uint8_t LABEL13;
	uint8_t LABEL14;
	uint8_t LABEL15;
	uint16_t CHAR_ABREV;
	uint8_t NUMUA;
	uint8_t LENUA;
	uint16_t UATYPE;
	uint8_t UADATALEN;
	uint8_t* pUADATAx;
};

struct DabAcfStatus{
	uint8_t RFU1;
	uint8_t RFU2;
	uint16_t AUDIO_LEVEL;
	uint16_t CMFT_NOISE_LEVEL;

};

struct DabTime{
	uint16_t YEAR;
	uint8_t MONTHS;
	uint8_t DAYS;
	uint8_t HOURS;
	uint8_t MINUTES;
	uint8_t	SECONDS;
};

struct DabAudioInfo{
	uint16_t AUDIO_BIT_RATE;
	uint16_t AUDIO_SAMPLE_RATE;
	bool AUDIO_PS_FLAG;
	bool AUDIO_SBR_FLAG;
	uint8_t AUDIO_MODE;
	uint8_t AUDIO_DRC_GAIN;
};

struct DabAnnouncementInfo{
	bool ANNO_Q_OVFL;
	uint8_t ANNO_Q_SIZE;
	uint8_t CLUSTER_ID;
	bool ANNO_STAT;
	bool REGION_FLAG;
	uint8_t SRC;
	uint16_t ASW;
	uint16_t ID1;
	uint16_t ID2;
	uint8_t REGIONID1;
	uint8_t REGIONID2;
};

struct DabEventStatus{
	bool RECFGINT;
	bool RECFGWRNINT;
	bool ANNOINT;
	bool OESERVINT;
	bool SERVLINKINT;
	bool FREQINFOINT;
	bool SVRLISTINT;
	bool ANNO;
	bool OESERV;
	bool SERVLINK;
	bool FREQ_INFO;
	bool SVRLIST;
	uint16_t SVRLISTVER;
};

struct DabDigitalStatus{
	bool FICERRINT;
	bool ACQINT;
	bool RSSIHINT;
	bool RSSILINT;
	bool FICERR;
	bool ACQ;
	bool VALID;
	uint8_t RSSI;
	uint8_t SNR;
	uint8_t FIC_QUALITY;
	uint8_t CNR;
	uint16_t FIB_ERROR_COUNT;
	uint32_t TUNE_FREQ;
	uint8_t TUNE_INDEX;
	uint8_t FFT_OFFSET;
	uint16_t READANTCAP;
	uint16_t CU_LEVEL;
	uint8_t FAST_DECT;
};

struct DabGetTunedFreqInfo{
	uint32_t LENGTH_FI_LIST;
	uint32_t FI_ID_0;
	uint32_t FI_FREQ_0;
	uint8_t FI_FREQ_INDEX_0;
	uint8_t FI_RNM_0;
	uint8_t FI_CONTINUITY_0;
	uint8_t FI_CONTROL_0;
};

struct DigitalServiceListRaw{			//struktura przechowująca "surową serwis listę (a dokładnie kopię odpowiedzi z s
	uint16_t sizeOfServiceList;
	uint8_t* pServiceListData;
};


#define MAX_SERVICES 32		//todo: make it dynamic
#define MAX_COMPONENTS 15

struct dab_service_t{
	uint32_t service_id;
	uint8_t service_info1;
	uint8_t service_info2;
	uint8_t service_info3;
	char service_label[17];
	uint8_t num_components;
	// only one component by now
	uint16_t component_id[MAX_COMPONENTS];
	uint8_t component_info[MAX_COMPONENTS];
	uint8_t component_valid_flags[MAX_COMPONENTS];
};

struct dab_service_list_t{
	uint16_t list_size;
	uint16_t version;
	uint8_t num_services;
	struct dab_service_t services[MAX_SERVICES];
};

/*
 * si468xCore  is class to manage Digital Audio Broadcasting tuner.
 *
 */
class si468xDAB: public si468xCore
{
public:
	si468xDAB(GPIO_TypeDef* _resetPort, uint16_t _resetPin, I2C_HandleTypeDef *_hi2c, uint16_t _DevAddress, uint32_t imgAddress, uint32_t* pFreqList, uint8_t itemsOnFerqList);
	//si468xDAB(GPIO_TypeDef* _resetPort, uint16_t _resetPin, SPI_HandleTypeDef *_hspi, GPIO_TypeDef* _spiEnablePort, uint16_t _spiEnablePin, uint32_t imgAddress, uint32_t* pFreqList, uint8_t itemsOnFerqList);

	//TODO: konstruktor dla SPI i INTERRUPT PIN
	~si468xDAB();
	uint8_t dabBandScan(uint8_t FREQ_INDEX /*should be in range of uint8_t)*/, uint8_t serviceType, bool seekDirection= si468xSeekUp, uint16_t ANTCAP=0, uint8_t INJECTION=0);
	uint16_t dabParsedServiceListSize(void);
	bool dabStartDigitalService(uint8_t ServiceNo, bool SERTYPE= false);
	bool dabStopDigitalService(uint8_t ServiceNo, bool SERTYPE= false);


protected:
	DabGetTunedFreqInfo dabGetFreqInfo(void);
	void getDabFrequencyList(void);
	void setDabFrequencyList(uint32_t* pFreqList, uint8_t itemsOnFerqList);
	DabComponentInfo getDabComponentInfo(uint32_t SERVICEID, uint32_t COMPID);
	DabAcfStatus getDabAcfStatus(void);
	DabTime getDabTime(bool TIME_TYPE=0);
	DabAudioInfo getDabAudioInfo(void);
	DabAnnouncementInfo getDabAnnouncementInfo(void);
	DabEventStatus	getDabEvebtStatus(bool clearEVENT_ACK =0);
	DabDigitalStatus getDabDigradStatus(bool clearDIGRAD_ACK=0, bool ATTUNE=0, bool clearFIBERR_ACK=0, bool clearSTC_ACK=1);		//i.e. returns service frequency as well as freq_index
	void dabTuneFreq(uint8_t FREQ_INDEX, uint16_t ANTCAP=0, uint8_t INJECTION=0);
	bool GetDigitalSerivceList(uint8_t SERTYPE);
	bool dabParseDigitalServiceListRAW(void);
	void sortParsedServiceList(void);

private:
	void _dabStartStopDigitalService(uint8_t StopStart, uint8_t ServiceNo, bool SERTYPE);
	bool _constructorDABCommon(uint32_t imgAddress, uint32_t* pFreqList, uint8_t itemsOnFerqList);
	void _setDABProperties(void);

	bool _createArray_DabFreqListOnChip(uint8_t allocationSize); //refers to: si468xDabFreqListOnChip
	void _clearData_DabFreqListOnChip(void);				//refers to: si468xDabFreqListOnChip
	void _destroyArray_DabFreqListOnChip(void);				//refers to: si468xDabFreqListOnChip

	bool _createArray_RawDigitalServiceList(uint16_t allocationSize);
	void _clearData_RawDigitalServiceList(void);		//refers to:dynamicalyAlocatedData.getDigitalServiceList
	void _destroyArray_RawDigitalServiceList(void);		//refers to:dynamicalyAlocatedData.getDigitalServiceList
	void _swap_services(uint8_t first, uint8_t second);



	struct _dynamicalyAlocatedData{			//should be deleted in the destructor

		struct {
			uint8_t NUM_FREQS;
			uint32_t* pFrequTable;
		} si468xDabFreqListOnChip;		//aka tuningParameters

		DigitalServiceListRaw si468xDigitalServiceListRAW;		//output of GET_DIGITAL_SERWICE_LIST
		dab_service_list_t dabServiceListParsed;					//parsed (from si468xDigitalServiceListRAW) service list
		char* service_label;
	} dynamicalyAlocatedData;
};

#endif /* SRC_SILELIS_CODE_SI468XDAB_H_ */

/*
 * si468x_defines.h
 *
 *  Created on: Dec 21, 2020
 *      Author: dbank
 */

#ifndef _SI468X_DEFINES_H_
#define _SI468X_DEFINES_H_


//FM AM seek parameters
#define si468xSeekDown		0
#define si468xSeekUp		1
#define si468xWRAPHaltSeek	0
#define si468xWRAPSeekFromOppositeBandLimit	1
//FM AM seek parameters


// General:
//#define Si4684_ADDRESS			0x64

//Commands common FMHD DAB AMHD
#define RD_REPLY				0x00		//Returns the status byte and data for the last command sent to the device.
#define POWER_UP				0x01		//Power-up the device and set system settings.
#define HOST_LOAD               0x04		//Loads an image from HOST over command interface
#define FLASH_LOAD              0x05		//Loads an image from external FLASH over secondary SPI bus
#define LOAD_INIT               0x06		//Prepares the bootloader to receive a new image.
#define BOOT					0x07		//Boots the image currently loaded in RAM.
#define GET_PART_INFO			0x08		//Reports basic information about the device.
#define GET_SYS_STATE           0x09		// Reports system state information.
#define GET_POWER_UP_ARGS       0x0A		//Reports basic information about the device such as arguments used during POWER_UP.
//#define READ_OFFSET				0x10		//Reads a portion of response buffer from an offset.
											//todo: I will do this when I will find more time to make investigation
#define GET_FUNC_INFO			0x12		//Returns the Function revision information of the device.
#define SET_PROPERTY            0x13		//Sets the value of a property.
#define	GET_PROPERTY			0x14		//Retrieve the value of a property.
#define GET_AGC_STATUS			0x17 			//Reports the status of the AGC.
#define TEST_GET_RSSI			0xE5 		//Returns the reported RSSI in 8.8 format.


//Commands Summary—FMHD
#define FM_TUNE_FREQ			0x30 		//Tunes the FM receiver to a frequency in 10 kHz steps.
#define FM_SEEK_START			0x31 		//Initiates a seek for a channel that meets the validation criteria for FM.
#define FM_RSQ_STATUS			0x32		//Returns status information about the received signal quality.
#define FM_ACF_STATUS 			0x33 		//Returns status information about automatically controlled features.
#define FM_RDS_STATUS			0x34 		//Queries the status of RDS decoder and Fifo.
#define FM_RDS_BLOCKCOUNT		0x35 		//Queries the block statistic info of RDS decoder.



//Commands Summary—AMHD
#define AM_TUNE_FREQ 			0x40 		//Tunes the AM receiver to a frequency in 1 kHz steps.
#define AM_SEEK_START 			0x41 		//Initiates a seek for a channel that meets the validation criteria for AM.
#define AM_RSQ_STATUS 			0x42 		//Returns status information about the received signal quality.
#define AM_ACF_STATUS 		0x43 		//Returns status information about automatically controlled features.



//Commands Summary—FMHD AMHD
//#define HD_DIGRAD_STATUS		0x92 		//Returns status information about the digital radio and ensemble.
//#define HD_GET_EVENT_STATUS	0x93 		//Gets information about the various events related to the HD services.
//#define HD_GET_STATION_INFO	0x94 		//Retrieves information about the ensemble broadcaster.
//#define HD_GET_PSD_DECODE		0x95 		//Retrieves PSD information.
//#define HD_GET_ALERT_MSG		0x96 		//Retrieves the HD Alert message.
//#define HD_PLAY_ALERT_TONE	0x97 		//Plays the HD Alert Tone.
//#define HD_TEST_GET_BER_INFO	0x98 		//Reads the current BER information.
//#define HD_SET_ENABLED_PORTS	0x99 		//Sets default ports retrieved after acquisition.
//#define HD_GET_ENABLED_PORTS	0x9A 		//Gets default ports retrieved after acquisition.
//#define HD_ACF_STATUS			0x9C 		//This command Returns status information about HD automatically controlled features.



//Commands Summary—DAB
//#define WRITE_STORAGE 		0x15		//Writes data to the on board storage area at a specified offset.
//#define READ_STORAGE 			0x16		//Reads data from the on board storage area from a specified offset.
#define GET_DIGITAL_SERVICE_LIST 	0x80 	//	Gets a service list of the ensemble.
#define START_DIGITAL_SERVICE	0x81 		//Starts an audio or data service.
#define STOP_DIGITAL_SERVICE	0x82 		//Stops an audio or data service.
//#define GET_DIGITAL_SERVICE_DATA	0x84 	//Gets a block of data associated with one of the enabled data components of a digital services.
#define DAB_TUNE_FREQ			0xB0		//Tunes the DAB Receiver to tune to a frequency between 168.16 and 239.20 MHz defined by the frequency table through DAB_SET_FREQ_LIST.
#define DAB_DIGRAD_STATUS 	0xB2		//Returns status information about the digital radio and ensemble.
#define DAB_GET_EVENT_STATUS 	0xB3		//Gets information about the various events related to the DAB radio.
//#define DAB_GET_ENSEMBLE_INFO 0xB4		//Gets information about the current ensemble
//#define DAB_GET_ANNOUNCEMENT_SUPPORT_INFO		0xB5		//DAB_GET_ANNOUNCEMENT_SUPPORT_INFO gets the announcement support information.
#define DAB_GET_ANNOUNCEMENT_INFO 			0xB6		//DAB_GET_ANNOUNCEMENT_INFO gets announcement information from the announcement queue.
//#define DAB_GET_SERVICE_LINKING_INFO 			0xB7 		//Provides service linking (FIG 0/6) information for the passed in service ID.
#define DAB_SET_FREQ_LIST 	0xB8 		//Sets the DAB frequency table. The frequencies are in units of kHz.
#define DAB_GET_FREQ_LIST 	0xB9 		//Gets the DAB frequency table
//#define DAB_GET_COMPONENT_INFO	0xBB 		//Gets information about components within the ensemble if available.
#define DAB_GET_TIME 			0xBC 		//Gets the ensemble time adjusted for the local time offset or the UTC.
#define DAB_GET_AUDIO_INFO 	0xBD 		//Gets audio service info
//#define DAB_GET_SUBCHAN_INFO 	0xBE 		//Gets sub-channel info
#define DAB_GET_FREQ_INFO 	0xBF 		//Gets ensemble freq info
//#define DAB_GET_SERVICE_INFO 	0xC0 		//Gets information about a service.
//#define DAB_GET_OE_SERVICES_INFO 				0xC1 		//Provides other ensemble (OE) services (FIG 0/24) information for the passed in service ID.
#define DAB_ACF_STATUS 		0xC2 		//Returns status information about automatically controlled features.
//#define DAB_TEST_GET_BER_INFO	0xE8 		//Reads the current BER rate




//NVSPI Flash Pass-through Command Summary
//#define	FLASH_CHECK_CRC32								0x02	//Check CRC32 of a sequence of blocks on flash
//#define	FLASH_ERASE_CHIP								0xFF 	//Erase the entire flash
//#define	FLASH_ERASE_SECTOR								0xFE 	//Erase a single sector of flash
//#define	FLASH_GET_PROPERTY								0x11 	//Get the value of a single property
#define	FLASH_LOAD_IMG									0x00 	//Load a boot_img from flash
//#define	FLASH_LOAD_IMG_CHECK_CRC32						0x01 	//Load a boot_img from flash. Check the CRC32 of the boot_img as the boot_img is loaded
//#define	FLASH_SET_PROP_LIST								0x10 	//Set the value of a list of properties
//#define	FLASH_WRITE_BLOCK								0xF0 	//Write a block of bytes to the flash
//#define	FLASH_WRITE_BLOCK_READBACK_VERIFY				0xF1 	//Write a block of bytes to the flash. Reads back the block after write to verify the block
//																//		was written correctly by comparing the CRC32
//#define	FLASH_WRITE_BLOCK_PACKET_VERIFY					0xF2 	//Write a block of data to the flash. Compares the CRC32 of the block of data received by
//																//		the command.
//#define	FLASH_WRITE_BLOCK_READBACK_AND_PACKET_VERIFY	0xF3 	//Write a block of data to the flash. Reads back the block after write to verify the block
//																//		was written correctly by comparing the CRC32. Also compares the CRC32 of the block
//																//		of data received by the command.



// Properties common:
#define PROP_INT_CTL_ENABLE						0x0000
#define PROP_INT_CTL_REPEAT						0x0001
#define PROP_DIGITAL_IO_OUTPUT_SELECT			0x0200
#define PROP_DIGITAL_IO_OUTPUT_SAMPLE_RATE		0x0201
//#define 		PROP_DIGITAL_IO_OUTPUT_FORMAT 		0x0202
//#define 		PROP_DIGITAL_IO_OUTPUT_FORMAT_OVERRIDES_1 		0x0203
//#define 		PROP_DIGITAL_IO_OUTPUT_FORMAT_OVERRIDES_2 		0x0204
//#define 		PROP_DIGITAL_IO_OUTPUT_FORMAT_OVERRIDES_3 		0x0205
//#define 		PROP_DIGITAL_IO_OUTPUT_FORMAT_OVERRIDES_4 		0x0206
//#define 		PROP_AUDIO_ANALOG_VOLUME 		0x0300
//#define 		PROP_AUDIO_MUTE 				0x0301
//#define 		PROP_AUDIO_OUTPUT_CONFIG 		0x0302
#define PROP_PIN_CONFIG_ENABLE					0x0800
#define PROP_DIGITAL_SERVICE_INT_SOURCE			0x8100
//#define 		PROP_WAKE_TONE_ENABLE 		0x0900
//#define 		PROP_WAKE_TONE_PERIOD 		0x0901
//#define 		PROP_WAKE_TONE_FREQ 		0x0902
//#define 		PROP_WAKE_TONE_AMPLITUDE 		0x0903
//#define 		PROP_DIGITAL_SERVICE_RESTART_DELAY 		0x8101


//AM_FM
//#define 		PROP_AGC_RF_THRESHOLD 		0x170C
//#define 		PROP_HD_BLEND_OPTIONS 		0x9101
//#define 		PROP_HD_BLEND_ANALOG_TO_HD_TRANSITION_-TIME 		0x9102
//#define 		PROP_HD_BLEND_HD_TO_ANALOG_TRANSITION_-TIME 		0x9103
//#define 		PROP_HD_BLEND_DYNAMIC_GAIN 		0x9106
//#define 		PROP_HD_BLEND_DECISION_ANA_TO_DIG_-THRESHOLD 		0x9109
//#define 		PROP_HD_BLEND_SERV_LOSS_RAMP_DOWN_-TIME 		0x910C
//#define 		PROP_HD_BLEND_SERV_LOSS_NOISE_RAMP_UP_TIME 		0x910D
//#define 		PROP_HD_BLEND_SERV_LOSS_NOISE_RAMP_DOWN_TIME 		0x910E
//#define 		PROP_HD_BLEND_SERV_LOSS_NOISE_LEVEL 		0x910F
//#define 		PROP_HD_BLEND_SERV_LOSS_NOISE_DAAI_-THRESHOLD 		0x9110
//#define 		PROP_HD_BLEND_SERV_LOSS_NOISE_AUDIO_START_DELAY 		0x9111
//#define 		PROP_HD_DIGRAD_INTERRUPT_SOURCE 		0x9200
//#define 		PROP_HD_DIGRAD_CDNR_LOW_THRESHOLD 		0x9201
//#define 		PROP_HD_DIGRAD_CDNR_HIGH_THRESHOLD 		0x9202
//#define 		PROP_HD_EVENT_INTERRUPT_SOURCE 		0x9300
//#define 		PROP_HD_EVENT_SIS_CONFIG 		0x9301
//#define 		PROP_HD_EVENT_ALERT_CONFIG 		0x9302
//#define 		PROP_HD_ACF_INTERRUPT_SOURCE 		0x9400
//#define 		PROP_HD_ACF_COMF_NOISE_TOLERANCE 		0x9404
//#define 		PROP_HD_PSD_FIELD_MASK 		0x9501
//#define 		PROP_HD_AUDIO_CTRL_FRAME_DELAY 		0x9700
//#define 		PROP_HD_AUDIO_CTRL_PROGRAM_LOSS_-THRESHOLD 		0x9701
//#define 		PROP_HD_AUDIO_CTRL_BALL_GAME_ENABLE 		0x9702
//#define 		PROP_HD_CODEC_MODE_0_BLEND_THRESHOLD 		0x9900
//#define 		PROP_HD_CODEC_MODE_0_SAMPLES_DELAY 		0x9901
//#define 		PROP_HD_CODEC_MODE_0_BLEND_RATE 		0x9902
//#define 		PROP_HD_CODEC_MODE_2_SAMPLES_DELAY 		0x9904
//#define 		PROP_HD_CODEC_MODE_2_BLEND_RATE 		0x9905
//#define 		PROP_HD_CODEC_MODE_10_BLEND_THRESHOLD 		0x9906
//#define 		PROP_HD_CODEC_MODE_10_SAMPLES_DELAY 		0x9907
//#define 		PROP_HD_CODEC_MODE_10_BLEND_RATE 		0x9908
//#define 		PROP_HD_CODEC_MODE_13_SAMPLES_DELAY 		0x990A
//#define 		PROP_HD_CODEC_MODE_13_BLEND_RATE 		0x990B
//#define 		PROP_HD_CODEC_MODE_1_SAMPLES_DELAY 		0x990D
//#define 		PROP_HD_CODEC_MODE_1_BLEND_RATE 		0x990E
//#define 		PROP_HD_CODEC_MODE_3_BLEND_RATE 		0x9911
//#define 		PROP_HD_EZBLEND_ENABLE 		0x9B00
//#define 		PROP_HD_EZBLEND_MPS_BLEND_THRESHOLD 		0x9B01
//#define 		PROP_HD_EZBLEND_MPS_BLEND_RATE 		0x9B02
//#define 		PROP_HD_EZBLEND_MPS_SAMPLES_DELAY 		0x9B03
//#define 		PROP_HD_EZBLEND_SPS_BLEND_THRESHOLD 		0x9B04
//#define 		PROP_HD_EZBLEND_SPS_BLEND_RATE 		0x9B05
//#define 		PROP_HD_TEST_BER_CONFIG 		0xE800


//AM
//#define 		PROP_AM_AVC_MIN_GAIN 		0x0500
//#define 		PROP_AM_AVC_MAX_GAIN 		0x0501
//#define 		PROP_AM_CHBW_SQ_LIMITS 		0x2200
//#define 		PROP_AM_CHBW_SQ_CHBW 		0x2201
//#define 		PROP_AM_CHBW_SQ_WIDENING_TIME 		0x2202
//#define 		PROP_AM_CHBW_SQ_NARROWING_TIME 		0x2203
//#define 		PROP_AM_CHBW_OVERRIDE_BW 		0x2204
#define 		PROP_AM_SEEK_BAND_BOTTOM 		0x4100
#define 		PROP_AM_SEEK_BAND_TOP 			0x4101
#define 		PROP_AM_SEEK_FREQUENCY_SPACING	0x4102
//#define 		PROP_AM_VALID_MAX_TUNE_ERROR 		0x4200
//#define 		PROP_AM_VALID_RSSI_TIME 		0x4201
//#define 		PROP_AM_VALID_RSSI_THRESHOLD 		0x4202
//#define 		PROP_AM_VALID_SNR_TIME 		0x4203
//#define 		PROP_AM_VALID_SNR_THRESHOLD 		0x4204
//#define 		PROP_AM_VALID_HDLEVEL_THRESHOLD 		0x4205
//#define 		PROP_AM_RSQ_INTERRUPT_SOURCE 		0x4300
//#define 		PROP_AM_RSQ_SNR_HIGH_THRESHOLD 		0x4301
//#define 		PROP_AM_RSQ_SNR_LOW_THRESHOLD 		0x4302
//#define 		PROP_AM_RSQ_RSSI_HIGH_THRESHOLD 		0x4303
//#define 		PROP_AM_RSQ_RSSI_LOW_THRESHOLD 		0x4304
//#define 		PROP_AM_RSQ_HD_DETECTION 		0x4305
//#define 		PROP_AM_RSQ_HD_LEVEL_TIME_CONST 		0x4306
//#define 		PROP_AM_RSQ_HDDETECTED_THD 		0x4307
//#define 		PROP_AM_RSQ_HDLEVEL_HIGH_THRESHOLD 		0x4308
//#define 		PROP_AM_RSQ_HDLEVEL_LOW_THRESHOLD 		0x4309
//#define 		PROP_AM_ACF_INTERRUPT_SOURCE 		0x4400
//#define 		PROP_AM_ACF_SOFTMUTE_THRESHOLD 		0x4401
//#define 		PROP_AM_ACF_HIGHCUT_THRESHOLD 		0x4402
//#define 		PROP_AM_ACF_SOFTMUTE_TOLERANCE 		0x4403
//#define 		PROP_AM_ACF_HIGHCUT_TOLERANCE 		0x4404
//#define 		PROP_AM_ACF_CONTROL_SOURCE 		0x4405
//#define 		PROP_AM_SOFTMUTE_SQ_LIMITS 		0x4500
//#define 		PROP_AM_SOFTMUTE_SQ_ATTENUATION 		0x4501
//#define 		PROP_AM_SOFTMUTE_SQ_ATTACK_TIME 		0x4502
//#define 		PROP_AM_SOFTMUTE_SQ_RELEASE_TIME 		0x4503
//#define 		PROP_AM_HIGHCUT_SQ_LIMITS 		0x4600
//#define 		PROP_AM_HIGHCUT_SQ_CUTOFF_FREQ 		0x4601
//#define 		PROP_AM_HIGHCUT_SQ_ATTACK_TIME 		0x4602
//#define 		PROP_AM_HIGHCUT_SQ_RELEASE_TIME 		0x4603
//#define 		PROP_AM_DEMOD_AFC_RANGE 		0x4800
//#define 		PROP_HD_BLEND_BWM_CTRL_THRES 		0x9120
//#define 		PROP_HD_BLEND_BWM_CTRL_LEVEL 		0x9121
//#define 		PROP_HD_BLEND_BWM_CTRL_RAMP_UP_TIME 		0x9122
//#define 		PROP_HD_BLEND_BWM_CTRL_RAMP_DOWN_-TIME 		0x9123
//#define 		PROP_HD_BLEND_BWM_BLEND_THRES 		0x9124
//#define 		PROP_HD_BLEND_BWM_BLEND_LEVEL 		0x9125
//#define 		PROP_HD_BLEND_BWM_BLEND_RAMP_UP_TIME 		0x9126
//#define 		PROP_HD_BLEND_BWM_BLEND_RAMP_DOWN_-TIME 		0x9127
//#define 		PROP_HD_ACF_COMF_NOISE_THRESHOLD 		0x9401
//#define 		PROP_HD_ACF_AUDIO_BW_THRESHOLD 		0x9402
//#define 		PROP_HD_ACF_STEREO_SEP_THRESHOLD 		0x9403
//#define 		PROP_HD_ACF_AUDIO_BW_TOLERANCE 		0x9405
//#define 		PROP_HD_ACF_STEREO_SEP_TOLERANCE 		0x9406
//#define 		PROP_HD_PSD_ENABLE 		0x9500
//#define 		PROP_HD_CODEC_MODE_2_BLEND_THRESHOLD 		0x9903
//#define 		PROP_HD_CODEC_MODE_13_BLEND_THRESHOLD 		0x9909
//#define 		PROP_HD_CODEC_MODE_1_BLEND_THRESHOLD 		0x990C
//#define 		PROP_HD_CODEC_MODE_3_BLEND_THRESHOLD 		0x990F
//#define 		PROP_HD_CODEC_MODE_3_SAMPLES_DELAY 		0x9910
//#define 		PROP_HD_ENHANCED_STREAM_HOLDOFF_CONFIG 		0x9F00
//#define 		PROP_HD_ENHANCED_STREAM_HOLDOFF_-THRESHOLDS 		0x9F01

//FM
//#define 		PROP_FM_TUNE_FE_VARM 		0x1710
//#define 		PROP_FM_TUNE_FE_VARB 		0x1711
#define 		PROP_FM_TUNE_FE_CFG 		0x1712
#define 		PROP_FM_SEEK_BAND_BOTTOM 		0x3100
#define 		PROP_FM_SEEK_BAND_TOP 		0x3101
#define 		PROP_FM_SEEK_FREQUENCY_SPACING 		0x3102
//#define 		PROP_FM_VALID_MAX_TUNE_ERROR 		0x3200
//#define 		PROP_FM_VALID_RSSI_TIME 		0x3201
#define 		PROP_FM_VALID_RSSI_THRESHOLD 		0x3202
//#define 		PROP_FM_VALID_SNR_TIME 		0x3203
#define 		PROP_FM_VALID_SNR_THRESHOLD 		0x3204
//#define 		PROP_FM_VALID_HDLEVEL_THRESHOLD 		0x3206
//#define 		PROP_FM_RSQ_INTERRUPT_SOURCE 		0x3300
//#define 		PROP_FM_RSQ_SNR_HIGH_THRESHOLD 		0x3301
//#define 		PROP_FM_RSQ_SNR_LOW_THRESHOLD 		0x3302
//#define 		PROP_FM_RSQ_RSSI_HIGH_THRESHOLD 		0x3303
//#define 		PROP_FM_RSQ_RSSI_LOW_THRESHOLD 		0x3304
//#define 		PROP_FM_RSQ_HD_DETECTION 		0x3307
//#define 		PROP_FM_RSQ_HD_LEVEL_TIME_CONST 		0x3308
//#define 		PROP_FM_RSQ_HDDETECTED_THD 		0x3309
//#define 		PROP_FM_RSQ_HDLEVEL_HIGH_THRESHOLD 		0x330A
//#define 		PROP_FM_RSQ_HDLEVEL_LOW_THRESHOLD 		0x330B
//#define 		PROP_FM_ACF_INTERRUPT_SOURCE 		0x3400
//#define 		PROP_FM_ACF_SOFTMUTE_THRESHOLD 		0x3401
//#define 		PROP_FM_ACF_HIGHCUT_THRESHOLD 		0x3402
//#define 		PROP_FM_ACF_BLEND_THRESHOLD 		0x3403
//#define 		PROP_FM_ACF_SOFTMUTE_TOLERANCE 		0x3404
//#define 		PROP_FM_ACF_HIGHCUT_TOLERANCE 		0x3405
//#define 		PROP_FM_ACF_BLEND_TOLERANCE 		0x3406
#define 		PROP_FM_SOFTMUTE_SNR_LIMITS 		0x3500
//#define 		PROP_FM_SOFTMUTE_SNR_ATTENUATION 		0x3501
//#define 		PROP_FM_SOFTMUTE_SNR_ATTACK_TIME 		0x3502
//#define 		PROP_FM_SOFTMUTE_SNR_RELEASE_TIME 		0x3503
//#define 		PROP_FM_HIGHCUT_RSSI_LIMITS 		0x3600
//#define 		PROP_FM_HIGHCUT_RSSI_CUTOFF_FREQ 		0x3601
//#define 		PROP_FM_HIGHCUT_RSSI_ATTACK_TIME 		0x3602
//#define 		PROP_FM_HIGHCUT_RSSI_RELEASE_TIME 		0x3603
//#define 		PROP_FM_HIGHCUT_SNR_LIMITS 		0x3604
//#define 		PROP_FM_HIGHCUT_SNR_CUTOFF_FREQ 		0x3605
//#define 		PROP_FM_HIGHCUT_SNR_ATTACK_TIME 		0x3606
//#define 		PROP_FM_HIGHCUT_SNR_RELEASE_TIME 		0x3607
//#define 		PROP_FM_HIGHCUT_MULTIPATH_LIMITS 		0x3608
//#define 		PROP_FM_HIGHCUT_MULTIPATH_CUTOFF_FREQ 		0x3609
//#define 		PROP_FM_HIGHCUT_MULTIPATH_ATTACK_TIME 		0x360A
//#define 		PROP_FM_HIGHCUT_MULTIPATH_RELEASE_TIME 		0x360B
//#define 		PROP_FM_BLEND_RSSI_LIMITS 		0x3700
//#define 		PROP_FM_BLEND_RSSI_ATTACK_TIME 		0x3702
//#define 		PROP_FM_BLEND_RSSI_RELEASE_TIME 		0x3703
//#define 		PROP_FM_BLEND_SNR_LIMITS 		0x3704
//#define 		PROP_FM_BLEND_SNR_ATTACK_TIME 		0x3706
//#define 		PROP_FM_BLEND_SNR_RELEASE_TIME 		0x3707
//#define 		PROP_FM_BLEND_MULTIPATH_LIMITS 		0x3708
//#define 		PROP_FM_BLEND_MULTIPATH_ATTACK_TIME 		0x370A
//#define 		PROP_FM_BLEND_MULTIPATH_RELEASE_TIME 		0x370B
#define 		PROP_FM_AUDIO_DE_EMPHASIS 		0x3900
#define 		PROP_FM_RDS_INTERRUPT_SOURCE 		0x3C00
#define 		PROP_FM_RDS_INTERRUPT_FIFO_COUNT 		0x3C01
#define 		PROP_FM_RDS_CONFIG 		0x3C02
#define 		PROP_FM_RDS_CONFIDENCE 		0x3C03
//#define 		PROP_HD_BLEND_DECISION_ANA_TO_DIG_DELAY 		0x910A
//#define 		PROP_HD_BLEND_SERV_LOSS_RAMP_UP_TIME 		0x910B
//#define 		PROP_HD_BLEND_SERV_SWITCH_RAMP_UP_TIME 		0x9112
//#define 		PROP_HD_BLEND_SERV_SWITCH_RAMP_DOWN_-TIME 		0x9113
//#define 		PROP_HD_ACF_COMF_NOISE_THRESHOLD 		0x9401
//#define 		PROP_HD_PSD_ENABLE 		0x9500
//#define 		PROP_HD_CODEC_MODE_2_BLEND_THRESHOLD 		0x9903
//#define 		PROP_HD_CODEC_MODE_13_BLEND_THRESHOLD 		0x9909
//#define 		PROP_HD_CODEC_MODE_1_BLEND_THRESHOLD 		0x990C
//#define 		PROP_HD_CODEC_MODE_3_BLEND_THRESHOLD 		0x990F
//#define 		PROP_HD_CODEC_MODE_3_SAMPLES_DELAY 		0x9910
//#define 		PROP_HD_SERVICE_MODE_CONTROL_MP11_ENABLE 		0x9A00
//#define 		PROP_HD_TEST_DEBUG_AUDIO 		0xE801

//DAB
#define 		PROP_DAB_TUNE_FE_VARM 		0x1710
#define 		PROP_DAB_TUNE_FE_VARB 		0x1711
#define 		PROP_DAB_TUNE_FE_CFG 		0x1712
//#define 		PROP_DAB_DIGRAD_INTERRUPT_SOURCE 		0xB000
//#define 		PROP_DAB_DIGRAD_RSSI_HIGH_THRESHOLD 		0xB001
//#define 		PROP_DAB_DIGRAD_RSSI_LOW_THRESHOLD 		0xB002
//#define 		PROP_DAB_VALID_RSSI_TIME 		0xB200
//#define 		PROP_DAB_VALID_RSSI_THRESHOLD 		0xB201
//#define 		PROP_DAB_VALID_ACQ_TIME 		0xB202
//#define 		PROP_DAB_VALID_SYNC_TIME 		0xB203
//#define 		PROP_DAB_VALID_DETECT_TIME 		0xB204
#define 		PROP_DAB_EVENT_INTERRUPT_SOURCE 		0xB300
//#define 		PROP_DAB_EVENT_MIN_SVRLIST_PERIOD 		0xB301
//#define 		PROP_DAB_EVENT_MIN_SVRLIST_PERIOD_RECONFIG 		0xB302
//#define 		PROP_DAB_EVENT_MIN_FREQINFO_PERIOD 		0xB303
#define 		PROP_DAB_XPAD_ENABLE 		0xB400
//#define 		PROP_DAB_DRC_OPTION 		0xB401
//#define 		PROP_DAB_ACF_ENABLE 		0xB500
#define 		PROP_DAB_ACF_MUTE_SIGLOSS_THRESHOLD 		0xB501
//#define 		PROP_DAB_ACF_SOFTMUTE_BER_LIMITS 		0xB503
//#define 		PROP_DAB_ACF_SOFTMUTE_ATTENUATION_LEVEL 		0xB504
#define 		PROP_DAB_ACF_SOFTMUTE_ATTACK_TIME 		0xB505
//#define 		PROP_DAB_ACF_SOFTMUTE_RELEASE_TIME 		0xB506
//#define 		PROP_DAB_ACF_CMFTNOISE_BER_LIMITS 		0xB507
//#define 		PROP_DAB_ACF_CMFTNOISE_LEVEL 		0xB508
//#define 		PROP_DAB_ACF_CMFTNOISE_ATTACK_TIME 		0xB509
//#define 		PROP_DAB_ACF_CMFTNOISE_RELEASE_TIME 		0xB50A
//#define 		PROP_DAB_ANNOUNCEMENT_ENABLE 		0xB700
//#define 		PROP_DAB_TEST_BER_CONFIG 		0xE800

//NVSPI FLash props
#define PROP_SPI_CLOCK_FREQ_KHZ			0x0001
//#define PROP_SPI_MODE 0x03				0x0002
//#define PROP_READ_CMD 0x03 Flash			0x0101
//#define PROP_HIGH_SPEED_READ_CMD			0x0102
#define PROP_HIGH_SPEED_READ_MAX_FREQ_MHZ	0x0103
//#define PROP_WRITE_CMD					0x0201
//#define PROP_ERASE_SECTOR_CMD 			0x0202
//#define PROP_ERASE_CHIP_CMD				0x0204


// DAB channels
#define CHAN_5A 174928
#define CHAN_5B 176640
#define CHAN_5C 178352
#define CHAN_5D 180064
#define CHAN_6A 181936
#define CHAN_6B 183648
#define CHAN_6C 185360
#define CHAN_6D 187072
#define CHAN_7A 188928
#define CHAN_7B 190640
#define CHAN_7C 192352
#define CHAN_7D 194064
#define CHAN_8A 195936
#define CHAN_8B 197648
#define CHAN_8C 199360
#define CHAN_8D 201072
#define CHAN_9A 202928
#define CHAN_9B 204640
#define CHAN_9C 206352
#define CHAN_9D 208064
#define CHAN_10A 209936
#define CHAN_10B 211648
#define CHAN_10C 213360
#define CHAN_10D 215072
#define CHAN_11A 216928
#define CHAN_11B 218640
#define CHAN_11C 220352
#define CHAN_11D 222064
#define CHAN_12A 223936
#define CHAN_12B 225648
#define CHAN_12C 227360
#define CHAN_12D 229072
#define CHAN_13A 230784
#define CHAN_13B 232496
#define CHAN_13C 234208
#define CHAN_13D 235776
#define CHAN_13E 237488
#define CHAN_13F 239200


#endif /* SRC_SILELIS_CODE_SI468X_DEFINES_H_ */

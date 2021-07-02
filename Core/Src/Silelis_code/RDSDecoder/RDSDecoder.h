/* Arduino RDS/RBDS (IEC 62016/NRSC-4-B) Decoding Library
 * See the README file for author and licensing information. In case it's
 * missing from your distribution, use the one here as the authoritative
 * version: https://github.com/csdexter/RDSDecoder/blob/master/README
 *
 * This library is for decoding RDS/RBDS data streams (groups).
 * See the example sketches to learn how to use the library in your code.
 *
 * This is the main include file for the library.
 */

#ifndef _RDSDECODER_H_INCLUDED
#define _RDSDECODER_H_INCLUDED

#if defined(__GNUC__)
	# if defined(__AVR__)
		// Bug in AVR-libc: *printf() macros are gated by __STDC_LIMIT_MACROS instead
		// of __STDC_FORMAT_MACROS as the standard dictates. Define both to
		// future-proof. Also, this needs to be defined here as Arduino.h acts as
		// an umbrella header for most of the system ones.
		#  define __STDC_FORMAT_MACROS
		#  define __STDC_LIMIT_MACROS
		#  if defined(ARDUINO) && ARDUINO >= 100
			#   include <Arduino.h>
		#  else
			#   include <WProgram.h>
	#  endif
	# elif defined(__i386__) || defined(__x86_64__)
		#  include <stdint.h>
		#  include <stdbool.h>
		#  include <stddef.h>
		#  define word uint16_t
		#  define byte uint8_t
	#else
		#  include <stdint.h>
		#  include <stdbool.h>
		#  include <stddef.h>
		#  define word uint16_t
		#  define byte uint8_t
	# endif
#else
	# warning Non-GNU compiler detected, you are on your own!
#endif

//Define the Locale options
#define RDS_LOCALE_US 0
#define RDS_LOCALE_EU 1

//Define RDS-related public flags (that may come handy to the user)
#define RDS_DI_STEREO 0x01
#define RDS_DI_ARTIFICIAL_HEAD 0x02
#define RDS_DI_COMPRESSED 0x04
#define RDS_DI_DYNAMIC_PTY 0x08
#define RDS_PI_AREA_LOCAL 0x00
#define RDS_PI_AREA_INTERNATIONAL 0x01
#define RDS_PI_AREA_NATIONAL 0x02
#define RDS_PI_AREA_SUPRAREGIONAL 0x03
#define RDS_PI_AREA_REGIONAL_FIRST 0x04
#define RDS_PI_AREA_LPSR_AF 0x01
#define RDS_PI_PROGRAM_LPSR 0x00
#define RDS_AF_FILLER 0xCD
#define RDS_AF_NODATA 0xE0
#define RDS_AF_FOLLOWS_FM_FIRST 0xE1
#define RDS_AF_FOLLOWS_AM 0xFA
#define RDS_TMC_EAG_TEST_CLEAR 0x0
#define RDS_TMC_EAG_TEST_STATIC 0x1
#define RDS_TMC_EAG_TEST_DYNAMIC 0x3
#define RDS_TMC_ANY_SID 0x00
#define RDS_TMC_MESSAGE_VARIANT_SPN_A 0x4
#define RDS_TMC_MESSAGE_VARIANT_SPN_B 0x5
#define RDS_TMC_MESSAGE_VARIANT_EON_AF 0x6
#define RDS_TMC_MESSAGE_VARIANT_EON_TM 0x7
#define RDS_TMC_MESSAGE_VARIANT_EON_PI 0x8
#define RDS_TMC_MESSAGE_VARIANT_EON_EX 0x9
#define RDS_TMC_MESSAGE_ENC_VARIANT_EAG 0x0
#define RDS_TMC_LABEL_DURATION 0x0
#define RDS_TMC_LABEL_CONTROL 0x1
#define RDS_TMC_LABEL_LENGTH 0x2
#define RDS_TMC_LABEL_SPEED 0x3
#define RDS_TMC_LABEL_QUANTIFIER_5 0x4
#define RDS_TMC_LABEL_QUANTIFIER_8 0x5
#define RDS_TMC_LABEL_SUPPLEMENTARY 0x6
#define RDS_TMC_LABEL_START 0x7
#define RDS_TMC_LABEL_STOP 0x8
#define RDS_TMC_LABEL_ADDITIONAL 0x9
#define RDS_TMC_LABEL_DIVERSION 0xA
#define RDS_TMC_LABEL_DESTINATION 0xB
#define RDS_TMC_LABEL_RESERVED1 0xC
#define RDS_TMC_LABEL_XREF 0xD
#define RDS_TMC_LABEL_SEPARATOR 0xE
#define RDS_TMC_LABEL_RESERVED2 0xF
#define RDS_TMC_L1_URGENCY_INC 0x00
#define RDS_TMC_L1_URGENCY_DEC 0x01
#define RDS_TMC_L1_DIRECTION_INV 0x02
#define RDS_TMC_L1_DURATION_INV 0x03
#define RDS_TMC_L1_SPOKEN_INV 0x04
#define RDS_TMC_L1_DIVERSION 0x05
#define RDS_TMC_L1_EXTENT_ADD8 0x06
#define RDS_TMC_L1_EXTENT_ADD16 0x07
#define RDS_RTP_CLASS_DUMMY 0
#define RDS_RTP_CLASS_ITEM_TITLE 1
#define RDS_RTP_CLASS_ITEM_ALBUM 2
#define RDS_RTP_CLASS_ITEM_TRACK 3
#define RDS_RTP_CLASS_ITEM_ARTIST 4
#define RDS_RTP_CLASS_ITEM_COMPOSITION 5
#define RDS_RTP_CLASS_ITEM_MOVEMENT 6
#define RDS_RTP_CLASS_ITEM_CONDUCTOR 7
#define RDS_RTP_CLASS_ITEM_COMPOSER 8
#define RDS_RTP_CLASS_ITEM_BAND 9
#define RDS_RTP_CLASS_ITEM_COMMENT 10
#define RDS_RTP_CLASS_ITEM_GENRE 11
#define RDS_RTP_CLASS_INFO_NEWS 12
#define RDS_RTP_CLASS_INFO_NEWSLOCAL 13
#define RDS_RTP_CLASS_INFO_STOCKS 14
#define RDS_RTP_CLASS_INFO_SPORT 15
#define RDS_RTP_CLASS_INFO_LOTTERY 16
#define RDS_RTP_CLASS_INFO_HOROSCOPE 17
#define RDS_RTP_CLASS_INFO_DAILY 18
#define RDS_RTP_CLASS_INFO_HEALTH 19
#define RDS_RTP_CLASS_INFO_EVENT 20
#define RDS_RTP_CLASS_INFO_SCENE 21
#define RDS_RTP_CLASS_INFO_CINEMA 22
#define RDS_RTP_CLASS_INFO_TV 23
#define RDS_RTP_CLASS_INFO_DATE 24
#define RDS_RTP_CLASS_INFO_WEATHER 25
#define RDS_RTP_CLASS_INFO_TRAFFIC 26
#define RDS_RTP_CLASS_INFO_ALARM 27
#define RDS_RTP_CLASS_INFO_AD 28
#define RDS_RTP_CLASS_INFO_URL 29
#define RDS_RTP_CLASS_INFO_OTHER 30
#define RDS_RTP_CLASS_STATION_SHORT 31
#define RDS_RTP_CLASS_STATION_LONG 32
#define RDS_RTP_CLASS_PROGRAMME_NOW 33
#define RDS_RTP_CLASS_PROGRAMME_NEXT 34
#define RDS_RTP_CLASS_PROGRAMME_PART 35
#define RDS_RTP_CLASS_PROGRAMME_HOST 36
#define RDS_RTP_CLASS_PROGRAMME_EDITORIAL 37
#define RDS_RTP_CLASS_PROGRAMME_FREQUENCY 38
#define RDS_RTP_CLASS_PROGRAMME_HOMEPAGE 39
#define RDS_RTP_CLASS_PROGRAMME_SUBCHANNEL 40
#define RDS_RTP_CLASS_PHONE_HOTLINE 41
#define RDS_RTP_CLASS_PHONE_STUDIO 42
#define RDS_RTP_CLASS_PHONE_OTHER 43
#define RDS_RTP_CLASS_SMS_STUDIO 44
#define RDS_RTP_CLASS_SMS_OTHER 45
#define RDS_RTP_CLASS_EMAIL_HOTLINE 46
#define RDS_RTP_CLASS_EMAIL_STUDIO 47
#define RDS_RTP_CLASS_EMAIL_OTHER 48
#define RDS_RTP_CLASS_MMS_OTHER 49
#define RDS_RTP_CLASS_CHAT 50
#define RDS_RTP_CLASS_CHAT_CENTRE 51
#define RDS_RTP_CLASS_VOTE_QUESTION 52
#define RDS_RTP_CLASS_VOTE_CENTRE 53
#define RDS_RTP_CLASS_PRIVATE_FIRST 56
#define RDS_RTP_CLASS_PRIVATE_LAST 58
#define RDS_RTP_CLASS_DESCRIPTOR_PLACE 59
#define RDS_RTP_CLASS_DESCRIPTOR_APPOINTMENT 60
#define RDS_RTP_CLASS_DESCRIPTOR_IDENTIFIER 61
#define RDS_RTP_CLASS_DESCRIPTOR_PURCHASE 62
#define RDS_RTP_CLASS_DESCRIPTOR_GETDATA 63
#define RDS_PAGING_AB word(0x0010)

// Values for TRDSPage.pageType
#define RDS_PAGING_NOMESSAGE 0x0
#define RDS_PAGING_FUNCTION 0x1
#define RDS_PAGING_DIGIT 0x2
#define RDS_PAGING_ALPHA 0x3

//RDS Decoder callback types
#define RDS_CALLBACK_AF 0x00
#define RDS_CALLBACK_TDC 0x01
#define RDS_CALLBACK_AID 0x02
#define RDS_CALLBACK_EON 0x03
#define RDS_CALLBACK_RT 0x04
#define RDS_CALLBACK_TMC 0x05
#define RDS_CALLBACK_RTP 0x06
#define RDS_CALLBACK_ERT 0x07
#define RDS_CALLBACK_SLP 0x08
#define RDS_CALLBACK_P7 0x09
#define RDS_CALLBACK_P13 0x0A
#define RDS_CALLBACK_LAST RDS_CALLBACK_P13

//This holds time of day as received via RDS. Mimicking struct tm from
//<time.h> for familiarity.
//NOTE: RDS does not provide seconds, only guarantees that the minute update
//      will occur close enough to the seconds going from 59 to 00 to be
//      meaningful -- so we don't provide tm_sec
//NOTE: RDS does not provide DST information so we don't provide tm_isdst
//NOTE: we will provide tm_wday (day of week) but not tm_yday (day of year)
typedef struct {
    byte tm_min;
    byte tm_hour;
    byte tm_mday;
    byte tm_mon;
    word tm_year;
    byte tm_wday;
    int8_t tm_tz;
} TRDSTime;

typedef struct __attribute__ ((__packed__)) {
    uint8_t country:4;
    uint8_t area:4;
    byte program;
} TRDSPI;

typedef struct __attribute__ ((__packed__)) {
    uint8_t romClassNumber:5;
    uint16_t serialNumber:10;
    uint8_t scopeFlag:1;
} TRDSIRDSMessage3;

typedef struct __attribute__ ((__packed__)) {
  uint8_t variantCode:2;
  union {
    struct {
      uint8_t unused:2;
      uint8_t locationTableNumber:6;
      uint8_t alternateFrequencyIndicator:1;
      uint8_t mode:1;
      uint8_t international:1;
      uint8_t national:1;
      uint8_t regional:1;
      uint8_t urban:1;
    };
    struct {
      uint8_t gapParameter:2;
      uint8_t serviceIdentifier:6;
      uint8_t activityTime:2;
      uint8_t windowTime:2;
      uint8_t delayTime:2;
    };
  };
} TRDSTMCMessage3;

typedef struct __attribute__ ((__packed__)) {
    // uint8_t reserved:2;
    uint8_t eRT:1;
    uint8_t cB:1;
    uint8_t serverControlBits:4;
    uint8_t templateNumber;
} TRDSRTPlusMessage3;

typedef struct __attribute__ ((__packed__)) {
    // uint16_t reserved:10;
    uint8_t characterTable:4;
    uint8_t rtl:1;
    uint8_t utf8:1;
} TRDSERTMessage3;

typedef struct __attribute__ ((__packed__)) {
  // bits X4-X0
  uint8_t systemMessage:1;
  union {
    uint8_t variantCode:4;
    struct {
      uint8_t single:1;
      union {
        uint8_t duration:3;
        uint8_t continuationIndicator:3;
      };
    };
  };
  // bits Y15-Y0 + Z15-Z0
  union {
    //Encrypted TMC
    struct {
      uint8_t encVariantCode:3;
      uint8_t test:2;
      uint8_t encServiceIdentifier:6;
      uint8_t encId:5;
      uint8_t encLocationTableNumber:6;
      uint16_t reserved:10;
    };
    //Non-system messages
    struct {
      union {
        uint8_t first:1;
        uint8_t diversion:1;
      };
      union {
        struct {
          uint8_t direction:1;
          uint8_t extent:3;
          uint16_t event:11;
          uint16_t location;
        };
        struct {
          uint8_t second:1;
          uint8_t sequence:2;
          uint32_t data:28;
        };
      };
    };
    //System messages
    char serviceProviderName[4];
    struct {
      union {
        uint8_t alternativeFrequency[2];
        struct {
          uint8_t tuningFrequency;
          uint8_t mappedFrequency;
        };
        word programIdentifier1;
        struct {
          uint8_t locationTableNumber:6;
          uint8_t international:1;
          uint8_t national:1;
          uint8_t regional:1;
          uint8_t urban:1;
          uint8_t serviceIdentifier:6;
        };
      };
      word programIdentifier2;
    };
  };
} TRDSTMCMessage8;

typedef struct __attribute__ ((__packed__)) {
    uint8_t magic:6;
    uint8_t country:4;
    uint8_t locationTableNumber:6;
} TRDSTMCFLT;

typedef struct __attribute__ ((__packed__)) {
    uint8_t bitIndex:5;
    uint8_t sliceIndex:3;
} TRDSTMCContainerIndex;

typedef struct {
    uint8_t type;
    uint16_t value;
} TRDSTMCLabel;

typedef struct __attribute__ ((__packed__)) {
    uint8_t itemToggle:1;
    uint8_t itemRunning:1;
    uint8_t contentType1:6;
    uint8_t startMarker1:6;
    uint8_t lengthMarker1:6;
    uint8_t contentType2:6;
    uint8_t startMarker2:6;
    uint8_t lengthMarker2:5;
} TRDSRTPlusMessage11;

typedef struct {
    byte fiveBits;
    word blockC, blockD;
} TRDSRawData;

typedef struct {
    byte pageType;
    byte enhancedFlags;
    byte callCounter;
    byte groupCode;
    word individualCode;
    word countryCode;
    char *pageMessage;
} TRDSPage;

typedef struct __attribute__ ((__packed__)) {
    uint8_t cycleSelection:2;
    uint8_t variantCode:3;
    uint8_t interval:4;
    uint8_t messageSorting:2;
    // uint8_t reserved:1;
    uint32_t addressNotification:25;
} TRDSPagingMessage13;

typedef struct {
    byte carriedInGroup;
    word message;
    word applicationIdentification;
} TRDSAppID;

typedef struct __attribute__ ((__packed__)) {
    uint8_t linkageActuator:1;
    uint8_t extendedGeneric:1;
    uint8_t internationalLinkageSet:1;
    uint16_t linkageSet:12;
} TRDSLinkageInformation;

typedef struct {
    word programIdentifier;
    bool TP, TA;
    byte PTY;
    char programService[9];
    word programItemNumber;
    TRDSLinkageInformation linkageInformation;
} TRDSEON;

typedef struct {
    word programIdentifier;
    uint8_t TP:1;
    uint8_t TA:1;
    uint8_t MS:1;
    uint8_t DICC:4;
    byte PTY;
    char programService[9];
    char programTypeName[9];
    char radioText[65];
    word programItemNumber;
    bool linkageActuator;
    byte pagingOperatorCode;
    byte extendedCountryCode;
    byte languageCode;
    word tmcIdentification;
    byte pagingAreaCode;
    byte currentCarrierFrequency;
    TRDSAppID IRDS;
    TRDSAppID TMC;
    TRDSAppID RTP;
    TRDSAppID ERT;
    TRDSEON EON;
} TRDSData;

//RDS Decoder callback prototype.
//In general, the first argument is the semantic equivalent of the segment
//address, the second is true if this was an A group and the third parameter
//contains meaningful data; while the last two parameters are data from the
//particular RDS group blocks C and D.
//RDS_CALLBACK_AF:
//    First parameter always 0x00, second always true, third contains one pair
//    of AF codes, as presented in a 0A group, fourth is undefined.
//RDS_CALLBACK_TDC:
//    First parameter contains the segment address, second true if this was a
//    5A group, third and fourth contain data in blocks C and D of source group.
//RDS_CALLBACK_AID:
//    First parameter contains the carried in group, second always true, third
//    and fourth contain the application message and AID.
//RDS_CALLBACK_EON:
//    First parameter is 1 if this is an AF pair (variant 4), 2 if this is a
//    mapped FM frequency pair (variants 5-8) or 3 if this is a mapped AM
//    frequency pair (variant 9); second is always true, third contains the
//    frequency pair and fourth is undefined.
//RDS_CALLBACK_RT:
//    First parameter as well as the last two are always zero; the second is
//    true if it was a 2A group that triggered this RT change. The callee is
//    expected to call getRDSData() to fetch the current RT before it gets
//    replaced as signalled by the group currently being processed.
//RDS_CALLBACK_TMC:
//    First parameter is the first 5 bits of the TMC message, the second is
//    always true and the last two contain the remaining 32 bits of the TMC
//    message.
//RDS_CALLBACK_RTP:
//    First parameter is the first 5 bits of the RT+ message, the second is
//    always true and the last two contain the remaining 32 bits of the RT+
//    message.
//RDS_CALLBACK_ERT:
//    First parameter is the 5 bit character pair address code, the second is
//    always true and the last two each contain one character of the eRT text
//    message. This is implemented like this and not as a static field in
//    TRDSData because it's UCS-2 or UTF-8 and an MCU has no business dealing
//    with that or, even less so, storing 128 bytes of it. NOTE: with UTF-8
//    encoding, a single eRT message may contain anywhere from one to four
//    Unicode codepoints. NOTE: RT+ counts [displayed] characters, not bytes.
//RDS_CALLBACK_SLP:
//    First parameter is the 5 bit Paging field in the 1A group, the second is
//    always true, the third contains the slow labelling code (when used for
//    paging) and the fourth contains the paging information transmitted in the
//    PIN field, if applicable (i.e. if the day subfield of PIN is all-zeros).
//    NOTE: this is only meant for decoding the J3-J0 bits storing the interval
//    number so that a receiver can synchronize to its corresponding time slice,
//    all the other information in the group 1A paging message is already
//    available in the TRDSData struct.
//RDS_CALLBACK_P7:
//RDS_CALLBACK_P13:
//    First parameter is the Paging A/B bit and the 4-bit Paging Segment
//    Address, the second is always true and the third and fourth contain the
//    remaining 32 bits of the paging message. RDS_CALLBACK_P7 is for normal
//    paging transmitted in group 7A, whereas RDS_CALLBACK_P13 is for enhanced
//    paging transmitted in group 13A.
typedef void (*TRDSCallback)(byte, bool, word, word);

class RDSDecoder
{
    public:
        /*
        * Description:
        *   Default constructor, sets locale for further use.
        */
        RDSDecoder(byte locale = RDS_LOCALE_EU);

        /*
        * Description:
        *   Registers a new callback of the given type, which is one of the
        *   RDS_CALLBACK_* constants. The second parameter is the
        *   callback. Using NULL for the second parameter causes the currently
        *   registered callback of this type (if any) to be removed.
        */
        void registerCallback(byte type, TRDSCallback callback = NULL);
        //void unregisterCallbacks(void);

        /*
        * Description:
        *   Decodes one RDS group and updates internal data structures.
        */
        void decodeRDSGroup(word block[]);

        /*
        * Description:
        *   Returns currently decoded RDS data, filling a struct
        *   RDS_Data.
        */
        void getRDSData(TRDSData* rdsdata);

        /*
        * Description:
        *   Returns currently decoded RDS CT information filling a struct
        *   TRDSTime, if any is available, and returns true; otherwise
        *   returns false and does not touch rdstime.
        * Parameters:
        *   rdstime - pointer to a struct RDS_Time to be filled with
        *             CT information, ignore if only interested in CT
        *             availability and not actual value.
        */
        bool getRDSTime(TRDSTime* rdstime = NULL);

        /*
        * Description:
        *   Resets internal data structures, use when switching to a new
        *   station.
        */
        void resetRDS(void);

    private:
        TRDSData _status;
        TRDSTime _time;
        bool _rdstextab, _rdsptynab, _havect;
        TRDSCallback _callbacks[RDS_CALLBACK_LAST + 1];
        byte _locale;

        /*
        * Description:
        *   Filters str in place to only contain printable characters and also
        *   replaces 0x0D (CR) with 0x00 effectively ending the string at that
        *   point as per RDS §6.1.5.3.
        *   Makes a good-will effort to map the RDS character set to the one on
        *   a European CGROM Hitachi HD44780 as most users will want to display
        *   RDS information on such a display.
        *   Any unprintable character is converted to a question mark ("?"),
        *   as is customary. This helps with filtering out noisy strings.
        */
        void makePrintable(char* str);

        /*
        * Description:
        *   Maps a short PTY(ON) code as sent in Group 14B to a PTY value,
        *   obeying the current locale.
        * Parameters:
        *   shortPTY - the short PTY(ON) to map (decode)
        * Returns:
        *   mapped PTY code, according to the current locale.
        */
        byte mapShortPTY(byte shortPTY);

        /*
        * Description:
        *   When treating word values as two characters, RDS and AVR have a
        *   problem because RDS uses big endian semantics while the AVR is
        *   little endian. Hence this function to translate between the two
        *   worlds.
        * Parameters:
        *   value - the word to be switched
        */
        inline word swab(word value) { return (value >> 8) | (value << 8); }
};

typedef void (*TBlockFetcher)(const void *, void *, size_t);

class RDSTranslator
{
    public:
        /*
        * Description:
        *   Constructor, sets locale to be used for all further operations.
        */
        RDSTranslator(byte locale = RDS_LOCALE_EU) { _locale = locale; }

        /*
        * Description:
        *   Translates the given PTY into human-readable text, according to the
        *   current locale. At most textsize-1 characters will be copied to the
        *   buffer at text.
        */
        void getTextForPTY(byte PTY, char* text, byte textsize);

        /*
        * Description:
        *   Decodes the station callsign out of the PI using the method
        *   defined in the RBDS standard for North America. Note that
        *   even under RBDS, the station may have changed its PI high nibble to
        *   0x01 to signal itself as TMC capable, which will render callsign
        *   decoding impossible or errorneous.
        * 
        * Parameters:
        *   programIdentifier - a word containing the Program Identifier value
        *                       from RBDS
        *   callSign - pointer to a char[] at least 5 characters long that
        *              receives the decoded station call sign
        * Returns:
        *   true if the PI was decoded and callSign filled or false if the
        *   given PI cannot be decoded according to RBDS §D.7.1
        */
        bool decodeCallSign(word programIdentifier, char* callSign);

        /*
        * Description:
        *   Unpacks a PI value into a TRDSPI struct according to RDS §D1.
        * Parameters:
        *   programIdentifier - a word containing the Program Identifier value
        *                       from RDS
        *   unpacked - pointer to a TRDSPI struct that will receive the unpacked
        *              data.
        */
        void unpackEBUPI(word programIdentifier, TRDSPI *unpacked);

        /*
        * Description:
        *   Unpacks a PIN value into a TRDSTime struct according to
        *   RDS §6.1.5.2.
        * Parameters:
        *   programItemNumber - a word containing the Program Item Number value
        *                       from RDS
        *   unpacked - pointer to a TRDSTime struct that will receive the
        *              unpacked data.
        */
        void unpackPIN(word programItemNumber, TRDSTime *unpacked);

        /*
        * Description:
        *   Translates an RDS-TMC event distance code into a human readable
        *   measurement in km. Returns 0xFF to signal "more than 100km".
        */
        byte decodeTMCDistance(byte length);

        /*
        * Description:
        *   Translates an RDS-TMC event duration code into a human readable
        *   representation.
        */
        void decodeTMCDuration(byte length, TRDSTime* tmctime);

        /*
        * Description:
        *   Translates an AF frequency code into a human readable measurement
        *   in kHz (or tens of kHz if FM is true), valid for the current locale.
        */
        word decodeAFFrequency(byte AF, bool FM = true);

        /*
        * Description:
        *   Translates a timezone value to a human readable measurement in
        *   minutes.
        */
        int16_t decodeTZValue(int8_t tz);

        /*
        * Description:
        *   Unpacks a Group 3A TMC message into a TRDSTMCMessage3 struct.
        * Parameters:
        *   tmcMessage - a word containing block C of group 3A, when associated
        *                with the AID of TMC.
        *   unpacked - pointer to a TRDSTMCMessage3 struct that will receive the
        *              unpacked data.
        */
        void unpackTMCMessage3(word tmcMessage, TRDSTMCMessage3 *unpacked);

        /*
        * Description:
        *   Decrypts a TMC location table index according to ISO 14819-6 §9.4.
        * Parameters:
        *   location - a word containing the encrypted location table index.
        *   key - a word containing the packed decryption parameters, in the
        *         order given in ISO 14819-6 Table 4.
        *   xorValue - a byte containing the standalone XOR value for
        *              decryption.
        *   start - a nibble containing the start bit value for decryption.
        *   rol - a nibble containing the argument of the bitwise rotate left
        *         operation for decryption.
        *   encId - a nibble containing the line index into the Encryption Table
        *           where to find the decryption key.
        *   table - pointer to a table of encryption keys, either an array of 32
        *           words which are the packed decryption parameters for one
        *           Service Key; or an array of 8x32 words which are all the
        *           Service Keys.
        *   in_flash - bool, true if the table should be looked-up in Flash as
        *              opposed to RAM.
        *   serviceKey - a nibble containing the index of the Service Key to
        *                use for decryption.
        */
        word decryptLocation(word location, word key);
        word decryptLocation(word location, byte xorValue, byte start,
                             byte rol);
        word decryptLocation(word location, byte encId, const word table[],
                             bool in_flash=false);
        word decryptLocation(word location, byte serviceKey, byte encId,
                             const word table[][32], bool in_flash=false);

        /*
        * Description:
        *   Unpacks a Group 8A TMC message into a TRDSTMCMessage8 struct.
        * Parameters:
        *   tmcXbits - a byte containing bits X4-X0 of the TMC message.
        *   tmcYbits - a word containing bits Y15-Y0 of the TMC message.
        *   tmcZbits - a word containing bits Z15-Z0 of the TMC message.
        *   unpacked - pointer to a TRDSTMCMessage8 struct that will receive the
        *              unpacked data.
        */
        void unpackTMCMessage8(byte tmcXbits, word tmcYbits, word tmcZbits,
                               TRDSTMCMessage8 *unpacked);

        /*
        * Description:
        *   Glues together multiple 28 bit containers in place as used in TMC
        *   multi-group messages
        * Parameters:
        *   slices - an array of 4 uint32_t expected to contain TMC message
        *            container slices in their lower 28 bits that are to be
        *            glued in place (via SHL) to get an array of 4 uint32_t that
        *            contains a contiguous TMC message container. As per
        *            ISO 14819-1 §7.6.2, the unused bits at the end of the
        *            container will be set to zero.
        */
        void glueTMCContainerSlices(uint32_t slices[4]);

        /*
        * Description:
        *   Adjusts a TMC multi-group bit container as needed in the presence
        *   of a FLT in the location field, according to ISO 14819-1 §6.7.2. If
        *   the location given is not a FLT code, exits without touching any
        *   of the output parameters.
        * Parameters:
        *   slices - an array of 4 uint32_t containing the TMC message bit
        *            container.
        *   maybeFLT - a pointer to a word initially containing the presumed
        *              FLT. If a FLT is indeed found there, it will be
        *              overwritten with the actual location code.
        *   unpacked - a pointer to a TRDSTMCFLT struct that will receive the
        *              unpacked FLT code.
        */
        void adjustTMCContainerForFLT(uint32_t slices[4], word *maybeFLT,
                                      TRDSTMCFLT *unpacked);

        /*
        * Description:
        *   Reads the next label from the bit container.
        * Parameters:
        *   slices - an array of 4 uint32_t containing the TMC message bit
        *            container.
        *   fp - a pointer to a TRDSTMCContainerIndex struct holding the current
        *        read pointer for the container. On the first invocation (for a
        *        new container), zero out the struct to signal that reading
        *        should start from the beginning of the container; on subsequent
        *        invocations, pass the same struct to advance from the last read
        *        location.
        *   label - a pointer to a TRDSTMCLabel struct that will receive the
        *           label read.
        * Returns:
        *   bool, true if the read succeeded, false otherwise (most likely
        *   reached end of container or no more labels present). Note that
        *   according to ISO 14819-1 §5.5.1, Label 15 has an undefined size
        *   which means that a true return with a label->type of 15 should be
        *   interpreted as the end of the container.
        */
        bool readNextTMCLabel(const uint32_t slices[4],
                              TRDSTMCContainerIndex *fp, TRDSTMCLabel *label);

        /*
        * Description:
        *   Given an expected quantifier type and the corresponding label which
        *   defines it, generate a string with the human readable representation
        *   of the quantifier.
        * Parameters:
        *   qType - a nibble specifying the quantifier type, according to
        *           ISO 14819-2 §3.1.2
        *   label - a pointer to a TRDSTMCLabel struct containing the decoded
        *           quantifier value
        *   buf - a pointer to a character string buffer that will receive the
        *         human readable representation.
        *   size - the size of the buffer provided.
        */
        void decodeQuantifier(byte qType, TRDSTMCLabel *label, char *buf,
                              size_t size);

        /*
        * Description:
        *   Unpacks a Group 3A RT+ message into a TRDSRTPlusMessage3 struct.
        * Parameters:
        *   rTPMessage - a word containing block C of group 3A, when associated
        *                with the AID of RT+.
        *   unpacked - pointer to a TRDSRTPlusMessage3 struct that will receive
        *              the unpacked data.
        */
        void unpackRTPlusMessage3(word rTPMessage,
                                  TRDSRTPlusMessage3 *unpacked);

        /*
        * Description:
        *   Unpacks a Group 11A RT+ message into a TRDSRTPlusMessage11 struct.
        * Parameters:
        *   rTPbits1 - a byte containing bits 36-32 of the RT+ message.
        *   rTPbits2 - a word containing bits 31-16 of the RT+ message.
        *   rTPbits3 - a word containing bits 15-0 of the RT+ message.
        *   unpacked - pointer to a TRDSRTPlusMessage11 struct that will receive
        *              the unpacked data.
        */
        void unpackRTPlusMessage11(byte rTPbits1, word rTPbits2, word rTPbits3,
                                   TRDSRTPlusMessage11 *unpacked);

        /*
        * Description:
        *   Unpacks a Group 3A eRT message into a TRDSERTMessage3 struct.
        * Parameters:
        *   eRTMessage - a word containing block C of group 3A, when associated
        *                with the AID of eRT.
        *   unpacked - pointer to a TRDSERTMessage3 struct that will receive
        *              the unpacked data.
        */
        void unpackERTMessage3(word eRTMessage, TRDSERTMessage3 *unpacked);

        /*
        * Description:
        *   Unpacks a Group 13A Enhanced Paging message into a 
        *   TRDSPagingMessage13 struct.
        * Parameters:
        *   ePbits1 - a byte containing the CS and STY bits of the message.
        *   ePbits2 - a word containing the IT, S, X and high address
        *             notification bits of the message.
        *   ePbits3 - a word containing the low address notification bits of the
        *             message.
        *   unpacked - pointer to a TRDSPagingMessage13 struct that will receive
        *              the unpacked data.
        */
        void unpackPagingMessage13(byte ePbits1, word ePbits2, word ePbits3,
                                   TRDSPagingMessage13 *unpacked);

        /*
        * Description:
        *   Due to the encoding of the RDS pager messages, all parts comprising
        *   such a message have to be known before the type of the message (and
        *   thus the decoding method to be used) can be evaluated. This method
        *   takes an array of TRDSRawData structs and fills in a TRDSPage struct
        *   with the re-assembled and unpacked page. If the page contained a
        *   message, enough memory is allocated for its string representation
        *   and a pointer to that memory is also stored in the TRDSPage struct.
        *   Ownership of that allocation is transferred to the caller.
        * Parameters:
        *   page - an array of TRDSRawData structs, as received between two
        *          transitions of the Paging A/B flag (i.e. one complete and
        *          contiguous page).
        *   size - the number of Group 7A messages this page was made up of.
        *   unpacked - pointer to a TRDSPage struct that will receive the
        *              unpacked and re-assembled data.
        */
        void unpackRDSPage(TRDSRawData page[], byte size, TRDSPage *unpacked);

    private:
        byte _locale;

        /*
        * Description:
        *   Reads from a TMC multi-group message bit container.
        * Parameters:
        *   slices - an array of 4 uint32_t containing the TMC message bit
        *            container.
        *   fp - a pointer to a TRDSTMCContainerIndex struct holding the current
        *        read pointer for the container. On the first invocation (for a
        *        new container), zero out the struct to signal that reading
        *        should start from the beginning of the container; on subsequent
        *        invocations, pass the same struct to advance from the last read
        *        location.
        *   size - a nibble containing the number of bits to read from the
        *          container.
        * Returns:
        *   The bits read from the container, LSB-aligned to a word.
        */
        word readFromTMCContainer(const uint32_t slices[4],
                                  TRDSTMCContainerIndex *fp, byte size);

        /*
        * Description:
        *   Unpacks a TMC FLT code into a TRDSTMCFLT struct.
        * Parameters:
        *   flt - a word containing the FLT code.
        *   unpacked - pointer to a TRDSTMCFLT struc that will receive the
        *              unpacked data.
        */
        void unpackTMCFLT(word flt, TRDSTMCFLT *unpacked);

        /*
        * Description:
        *   Finds a record by id in an array. Used to lookup event message or
        *   supplementary information records. The array is assumed to be sorted
        *   ascendingly by id and to start at id == 1.
        * Parameters:
        *   table - pointer to the start of the contiguous sorted array.
        *   recSize - size in bytes of the records in the array.
        *   tableSize - size in records of the array.
        *   idOffset - offset in bytes to the id field in each record.
        *   wordId - true if the id is a word, false if it's a byte.
        *   key - the id to look for.
        *   record - pointer to a buffer at least recSize bytes long that will
        *   receive the target record if found.
        *   blockFetcher - pointer to a function used to read an arbitrarily
        *                  sized block from the record array.
        * Returns:
        *   true if a record with an id of key was found and copied to *record,
        *   false otherwise.
        */
        bool locateMessageRecord(const void *table, size_t recSize,
                                 size_t tableSize, size_t idOffset, bool wordId,
                                 word key, void *record,
                                 TBlockFetcher blockFetcher);

        /*
        * Description:
        *   Unpacks the RDS paging message header into certain members of a
        *   TRDSPage struct. NOTE: this is compatible with both normal and
        *   enhanced paging. NOTE: that this is not meant to be auto-detected,
        *   as the standard assumes that each operator's RDS broadcast and page
        *   receiver configuration would match apriori. Specifically, a national
        *   first (i.e. not repeated) enhanced alphanumeric page to a pager
        *   address in which all digits are smaller than 0xA, on a network which
        *   does not implement the call counter feature is INDISTINGUISHABLE
        *   from a basic page to the same receiver.
        * Parameters:
        *   block3 - the Y1, Y2, Z1 and Z2 digits from a Group 7A message.
        *   block4 - the Z3 and Z4 digits and (in the case of enhanced paging)
        *            the X1X2 control byte from a Group 7A message.
        *   unpacked - pointer to a TRDSPage struct whose members will receive
        *   the unpacked data.
        * Returns:
        *   true if enhanced paging was detected, false otherwise.
        */
        bool unpackPageHeader(word block3, word block4, TRDSPage *unpacked);

        /*
        * Description:
        *   Decodes BCD-encoded nibbles to their ASCII equivalents.
        * Parameters:
        *   BCD - the nibbles containing BCD values to convert to text.
        *   text - pointer to a buffer that will receive the decoded output. 0xA
        *          (an illegal BCD value) in input will be translated to '\x20'
        *          (i.e. space) in the output, as per §M.2.5.3 of RDS. NOTE: no
        *          '\0' will be added after the decoded output!
        */
        void BCD2Char(byte BCD, char *text);
        void BCD2Char(word BCD, char *text);
        void BCD2Char(word BCD1, word BCD2, char *text);

        /*
        * Description:
        *   When treating word values as two characters, RDS and AVR have a
        *   problem because RDS uses big endian semantics while the AVR is
        *   little endian. Hence this function to translate between the two
        *   worlds.
        * Parameters:
        *   value - the word to be switched
        */
        inline word swab(word value) { return (value >> 8) | (value << 8); }
};

#endif

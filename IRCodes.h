#define IR_TEUFEL_ONOFF 			    0x807F50AF

#define IR_TEUFEL_MUTE				    0x807FD02F

enum TEUFEL_CHANNELS {_51, OPT1, OPT2, COAX1, COAX2, TV, CD, AUX};
typedef enum TEUFEL_CHANNELS channels_t;
String TEUFEL_CHANNELS_STRING[] = {"5.1", "Opt1", "Opt2", "Coax1", "Coax2", "TV", "CD", "Aux"};

#define IR_TEUFEL_CHANNEL_51		  0x807F708F
#define IR_TEUFEL_CHANNEL_OPT1		0x807F609F
#define IR_TEUFEL_CHANNEL_OPT2		0x807FF00F
#define IR_TEUFEL_CHANNEL_COAX1		0x807F48B7
#define IR_TEUFEL_CHANNEL_COAX2		0x807FE01F
#define IR_TEUFEL_CHANNEL_TV		  0x807FC837
#define IR_TEUFEL_CHANNEL_CD		  0x807F6897
#define IR_TEUFEL_CHANNEL_AUX		  0x807F40BF

#define IR_TEUFEL_DISPLAYONOFF		0x807FE817

#define IR_TEUFEL_NAV_RETURN		  0x807FC03F

#define IR_TEUFEL_MODE				    0x807FE21D
#define IR_TEUFEL_TONE				    0x807F629D
#define IR_TEUFEL_TEST				    0x807FA25D

#define IR_TEUFEL_VOL_UP			    0x807F7A85
#define IR_TEUFEL_VOL_DOWN			  0x807F6A95
#define IR_TEUFEL_NAV_UP			    0x807F7A85
#define IR_TEUFEL_NAV_DOWN		  	0x807F6A95

#define IR_TEUFEL_NAV_LEFT			  0x807FDA25
#define IR_TEUFEL_NAV_RIGHT			  0x807F1AE5
#define IR_TEUFEL_NAV_OK			    0x807F5AA5

#define IR_TEUFEL_SPEAKER_FL		  0x807FCA35
#define IR_TEUFEL_SPEAKER_C 		  0x807F4AB5
#define IR_TEUFEL_SPEAKER_FR		  0x807F8A75
#define IR_TEUFEL_SPEAKER_RL		  0x807FF20D
#define IR_TEUFEL_SPEAKER_SUB		  0x807F728D
#define IR_TEUFEL_SPEAKER_RR 		  0x807FB24D



#ifndef _CONST_H_
#define _CONST_H_

#define c_MaxDevFullNameLength		40 
#define c_MaxDevNameLength			8
#define c_MaxDevNOLength			10
#define c_MaxDevDescLength			40
#define c_MaxDriverProgNameLength	128 
#define c_MaxDevFactoryLength		40
#define c_MaxCommPortNameLength		6
#define c_MaxConnectDateLength		20
#define c_MaxComputerNameLength		8
#define c_MaxWorkingIDLength        6//工作单长度

#define c_StoppedStatus				0
#define c_RunningStatus				1

//for Instrument Dict
#define	c_DevTableKey 			"INSTRUMENT_DICT" 
#define c_DevFullNameKey		"INST_NAME"
#define c_DevNameKey			"INST_ID"
#define c_DevNOKey				"INST_NO"
#define c_DevFactoryKey			"FACTORY"
#define c_DevDescKey			"DESCRIPTION"
#define	c_DevStatusKey			"SERVE_STATUS"
#define	c_AutoLoadKey			"AUTO_LOAD"
#define c_ConnDateKey	    	"CONNECT_DATE"
#define	c_CommPortKey 			"COMM_PORT" 
#define	c_BaudRateKey 			"BAUD_RATE" 
#define c_DataSizeKey 			"BYTE_SIZE" 
#define c_ParityKey				"PARITY" 
#define c_StopBitsKey 			"STOP_BITS" 
#define c_fOutXKey				"F_OUTX" 
#define c_fInXKey 				"F_INX" 
#define c_fHardWareKey			"F_HARDWARE" 
#define c_OutQueueSizeKey		"TX_QUEUESIZE" 
#define c_InQueueSizeKey		"RX_QUEUESIZE" 
#define c_XonLimKey				"XON_LIM" 
#define c_XoffLimKey			"XOFF_LIM" 
#define c_XonCharKey			"XON_CHAR" 
#define c_XoffCharKey 			"XOFF_CHAR" 
#define c_ErrorCharKey			"ERROR_CHAR" 
#define c_EvtCharKey			"EVENT_CHAR" 
#define c_DriverProgKey			"DRIVER_PROG" 
#define c_DevInfoKey			"DEVINFO"
#define c_DevNamesKey			"DEVNAMES"
#define c_DrvPathKey		    "DEVDRVPATH"
#define c_StoreDataFileKey		"STOREDATAFILE"
#define c_TransmitDataFileKey	"TRANSMITDATAFILE"
#define c_CommThreadPriorityKey	"PRIORITY"
#define c_IsBothSidesKey		"DUPLEX_FLAG"
#define c_ComputerNameKey		"COMPUTER_NAME"

#define c_StartRecordKey		"StartRecord"
#define c_IntervalKey			"Interval"

#define c_CompanyName			"CHYY"

//for data out table
#define c_DataOutTableKey	"ONLINE_DATA"
#define c_OutDeviceNameKey	"INST_ID"
#define c_OutSampleIDKey	"SAMPLE_ID"
#define c_OutItemNameKey	"ITEM_ID"
#define c_OutResultKey		"ASSAY_RESULT"
#define c_OutDateKey		"ASSAY_DATE"
#define c_OutResultTypeKey	"SAMPLE_TYPE"
#define c_OutErrorCodeKey	"RESULT_STATUS"
#define c_OutCommentKey		"STATUS_COMMENT"
#define c_OutSeriesNoKey	"SERIAL_NO"

#define c_ParityNo				0
#define c_ParityOdd				1
#define c_ParityEven			2
#define c_ParityMark			3
#define c_ParitySpace			4

#define c_Bootkey            "Software\\CHYY\\联机95\\"
#define c_LoginInfoFileKey   "LoginInfoFile"

#define c_DataSourceKey		"DataSource"
#define c_DataOutKey		"DataOut"
#define c_BothSidesKey		"BothSides"
#define c_ComputerKey		"Computer"
#define c_BaseNameKey		"BaseName"

//for DrvDll
#define	STX			0x02	   
#define	ETX			0x03	   
#define	EOT			0x04	   
#define	ENQ			0x05	   
#define	ACK			0x06	   
#define	NAK			0x15	   
#define	ETB			0x17	   
#define	DLE			0x10
#define	XON			0x11		
#define	DC1			0x11
#define	DC2			0x12	
#define	XOFF		0x13		
#define	DC3			0x13		
#define	DC4			0x14		
#define	CR			0x0d
#define	LF			0x0a
#define	ERRCHAR		0x3f
#define	SPACE		0x20
#define	CTRLZ		0x1a

#define	SUCCESS_NONE		0
#define	SUCCESS_RECEIVED	1


#define	c_MaxFilePathLength	128

#define	c_TypeSample	0x01
#define	c_TypeRoutine	0x02
#define	c_TypeNumber	0x03
#define	c_TypeQC		0x11
#define	c_TypeGraph		0x12
#define	c_TypeStat		0x13
#define	c_TypeString	0x04
#define	c_TypeEmergency	0x05
#define c_TypeUrine		0x06
#define c_TypeRepeat 	0x07
#define c_TypeReagent	0x08
#define c_TypeACAL		0x09
#define	c_TypeCode		0x10
#define	c_TypeAlphabet	0x20
#define c_TypeAutoRerun		30


#define	c_Reliable			0
#define	c_UnReliable		1

#define	c_BelowValue		2
#define	c_AboveValue		3

#define	c_NULL_SampleID		0xffff
#define c_Normal			99900	
#define	c_Negative			99901	
#define	c_Trace				99902	
#define	c_Small				99903	
#define	c_Moderate			99904	
#define	c_Large				99905
#define c_Positive			99906	
 
#define c_Yellow			99910
#define c_Dk_Yellow			99911
#define c_Straw				99912
#define c_Amber				99913
#define c_Red				99914
#define c_Orange			99915
#define c_Green				99916
#define c_OtherColor		99917
 
#define c_Clear				99930
#define c_Sl_Cloudy			99931
#define c_Cloudy			99932
#define c_Turbid			99933
#define c_OtherClarity		99934

#endif
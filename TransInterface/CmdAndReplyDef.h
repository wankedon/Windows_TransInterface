#pragma once
#include "TransComDef.h"

enum CommandID
{
	MSG_HEADER = 0X7E8118E7,        //����ͷ
	MSG_TAIL = 0X8F9009F8,          //����β
	//����
	SELF_CHECK_CMD = 0X1A00,	    //�豸�Լ�
	SYSTEM_TIME_CALIBRATION_CMD = 0X1A01,	//ϵͳУʱ
	DEVICE_RESET_CMD = 0X1A02,	    //�豸��λ
	LOG_CMD = 0X1A03,	            //��־�������
	BEAT_CMD = 0X1A04,	            //����
	DEVICE_INFO_CMD = 0X1A0E,       //������Ϣ��ȡ
	IP_CMD = 0X1A0F,			    //IP��ַ��ȡ
	SELF_CHECK_REPLY = 0XA100,	    //�ϱ��Լ���
	RESPONSE_REPLY = 0XA101,	    //�ϱ�Ӧ��ָ��
	DEVICE_TIME_REPLY = 0XA103,	    //�ϱ��豸ʱ��
	BEAT_REPLY = 0XA104,	        //����Ӧ��
	DEVICE_INFO_REPLY = 0XA10E,     //�ϱ�������Ϣ
	IP_REPLY = 0XA10F,			    //�ϱ�������ַ
	//���Ƶ�׼��
	START_PANORAMA_CMD = 0X1B00,	//����ȫ����������
	STOP_PANORAMA_CMD = 0X1B01,	    //ֹͣȫ����������
	PANORAMA_DATA_REPLY = 0XB100,   //�ϱ�ȫ������
	PA_DEVICE_STATUS_REPLY = 0XB104,//�ϱ��豸״̬��Ϣ
	//���Ŀ������
	START_IF_CMD = 0X1B10,	        //������Ƶ���ݲɼ�����
	STOP_IF_CMD = 0X1B11,	        //ֹͣ��Ƶ���ݲɼ�����
	IF_FILE_REPLY = 0XB110,	        //�ϱ���Ƶ���ݲɼ��ļ�
	IF_DEVICE_STATUS_REPLY = 0XB114,//�ϱ��豸״̬��Ϣ
	//IQ
	IQ_frameHeader = 0x7E7E7E7E,
	IQ_packHeader = 0X7E7E0705,
	IQ_dataHeader = 0X360B,
};

#pragma pack(1)
struct DataBlock
{
	std::size_t len; //����
	void* addr;	     //��ַ
};

struct paramheader
{
	uint32_t sweepNumber;            //ɨ�����
	uint32_t blocksNumber;           //ɨ�����
	uint32_t tranferSampleNumber;    //�ɼ�����
	uint32_t timerTiggerType;        //��������
	int64_t timeStamp;               //����ʱ��
	uint32_t sweepInterval;          //ɨ����
	int64_t segmentInterval;         //Ƶ����
	uint32_t segmentNumber;          //Ƶ�����
};

struct SystemDeviceCode
{
	transType systemCodeLength;    //ϵͳ���ų���
	unsigned char systemCode[20]; //ϵͳ������
	transType deviceCodeLength;    //�������ų���
	unsigned char deviceCode[20]; //����������
};

struct IQMessage
{
	int32_t header;      //֡ͷ
	int32_t	length;      //֡��
	int64_t	index;       //֡��
	IQData pack;
};

struct Message
{
	transType header;              //����ͷ
	transType length;              //��ǰ�����ݳ���
	unsigned short destAdress;    //Ŀ�ĵ�ַ
	unsigned short sourceAdress;  //Դ��ַ
	unsigned char domainId;       //��ID
	unsigned char topicId;        //����ID
	unsigned short infoType;      //��Ϣ����
	uint64_t  sendTime;           //��������ʱ��
	transType sequenceNum;         //���к�
	transType totalPackCounts;     //������
	transType currentPackNum;      //��ǰ����
	transType dataLength;          //�����ܳ���
	unsigned short version;       //�汾��
	char reserved[6];             //�����ֶ�
	BYTE* dataBlock;              //��������
	transType checkSum;            //У���   
	transType tail;                //����β
};

struct MsgHeader
{
	transType infoLength;        //��Ϣ����
	int64_t taskSeriaNumber;    //������ˮ��
};

struct SelfCheckCmd
{
	MsgHeader msgHeader;
	unsigned char selfCheckType;   //�Լ�����
	unsigned short selfCheckCycle; //�Լ�����
	unsigned char selfCheckCount;  //�Լ�����
	unsigned char deviceId[4];    //���쵥������
};

struct SystemTimeCalibParam
{
	unsigned char systemTimeType;  //Уʱģʽ
	SYSTEMTIME systemTime1;         //ʱ��
};

struct SystemTimeCalibCmd
{
	MsgHeader msgHeader;
	SystemTimeCalibParam sysTimeCalParam;
};

struct LogCmd
{
	MsgHeader msgHeader;
	unsigned char execMode;     //ִ�з�ʽ
};

struct HeartCmd
{
	MsgHeader msgHeader;
	unsigned char heart; //����
};

struct DeviceInfoCmd
{
	MsgHeader msgHeader;
	SystemDeviceCode code;//ϵͳ�豸����
};

struct IpAddressCmd
{
	MsgHeader msgHeader;
	SystemDeviceCode code;//ϵͳ�豸����
};

struct StartPanoCmd
{
	MsgHeader msgHeader;
	StartPanoParam startpanoParam;
};

struct StopPanoCmd
{
	MsgHeader msgHeader;
	StopTaskParam stopPanoParam;
};

struct StartIFCmd
{
	MsgHeader msgHeader;
	StartIFParam startIFParam;
};

struct StopIFCmd
{
	MsgHeader msgHeader;
	StopTaskParam stopIFParam;
};

struct SelfCheckReply
{
	MsgHeader msgHeader;
	unsigned char deviceCode[4];  //���쵥������
	unsigned char count;           //����
	unsigned char moudleNumnber;   //ģ���
	unsigned short erroCode;       //����ԭ�����
};

struct CmdReply
{
	MsgHeader msgHeader;
	unsigned short infoType; //��Ӧ����Ϣ����
	unsigned short erroType; //��������
};

struct DeviceTimeReply
{
	MsgHeader msgHeader;
	SYSTEMTIME time;//ʱ��
};

struct HeartReply
{
	MsgHeader msgHeader;
	unsigned char heart; //����
};

struct DeviceInfoReply
{
	MsgHeader msgHeader;
	SystemDeviceCode code;    //ϵͳ�豸����
	unsigned char version[3]; //�汾��
	SYSTEMTIME time;          //�������ʱ��
};

struct IpAddressReply
{
	MsgHeader msgHeader;
	SystemDeviceCode code; //ϵͳ�豸����
	unsigned char ip[4];   //ip��ַ
};

struct PanoThrdReply
{
	MsgHeader msgHeader;        //������Ϣͷ
	unsigned short dataCount;   //������Ϣ����
	int64_t startFrequency;     //���޿�ʼƵ��
	int64_t  bandWidth;         //����Ƶ�ο��
	transType thredLine = 0;    //����������
	float* level = nullptr;     //���޷��ȵ�ƽ
};

struct PanoReply
{
	MsgHeader msgHeader;
	unsigned short dataCount; //��Ϣ����
	PanoData panoData;
	PanoThrdReply  panoThrdReply; //����
};

struct DeviceStatusReply
{
	MsgHeader msgHeader;
	unsigned char count;
	TrDeviceStatus deviceStatus;
};
#pragma pack()
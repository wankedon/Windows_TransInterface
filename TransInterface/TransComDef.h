#pragma once
#include <stdint.h>
#ifdef PLATFROM_WINDOWS
#include "winsock2.h"
#include <windows.h>
#else
typedef unsigned char BYTE;
#endif
typedef unsigned int transType;

//��־ģʽ
enum LogExecMode
{
	CLEAR_LOG = 0, //���
	DO_LOG = 1,    //��¼
	UNDO_LOG = 2,  //����¼
};

#pragma pack(1)
#ifdef PLATFROM_WINDOWS
#else
struct SYSTEMTIME
{
	short wYear;
	short wMonth;
	short wDayOfWeek;
	short wDay;
	short wHour;
	short wMinute;
	short wSecond;
	short wMilliseconds;
};
#endif

//ͨ�����߷��͵�����
struct RawData
{
	int len = 0;          //����
	char* addr = nullptr; //��ַ
};

//��ʼȫ���������
struct StartPanoParam
{
	int64_t startFrequency;     //��ʼƵ��
	int64_t  bandWidth;         //Ƶ�ο��
	int64_t  resolutionBW;      //�ֱ���
	unsigned short detectMode;  //�첨ģʽ
	unsigned short spectrumLine;//������
	unsigned short interval;    //�ش����
	unsigned char dataReport;   //�����ϱ�ģʽ
};

//IQ����
struct IFParam
{
	int64_t centerFreq;        //����Ƶ��
	int64_t sampleRate;        //������
	int32_t attenuationGain;   //����˥��
	int32_t antenna;           //����ѡ��
	unsigned short tiggerType; //��������
	short tiggerValue;         //������ƽ
};

//��ʼIQ�������
struct StartIFParam
{
	uint32_t sweepNumber;            //ɨ�����
	uint32_t blocksNumber;           //ɨ�����
	uint32_t tranferSampleNumber;    //�ɼ�����
	uint32_t timerTiggerType;        //��������
	int64_t timeStamp;               //����ʱ��
	uint32_t sweepInterval;          //ɨ����
	int64_t segmentInterval;         //Ƶ����
	uint32_t segmentNumber;          //Ƶ�����
	IFParam* ifParam;                //��Ƶ����
	unsigned short dataReportMethod; //���ݻ��ͷ�ʽ
	unsigned short fileNameLength;   //�ļ�������
	char* fileName;                  //�ļ���
};

//ֹͣȫ��/IQ�������
struct StopTaskParam
{
	unsigned short infCount; //��Ϣ����
	unsigned short* channel; //ͨ����
};

//�豸״̬
struct TrDeviceStatus
{
	unsigned char moudleNumnber;    //ģ���
	unsigned short electricCurrent; //����
	unsigned short voltage;         //��ѹ
	unsigned short temperature;     //�¶�
};

//�豸
struct TrDevice
{
	char* name = nullptr;   //�豸����
	char* ip = nullptr;   //ip��ַ
	TrDeviceStatus status;
};

//�豸�б�
struct DevList
{
	int deviceSize = 0;
	TrDevice* devicePtr = nullptr;
};

//ȫ���ź�
struct PaSignal
{
	int64_t frequency;   //Ƶ��
	int64_t  bandWidth;  //����
	float maxLevel;      //�����
	float minLevel;      //��С����
	float averageLevel;  //ƽ������
	int64_t occurTime;   //����ʱ��
	int32_t occurCount;  //���ִ���
};

//ȫ������
struct PanoData
{
	int64_t startFrequency;         //��ʼƵ��
	int64_t  bandWidth;             //Ƶ�ο��
	transType spectrumLine = 0;      //������
	SYSTEMTIME systime;             //ȫ���ɼ�ʱ��
	int32_t signalCount = 0;        //�ź�����
	float* level = nullptr;         //���ȵ�ƽ
	PaSignal* signalList = nullptr; //�ź�
	transType spectrumLine_thd = 0; //����������
	float* level_thd = nullptr;     //���޷��ȵ�ƽ
};

//IQ����
struct IQData
{
	int32_t packheader;      //���ݰ���־
	int32_t packbytes;       //���ݰ�����
	int64_t packindex;       //���ݰ���
	int16_t dataheader;      //���ݱ�־
	int16_t ddcId;           //�ŵ���
	int16_t channelType;     //�ŵ�����
	int16_t dataType;        //��������
	SYSTEMTIME samplingTime; //�ɼ�ʱ��
	float signalFrequ;       //�ź�Ƶ��
	float samplingRate;      //����Ƶ��
	int32_t signalID;        //�ź�ID
	int32_t dataBytes = 0;   //���ݳ���
	BYTE* dataPtr = nullptr; //IQ����
};
#pragma pack()

//��������������
typedef void ChangeLogModeHandler(const LogExecMode& mode, void* ptr);    //�ı���־ģʽ
typedef uint32_t StartPanoHandler(const StartPanoParam& param, void* ptr);//��ʼȫ��
typedef uint32_t StopPanoHandler(const StopTaskParam& param, void* ptr);  //ֹͣȫ��
typedef uint32_t StartIFHandler(const StartIFParam& param, void* ptr);    //��ʼIQ
typedef uint32_t StopIFHandler(const StopTaskParam& param, void* ptr);    //ֹͣIQ
struct ControlCallBacks
{
	void* owner = nullptr;
	ChangeLogModeHandler* changeLogMode = nullptr;
	StartPanoHandler* startPano = nullptr;
	StopPanoHandler* stopPano = nullptr;
	StartIFHandler* startIF = nullptr;
	StopIFHandler* stopIF = nullptr;
};

//�����ϱ���������
typedef void ReportPanoDataHandler(const PanoData& panodata, void* ptr);//�ϱ�ȫ������
typedef void ReportIQDataHandler(const IQData& iqdata, void* ptr);      //�ϱ�IQ����
typedef void ReportDevInfoHandler(const DevList& devcies, void* ptr);   //�ϱ��豸��Ϣ
struct ReportCallBacks
{
	void* owner = nullptr;
	ReportPanoDataHandler* reportPanoData = nullptr;
	ReportIQDataHandler* reportIQData = nullptr;
	ReportDevInfoHandler* reportDevInfo = nullptr;
};

//Nodeͨ�����߷�������
typedef void SendByBusHandler(char* pData, int nLength, const int infoType, void* ptr);//ͨ�����߷�������
struct BusSendCallBack
{
	void* owner = nullptr;
	SendByBusHandler* sendByBus = nullptr;
};
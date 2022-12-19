//#ifdef __cplusplus
//extern "C" {
//#endif
#ifdef PLATFROM_WINDOWS
#ifdef TRANSINTERFACE_EXPORTS
#define TRANSINTERFACE_API __declspec(dllexport)
#else
#define TRANSINTERFACE_API __declspec(dllimport)
#endif
#else
#define TRANSINTERFACE_API
#endif
#include "TransComDef.h"
//错误码
enum TR_Error
{
	TR_ERR_NONE = 0,
	TR_ERR_INIT = -1,
	TR_ERR_INVALID_HANDLE = -2,
	TR_ERR_SEND = -3,
	TR_ERR_NO_HANDLE_RESOURCE = -4,
};
//初始化库
TRANSINTERFACE_API TR_Error InitLib();
//创建通信
TRANSINTERFACE_API TR_Error CreateTransport(int* fd, const char* udpPoint, const ControlCallBacks& ccbs, const BusSendCallBack& bscb);
//输入原始数据
TRANSINTERFACE_API TR_Error InputRawData(int fd, const RawData& buffer);
//数据上报回调函数
TRANSINTERFACE_API TR_Error GetReportCallBacks(int fd, ReportCallBacks& rcbs);
//关闭通信
TRANSINTERFACE_API TR_Error CloseTransport(int fd);
//关闭库
TRANSINTERFACE_API TR_Error DeInitLib();
//#ifdef __cplusplus
//}
//#endif
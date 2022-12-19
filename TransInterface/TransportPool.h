#pragma once
class Transport;
class TransportPool
{
public:
	TransportPool();
	~TransportPool();

public:
	std::pair<int, TR_Error> newHandle(const char*  udpPoint, const ControlCallBacks& ccbs, const BusSendCallBack& bscb);
	TR_Error closeHandle(int handle);
	std::shared_ptr<Transport> getTransport(int handle);
private:
	std::map<int, std::shared_ptr<Transport>> m_socketsInUse; ///<���õ�DeviceSession
	std::list<int> m_Handles;		        ///�����
	const int MAX_HANDLE_COUNT = 64;	///�������
	const int MAGIC_NUM = 0xCE41;	    ///�����ʶ����
};


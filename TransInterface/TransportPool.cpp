#include "stdafx.h"
#include "TransInterface.h"
#include "TransportPool.h"
#include "Transport.h"

using namespace std;

TransportPool::TransportPool()
{
	for (auto i = 0; i < MAX_HANDLE_COUNT; i++)
	{
		m_Handles.push_back(MAGIC_NUM + i);   ///���캯���й���MAX_HANDLE_COUNT�����þ��
	}
}


TransportPool::~TransportPool()
{
}

/**
* @brief �����µ�����
* @param[in] peerAddress Զ��������ַ
* @param[in] handler �ص���������
* @return pair<int, PS_Error>
*/
std::pair<int, TR_Error> TransportPool::newHandle(const char*  udpPoint, const ControlCallBacks& ccbs, const BusSendCallBack& bscb)
{
	if (m_Handles.empty())
		return std::pair<int, TR_Error>{-1, TR_ERR_NO_HANDLE_RESOURCE};
	auto transport = make_shared<Transport>(udpPoint, ccbs, bscb);
	int handle = m_Handles.front();
	assert(m_socketsInUse.find(handle) == m_socketsInUse.end());
	m_socketsInUse[handle] = transport;
	m_Handles.pop_front();
	return std::pair<int, TR_Error>{handle, TR_ERR_NONE};
}

/**
* @brief ���DeviceSession��һ��ʵ��
* @param[in] handle PositionSocket���
* @return shared_ptr<DeviceSession>
* @retval DeviceSessionʵ����ָ��
*/
shared_ptr<Transport> TransportPool::getTransport(int handle)
{
	if (m_socketsInUse.find(handle) == m_socketsInUse.end()) 
		return nullptr;
	return m_socketsInUse[handle];
}

/**
* @brief �رվ��
* @details
* @param[int] handle PositionSocket���
* @return PS_Error
*/
TR_Error TransportPool::closeHandle(int handle)
{
	if (m_socketsInUse.find(handle) == m_socketsInUse.end())
		return TR_ERR_INVALID_HANDLE;
	m_socketsInUse[handle]->release();
	m_socketsInUse.erase(handle);			///< �ų�socket���Զ�����
	m_Handles.push_back(handle);		    ///< �黹�����
	return TR_ERR_NONE;
}
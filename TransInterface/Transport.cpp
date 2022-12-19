#include"stdafx.h"
#include"Transport.h"
#include"Interactor.h"
#include"UDPTransport.h"
#include"tools.h"
#include "Logger.h"

Transport::Transport(const char*  udpPoint, const ControlCallBacks& ccbs, const BusSendCallBack& bscb)
	:m_ccbs(ccbs)
{
	auto receiveByUdp = [this](asio::ip::udp::endpoint point, asio::const_buffer buffer)
	{
		std::vector<char> tempbuffer(buffer.size());
		memcpy(tempbuffer.data(), buffer.data(), buffer.size());	
		RawData rawdata;
		rawdata.addr = tempbuffer.data();
		rawdata.len = buffer.size();
		inputRawData(rawdata);
	};
	m_udp = std::make_unique<UDPTransport>(5555, udpPoint, receiveByUdp);
	auto sendByUdp = [this](const std::vector<char> buffer)
	{
		if (m_udp)
		{
			m_udp->send(asio::buffer(buffer));
		}
	};
	ReceiveCallBacks rcbs{
	std::bind(&Transport::onReceiveStartPano, this, std::placeholders::_1),
	std::bind(&Transport::onReceiveStopPano, this, std::placeholders::_1),
	std::bind(&Transport::onReceiveStartIF, this, std::placeholders::_1),
	std::bind(&Transport::onReceiveStopIF, this, std::placeholders::_1),
	std::bind(&Transport::onReceiveChangeLog, this, std::placeholders::_1)
	};
	m_inter = std::make_unique<Interactor>(sendByUdp, rcbs, bscb);
	startBeat();
	m_udp->start();
}

Transport::~Transport()
{
	release();
}

void Transport::release()
{
	if (m_udp)
		m_udp->stop();
	if (m_inter)
		m_inter.reset();
}

//��ʼ����
void Transport::startBeat()
{
	auto timeoutHandler = [this]()
	{
		m_inter->sendBeat();
	};
	m_udp->startTimer(BEAT_INTERVAL_MS, timeoutHandler);
}

//����ԭʼ����
void Transport::inputRawData(const RawData& buffer)
{
	if (m_inter)
		m_inter->inputRawData(buffer);
}

void Transport::reportCallBacks(ReportCallBacks& rcbs)
{
	rcbs.owner = this;
	rcbs.reportPanoData = sendPanoData;
	rcbs.reportIQData = sendIQData;
	rcbs.reportDevInfo = sendDevInfo;
}

//��־ָ��ص�����
void Transport::onReceiveChangeLog(const LogExecMode& mode)
{
	if (m_ccbs.changeLogMode)
		m_ccbs.changeLogMode(mode, m_ccbs.owner);
}

//��ʼȫ������ص�����
void Transport::onReceiveStartPano(const StartPanoParam& param)
{
	if (!m_ccbs.startPano)
		return;
	auto erroType = m_ccbs.startPano(param, m_ccbs.owner);
	sendTaskCmdReply(START_PANORAMA_CMD, erroType);
}

//ֹͣȫ������ص�����
void Transport::onReceiveStopPano(const StopTaskParam& param)
{
	if (!m_ccbs.startPano)
		return;
	auto erroType = m_ccbs.stopPano(param, m_ccbs.owner);
	sendTaskCmdReply(STOP_PANORAMA_CMD, erroType);
}

//��ʼIQ����ص�����
void Transport::onReceiveStartIF(const StartIFParam& param)
{
	if (!m_ccbs.startIF)
		return;
	auto erroType = m_ccbs.startIF(param, m_ccbs.owner);
	sendTaskCmdReply(START_IF_CMD, erroType);
}

//ֹͣIQ����ص�����
void Transport::onReceiveStopIF(const StopTaskParam& param)
{
	//m_iqDataToSend.clear();
	if (!m_ccbs.stopIF)
		return;
	auto erroType = m_ccbs.stopIF(param, m_ccbs.owner);
	sendTaskCmdReply(STOP_IF_CMD, erroType);
}

//����ȫ������
void Transport::sendPanoData(const PanoData& panodata,void* ptr)
{
	Transport* self = (Transport*)ptr;
	if (self->m_inter)
	{
		self->m_inter->sendPanoData(panodata);
	}
}

//����IQ����
void Transport::sendIQData(const IQData& iqdata, void* ptr)
{
	Transport* self = (Transport*)ptr;
	if (self->m_inter)
	{
		self->m_inter->sendIQData(iqdata);
	}
}

//�����豸��Ϣ
void Transport::sendDevInfo(const DevList& devcies, void* ptr)
{
	Transport* self = (Transport*)ptr;
	if (self->m_inter)
	{
		self->m_inter->updateDevcie(devcies);
	}
}

//����������Ӧ
void Transport::sendTaskCmdReply(const uint32_t infoType, const uint32_t erroType)
{
	if (m_inter)
		m_inter->sendTaskCmdReply(infoType, erroType);
}
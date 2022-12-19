/**
* @file BytesBuffer.cpp
* @brief 字节流缓冲区
* @author 装备事业部软件组 王克东
* @date 2022:06:03
* @Copyright (c) 2022 中国电子科技集团公司第四十一研究所
*/ 
#include"stdafx.h"
#include "BytesBuffer.h"
#include "MessageFrame.h"
#include "CmdAndReplyDef.h"
using namespace std;

BytesBuffer::BytesBuffer()
{
}

BytesBuffer::~BytesBuffer()
{
}

/**
* @brief 数据块放入缓冲，提取完整数据帧
* @param[in] db 接收数据块
*/
list<std::shared_ptr<MessageFrame>> BytesBuffer::inputBuffer(const std::vector<char>& recBuf)
{
	list<std::shared_ptr<MessageFrame>> result;
	vector<char> temp = recBuf;
	m_bytebuffer.insert(m_bytebuffer.end(), temp.begin(), temp.end());
	while (true)
	{
		if (m_bytebuffer.size() == 0)
			break;
		auto frameList = extractFullFrame(m_bytebuffer);
		if (frameList.size() == 0)
			break;
		for (auto fr : frameList)
		{
			auto ptr = *fr.get();
			vector<char> fb(ptr.size());
			memcpy(fb.data(), ptr.data(), ptr.size());
			auto frame = make_shared<MessageFrame>(fb);
			if (frame->size() == 0)
				break;
			result.push_back(frame);
			m_bytebuffer.erase(m_bytebuffer.begin(), m_bytebuffer.begin() + frame->size());
		}
	}
	return result;
}

/**
* @brief 提取完整帧
* @param[in] buffer 字节数组
*/
list<std::shared_ptr<vector<char>>> BytesBuffer::extractFullFrame(vector<char>& byteBuffer)
{
	list<std::shared_ptr<vector<char>>> frameList;
	if (byteBuffer.size() < 2 * sizeof(transType))
		return frameList;
	vector<char> allFrame;
	m_headIndex = extractFrameHead(allFrame, byteBuffer);
	if (m_headIndex.size() == 0)
		return frameList;
	if (m_headIndex.size() > 1)//包含多个帧头
	{
		multiFrameExtract(allFrame, frameList);
	}
	else//包含一个帧头
	{
		allFrame = { allFrame.begin() + m_headIndex[0],allFrame.end() };
	}
	//多帧剩余数据或包含单帧头
	singleFrameExtract(allFrame, frameList);
	return frameList;
}

/**
* @brief 提取帧头
* @param[out] allFrame char数组
* @param[in]  byteBuffer 字节数组
* @return vector<int> 帧头位置
*/
vector<int> BytesBuffer::extractFrameHead(vector<char>& allFrame, vector<char>& byteBuffer)
{
	allFrame = byteBuffer;
	vector<int> headIndex(0);//帧头位置
	for (int index = 0; index < byteBuffer.size(); index++)
	{
		transType header;
		memcpy(&header, &byteBuffer[index], sizeof(transType));
		if (header == MSG_HEADER)
		{
			headIndex.emplace_back(index);
		}
	}
	return headIndex;
}

/**
* @brief 多帧提取
* @param[out] allFrame 提取帧后剩余char数组
* @param[out]  frameList 提取的完整帧
*/
void BytesBuffer::multiFrameExtract(vector<char>& allFrame, list<std::shared_ptr<vector<char>>>& frameList)
{
	if (m_headIndex.size() == 0)
		return;
	int eraseSize = 0;//擦除的字节数
	for (int i = 0; i < m_headIndex.size() - 1; i++)
	{
		vector<char> oneFrame = { allFrame.begin() + m_headIndex[i],allFrame.begin() + m_headIndex[i + 1] };
		frameList.push_back(make_shared<vector<char>>(oneFrame));
		transType frameLength;
		memcpy(&frameLength, &allFrame[m_headIndex[i] + sizeof(transType)], sizeof(transType));
		eraseSize += frameLength;
	}
	//擦除完整帧
	allFrame.erase(allFrame.begin() + m_headIndex[0], allFrame.begin() + eraseSize);
}

/**
* @brief 单帧提取
* @param[out] allFrame 提取帧后剩余uint32_t数组
* @param[out]  frameList 提取的完整帧
*/
void BytesBuffer::singleFrameExtract(vector<char>& allFrame, list<std::shared_ptr<vector<char>>>& frameList)
{
	if (allFrame.size() < 2 * sizeof(transType))
		return;
	transType frameLength;
	memcpy(&frameLength, &allFrame[4], sizeof(transType));
	if (allFrame.size() < frameLength)
		return;
	vector<char> oneFrame = { allFrame.begin(),allFrame.begin() + frameLength };
	frameList.push_back(make_shared<vector<char>>(oneFrame));
	//擦除
	allFrame.erase(allFrame.begin(), allFrame.begin() + frameLength);
}
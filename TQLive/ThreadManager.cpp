/***********************************************************************************
*								ThreadManager.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-10
*
* Description: 通用线程启动实例
***********************************************************************************/

#include "ThreadManager.h"

QThread* ThreadManager::m_pAgentThread = NULL;

QThread* ThreadManager::getAgentThread()
{
	if (m_pAgentThread == NULL)
	{
		m_pAgentThread = new QThread;
	}
	return m_pAgentThread;
}

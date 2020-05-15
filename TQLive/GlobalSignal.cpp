/***********************************************************************************
*								GlobalSignal.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 全局信号类
***********************************************************************************/
#include "GlobalSignal.h"

GlobalSignal* GlobalSignal::m_pInstance = Q_NULLPTR;

GlobalSignal* GlobalSignal::instance()
{
	if (m_pInstance == Q_NULLPTR)
	{
		m_pInstance = new GlobalSignal;
	}
	return m_pInstance;
}

GlobalSignal::GlobalSignal(QObject *parent)
	: QObject(parent)
{

}

GlobalSignal::~GlobalSignal()
{

}

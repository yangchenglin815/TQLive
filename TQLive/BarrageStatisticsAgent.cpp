/***********************************************************************************
*								BarrageStatisticsAgent.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-10
*
* Description: 弹幕助手统计线程
***********************************************************************************/

#include "BarrageStatisticsAgent.h"
#include "EnumData.h"
#include "ConstData.h"
#include "GlobalSignal.h"

void BarrageStatisticsAgent::onUpdate(const BarrageStatisticsInfo & barrageStatisticsInfo)
{
	QMutexLocker locker(&m_pBarrageStatisticsMutex);
	m_pVector.push_back(barrageStatisticsInfo);
	m_pDoWork = true;
}

BarrageStatisticsAgent::BarrageStatisticsAgent(QObject *parent)
	: QThread(parent)
{
	InitConnect();
}

BarrageStatisticsAgent::~BarrageStatisticsAgent()
{

}

void BarrageStatisticsAgent::run()
{
	while (true)
	{
		if (m_pDoWork)
		{
			QMutexLocker locker(&m_pBarrageStatisticsMutex);
			int barrageMsgTotal = 0;
			int generalUserTotal = 0;
			int vipUserTotal = 0;
			int readlBarrageMsgTotal = 0;

			QMap<int, BarrageStatisticsInfo> barrageStatisticsInfoMap;
			for (auto &iter : m_pVector)
			{
				bool isExist = false;
				QMap<int, BarrageStatisticsInfo>::const_iterator index = barrageStatisticsInfoMap.find(iter.nUserId);
				if (index != barrageStatisticsInfoMap.end() && index.key() == iter.nUserId) {
					isExist = true;
					BarrageStatisticsInfo barrageStatisticsInfoTemp = index.value();
					switch (iter.nMsgType)
					{
					case USER_LOGOUT_LIVE:
						barrageStatisticsInfoTemp.bInRoom = false;
						break;
					case USER_LOGIN_LIVE:
						barrageStatisticsInfoTemp.bInRoom = true;
						break;
					case USER_SEND_MSG:
						if (barrageStatisticsInfoTemp.nUserMsgTotal < USER_BARRAGEMSG_NUMMAX)
							barrageStatisticsInfoTemp.nUserMsgTotal++;
						if (barrageStatisticsInfoTemp.nUserRole != -1) {
							barrageStatisticsInfoTemp.nRealUserMsgTotal++;
							break;
					default:
						break;
						}
					}
					barrageStatisticsInfoMap.insert(barrageStatisticsInfoTemp.nUserId, barrageStatisticsInfoTemp);
				}

				if (!isExist)
					barrageStatisticsInfoMap.insert(iter.nUserId, iter);
			}

			QMap<int, BarrageStatisticsInfo>::const_iterator i = barrageStatisticsInfoMap.constBegin();
			while (i != barrageStatisticsInfoMap.constEnd())
			{
				barrageMsgTotal += i.value().nUserMsgTotal;
				readlBarrageMsgTotal += i.value().nRealUserMsgTotal;

				if (i.value().bInRoom)
				{
					if (i.value().nUserVipLevel > 0)
						vipUserTotal++;
					else
						generalUserTotal++;
				}
				++i;
			}

				GlobalSignal::instance()->onSigReportBarrageMsgTotal(barrageMsgTotal, generalUserTotal, vipUserTotal, readlBarrageMsgTotal);
				m_pDoWork = false;
		}
		QThread::msleep(500);
	}
}

void BarrageStatisticsAgent::onClear()
{
	QMutexLocker locker(&m_pBarrageStatisticsMutex);
	m_pDoWork = false;
	m_pVector.clear();
	GlobalSignal::instance()->onSigReportBarrageMsgTotal(0, 0, 0, 0);
}

void BarrageStatisticsAgent::InitConnect()
{

}


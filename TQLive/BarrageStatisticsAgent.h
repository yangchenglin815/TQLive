#pragma once

#include <QObject>
#include <QMap>
#include <QMutex>
#include <QThread>
#include <QVector>
#include "StructData.h"

class BarrageStatisticsAgent : public QThread
{
	Q_OBJECT

public:
	BarrageStatisticsAgent(QObject *parent = Q_NULLPTR);
	~BarrageStatisticsAgent();

protected:
	virtual void run();

public slots:
	void onUpdate(const BarrageStatisticsInfo & barrageStatisticsInfo);
	void onClear();

private:
	void InitConnect();

private:
	QMutex m_pBarrageStatisticsMutex;
	QVector<BarrageStatisticsInfo> m_pVector;
	bool m_pDoWork = false;
};

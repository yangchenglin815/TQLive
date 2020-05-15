#pragma once

#include <QObject>
#include <QThread>

class ThreadManager : public QObject
{
	Q_OBJECT

public:
	static QThread* getAgentThread();

private:
	static QThread* m_pAgentThread;
};

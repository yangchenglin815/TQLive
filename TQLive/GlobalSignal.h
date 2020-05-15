#pragma once

#include <QObject>

class GlobalSignal : public QObject
{
	Q_OBJECT

public:
	static GlobalSignal* instance();

signals:
	void onSigUpdateLiveEndTime(QString durationTime);
	void onSigCatchException(int errCode, QString errMsg);
	void onSigReportBarrageMsgTotal(int barrageMsgTotal, int generalUserTotal, int vipUserTotal, int readBarrageMsgTotal);
	void onSigClearStatisticsData();
	void onSigInitStatisticsData();

	void onSigSourceSet();
	void onSigSourceUp();
	void onSigSourceDown();
	void onSigSourceDel();
	void onSigHaveSourceItems(bool bHave);
	void onLeftButtonClicked();

private:
	explicit GlobalSignal(QObject *parent = Q_NULLPTR);
	~GlobalSignal();

private:
	static GlobalSignal* m_pInstance;
};

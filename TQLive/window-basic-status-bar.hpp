#pragma once

#include <QStatusBar>
#include <QPointer>
#include <QTimer>
#include <util/platform.h>
#include <obs.h>
#include <QTimer>
#include "CDataCentre.h"
#include "EnumData.h"

class QLabel;
class ScrollTextComponent;

class OBSBasicStatusBar : public QStatusBar {
	Q_OBJECT

private:
	QLabel *codeRate;
	QLabel *fpsRate;
	QLabel *droppedFrames;
	QLabel *cpuUsage;
	QLabel *kbps;
	QLabel *liveIcon;
	QLabel *streamTime;

	QLabel *delayInfo;
	QLabel *statusSquare;

	ScrollTextComponent *advertisementText;

	obs_output_t *streamOutput = nullptr;
	obs_output_t *recordOutput = nullptr;
	bool active = false;
	bool overloadedNotify = true;

	int retries = 0;
	int totalStreamSeconds = 0;
	int totalRecordSeconds = 0;

	int reconnectTimeout = 0;

	int delaySecTotal = 0;
	int delaySecStarting = 0;
	int delaySecStopping = 0;

	int startSkippedFrameCount = 0;
	int startTotalFrameCount = 0;
	int lastSkippedFrameCount = 0;

	int      bitrateUpdateSeconds = 0;
	uint64_t lastBytesSent = 0;
	uint64_t lastBytesSentTime = 0;

	QPixmap transparentPixmap;
	QPixmap greenPixmap;
	QPixmap grayPixmap;
	QPixmap redPixmap;

	float lastCongestion = 0.0f;

	QPointer<QTimer> refreshTimer;

	obs_output_t *GetOutput();
	QList<ST_SYSTEM_ANNOUNCEMENT> m_pSystemNotificationList;
	QTimer *m_pSystemNotificationTimer;
	int     m_nCycleTimes;
	QString m_pLiveTime;

	void Activate();
	void Deactivate(int abortType);

	void UpdateDelayMsg();
	void UpdateBandwidth();
	void UpdateStreamTime();
	void UpdateRecordTime();
	void UpdateDroppedFrames();

	static void OBSOutputReconnect(void *data, calldata_t *params);
	static void OBSOutputReconnectSuccess(void *data, calldata_t *params);

signals:
	void onSigUpdateRecordTime(QString time);
	void onSigUpdateLiveTime(QString time);

private slots:
	void Reconnect(int seconds);
	void ReconnectSuccess();
	void UpdateStatusBar();
	void UpdateCPUUsage();
	void onGetSystemNotification(int nStatusCode, const QString &qsMsg, const QList<ST_SYSTEM_ANNOUNCEMENT> &obList, const void *userData);
	void onShowSystemNotification();

public:
	OBSBasicStatusBar(QWidget *parent);

	void StreamDelayStarting(int sec);
	void StreamDelayStopping(int sec);
	void StreamStarted(obs_output_t *output);
	void StreamStopped(int abortType);
	void RecordingStarted(obs_output_t *output);
	void RecordingStopped();

	void ReconnectClear();
};

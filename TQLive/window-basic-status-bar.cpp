#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>
#include "obs-app.hpp"
#include "window-basic-main.hpp"
#include "window-basic-status-bar.hpp"
#include "window-basic-main-outputs.hpp"
#include "ScrollTextComponent.h"
#include "HttpAgent.h"
#include "GlobalSignal.h"

OBSBasicStatusBar::OBSBasicStatusBar(QWidget *parent)
	: QStatusBar    (parent),
	  codeRate      (new QLabel),
	  fpsRate       (new QLabel),
	  droppedFrames (new QLabel),
	  cpuUsage      (new QLabel),
	  kbps          (new QLabel),
	  liveIcon      (new QLabel),
	  streamTime    (new QLabel),
	  delayInfo     (new QLabel),
	  advertisementText (new ScrollTextComponent),
	  m_pSystemNotificationTimer   (new QTimer),
	  transparentPixmap (20, 20),
	  greenPixmap       (20, 20),
	  grayPixmap        (20, 20),
	  redPixmap         (20, 20),
	  m_nCycleTimes     (0)
{
	codeRate->setText(QStringLiteral("码率:0kb/s"));
	fpsRate->setText(QString("FPS:30"));
	droppedFrames->setText(QStringLiteral("丢帧:0(0.00%)"));
	cpuUsage->setText(QString("CPU:00.00%"));
	kbps->setText(QString("kbps:0000"));
	streamTime->setText(QStringLiteral("未开播"));
	advertisementText->setCurrentText(QStringLiteral("探球公告轮播，每5秒往上滚动切换一条"), "");
	advertisementText->setMinimumWidth(advertisementText->width());
	m_pLiveTime = "00:00:00";

	connect(m_pSystemNotificationTimer, SIGNAL(timeout()), this, SLOT(onShowSystemNotification())); 
	m_pSystemNotificationTimer->setInterval(5000);

	HttpAgent::instance()->RequestOnGetSystemNotification();
	connect(HttpAgent::instance(), SIGNAL(onSigGetSystemNotification(int, const QString &, const QList<ST_SYSTEM_ANNOUNCEMENT> &, const void *)), 
		this, SLOT(onGetSystemNotification(int, const QString &, const QList<ST_SYSTEM_ANNOUNCEMENT> &, const void *)));

	QLabel *spaceLabel = new QLabel(this);
	spaceLabel->setStyleSheet("background:transparent;");
	spaceLabel->setFixedWidth(4);

	QLabel *spaceLabel1 = new QLabel(this);
	spaceLabel1->setStyleSheet("background:transparent;");
	spaceLabel1->setFixedWidth(4);

	QLabel *iconLabel = new QLabel(this);
	iconLabel->setFixedSize(QSize(20, 20));
	iconLabel->setStyleSheet("QLabel{border-image:url(:/TQLive/images/live tool_icon_announcement.png);background:transparent;}");

	QWidget *brWidget = new QWidget(this);
	brWidget->setStyleSheet("background:transparent;");
	QHBoxLayout *brLayout = new QHBoxLayout(brWidget);
	brLayout->setContentsMargins(20, 0, 0, 0);
	brLayout->setSpacing(11);

	brLayout->addWidget(iconLabel);
	brLayout->addWidget(advertisementText);
	//statusSquare = new QLabel(brWidget);
	//brLayout->addWidget(statusSquare);

// 	kbps = new QLabel(brWidget);
// 	brLayout->addWidget(kbps);

	brWidget->setLayout(brLayout);
	addWidget(brWidget);

	codeRate->hide();

	codeRate->setAlignment(Qt::AlignRight);
	codeRate->setAlignment(Qt::AlignVCenter);
	fpsRate->setAlignment(Qt::AlignRight);
	fpsRate->setAlignment(Qt::AlignVCenter);
	droppedFrames->setAlignment(Qt::AlignRight);
	droppedFrames->setAlignment(Qt::AlignVCenter);
	cpuUsage->setAlignment(Qt::AlignRight);
	cpuUsage->setAlignment(Qt::AlignVCenter);
	kbps->setAlignment(Qt::AlignRight);
	kbps->setAlignment(Qt::AlignVCenter);
	liveIcon->setAlignment(Qt::AlignRight);
	liveIcon->setAlignment(Qt::AlignVCenter);
	streamTime->setAlignment(Qt::AlignRight);
	streamTime->setAlignment(Qt::AlignVCenter);
// 	delayInfo->setAlignment(Qt::AlignRight);
// 	delayInfo->setAlignment(Qt::AlignVCenter);

	delayInfo->setIndent(6);
	codeRate->setIndent(4);
	fpsRate->setIndent(4);
	droppedFrames->setIndent(4);
	cpuUsage->setIndent(4);
	kbps->setIndent(4);

	addPermanentWidget(codeRate);
	addPermanentWidget(fpsRate);
	addPermanentWidget(droppedFrames);
	addPermanentWidget(cpuUsage);
	addPermanentWidget(kbps);
	addPermanentWidget(spaceLabel);
	addPermanentWidget(liveIcon);
	addPermanentWidget(streamTime);
	addPermanentWidget(delayInfo);

	transparentPixmap.fill(QColor(0, 0, 0, 0));
	greenPixmap.fill(QColor(0, 255, 0));
	grayPixmap.fill(QColor(72, 72, 72));
	redPixmap.fill(QColor(255, 0, 0));

	//statusSquare->setPixmap(transparentPixmap);

	liveIcon->setFixedSize(QSize(12, 12));
	liveIcon->setStyleSheet("QLabel{border-image:url(:/TQLive/images/live tool_icon_live.png);background:transparent;}");
	liveIcon->setHidden(true);

	codeRate->setStyleSheet("background:transparent;");
	fpsRate->setStyleSheet("background:transparent;");
	advertisementText->setStyleSheet("QWidget{background:red;} QLabel{color:#DCDCDC;background:transparent;}");
	cpuUsage->setStyleSheet("QLabel { background-color: rgba(47, 47, 47, 255);}");
	streamTime->setStyleSheet("QLabel { background-color: rgba(47, 47, 47, 255);}");
	delayInfo->setStyleSheet("QLabel { background-color: rgba(47, 47, 47, 255);}");
	droppedFrames->setStyleSheet("QLabel { background-color: rgba(47, 47, 47, 255);}");
	kbps->setStyleSheet("QLabel { background-color: rgba(47, 47, 47, 255);}");
	//statusSquare->setStyleSheet("QLabel { background-color: rgba(47, 47, 47, 255);}");
}

void OBSBasicStatusBar::Activate()
{
	if (!active) {
		refreshTimer = new QTimer(this);
		connect(refreshTimer, SIGNAL(timeout()),
				this, SLOT(UpdateStatusBar()));

		int skipped = video_output_get_skipped_frames(obs_get_video());
		int total   = video_output_get_total_frames(obs_get_video());

		totalStreamSeconds = 0;
		totalRecordSeconds = 0;
		lastSkippedFrameCount = 0;
		startSkippedFrameCount = skipped;
		startTotalFrameCount = total;

		refreshTimer->start(1000);
		active = true;

		if (streamOutput) {
			//statusSquare->setPixmap(grayPixmap);
		}
	}
}

void OBSBasicStatusBar::Deactivate(int abortType)
{
	OBSBasic *main = qobject_cast<OBSBasic*>(parent());
	if (!main)
		return;

	if (!streamOutput) {
		if (abortType == NORMAL_TERMINATED)
		{
			GlobalSignal::instance()->onSigUpdateLiveEndTime(m_pLiveTime);
			m_pLiveTime = "00:00:00";
		}
		streamTime->setText(QStringLiteral("未开播"));
		emit onSigUpdateLiveTime("00:00:00");
		totalStreamSeconds = 0;
	}

	if (!recordOutput) {
		emit onSigUpdateRecordTime("00:00:00");
		totalRecordSeconds = 0;
	}

	if (!main->outputHandler->Active()) {
		delete refreshTimer;

		delayInfo->setText("");
		codeRate->setText(QStringLiteral("码率:0kb/s"));
		fpsRate->setText(QString("FPS:30"));
		droppedFrames->setText(QStringLiteral("丢帧:0(0.00%)"));
		cpuUsage->setText(QString("CPU:0.0%"));
		kbps->setText(QString("kbps:0"));

		delaySecTotal = 0;
		delaySecStarting = 0;
		delaySecStopping = 0;
		reconnectTimeout = 0;
		active = false;
		overloadedNotify = true;

		//statusSquare->setPixmap(transparentPixmap);
	}
}

void OBSBasicStatusBar::UpdateDelayMsg()
{
	QString msg;

	if (delaySecTotal) {
		if (delaySecStarting && !delaySecStopping) {
			msg = QTStr("Basic.StatusBar.DelayStartingIn");
			msg = msg.arg(QString::number(delaySecStarting));

		} else if (!delaySecStarting && delaySecStopping) {
			msg = QTStr("Basic.StatusBar.DelayStoppingIn");
			msg = msg.arg(QString::number(delaySecStopping));

		} else if (delaySecStarting && delaySecStopping) {
			msg = QTStr("Basic.StatusBar.DelayStartingStoppingIn");
			msg = msg.arg(QString::number(delaySecStopping),
					QString::number(delaySecStarting));
		} else {
			msg = QTStr("Basic.StatusBar.Delay");
			msg = msg.arg(QString::number(delaySecTotal));
		}
	}

	delayInfo->setText(msg);
}

#define BITRATE_UPDATE_SECONDS 2

void OBSBasicStatusBar::UpdateBandwidth()
{
	if (!streamOutput)
		return;

	if (++bitrateUpdateSeconds < BITRATE_UPDATE_SECONDS)
		return;

	uint64_t bytesSent     = obs_output_get_total_bytes(streamOutput);
	uint64_t bytesSentTime = os_gettime_ns();

	if (bytesSent < lastBytesSent)
		bytesSent = 0;
	if (bytesSent == 0)
		lastBytesSent = 0;

	uint64_t bitsBetween   = (bytesSent - lastBytesSent) * 8;

	double timePassed = double(bytesSentTime - lastBytesSentTime) /
		1000000000.0;

	double kbitsPerSec = double(bitsBetween) / timePassed / 1000.0;

	QString text = QString("kb/s:%1").arg(QString::number(kbitsPerSec, 'f', 0));
	kbps->setText(text);
	kbps->setMinimumWidth(kbps->width());

	lastBytesSent        = bytesSent;
	lastBytesSentTime    = bytesSentTime;
	bitrateUpdateSeconds = 0;
}

void OBSBasicStatusBar::UpdateCPUUsage()
{
	OBSBasic *main = qobject_cast<OBSBasic*>(parent());
	if (!main)
		return;

	QString text = QString("CPU:%1%").arg(QString::number(main->GetCPUUsage(), 'f', 1));
	cpuUsage->setText(text);
	cpuUsage->setMinimumWidth(cpuUsage->width());

	QString fps = QString("FPS:%1").arg(QString::number(obs_get_active_fps(), 'f', 2));
	fpsRate->setText(fps);
	fpsRate->setMinimumWidth(fpsRate->width());
}

void OBSBasicStatusBar::onGetSystemNotification(int nStatusCode, const QString &qsMsg, const QList<ST_SYSTEM_ANNOUNCEMENT>& obList, const void * userData)
{
	Q_UNUSED(userData)

	if (nStatusCode == 200)
	{
		m_pSystemNotificationList = obList;
		m_pSystemNotificationTimer->start();
	} 
	else
	{
		QMessageBox::warning(this, QStringLiteral("请求失败"), QStringLiteral("获取系统公告请求失败：%1").arg(qsMsg));
	}
}

void OBSBasicStatusBar::onShowSystemNotification()
{
	if (m_nCycleTimes == m_pSystemNotificationList.size())
	{
		m_nCycleTimes = 0;
	}

	for (int i = 0; i < m_pSystemNotificationList.size(); ++i)
	{
		if (i == m_nCycleTimes)
		{
			advertisementText->setCurrentText(m_pSystemNotificationList[i].qsTitle, m_pSystemNotificationList[i].qsURL);
			advertisementText->setMinimumWidth(advertisementText->width());
			m_nCycleTimes++;
			break;
		}
	}
}

void OBSBasicStatusBar::UpdateStreamTime()
{
	totalStreamSeconds++;

	int seconds      = totalStreamSeconds % 60;
	int totalMinutes = totalStreamSeconds / 60;
	int minutes      = totalMinutes % 60;
	int hours        = totalMinutes / 60;

	QString text;
	text.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
	emit onSigUpdateLiveTime(text);
	m_pLiveTime = text;

	if (reconnectTimeout > 0) {
		QString msg = QTStr("Basic.StatusBar.Reconnecting")
			.arg(QString::number(retries),
					QString::number(reconnectTimeout));
		showMessage(msg);
		reconnectTimeout--;

	} else if (retries > 0) {
		QString msg = QTStr("Basic.StatusBar.AttemptingReconnect");
		showMessage(msg.arg(QString::number(retries)));
	}

	if (delaySecStopping > 0 || delaySecStarting > 0) {
		if (delaySecStopping > 0)
			--delaySecStopping;
		if (delaySecStarting > 0)
			--delaySecStarting;
		UpdateDelayMsg();
	}
}

void OBSBasicStatusBar::UpdateRecordTime()
{
	totalRecordSeconds++;

	int seconds      = totalRecordSeconds % 60;
	int totalMinutes = totalRecordSeconds / 60;
	int minutes      = totalMinutes % 60;
	int hours        = totalMinutes / 60;

	QString text;
	text.sprintf("%02d:%02d:%02d", hours, minutes, seconds);

	emit onSigUpdateRecordTime(text);
}

void OBSBasicStatusBar::UpdateDroppedFrames()
{
	if (!streamOutput)
		return;

	int totalDropped = obs_output_get_frames_dropped(streamOutput);
	int totalFrames  = obs_output_get_total_frames(streamOutput);
	double percent   = (double)totalDropped / (double)totalFrames * 100.0;

	if (!totalFrames)
		return;

	QString text = QTStr("DroppedFrames");
	text = text.arg(QString::number(totalDropped),
			QString::number(percent, 'f', 1));
	droppedFrames->setText(text);
	droppedFrames->setMinimumWidth(droppedFrames->width());

	/* ----------------------------------- *
	 * calculate congestion color          */

	float congestion = obs_output_get_congestion(streamOutput);
	float avgCongestion = (congestion + lastCongestion) * 0.5f;
	if (avgCongestion < congestion)
		avgCongestion = congestion;
	if (avgCongestion > 1.0f)
		avgCongestion = 1.0f;

	if (avgCongestion < EPSILON) {
		//statusSquare->setPixmap(greenPixmap);
	} else if (fabsf(avgCongestion - 1.0f) < EPSILON) {
		//statusSquare->setPixmap(redPixmap);
	} else {
		QPixmap pixmap(20, 20);

		float red = avgCongestion * 2.0f;
		if (red > 1.0f) red = 1.0f;
		red *= 255.0;

		float green = (1.0f - avgCongestion) * 2.0f;
		if (green > 1.0f) green = 1.0f;
		green *= 255.0;

		pixmap.fill(QColor(int(red), int(green), 0));
		//statusSquare->setPixmap(pixmap);
	}

	lastCongestion = congestion;
}

void OBSBasicStatusBar::OBSOutputReconnect(void *data, calldata_t *params)
{
	OBSBasicStatusBar *statusBar =
		reinterpret_cast<OBSBasicStatusBar*>(data);

	int seconds = (int)calldata_int(params, "timeout_sec");
	QMetaObject::invokeMethod(statusBar, "Reconnect", Q_ARG(int, seconds));
	UNUSED_PARAMETER(params);
}

void OBSBasicStatusBar::OBSOutputReconnectSuccess(void *data, calldata_t *params)
{
	OBSBasicStatusBar *statusBar =
		reinterpret_cast<OBSBasicStatusBar*>(data);

	QMetaObject::invokeMethod(statusBar, "ReconnectSuccess");
	UNUSED_PARAMETER(params);
}

void OBSBasicStatusBar::Reconnect(int seconds)
{
	OBSBasic *main = qobject_cast<OBSBasic*>(parent());

	if (!retries)
		main->SysTrayNotify(
				QTStr("Basic.SystemTray.Message.Reconnecting"),
				QSystemTrayIcon::Warning);

	reconnectTimeout = seconds;

	if (streamOutput) {
		delaySecTotal = obs_output_get_active_delay(streamOutput);
		UpdateDelayMsg();

		retries++;
	}
}

void OBSBasicStatusBar::ReconnectClear()
{
	retries              = 0;
	reconnectTimeout     = 0;
	bitrateUpdateSeconds = -1;
	lastBytesSent        = 0;
	lastBytesSentTime    = os_gettime_ns();
	delaySecTotal        = 0;
	UpdateDelayMsg();
}

void OBSBasicStatusBar::ReconnectSuccess()
{
	OBSBasic *main = qobject_cast<OBSBasic*>(parent());

	QString msg = QTStr("Basic.StatusBar.ReconnectSuccessful");
	showMessage(msg, 4000);
	main->SysTrayNotify(msg, QSystemTrayIcon::Information);
	ReconnectClear();

	if (streamOutput) {
		delaySecTotal = obs_output_get_active_delay(streamOutput);
		UpdateDelayMsg();
	}
}

void OBSBasicStatusBar::UpdateStatusBar()
{
	OBSBasic *main = qobject_cast<OBSBasic*>(parent());

	UpdateBandwidth();

	if (streamOutput)
		UpdateStreamTime();

	if (recordOutput)
		UpdateRecordTime();

	UpdateDroppedFrames();

	int skipped = video_output_get_skipped_frames(obs_get_video());
	int total   = video_output_get_total_frames(obs_get_video());

	skipped -= startSkippedFrameCount;
	total   -= startTotalFrameCount;

	int diff = skipped - lastSkippedFrameCount;
	double percentage = double(skipped) / double(total) * 100.0;

	if (diff > 10 && percentage >= 0.1f) {
		showMessage(QTStr("HighResourceUsage"), 4000);
		if (!main->isVisible() && overloadedNotify) {
			main->SysTrayNotify(QTStr("HighResourceUsage"),
					QSystemTrayIcon::Warning);
			overloadedNotify = false;
		}
	}

	lastSkippedFrameCount = skipped;
}

void OBSBasicStatusBar::StreamDelayStarting(int sec)
{
	OBSBasic *main = qobject_cast<OBSBasic*>(parent());
	if (!main || !main->outputHandler)
		return;

	streamOutput = main->outputHandler->streamOutput;

	delaySecTotal = delaySecStarting = sec;
	UpdateDelayMsg();
	Activate();
}

void OBSBasicStatusBar::StreamDelayStopping(int sec)
{
	delaySecTotal = delaySecStopping = sec;
	UpdateDelayMsg();
}

void OBSBasicStatusBar::StreamStarted(obs_output_t *output)
{
	//liveIcon->setHidden(false);
	streamTime->setText(QStringLiteral("正在直播"));
	streamOutput = output;

	signal_handler_connect(obs_output_get_signal_handler(streamOutput),
			"reconnect", OBSOutputReconnect, this);
	signal_handler_connect(obs_output_get_signal_handler(streamOutput),
			"reconnect_success", OBSOutputReconnectSuccess, this);

	retries           = 0;
	lastBytesSent     = 0;
	lastBytesSentTime = os_gettime_ns();
	Activate();
}

void OBSBasicStatusBar::StreamStopped(int abortType)
{
	liveIcon->setHidden(true);
	if (streamOutput) {
		signal_handler_disconnect(
				obs_output_get_signal_handler(streamOutput),
				"reconnect", OBSOutputReconnect, this);
		signal_handler_disconnect(
				obs_output_get_signal_handler(streamOutput),
				"reconnect_success", OBSOutputReconnectSuccess,
				this);

		ReconnectClear();
		streamOutput = nullptr;
		clearMessage();
		Deactivate(abortType);
	}
}

void OBSBasicStatusBar::RecordingStarted(obs_output_t *output)
{
	recordOutput = output;
	Activate();
}

void OBSBasicStatusBar::RecordingStopped()
{
	recordOutput = nullptr;
	Deactivate(EXCEPTION_TERMINATED);
}

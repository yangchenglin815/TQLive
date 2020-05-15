/***********************************************************************************
*								AddMediaSourceDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-11
*
* Description: 添加视频源对话框
***********************************************************************************/
#include "AddMediaSourceDlg.h"
#include "FramelessHelper.h"
#include "EnumData.h"
#include "CustomArrowWgt.h"
#include "UserConfig.h"
#include "CustomLoopAction.h"
#include "GlobalSignal.h"
#include "PlayMediaLitItem.h"
#include "window-basic-main.hpp"
#include "qt-wrappers.hpp"
#include "CommonFunc.h"
#include "SourceIniXML.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <QScrollBar>
#include <QTime>
#include <QDebug>

Q_DECLARE_METATYPE(OBSSource);

static bool isPressSlider = false;
static bool isPlay = false;

struct AddSourceData {
	obs_source_t *source;
	bool visible;
};

static void AddSource(void *_data, obs_scene_t *scene)
{
	AddSourceData *data = (AddSourceData *)_data;
	obs_sceneitem_t *sceneitem;

	sceneitem = obs_scene_add(scene, data->source);
	obs_sceneitem_set_visible(sceneitem, data->visible);
}

AddMediaSourceDlg::AddMediaSourceDlg(QWidget *parent, OBSSource source, bool initFlag)
	: QDialog(parent)
	, m_pSource(source)
	, m_pLoopMenu(NULL)
	, m_bNeedInit(initFlag)
	, properties(nullptr, obs_properties_destroy)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	setModal(true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	main = reinterpret_cast<OBSBasic*>(parent);
	startTimer(100);

	InitConnect();
	InitUI();
	InitSource();
}

AddMediaSourceDlg::~AddMediaSourceDlg()
{
// 	SourceIniData data;
// 	data.cycleType = m_nCurr_LoopType;
// 	data.voiceVal = m_nCurr_SoundsVal;
// 	data.listLength = m_pPlayMediaItemList.size();
// 	data.sourceName = QT_UTF8(obs_source_get_name(m_pSource));
// 
// 	for (auto &iter : m_pPlayMediaItemList)
// 		data.mediaSourceList.append(iter->getData());
// 
// 	SourceIniXML::instance()->m_pSourceIniList.append(data);
// 	SourceIniXML::instance()->WriteXml();
}

QString AddMediaSourceDlg::getSourceName()
{
	QString sourceName;
	if (m_pSource)
		sourceName = QT_UTF8(obs_source_get_name(m_pSource));
	return sourceName;
}

void AddMediaSourceDlg::Release()
{
	m_pPlayMediaItemList.clear();

	int index = -1;
	bool isOperate = false;
	for (auto &iter : SourceIniXML::instance()->m_pSourceIniList) {
		index++;
		if (iter.sourceName == getSourceName()) {
			isOperate = true;
			break;
		}
	}

	if (index > -1 && isOperate) {
		SourceIniXML::instance()->m_pSourceIniList.removeAt(index);
		SourceIniXML::instance()->WriteXml();
	}
}

void AddMediaSourceDlg::Load()
{
	show();
}

void AddMediaSourceDlg::onCurrentPageChanged(int index)
{
	switch (index) {
	case PAGE_MEDIA_SOURCE_ADD:
		ui.wgt_bottom->setHidden(true);
		break;
	case PAGE_MEDIA_SOURCE_SET:
		ui.wgt_bottom->setHidden(false);
		break;
	default:
		break;
	}
}

void AddMediaSourceDlg::onAddMediaClicked()
{
	obs_property_t *property = obs_properties_first(properties.get());
	const char    *desc = obs_property_description(property);
	const char    *filter = obs_property_path_filter(property);
	const char    *default_path = obs_property_path_default_path(property);

	QString path = QFileDialog::getOpenFileName(this, QT_UTF8(desc),
		QT_UTF8(default_path),
		QT_UTF8("Videos(*.rmvb *.avi *.mp4 *.mkv *.flv *.mp3 *.wma *.rm *.torrent)"));
	if (path.isEmpty())
		return;

	MediaSourceData mediaData;
	mediaData.sMediaName = CommonFunc::getMediaNameByPath(path);
	mediaData.sMediaPath = path;
	mediaData.nId = m_pPlayMediaItemList.size();

	AddItem(mediaData);
	ui.wgt_middle->setCurrentWidget(ui.page_mediaList);
	ui.slider_play->setValue(0);
	m_bNeedInit = false;
}

void AddMediaSourceDlg::onLoopClicked()
{
	if (m_pLoopMenu != NULL) {
		m_pLoopMenu->deleteLater();
		m_pLoopMenu = NULL;
	}

	QListWidget *listWidget = new QListWidget;
	listWidget->setStyleSheet("QListWidget { background:transparent; border-radius:4px; border:none; } \
                                   QListWidget::item:hover { background:transparent; }");

	auto loopTypeData = UserConfig::instance()->getLoopIni();
	for (auto& it : loopTypeData) {
		CustomLoopAction *button = new CustomLoopAction(it);
		connect(button, SIGNAL(onLoopClicked(int)), this, SLOT(onLoopTypeChanged(int)));

		QListWidgetItem *itemWidget = new QListWidgetItem;
		listWidget->addItem(itemWidget);
		listWidget->setItemWidget(itemWidget, button);
	}

	m_pLoopMenu = new CustomArrowWgt(this);
	m_pLoopMenu->setCenterWidget(listWidget);

	QPoint p = ui.button_loop->mapToGlobal(QPoint(0, 0));
	int posX = p.x() - (m_pLoopMenu->width() - ui.button_loop->width()) / 2;
	int posY = p.y() - m_pLoopMenu->height() + 9;
	m_pLoopMenu->move(posX, posY);
	m_pLoopMenu->show();
}

void AddMediaSourceDlg::onLeftButtonClicked()
{
	if (m_pLoopMenu != NULL)
		m_pLoopMenu->close();
}

void AddMediaSourceDlg::onActiveItem(QListWidgetItem * current)
{
	PlayMediaLitItem *currentItem = qobject_cast<PlayMediaLitItem*>(ui.listWidget_playList->itemWidget(current));
	if (currentItem) {
		currentItem->setState(STATE_ACTIVE);
		MediaSourceData data = currentItem->getData();
		m_nCurrPlayIndex = data.nId;
		for (auto &item : m_pPlayMediaItemList) {
			MediaSourceData datdTemp = item->getData();
			if (datdTemp.nId == data.nId) {
				datdTemp.bActive = true;
				item->setData(datdTemp);
			}
			else {
				datdTemp.bActive = false;
				item->setData(datdTemp);
				item->setState(STATE_COMMON);
			}
		}

		if (m_pSource) {
			obs_data_t *settings = obs_source_get_settings(m_pSource);
			obs_data_set_string(settings, "local_file", QT_TO_UTF8(data.sMediaPath));
			obs_source_update(m_pSource, settings);
			obs_data_release(settings);
		}
	}
}

void AddMediaSourceDlg::onPlaySliderPressed()
{
	isPressSlider = true;
}

void AddMediaSourceDlg::onPlaySliderReleased()
{
	isPressSlider = false;

	if (isPlay) {
		float pos = 0;
		pos = (float)ui.slider_play->value() / (float)(ui.slider_play->maximum() + 1);

		if (m_pSource) {
			proc_handler_t *ph =
				obs_source_get_proc_handler(m_pSource);
			calldata_t cd = { 0 };
			calldata_set_float(&cd, "pos", pos);
			proc_handler_call(ph, "set_frame_pos", &cd);
			calldata_free(&cd);
		}
	}
}

void AddMediaSourceDlg::onDeleteItem(int id)
{
	bool needPlay = false;
	if (m_pPlayMediaItemList[id]->getData().bActive)
		needPlay = true;
	m_pPlayMediaItemList.removeAt(id);

	QList<MediaSourceData> dataList;
	for (int i = 0; i < m_pPlayMediaItemList.size(); ++i) {
		MediaSourceData data = m_pPlayMediaItemList[i]->getData();
		data.nId = i;
		dataList.append(data);
	}

	UpdateList(dataList);

	if (dataList.size() == 0) {
		ui.wgt_middle->setCurrentWidget(ui.page_addMedia);

		if (m_pSource) {
			obs_data_t *settings = obs_source_get_settings(m_pSource);
			obs_data_set_string(settings, "local_file", "");
			obs_source_update(m_pSource, settings);
			obs_data_release(settings);
		}
	}
	else if (needPlay) {
		if (id == dataList.size())
			id = 0;

		QListWidgetItem *item = ui.listWidget_playList->item(id);
		if (item)
			onActiveItem(item);
	}
}

void AddMediaSourceDlg::onUpItem(int id)
{
	if (id == 0)
		return;

	MediaSourceData thisData = m_pPlayMediaItemList[id]->getData();
	MediaSourceData lastData = m_pPlayMediaItemList[id - 1]->getData();
	m_pPlayMediaItemList[id]->setData(lastData);
	m_pPlayMediaItemList[id - 1]->setData(thisData);

	QList<MediaSourceData> dataList;
	for (int i = 0; i < m_pPlayMediaItemList.size(); ++i) {
		MediaSourceData data = m_pPlayMediaItemList[i]->getData();
		data.nId = i;
		dataList.append(data);
	}

	UpdateList(dataList);
}

void AddMediaSourceDlg::onDownItem(int id)
{
	if (id == m_pPlayMediaItemList.size() - 1)
		return;

	MediaSourceData thisData = m_pPlayMediaItemList[id]->getData();
	MediaSourceData nextData = m_pPlayMediaItemList[id + 1]->getData();
	m_pPlayMediaItemList[id]->setData(nextData);
	m_pPlayMediaItemList[id + 1]->setData(thisData);

	QList<MediaSourceData> dataList;
	for (int i = 0; i < m_pPlayMediaItemList.size(); ++i) {
		MediaSourceData data = m_pPlayMediaItemList[i]->getData();
		data.nId = i;
		dataList.append(data);
	}

	UpdateList(dataList);
}

void AddMediaSourceDlg::onPlayLastMedia()
{
	int row;
	if (m_nCurrPlayIndex == -1)
		row = 0;
	else if (m_nCurrPlayIndex == 0)
		row = m_pPlayMediaItemList.size() - 1;
	else
		row = m_nCurrPlayIndex - 1;

	QListWidgetItem *item = ui.listWidget_playList->item(row);
	if (item)
		onActiveItem(item);
}

void AddMediaSourceDlg::onPlayNextMedia()
{
	int row;
	if (m_nCurrPlayIndex == -1 || m_nCurrPlayIndex == m_pPlayMediaItemList.size() - 1)
		row = 0;
	else
		row = m_nCurrPlayIndex + 1;

	QListWidgetItem *item = ui.listWidget_playList->item(row);
	if (item)
		onActiveItem(item);
}

void AddMediaSourceDlg::onSoundsValueChanged(int val)
{
	if (val == 0)
		ui.button_sound->setChecked(true);
	else
		ui.button_sound->setChecked(false);

	if (m_pSource) {
		QString sourceName = QTStr(obs_source_get_name(m_pSource));
		emit onSigMediaSoundsValChanged(val, sourceName);
	}
}

void AddMediaSourceDlg::onSoundsButtonClicked()
{
	if (ui.button_sound->isChecked()) {
		m_nCurr_SoundsVal = ui.slider_sound->value();
		ui.slider_sound->setValue(0);
	}
	else
		ui.slider_sound->setValue(m_nCurr_SoundsVal);
}

void AddMediaSourceDlg::onLoopTypeChanged(int loopType)
{
	m_pLoopMenu->close();
	ui.button_loop->setLoopType(loopType);

	m_nCurr_LoopType = loopType;
}

void AddMediaSourceDlg::onDialogClosed()
{
	if (m_bNeedInit) {
		OBSBasic *main = reinterpret_cast<OBSBasic*>(App()->GetMainWindow());
		main->CancelCurrentSource(getSourceName());
	}
	else {
		onLeftButtonClicked();

		SourceIniData data;
		data.cycleType = m_nCurr_LoopType;
		data.voiceVal = m_nCurr_SoundsVal;
		data.listLength = m_pPlayMediaItemList.size();
		data.sourceName = getSourceName();

		for (auto &iter : m_pPlayMediaItemList)
			data.mediaSourceList.append(iter->getData());

		bool isUpdate = false;
		for (auto &iter : SourceIniXML::instance()->m_pSourceIniList) {
			if (iter.sourceName == data.sourceName) {
				iter = data;
				isUpdate = true;
				break;
			}
		}

		if (!isUpdate)
			SourceIniXML::instance()->m_pSourceIniList.append(data);

		SourceIniXML::instance()->WriteXml();
	}
	
	close();
}

void AddMediaSourceDlg::onMediaPlayFinished()
{
	if (isPlay) {
		isPlay = false;
		m_nCurrPts = 0;

		ui.slider_play->setValue(ui.slider_play->maximum());
		ui.label_currTime->setText(ui.label_totalTime->text());

		switch (m_nCurr_LoopType) {
		case LOOP_RANDOM:
			LoopOnRandomPlay();
			break;
		case LOOP_SINGLE:
			LoopOnSingleCycle();
			break;
		case LOOP_CYCLE:
			LoopOnListCycle();
			break;
		default:
			break;
		}
	}
}

void AddMediaSourceDlg::timerEvent(QTimerEvent * event)
{
	bool m_eof = false;
	if (m_pSource) {
		proc_handler_t *ph =
			obs_source_get_proc_handler(m_pSource);
		calldata_t cd = { 0 };
		proc_handler_call(ph, "get_duration", &cd);
		m_nTotalMs = (uint64_t)calldata_int(&cd, "duration") / 1000000;
		m_nCurrPts = (uint64_t)calldata_int(&cd, "pts") / 1000;
		m_eof = calldata_bool(&cd, "m_eof");
		calldata_free(&cd);

		ui.label_currTime->setText(CommonFunc::getTimeDurationByInt(m_nCurrPts));
		ui.label_totalTime->setText(CommonFunc::getTimeDurationByInt(m_nTotalMs));
	}
	

	if (m_nTotalMs > 0) {
		isPlay = true;
		for (auto &item : m_pPlayMediaItemList) {
			MediaSourceData data = item->getData();
			if (data.bActive) {
				data.nTotalTimeDuration = m_nTotalMs;
				item->setData(data);

				break;
			}
		}

		double rate = (double)m_nCurrPts * 1000 / (double)m_nTotalMs;
		if (!isPressSlider)
			ui.slider_play->setValue(rate);

		if (m_eof)
			onMediaPlayFinished();
	}
}

bool AddMediaSourceDlg::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == ui.slider_play) {
		if (event->type() == QEvent::MouseButtonPress) {
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::LeftButton) {
				int dur = ui.slider_play->maximum() - ui.slider_play->minimum();
				int pos = ui.slider_play->minimum() + dur * ((double)mouseEvent->x() / ui.slider_play->width());
				if (pos != ui.slider_play->sliderPosition()) {
					ui.slider_play->setValue(pos);
				}
			}
		}
	}
	return QDialog::eventFilter(obj, event);
}

void AddMediaSourceDlg::InitUI()
{
	ui.button_loop->setLoopType(LOOP_CYCLE);
	ui.slider_play->installEventFilter(this);
	ui.slider_sound->setMinimum(0);
	ui.slider_sound->setMaximum(100);
	ui.slider_sound->setValue(100);

	ui.button_addMediaFirst->setCursor(Qt::PointingHandCursor);
	ui.button_addMedia->setCursor(Qt::PointingHandCursor);
	ui.button_last->setCursor(Qt::PointingHandCursor);
	ui.button_next->setCursor(Qt::PointingHandCursor);
	ui.button_sound->setCursor(Qt::PointingHandCursor);
	ui.button_close->setCursor(Qt::PointingHandCursor);

	if (m_pSource) {
		QString sourceName = QT_UTF8(obs_source_get_name(m_pSource));
		for (auto &iter : SourceIniXML::instance()->m_pSourceIniList) {
			if (iter.sourceName == sourceName) {
				ui.slider_sound->setValue(iter.voiceVal);
				ui.button_loop->setLoopType(iter.cycleType);
				m_nCurr_LoopType = iter.cycleType;
				m_nPageType = iter.listLength > 0 ? PAGE_MEDIA_SOURCE_SET : PAGE_MEDIA_SOURCE_ADD;

				for (auto &it : iter.mediaSourceList)
					AddItem(it);
				break;
			}
		}
	}

	switch (m_nPageType) {
	case PAGE_MEDIA_SOURCE_ADD:
		ui.wgt_middle->setCurrentWidget(ui.page_addMedia);
		onCurrentPageChanged(PAGE_MEDIA_SOURCE_ADD);
		break;
	case PAGE_MEDIA_SOURCE_SET:
		ui.wgt_middle->setCurrentWidget(ui.page_mediaList);
		onCurrentPageChanged(PAGE_MEDIA_SOURCE_SET);
		break;
	default:
		break;
	}
}

void AddMediaSourceDlg::InitConnect()
{
	connect(ui.button_close, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.wgt_middle, SIGNAL(currentChanged(int)), this, SLOT(onCurrentPageChanged(int)));
	connect(ui.button_addMediaFirst, SIGNAL(clicked()), this, SLOT(onAddMediaClicked()));
	connect(ui.button_loop, SIGNAL(clicked()), this, SLOT(onLoopClicked()));
	connect(ui.button_addMedia,SIGNAL(clicked()), this, SLOT(onAddMediaClicked()));
	connect(ui.button_last, SIGNAL(clicked()), this, SLOT(onPlayLastMedia()));
	connect(ui.button_next, SIGNAL(clicked()), this, SLOT(onPlayNextMedia()));
	connect(ui.listWidget_playList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), 
		this, SLOT(onActiveItem(QListWidgetItem*)));
	connect(ui.slider_play, SIGNAL(sliderPressed()), this, SLOT(onPlaySliderPressed()));
	connect(ui.slider_play, SIGNAL(sliderReleased()), this, SLOT(onPlaySliderReleased()));
	connect(ui.slider_sound, SIGNAL(valueChanged(int)), this, SLOT(onSoundsValueChanged(int)));
	connect(ui.button_sound, SIGNAL(clicked()), this, SLOT(onSoundsButtonClicked()));
	connect(GlobalSignal::instance(), SIGNAL(onLeftButtonClicked()), this, SLOT(onLeftButtonClicked()));
}

void AddMediaSourceDlg::InitSource()
{
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
		return;

	if (!m_pSource) {
		int index = 1;
		while (true) {
			QString name = QStringLiteral("视频%1").arg(index);
			obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
			if (source) {
				obs_source_release(source);
				index++;
			}
			else {
				source = obs_source_create("ffmpeg_source", name.toStdString().c_str(), NULL, nullptr);
				if (source) {
					AddSourceData data;
					data.source = source;
					data.visible = true;

					obs_enter_graphics();
					obs_scene_atomic_update(scene, AddSource, &data);
					obs_leave_graphics();

					m_pSource = source;
				}
				break;
			}
		}
	}

	properties.reset(obs_source_properties(m_pSource));

	obs_data_t *settings = obs_source_get_settings(m_pSource);
	obs_data_set_bool(settings, "is_local_file", true);
	obs_data_release(settings);

}

void AddMediaSourceDlg::AddItem(const MediaSourceData & mediaData)
{
	PlayMediaLitItem *item = new PlayMediaLitItem;
	connect(item, SIGNAL(onSigDeleteItem(int)), this, SLOT(onDeleteItem(int)));
	connect(item, SIGNAL(onSigUpItem(int)), this, SLOT(onUpItem(int)));
	connect(item, SIGNAL(onSigDownItem(int)), this, SLOT(onDownItem(int)));
	item->setData(mediaData);
	m_pPlayMediaItemList.append(item);

	if (mediaData.bActive)
		m_nCurrPlayIndex = mediaData.nId;

	QListWidgetItem *itemWidget = new QListWidgetItem;
	itemWidget->setSizeHint(QSize(480, 34));
	ui.listWidget_playList->addItem(itemWidget);
	ui.listWidget_playList->setItemWidget(itemWidget, item);
}

void AddMediaSourceDlg::UpdateList(QList<MediaSourceData> list)
{
	int val = ui.listWidget_playList->verticalScrollBar()->value();

	ui.listWidget_playList->clear();
	m_pPlayMediaItemList.clear();

	for (auto &iter : list)
		AddItem(iter);

	ui.listWidget_playList->verticalScrollBar()->setValue(val);
}

void AddMediaSourceDlg::LoopOnRandomPlay()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	int random = qrand() % m_pPlayMediaItemList.size();

	QListWidgetItem *item = ui.listWidget_playList->item(random);
	if (item)
		onActiveItem(item);
}

void AddMediaSourceDlg::LoopOnSingleCycle()
{
	QListWidgetItem *item = ui.listWidget_playList->item(m_nCurrPlayIndex);
	if (item)
		onActiveItem(item);
}

void AddMediaSourceDlg::LoopOnListCycle()
{
	int playIndex;
	if (m_nCurrPlayIndex == m_pPlayMediaItemList.size() - 1)
		playIndex = 0;
	else
		playIndex = m_nCurrPlayIndex + 1;

	QListWidgetItem *item  = ui.listWidget_playList->item(playIndex);
	if (item) 
		onActiveItem(item);
}
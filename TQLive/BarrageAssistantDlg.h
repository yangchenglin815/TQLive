#pragma once

#include <QDialog>
#include <QMutex>
#include "ui_BarrageAssistantDlg.h"
#include "StructData.h"
#include "BarrageStatisticsAgent.h"

class BarrageAssistantDlg : public QDialog
{
	Q_OBJECT

public:
	BarrageAssistantDlg(QWidget *parent);
	~BarrageAssistantDlg();

signals:
	void onSigUpdateThread(const BarrageStatisticsInfo & barrageStatisticsInfo);
	void onSigClearStatisticsData();
	void onSigInitStatisticsData();
	void onSigMicHotKeyTrigger();
	void onSigLiveHotKeyTrigger();

protected:
	void keyPressEvent(QKeyEvent *event);

private slots:
    /*接收直播间的弹幕信息*/
    void onRecvChatRoomMsg(const BarrageDetialInfo &barrageDetialInfo);

	void onClearStatisticsThread();
	void onInitStatisticsThread();

private:
	/*初始化UI*/
	void InitUI();
	/*初始化链接器*/
	void InitConnect();
	/*字体大小变化后更新列表*/
	void UpdateList();
	/*给图片添加相应的用户级别*/
	void ImageAddText(QString imagePath, int userLevel);
	void roleImageAddText(QString imagePath, int userRole);
	/*初始化弹幕池*/
	void InitBarragePool(int barrageMsgType);
	/*初始化礼物池*/
	void InitGiftPool();
	/*根据用户级别获取相应的图片资源路径*/
	QString getImagePathByUserLevel(int userLevel);
	/*根据用户角色获取相应的图片资源路径*/
	QString getImagePathByUserRole(int userRole);
	/*根据用户角色获取相应的文本内容*/
	QString getTextByUserRole(int userRole);
	/*相应快捷键操作*/
	void RespondHotKey(QString hotKey);

private:
	Ui::BarrageAssistantDlg ui;
	BarrageDetialInfo m_pBarrageDetialInfo;          //实时存储每条弹幕消息
	QList<BarrageDetialInfo> m_pBarragePoolRecord;   //弹幕历史消息缓存
	int m_curr_fontSize;                             //当前弹幕字体大小
	BarrageStatisticsAgent *m_pStatisticsThread;
	bool m_pIsLiving = false;
};

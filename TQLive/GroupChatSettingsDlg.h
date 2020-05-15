#pragma once

#include <QDialog>
#include <QButtonGroup>
#include "ui_GroupChatSettingsDlg.h"
#include "StructData.h"
#include "CDataCentre.h"
#include "CheckBox.h"

class GroupChatSettingsDlg : public QDialog
{
	Q_OBJECT

public:
	explicit GroupChatSettingsDlg(QWidget *parent);
	~GroupChatSettingsDlg();

	void Init(int groupId);

signals:
	void onSigCheckedGroupId(int groupId);

private slots:
    void onCheckBoxStateChanged(ST_GROUP_INFO &groupChatInfo);
	void onConfirmClicked();
	void onGetGroupChatList(int nStatuCode, const QString &qsInfo, QList<ST_GROUP_INFO> groupList);

private:
	void initData(const QList<ST_GROUP_INFO> &groupChatInfoList);
	void InitConnect();

private:
	Ui::GroupChatSettingsDlg ui;
	QList<ST_GROUP_INFO> m_pGroupChatInfoList;
	ST_GROUP_INFO m_curr_selected_groupChatInfo;
	QButtonGroup *m_buttonGroup;
	int m_pColumn;
	int m_pRow;
	int m_pGroupId;
};

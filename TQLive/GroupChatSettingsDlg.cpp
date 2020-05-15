/***********************************************************************************
*								GroupChatSettingsDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 群聊助手对话框
***********************************************************************************/

#include "GroupChatSettingsDlg.h"
#include "FramelessHelper.h"
#include "HttpAgent.h"
#include "GlobalSignal.h"
#include "UserConfig.h"

GroupChatSettingsDlg::GroupChatSettingsDlg(QWidget *parent)
	: QDialog(parent)
	, m_pColumn(2)
	, m_pRow(0)
	, m_pGroupId(-1)
	, m_buttonGroup(NULL)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	//m_buttonGroup = new QButtonGroup(this);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	InitConnect();
}

GroupChatSettingsDlg::~GroupChatSettingsDlg()
{

}

void GroupChatSettingsDlg::initData(const QList<ST_GROUP_INFO> &groupChatInfoList)
{
	m_pRow = 0;

	ui.tableWidget->clear();
	m_pGroupChatInfoList.clear();
	m_pGroupChatInfoList = groupChatInfoList;

	if (m_buttonGroup != NULL)
	{
		m_buttonGroup->deleteLater();
		m_buttonGroup = NULL;
	}
	m_buttonGroup = new QButtonGroup(this);

	int dataSize = m_pGroupChatInfoList.length();
	if (dataSize >= 0)
	{
		ST_GROUP_INFO groupChatInfo;
		groupChatInfo.qsGroupName = QStringLiteral("不使用任何群");
		groupChatInfo.nGroupID = -1;
		m_pGroupChatInfoList.append(groupChatInfo);
		dataSize++;
	}

	if (dataSize % m_pColumn == 0)
	{
		m_pRow = dataSize / m_pColumn;
	}
	else
	{
		m_pRow = (dataSize / m_pColumn) + (dataSize % m_pColumn);
	}

	ui.tableWidget->setColumnCount(m_pColumn);
	ui.tableWidget->setRowCount(m_pRow);

	for (int i = 0; i < m_pColumn; ++i)
	{
		ui.tableWidget->setColumnWidth(i, 228);
	}

	int index = -1;
	bool flag = false;
	for (int i = 0; i < m_pRow; ++i)
	{
		for (int j = 0; j < m_pColumn; ++j)
		{
			index++;

			if (index == dataSize)
			{
				QLabel *label = new QLabel;
				label->setStyleSheet("background:transparent;");

				ui.tableWidget->setCellWidget(i, j, label);
			}
			else
			{
				CheckBox *checkBox = new CheckBox;
				m_buttonGroup->addButton(checkBox, index);
				checkBox->setData(m_pGroupChatInfoList[index]);
				connect(checkBox, SIGNAL(sigOnStateChanged(ST_GROUP_INFO &)), this, SLOT(onCheckBoxStateChanged(ST_GROUP_INFO &)));
				if (m_pGroupChatInfoList[index].nGroupID == m_pGroupId)
				{
					checkBox->setChecked(true);
					flag = true;
				}

				ui.tableWidget->setCellWidget(i, j, checkBox);
			}
		}
	}

	if (!flag)
	{
		int curr_row = m_pRow - 1;
		int curr_column = (index == dataSize) ? 0 : 1;

		CheckBox *checkBox = qobject_cast<CheckBox*>(ui.tableWidget->cellWidget(curr_row, curr_column));
		if (checkBox)
		{
			checkBox->setChecked(true);
		}
	}
}

void GroupChatSettingsDlg::onCheckBoxStateChanged(ST_GROUP_INFO &groupChatInfo)
{
	m_curr_selected_groupChatInfo.qsGroupName = groupChatInfo.qsGroupName;
	m_curr_selected_groupChatInfo.nGroupID = groupChatInfo.nGroupID;
	m_curr_selected_groupChatInfo.qsGroupNumber = groupChatInfo.qsGroupNumber;
	m_curr_selected_groupChatInfo.nGroupNumOfPeople = groupChatInfo.nGroupNumOfPeople;
}

void GroupChatSettingsDlg::onConfirmClicked()
{
	m_pGroupId = m_curr_selected_groupChatInfo.nGroupID;

	UserConfig::instance()->setGroupNumber(m_curr_selected_groupChatInfo.qsGroupNumber);
	HttpAgent::instance()->RequestOnGetChatGroupInfo(m_curr_selected_groupChatInfo.qsGroupNumber);

	emit onSigCheckedGroupId(m_pGroupId);
}

void GroupChatSettingsDlg::onGetGroupChatList(int nStatuCode, const QString & qsInfo, QList<ST_GROUP_INFO> groupList)
{
	if (nStatuCode == 200)
	{
		initData(groupList);

		this->exec();
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatuCode, qsInfo);
	}
}

void GroupChatSettingsDlg::Init(int groupId)
{
	m_pGroupId = groupId;

	HttpAgent::instance()->RequestOnGetGroupChatList();
}

void GroupChatSettingsDlg::InitConnect()
{
	connect(ui.btn_close, &QPushButton::clicked, this, &QDialog::close);
	connect(ui.btn_cancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
	connect(HttpAgent::instance(), SIGNAL(onSigGetGroupChatList(int, const QString &, QList<ST_GROUP_INFO>)),
		this, SLOT(onGetGroupChatList(int, const QString &, QList<ST_GROUP_INFO>)));
}

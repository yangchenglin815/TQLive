/***********************************************************************************
*								ForbiddenRecordTableView.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 禁言记录列表
***********************************************************************************/
#include "ForbiddenRecordTableView.h"
#include <QHeaderView>
#include "HttpAgent.h"
#include <QDebug>

ForbiddenRecordTableView::ForbiddenRecordTableView(QWidget *parent)
	: QTableView(parent)
	, m_pTipsDlg(NULL)
{
	initUI();
}

ForbiddenRecordTableView::~ForbiddenRecordTableView()
{
	delete m_pDataModel;
}

void ForbiddenRecordTableView::onForbiddenClicked(const QModelIndex &index)
{
	if (m_pTipsDlg == NULL)
	{
		m_pTipsDlg = new TipsDlg(DIALOG_CANCEL_FORBIDDEN_OPERATOR, this);
	}

	if (QDialog::Accepted == m_pTipsDlg->exec())
	{
		int row = index.row();
		QString curr_user = m_pDataModel->index(row, 0).data().toString();
		for (int i = 0; i < m_pForbiddenRecordList.size(); ++i)
		{
			if (m_pForbiddenRecordList[i].qsNickName == curr_user)
			{
				HttpAgent::instance()->RequestOnCancelForbiddenUser(m_pForbiddenRecordList[i].nUserID);
			}
		}

// 		m_pDataModel->operationToRemoveItemAtRow(index.row());
// 		qDebug() << m_pDataModel->index(index.row(), 0).data().toString();
	}
}

void ForbiddenRecordTableView::initData(const QList<ST_PROHIBIT_TO_SPEAK_RECORD> &forbiddenRecordList)
{
	m_pForbiddenRecordList.clear();
	m_pForbiddenRecordList = forbiddenRecordList;

	QVector<QStringList> data;
	for (int i = 0; i < m_pForbiddenRecordList.size(); ++i)
	{
		data.append(QStringList() << m_pForbiddenRecordList[i].qsNickName
			<< m_pForbiddenRecordList[i].qsOperator
			<< m_pForbiddenRecordList[i].qsProhibitEndTime);
	}
	m_pDataModel->setData(data);
	emit m_pDataModel->layoutChanged();

	m_buttonDelegate = new ForbiddenDelegate(this);
	connect(m_buttonDelegate, SIGNAL(onForbiddenClicked(const QModelIndex &)), this, SLOT(onForbiddenClicked(const QModelIndex &)));
	this->setItemDelegateForColumn(3, m_buttonDelegate);
}

void ForbiddenRecordTableView::initUI()
{
	this->verticalHeader()->hide();
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setSelectionMode(QAbstractItemView::NoSelection);
	this->setFocusPolicy(Qt::NoFocus);
	this->setFixedWidth(520);

	m_pDataModel = new ForbiddenRecordTableModel();
	this->setModel(m_pDataModel);
	QStringList headers;
	headers << QStringLiteral("被封禁用户") << QStringLiteral("操作人") << QStringLiteral("自动解封时间") << QStringLiteral("操作");
	m_pDataModel->setHorizontalHeader(headers);
	emit m_pDataModel->layoutChanged();

	this->horizontalHeader()->setFixedHeight(40);
	this->verticalHeader()->setDefaultSectionSize(34);
	this->setColumnWidth(0, 139);
	this->setColumnWidth(1, 120);
	this->setColumnWidth(2, 161);
	this->setColumnWidth(3, 100);

	for (int i = 0; i < this->horizontalHeader()->count(); ++i)
	{
		this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
	}
}

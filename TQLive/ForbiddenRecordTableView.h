#pragma once

#include <QTableView>
#include <QMouseEvent>
#include "ForbiddenRecordTableModel.h"
#include "ForbiddenDelegate.h"
#include "TipsDlg.h"
#include "StructData.h"
#include "CDataCentre.h"

class ForbiddenRecordTableView : public QTableView
{
	Q_OBJECT

public:
	explicit ForbiddenRecordTableView(QWidget *parent = 0);
	~ForbiddenRecordTableView();

	ForbiddenRecordTableModel* tableModel() { return m_pDataModel; }
	void initData(const QList<ST_PROHIBIT_TO_SPEAK_RECORD> &forbiddenRecordList);

private slots:
	void onForbiddenClicked(const QModelIndex &index);

private:
	void initUI();

private:
	ForbiddenRecordTableModel *m_pDataModel;
	ForbiddenDelegate *m_buttonDelegate;
	QList<ST_PROHIBIT_TO_SPEAK_RECORD> m_pForbiddenRecordList;
	TipsDlg *m_pTipsDlg;
};

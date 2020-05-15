#pragma once

#include <QObject>
#include "CDataCentre.h"

class CCustomTableView;
class QTableView;


class CTableViewImpl : public QObject
{
	Q_OBJECT

public:
	CTableViewImpl(QWidget *parent);
	~CTableViewImpl();
	void ResetData(QList<ST_MATCH_INFO> &obLInfo);
	QTableView *currentTableView();

signals:
	void signalConfirmChoice(int);
	void signalItemSelection(ST_MATCH_INFO &stMatchInfo);
private:
	CCustomTableView *m_pTableView;
};

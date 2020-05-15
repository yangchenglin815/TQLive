#pragma once

#include <QWidget>
#include "ui_CSynthesisSW.h"
#include "CDataCentre.h"

class CSynthesisSW : public QWidget
{
	Q_OBJECT

public:
	CSynthesisSW(QWidget *parent = Q_NULLPTR);
	~CSynthesisSW();

public slots:
	void slotMatchesSelection();
	void slotModuleSwitch(int nModuleType);
	void slotAcquireCompetitionList(int, const QString &, QList<ST_COMPETITION_INFO> obLCompetitionList, const void *userData);

signals:
	void signalItemSelection(ST_MATCH_INFO &stMatchInfo);

private:
	Ui::CSynthesisSW ui;
	bool m_bNeedLoad;
	QMap<int, QString> m_obMType;
};

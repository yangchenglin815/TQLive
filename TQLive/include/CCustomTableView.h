#ifndef MUSICINFOTABLEVIEW_H
#define MUSICINFOTABLEVIEW_H

#include <QTableView>
#include "CCustomTableModel.h"


class CCustomTableView : public QTableView
{
    Q_OBJECT

public:
    CCustomTableView(QWidget *parent = NULL);
    ~CCustomTableView();
    void SetContextMenu(QMenu *pContextMenu);
    void contextMenuEvent(QContextMenuEvent *event);
    //void mousePressEvent(QMouseEvent *event);
	void ResetData(QList<ST_COMPETITION_INFO> &obLInfo);


private:
    int m_nHoverRowSeq;
    QMenu *m_pContextMenu;
	CCustomTableModel *m_pCustomModel;


private slots:
    void slotEditorBtnClicked(int nRowSeqNum);
    //void slotCustomContextMenu(QPoint);
signals:
    void signalEditorBtnClicked(int nRowSeqNum);
};

#endif // MUSICINFOTABLEVIEW_H

#pragma once

#include <QLabel>

class CCustomLabel : public QLabel
{
	Q_OBJECT

public:
	CCustomLabel(QWidget *parent);
	~CCustomLabel();
	void paintEvent(QPaintEvent *event);
	void SetBkImagePath(const QString &qsPath);

signals:
	void clicked();

protected:
	virtual void mouseReleaseEvent(QMouseEvent *);

private:
	QString m_qsBkImagePath;
};

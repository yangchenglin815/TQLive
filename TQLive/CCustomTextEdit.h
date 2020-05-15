#pragma once

#include <QTextEdit>

class CCustomTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	CCustomTextEdit(QWidget *parent, bool isCursorVisible = false);
	~CCustomTextEdit();
	void SetCursorVisible(bool isCursorVisible);

protected:
	void mouseDoubleClickEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *);

private:
	QTimer *m_pCursorTimer;
	bool m_cursorVisible;
};

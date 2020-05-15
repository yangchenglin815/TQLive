#pragma once

#include <QObject>
#include <QLineEdit>

class HotKeyInputLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	HotKeyInputLineEdit(QWidget *parent);
	~HotKeyInputLineEdit();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void SetInputContent(QString content);
};

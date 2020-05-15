#pragma once

#include <QObject>
#include <QWidget>

class TextTemplateCom : public QWidget
{
	Q_OBJECT

public:
	TextTemplateCom(QWidget *parent);
	~TextTemplateCom();

public:
	void RenderText(QString text, QString path);

protected:
	void paintEvent(QPaintEvent *event);

private:
	int getTextLength() const;

private:
	QString m_sText;
	QString m_sPath;
};

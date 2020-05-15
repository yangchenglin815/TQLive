#pragma once

#include <QWidget>
#include "ui_AudioControlWindow.h"

class AudioControlWindow : public QWidget
{
	Q_OBJECT

public:
	AudioControlWindow(QWidget *parent = Q_NULLPTR);
	~AudioControlWindow();

	void InitMicSource();
	void SetMicHotKey(QString micHotKey);

signals:
	void onSigMicValueChanged(int val);
	void onSigJblValueChanged(int val);

private slots:
    void onSoundOffMIC();
    void onSoundOffJBL();

	void onMICValueChanged(int val);
	void onJBLValueChanged(int val);

private:
	void InitConenct();


private:
	Ui::AudioControlWindow ui;
	int m_nCurr_MICValue;
	int m_nCurr_JBLValue;
};

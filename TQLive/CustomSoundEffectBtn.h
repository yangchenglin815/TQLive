#pragma once

#include <QPushButton>
#include "StructData.h"

class CustomSoundEffectBtn : public QPushButton
{
	Q_OBJECT

public:
	CustomSoundEffectBtn(QWidget *parent = Q_NULLPTR);
	~CustomSoundEffectBtn();

signals:
	void onSigPlayAudio(QString audioFile);

public:
	void setData(const SoundEffectTypeData &data);
	SoundEffectTypeData& getData();

private:
	SoundEffectTypeData m_pSoundEffectTypeData;
};

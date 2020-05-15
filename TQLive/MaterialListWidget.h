#pragma once

#include <QListWidget>

class MaterialListWidget : public QListWidget
{
	Q_OBJECT

public:
	MaterialListWidget(QWidget *parent);
	~MaterialListWidget();

signals:
	void onSigAddMaterialOfCamera();
	void onSigAddMaterialOfVideo();
	void onSigAddMaterialOfImage();
	void onSigAddMaterialOfText();
	void onSigAddMaterialOfSource();
	void onSigAddMaterialOfCapture();
	void onSigAddMaterialOfFullScreen();
	void onSigAddMaterialOfWindow();

private slots:
	void onCustomMaterialButtonActive(int materialType);

private:
	void InitUI();
	void InitConnect();
};

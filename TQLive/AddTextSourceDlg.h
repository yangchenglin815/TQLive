#pragma once

#include <QDialog>
#include <obs.hpp>
#include <memory>
#include "ui_AddTextSourceDlg.h"
#include "StructData.h"

#define TEXT_LENGTH_MAX 20

class OBSBasic;
class AddTextSourceDlg : public QDialog
{
	Q_OBJECT
	using properties_delete_t = decltype(&obs_properties_destroy);
	using properties_t =
		std::unique_ptr<obs_properties_t, properties_delete_t>;

public:
	AddTextSourceDlg(QWidget *parent, OBSSource source, bool initFlag);
	~AddTextSourceDlg();

public:
	void Load();
	QString getSourceName();
	void Release();

private:
	void InitUI();
	void InitConnect();
	void InitSource();

private slots:
	void onDialogClosed();
	void onConfirmClicked();
	void onTextChanged(const QString &text);

private:
	Ui::AddTextSourceDlg ui;
	bool m_bNeedInit;
	TextSourceIniData m_textSourceIniData;

	OBSBasic *main;
	OBSSource m_pSource;
	properties_t properties = nullptr;
	obs_property_t *text_property = nullptr;
};

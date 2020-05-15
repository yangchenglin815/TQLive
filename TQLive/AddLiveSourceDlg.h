#pragma once

#include <QDialog>
#include <obs.hpp>
#include <memory>
#include "ui_AddLiveSourceDlg.h"

class OBSBasic;
class AddLiveSourceDlg : public QDialog
{
	Q_OBJECT
	using properties_delete_t = decltype(&obs_properties_destroy);
	using properties_t =
		std::unique_ptr<obs_properties_t, properties_delete_t>;

public:
	AddLiveSourceDlg(QWidget *parent, OBSSource source, bool initFlag);
	~AddLiveSourceDlg();

public:
	void Load();
	QString getSourceName();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
    void onDialogClosed();
    void onConfirmClicked();

private:
	void InitUI();
	void InitConnect();
	void InitSource();

private:
	Ui::AddLiveSourceDlg ui;
	bool m_bNeedInit;

	OBSBasic *main;
	OBSSource m_pSource;
	properties_t properties = nullptr;
	obs_property_t *input_property = nullptr;
};

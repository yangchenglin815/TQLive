#pragma once

#include <QDialog>
#include <obs.hpp>
#include <memory>
#include "ui_AddFullScreenSourceDlg.h"

class OBSBasic;
class AddFullScreenSourceDlg : public QDialog
{
	Q_OBJECT
	using properties_delete_t = decltype(&obs_properties_destroy);
	using properties_t =
		std::unique_ptr<obs_properties_t, properties_delete_t>;

public:
	AddFullScreenSourceDlg(QWidget *parent);
	~AddFullScreenSourceDlg();

public:
	void Load();
	QString getSourceName();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void onDialogClosed();
	void onConfirmClicked();
	void onSelectedChanged(int index);

private:
	void InitUI();
	void InitConnect();
	void InitSource();

private:
	Ui::AddFullScreenSourceDlg ui;
	OBSBasic *main;
	OBSSource m_pSource;
	properties_t properties = nullptr;
	obs_property_t *monitor_property = nullptr;
};

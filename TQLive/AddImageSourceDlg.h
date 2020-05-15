#pragma once

#include <QDialog>
#include <obs.hpp>
#include <memory>
#include "ui_AddImageSourceDlg.h"

class OBSBasic;
class OpacityButton;
class AddImageSourceDlg : public QDialog
{
	Q_OBJECT
	using properties_delete_t = decltype(&obs_properties_destroy);
	using properties_t =
		std::unique_ptr<obs_properties_t, properties_delete_t>;

public:
	AddImageSourceDlg(QWidget *parent, OBSSource source, bool initFlag, QString path);
	~AddImageSourceDlg();

public:
	void Load();
	QString getSourceName();
	void Release();

protected:
	void showEvent(QShowEvent *event);
	void moveEvent(QMoveEvent *event);
	void closeEvent(QCloseEvent *event);

private slots:
    void onDialogClosed();
    void onConfirmClicked();
    void onOpenImage();
    void onOpacityValChanged(int val);

private:
	void InitUI();
	void InitConnect();
	void InitSource();
	void AddNewFilter(const char* id);

	static void DrawPreview(void *data, uint32_t cx, uint32_t cy);

private:
	Ui::AddImageSourceDlg ui;
	OpacityButton *m_pSelectedButton;
	bool m_bNeedInit;
	QString m_ImgPath;

	OBSBasic *main;
	OBSSource m_pSource;
	properties_t properties = nullptr;
	obs_property_t *file_property = nullptr;
};

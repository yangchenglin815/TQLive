#pragma once

#include <QObject>

class AppInitializer : public QObject
{
	Q_OBJECT

public:
	AppInitializer(QObject *parent = 0);
	~AppInitializer();

	/*应用程序初始化*/
	void init();

private:
	/*单例初始化*/
	void initSingleObjects();

	/*配置文件初始化*/
	void initConfig();
};

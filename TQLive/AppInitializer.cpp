/***********************************************************************************
*								AppInitializer.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 用于进程启动时的一些初始化操作
***********************************************************************************/
#include "AppInitializer.h"
#include "UserConfig.h"
#include "HttpAgent.h"
#include "CTaskNotification.h"
#include "CController.h"
#include "SourceIniXML.h"
#include "SourceIniJson.h"
#include "CameraCacheJson.h"

AppInitializer::AppInitializer(QObject *parent)
	: QObject(parent)
{

}

AppInitializer::~AppInitializer()
{

}

void AppInitializer::init()
{
	//初始化
	initSingleObjects();
	initConfig();
}

void AppInitializer::initSingleObjects()
{
	//实例化单例
	UserConfig::instance();
	HttpAgent::instance();

	CTaskNotification::GetInstance();
	CController::GetInstance();

	SourceIniXML::instance()->ReadXml();
	SourceIniJson::instance()->Read();
	CameraCacheJson::instance()->Read();
}

void AppInitializer::initConfig()
{
	//加载配置信息
	UserConfig::instance()->load();
}
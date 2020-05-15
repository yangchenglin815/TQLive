/***********************************************************************************
*								DataPathManager.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 进程目录管理器实现
***********************************************************************************/
#include "DataPathManager.h"
#include <Windows.h>
#include <ShlObj.h>
#include <QFileInfo>

string DataPathManager::getUserDir()
{
	//获取应用程序数据文件存放目录
	char personalPath[MAX_PATH] = { 0 };
	setlocale(LC_ALL, "chs");
	bool bRet = SHGetSpecialFolderPathA(NULL, personalPath, CSIDL_PERSONAL, FALSE);

	string dir = string(personalPath);
	string userDir = dir + "\\TQLive";
	::CreateDirectoryA(userDir.c_str(), NULL);

	return userDir;
}

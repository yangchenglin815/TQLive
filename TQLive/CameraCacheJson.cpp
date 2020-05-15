/***********************************************************************************
*								CameraCacheJson.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2020-03
*
* Description: 摄像头素材缓存数据类
***********************************************************************************/
#include "CameraCacheJson.h"
#include "DataPathManager.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

CameraCacheJson* CameraCacheJson::m_pInstance = Q_NULLPTR;

CameraCacheJson * CameraCacheJson::instance()
{
	if (!m_pInstance) {
		m_pInstance = new CameraCacheJson;
	}
	return m_pInstance;
}

void CameraCacheJson::release()
{
	m_pCameraCacheList.clear();
}

void CameraCacheJson::Read()
{
	QFile file(m_pJsonPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QByteArray bytaArray = file.readAll();
	QString value = QString::fromLocal8Bit(bytaArray.data());
	file.close();

	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &jsonError);
	if (!document.isNull() && jsonError.error == QJsonParseError::NoError) {
		if (document.isObject()) {
			QJsonObject obj = document.object();
			if (obj.contains("CameraDevCache")) {
				QJsonValue arrayValue = obj.value("CameraDevCache");
				if (arrayValue.isArray()) {
					QJsonArray array = arrayValue.toArray();
					for (int i = 0; i < array.size(); ++i) {
						QJsonValue dataValue = array.at(i);
						QJsonObject childrens = dataValue.toObject();

						CameraCacheData data;
						data.sourceName = childrens["name"].toString();
						data.cameraItemIdx = childrens["deviceIdx"].toInt();
						data.resoultionItemIdx = childrens["resoultionIdx"].toInt();
						data.filterOfBgSelected = childrens["fliterOfBgSwitch"].toBool();
						data.filterOfBgColor = childrens["fliterOfBgColor"].toInt();
						data.similarityVal = childrens["similarityVal"].toInt();
						data.smoothnessVal = childrens["smoothnessVal"].toInt();
						data.spillVal = childrens["spillVal"].toInt();
						data.flipTypeVal = childrens["flipTypeVal"].toInt();
						m_pCameraCacheList.append(data);
					}
				}
			}
		}
	}
}

void CameraCacheJson::Write()
{
	QVariantHash data;
	QJsonArray array;
	for (auto &iter : m_pCameraCacheList) {
		QJsonObject obj;
		obj.insert("name", iter.sourceName);
		obj.insert("deviceIdx", iter.cameraItemIdx);
		obj.insert("resoultionIdx", iter.resoultionItemIdx);
		obj.insert("fliterOfBgSwitch", iter.filterOfBgSelected);
		obj.insert("fliterOfBgColor", iter.filterOfBgColor);
		obj.insert("similarityVal", iter.similarityVal);
		obj.insert("smoothnessVal", iter.smoothnessVal);
		obj.insert("spillVal", iter.spillVal);
		obj.insert("flipTypeVal", iter.flipTypeVal);
		array.append(obj);
	}
	data.insert("CameraDevCache", array);

	QJsonObject root = QJsonObject::fromVariantHash(data);
	QJsonDocument document;
	document.setObject(root);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QString json_str(byteArray);

	QFile file(m_pJsonPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		return;

	QTextStream in(&file);
	in << json_str;
	file.close();
}

CameraCacheJson::CameraCacheJson()
{
	m_pJsonPath = QString::fromStdString(DataPathManager::getUserDir() + "\\cameraCache.json");

	QFileInfo fileInfo(m_pJsonPath);
	if (!fileInfo.isFile()) {
		m_bIsExist = false;
	}
	else {
		m_bIsExist = true;
	}
}
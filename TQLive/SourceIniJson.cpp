/***********************************************************************************
*								SourceIniJson.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2020-03
*
* Description: 文本源配置JSON文件操作类
***********************************************************************************/
#include "SourceIniJson.h"
#include "DataPathManager.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

SourceIniJson* SourceIniJson::m_pInstance = Q_NULLPTR;

SourceIniJson * SourceIniJson::instance()
{
	if (!m_pInstance) {
		m_pInstance = new SourceIniJson;
	}
	return m_pInstance;
}

void SourceIniJson::release()
{
	m_pSourceIniList.clear();
}

void SourceIniJson::Read()
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
			if (obj.contains("TextSourceInfo")) {
				QJsonValue arrayValue = obj.value("TextSourceInfo");
				if (arrayValue.isArray()) {
					QJsonArray array = arrayValue.toArray();
					for (int i = 0; i < array.size(); ++i) {
						QJsonValue dataValue = array.at(i);
						QJsonObject childrens = dataValue.toObject();

						TextSourceIniData data;
						data.sourceName = childrens["name"].toString();
						data.text = childrens["text"].toString();
						data.path = childrens["path"].toString();
						m_pSourceIniList.append(data);
					}
				}
			}
		}
	}
}

void SourceIniJson::Write()
{
	QVariantHash data;
	QJsonArray array;
	for (auto &iter : m_pSourceIniList) {
		QJsonObject obj;
		obj.insert("name", iter.sourceName);
		obj.insert("text", iter.text);
		obj.insert("path", iter.path);
		array.append(obj);
	}
	data.insert("TextSourceInfo", array);

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

SourceIniJson::SourceIniJson()
{
	m_pJsonPath = QString::fromStdString(DataPathManager::getUserDir() + "\\settings.json");

	QFileInfo fileInfo(m_pJsonPath);
	if (!fileInfo.isFile()) {
		m_bIsExist = false;
	}
	else {
		m_bIsExist = true;
	}
}

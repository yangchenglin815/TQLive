/***********************************************************************************
*								SourceIniXML.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-12
*
* Description: 媒体源配置XML文件操作类
***********************************************************************************/
#include "SourceIniXML.h"
#include "DataPathManager.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QTextStream>
#include <QDomDocument>
#include <QDomProcessingInstruction>

SourceIniXML* SourceIniXML::m_pInstance = Q_NULLPTR;

SourceIniXML* SourceIniXML::instance()
{
	if (!m_pInstance) {
		m_pInstance = new SourceIniXML;
	}
	return m_pInstance;
}

SourceIniXML::SourceIniXML()
{
	m_pXmlPath = QString::fromStdString(DataPathManager::getUserDir() + "\\settings.xml");

	QFileInfo fileInfo(m_pXmlPath);
	if (!fileInfo.isFile()) {
		m_bIsExist = false;
	}
	else {
		m_bIsExist = true;
	}
}

void SourceIniXML::ReadXml()
{
	QFile file(m_pXmlPath);
	if (!file.open(QIODevice::ReadOnly))
		return;

	QDomDocument doc;
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	file.close();

	QDomElement root = doc.documentElement();
	QDomNode node = root.firstChild();
	while (!node.isNull()) {
		SourceIniData sourceIniData;
		QDomElement element = node.toElement();
		sourceIniData.sourceName = element.attribute("name");

		QDomNodeList list = element.childNodes();
		for (int i = 0; i < list.count(); i++) {
			QDomNode n = list.at(i);
			if (n.isElement()) {
				if (n.nodeName() == "cycleType") {
					sourceIniData.cycleType = n.toElement().text().toInt();
				}
				else if (n.nodeName() == "voiceVal") {
					sourceIniData.voiceVal = n.toElement().text().toInt();
				}
				else if (n.nodeName() == "listLength") {
					sourceIniData.listLength = n.toElement().text().toInt();
				}
				else if (n.nodeName() == "playList") {
					QDomElement e = n.toElement();
					QDomNode node_ = e.firstChild();
					while (!node_.isNull()) {
						MediaSourceData data;
						QDomElement element_ = node_.toElement();
						data.sMediaName = element_.attribute("name");
						QDomNodeList list_ = element_.childNodes();

						for (int j = 0; j < list_.count(); j++) {
							QDomNode n_ = list_.at(j);
							if (n_.isElement()) {
								if (n_.nodeName() == "id") {
									data.nId = n_.toElement().text().toInt();
								}
								else if (n_.nodeName() == "path") {
									data.sMediaPath = n_.toElement().text();
								}
								else if (n_.nodeName() == "active") {
									data.bActive = n_.toElement().text().toInt();
								}
							}
						}

						sourceIniData.mediaSourceList.append(data);
						node_ = node_.nextSibling();
					}
				}
			}
		}

		m_pSourceIniList.append(sourceIniData);
		node = node.nextSibling();
	}
}

void SourceIniXML::WriteXml()
{
	QFile file(m_pXmlPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		return;

	QDomDocument doc;
	QDomProcessingInstruction instruction;
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);

	QDomElement root = doc.createElement("source");
	doc.appendChild(root);

	for (auto &iter : m_pSourceIniList) {
		QDomElement media = doc.createElement("media");
		media.setAttribute("name", iter.sourceName);

		QDomElement cycleType = doc.createElement("cycleType");
		QDomText text = doc.createTextNode(QString::number(iter.cycleType));
		cycleType.appendChild(text);
		media.appendChild(cycleType);

		QDomElement voiceVal = doc.createElement("voiceVal");
		text = doc.createTextNode(QString::number(iter.voiceVal));
		voiceVal.appendChild(text);
		media.appendChild(voiceVal);

		QDomElement listLength = doc.createElement("listLength");
		text = doc.createTextNode(QString::number(iter.listLength));
		listLength.appendChild(text);
		media.appendChild(listLength);

		QDomElement playList = doc.createElement("playList");
		for (auto &it : iter.mediaSourceList) {
			QDomElement item = doc.createElement("item");
			item.setAttribute("name", it.sMediaName);

			QDomElement id = doc.createElement("id");
			text = doc.createTextNode(QString::number(it.nId));
			id.appendChild(text);
			item.appendChild(id);

			QDomElement path = doc.createElement("path");
			text = doc.createTextNode(it.sMediaPath);
			path.appendChild(text);
			item.appendChild(path);

			QDomElement active = doc.createElement("active");
			text = doc.createTextNode(QString::number(it.bActive));
			active.appendChild(text);
			item.appendChild(active);

			playList.appendChild(item);
		}
		media.appendChild(playList);
		
		root.appendChild(media);
	}

	QTextStream out_stream(&file);
	doc.save(out_stream, 4);
	file.close();
}

void SourceIniXML::release()
{
	m_pSourceIniList.clear();
}

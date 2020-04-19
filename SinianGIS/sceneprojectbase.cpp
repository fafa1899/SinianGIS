#include "sceneprojectbase.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <QJsonDocument>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "pathref.hpp"

using namespace std;

SceneProjectBase::SceneProjectBase()
{
    appDir = PathRef::GetAppDir();
}

SceneProjectBase::~SceneProjectBase()
{

}

void SceneProjectBase::read(std::string path)
{
    QFile file(QString::fromLocal8Bit(path.c_str()));
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("不能读取工程文件"));
        return;
    }

    QByteArray ba = file.readAll();
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
    if(e.error == QJsonParseError::NoError && !jsonDoc.isNull())
    {
        projectJSON = jsonDoc.object();
    }
}

void SceneProjectBase::write(std::string path)
{
    QJsonDocument document;
    document.setObject(projectJSON);
    QByteArray simpbyte_array = document.toJson(QJsonDocument::Indented);

    QFile file(QString::fromLocal8Bit(path.c_str()));
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("不能保存工程文件"));
        return;
    }
    file.write(simpbyte_array);
    file.close();
}

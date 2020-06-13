#include "mainwindow.h"
#include "osgshowwidget.h"
#include "pathref.hpp"
#include "ViewWidget"
#include "Settings.h"
#include "regedit.h"

#include <iostream>
#include <gdal_priv.h>
#include <QApplication>
#include <QStyleFactory>

using namespace std;

QSettings* gSettings = nullptr;

void InitGDALEnvi()
{
    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //支持中文路径
    CPLSetConfigOption("SHAPE_ENCODING", "");  //解决中文乱码问题
    string dir = PathRef::GetAppDir() + "/Resource/gdal_data";
    CPLSetConfigOption("GDAL_DATA", dir.c_str());
}

void regeditRef()
{
    QString className = QString::fromLocal8Bit("SinianGIS");                           // 自定义的类别
    QString appPath = QCoreApplication::applicationFilePath();        // 关联的程序目录
    QString ext = QString::fromLocal8Bit(".sj");                                       // 关联的文件类型
    QString extDes = QString::fromLocal8Bit("SinianGIS工程文件");                              // 该文件类型描述

    registerFileRelation(className, appPath, ext, extDes);

//    ext = QString::fromLocal8Bit(".obj");
//    registerFileRelation(className, appPath, ext, extDes);
}

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);

    //QSettings能记录一些程序中的信息，下次再打开时可以读取出来
    string iniPath = PathRef::GetAppDir() + "/SinianGIS.ini";
    QSettings setting(QString::fromLocal8Bit(iniPath.c_str()), QSettings::IniFormat);
    gSettings = &setting;

    InitGDALEnvi();
    //regeditRef();

    string filePath;
    if(argc>1)
    {
        filePath = argv[1];
    }

    MainWindow mainWindow(filePath);
    mainWindow.show();

    return a.exec();
}

//    错误调试代码
//    for(int i = 10; i>-1; i--)
//    {
//        int k =10/i;
//        cout<<k<<endl;
//    }

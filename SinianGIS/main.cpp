#include "mainwindow.h"
#include "osgshowwidget.h"
#include "pathref.hpp"
#include "ViewWidget"
#include "Settings.h"

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

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);

    //QSettings能记录一些程序中的信息，下次再打开时可以读取出来
    string iniPath = PathRef::GetAppDir() + "/SinianGIS.ini";
    QSettings setting(QString::fromLocal8Bit(iniPath.c_str()), QSettings::IniFormat);
    gSettings = &setting;

    InitGDALEnvi();

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}

//    错误调试代码
//    for(int i = 10; i>-1; i--)
//    {
//        int k =10/i;
//        cout<<k<<endl;
//    }

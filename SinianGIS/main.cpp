#include "mainwindow.h"
#include "osgshowwidget.h"

#include <QApplication>
#include <QStyleFactory>

#include "ViewWidget"

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);

//    OSGShowWidget* viewWidget = new OSGShowWidget();
//    viewWidget->setGeometry( 100, 100, 800, 600 );
//    viewWidget->show();
//    viewWidget->onStartTimer();

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}

//    ´íÎóµ÷ÊÔ´úÂë
//    for(int i = 10; i>-1; i--)
//    {
//        int k =10/i;
//        cout<<k<<endl;
//    }

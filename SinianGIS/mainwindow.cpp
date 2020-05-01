#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sceneproject3d.h"
#include "pathref.hpp"
#include "project3dform.h"
#include "loadphotogrammetrydialog.h"
#include "osgshowwidget.h"

#include <QDockWidget>
#include <QMenu>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    initWindow = false;

    ui->setupUi(this);

    //设置ribbon
    ui->ribbonTabWidget->setCurrentIndex(0);
    ui->toolBar->addWidget(ui->ribbonTabWidget);
    ui->toolBar->layout()->setMargin(0);
    ui->toolBar->layout()->setSpacing(0);

    //
    QMenu* ArcGISMenu=new QMenu(this);
    ArcGISMenu->addAction(ui->actionArcGISImage);
    ArcGISMenu->addAction(ui->actionArcGISTerrain);
    ui->tBArcGIS->setMenu(ArcGISMenu);
    ui->tBArcGIS->setPopupMode(QToolButton::InstantPopup);

    QMenu* BingMenu=new QMenu(this);
    BingMenu->addAction(ui->actionBingImage);
    BingMenu->addAction(ui->actionBingTerrain);
    ui->tBBing->setMenu(BingMenu);
    ui->tBBing->setPopupMode(QToolButton::InstantPopup);

    std::shared_ptr<SceneProject3D> _3dProject = make_shared<SceneProject3D>();
    string name = PathRef::DirOrPathGetName(_3dProject->getFileName());

    OSGShowWidget *tabWidget = new OSGShowWidget(ui->centralTabWidget);
    tabWidget->setMinimumSize(QSize(100, 100));
    tabWidget->load3DProject(_3dProject);

    ui->centralTabWidget->addTab(tabWidget, QString::fromLocal8Bit(name.c_str()));
    ui->centralTabWidget->setCurrentIndex(ui->centralTabWidget->indexOf(tabWidget));

    QDockWidget *projectDock = new QDockWidget(QString::fromLocal8Bit(name.c_str()), this);
    projectDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    Project3DForm *project3DForm = new Project3DForm(projectDock);
    project3DForm->LoadProject3d(_3dProject);
    projectDock->setWidget(project3DForm);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);

    connect(project3DForm, &Project3DForm::signalViewPoint, tabWidget, &OSGShowWidget::slotViewPoint);
    //connect(annotationHandler, &AnnotationHandler::signalPointAttributeDlg, this, &AnySceneWidget::signalAnnoPointAttributeDlg);

    curProj = std::dynamic_pointer_cast<SceneProjectBase>(_3dProject);
    curLeftDock = projectDock;
    projectMap.insert(make_pair(_3dProject->getFileName(), curProj));
    leftDockMap.insert(make_pair(_3dProject->getFileName(), curLeftDock));

    initWindow = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show()
{
    QWidget::show();
    //ui->showWidget->onStartTimer();

    OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->currentWidget());
    if(widget && !widget->isWork())
    {
        widget->onStartTimer();
    }
}

void MainWindow::on_tBNewProject_clicked()
{
}

void MainWindow::on_tBNewImageLayer_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/hunan-laiyan");
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开本地影像数据"),
                                                     dir,QString::fromLocal8Bit("本地影像数据(*.tif *.tiff *.img *.jpg *.png);;本地影像数据(*.*)"));
    if(filePath.isNull())
    {
        return;
    }

    QByteArray path = filePath.toLocal8Bit();

    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddLocalImage(path.data());
        OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->currentWidget());
        if(widget)
        {
            widget->SetTerrainLayerViewPoint(path.data());
        }

        Project3DForm* dock = dynamic_cast<Project3DForm*>(curLeftDock->widget());
        if(dock)
        {
            dock->AddImage(path.data());
        }
    }
}

void MainWindow::on_actionArcGISImage_triggered()
{
    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddArcGISImagery();
    }
}

void MainWindow::on_actionArcGISTerrain_triggered()
{
    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddArcGISTerrainImagery();
    }
}

void MainWindow::on_actionBingImage_triggered()
{
    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddBingImagery();
    }
}

void MainWindow::on_actionBingTerrain_triggered()
{
    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddBingTerrain();
    }
}

void MainWindow::on_tBNewTerrainLayer_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/hunan-laiyan");
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开本地地形数据"),
                                                     dir,QString::fromLocal8Bit("本地地形数据(*.tif *.img);;本地影像数据(*.*)"));
    if(filePath.isNull())
    {
        return;
    }

    QByteArray path = filePath.toLocal8Bit();

    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddLocalTerrain(path.data());
        OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->currentWidget());
        if(widget)
        {
            widget->SetTerrainLayerViewPoint(path.data());
        }

        Project3DForm* dock = dynamic_cast<Project3DForm*>(curLeftDock->widget());
        if(dock)
        {
            dock->AddTerrain(path.data());
        }
    }
}

void MainWindow::on_tBSaveProject_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/Test");
    QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("保存场景工程文件"),dir,
                                                    QString::fromLocal8Bit("场景工程文件(*.sj)"));
    if (fileName.isNull())
    {
        return;
    }
    QByteArray path = fileName.toLocal8Bit();

    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->write(path.data());
    }
}

void MainWindow::on_tBOpenProject_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/Test");
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开场景工程文件"),dir,
                                                    QString::fromLocal8Bit("场景工程文件(*.sj);;本地影像数据(*.*)"));
    if(filePath.isNull())
    {
        return;
    }
    QByteArray path = filePath.toLocal8Bit();

    std::shared_ptr<SceneProject3D> _3dProject = make_shared<SceneProject3D>();
    _3dProject->read(path.data());
    string name = PathRef::DirOrPathGetName(_3dProject->getFileName());

    OSGShowWidget *tabWidget = new OSGShowWidget(ui->centralTabWidget);
    ui->centralTabWidget->addTab(tabWidget, QString::fromLocal8Bit(name.c_str()));
    ui->centralTabWidget->setCurrentIndex(ui->centralTabWidget->indexOf(tabWidget));
    tabWidget->load3DProject(_3dProject);

    QDockWidget *projectDock = new QDockWidget(QString::fromLocal8Bit(name.c_str()), this);
    projectDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    Project3DForm *project3DForm = new Project3DForm(projectDock);
    projectDock->setWidget(project3DForm);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);

    tabifyDockWidget(curLeftDock, projectDock);
    //leftDockList.push_back(projectDock);

    project3DForm->LoadProject3d(_3dProject);
    projectDock->setVisible(true); //要先设置可见才能显示
    projectDock->raise();

    connect(project3DForm, &Project3DForm::signalViewPoint, tabWidget, &OSGShowWidget::slotViewPoint);

    curProj = std::dynamic_pointer_cast<SceneProjectBase>(_3dProject);
    curLeftDock = projectDock;
    projectMap[_3dProject->getFileName()] = curProj;
    leftDockMap[_3dProject->getFileName()] = curLeftDock;
}

void MainWindow::on_centralTabWidget_currentChanged(int index)
{
    if(!initWindow)
    {
        return;
    }

    for (int i = 0; i < ui->centralTabWidget->count(); i++)
    {
        if( i == index)
        {
            OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(i));
            if(widget && !widget->isWork())
            {
                widget->onStartTimer();
            }
        }
        else
        {
            OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(i));
            if(widget && widget->isWork())
            {
                widget->onStopTimer();
            }
        }
    }
}

void MainWindow::on_centralTabWidget_tabCloseRequested(int index)
{
    OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(index));
    if(widget && widget->isWork())
    {
        widget->onStopTimer();
    }

    ui->centralTabWidget->removeTab(index);
}

/*
void MainWindow::on_centralTabWidget_currentChanged(int index)
{
    if(!initWindow)
    {
        return;
    }

    for (int i = 0; i < ui->centralTabWidget->count(); i++)
    {
        if( i == index)
        {
            OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(i));
            if(widget && !widget->isWork())
            {
                cout<<"1111"<<endl;
                widget->onStartTimer();

                auto pit = projectMap.find(widget->GetName());
                if(pit != projectMap.end())
                {
                    curProj = pit->second;
                }

                auto dit = leftDockMap.find(widget->GetName());
                if(dit != leftDockMap.end())
                {
                    curLeftDock = dit->second;
                    curLeftDock->raise();
                }
            }
        }
        else
        {
            OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(i));
            if(widget && widget->isWork())
            {
                widget->onStopTimer();
            }
        }
    }
}

void MainWindow::on_centralTabWidget_tabCloseRequested(int index)
{
    OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(index));
    if(widget)
    {
        auto pit = projectMap.find(widget->GetName());
        if(pit != projectMap.end())
        {
            projectMap.erase(pit);
        }

        auto dit = leftDockMap.find(widget->GetName());
        if(dit != leftDockMap.end())
        {
            dit->second->close();
            leftDockMap.erase(dit);
        }

        delete widget;
        widget = nullptr;
    }
}*/

void MainWindow::closeEvent(QCloseEvent *e)
{
    for (int i = ui->centralTabWidget->count()-1; i >= 0; i--)
    {
        on_centralTabWidget_tabCloseRequested(i);
    }
}

void MainWindow::on_tBNewPhotogrammetry_clicked()
{
    LoadPhotogrammetryDialog loadDialog(this);
    if(QDialog::Rejected == loadDialog.exec())
    {
        return;
    }

    string dir = loadDialog.GetDataDir();
    if(dir.empty())
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件夹不能为空"));
        return;
    }

    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddPhotogrammetry(dir);
        OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->currentWidget());
        if(widget)
        {
            widget->SetNodeViewPoint(dir.data());
        }

        Project3DForm* dock = dynamic_cast<Project3DForm*>(curLeftDock->widget());
        if(dock)
        {
            dock->AddTiltingData(dir.data());
        }
    }
}

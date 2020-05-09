#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sceneproject3d.h"
#include "pathref.hpp"
#include "project3dform.h"
#include "loadphotogrammetrydialog.h"
#include "osgshowwidget.h"
#include "Settings.h"

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
    curProj = nullptr;
    curLeftDock = nullptr;

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
    loadProject(_3dProject);

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
    QString dir = gSettings->value("ImagePath").toString();
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开本地影像数据"),
                                                    dir,QString::fromLocal8Bit("本地影像数据(*.tif *.tiff *.img *.jpg *.png);;本地影像数据(*.*)"));
    if(filePath.isNull())
    {
        return;
    }
    gSettings->setValue("ImagePath", filePath);

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
    QString dir = gSettings->value("TerrainPath").toString();
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开本地地形数据"),
                                                    dir,QString::fromLocal8Bit("本地地形数据(*.tif *.img);;本地影像数据(*.*)"));
    if(filePath.isNull())
    {
        return;
    }
    gSettings->setValue("TerrainPath", filePath);

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
    QString dir = gSettings->value("SaveFilePath").toString();
    QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("保存场景工程文件"),dir,
                                                    QString::fromLocal8Bit("场景工程文件(*.sj)"));
    if (fileName.isNull())
    {
        return;
    }
    gSettings->setValue("SaveFilePath", fileName);

    QByteArray path = fileName.toLocal8Bit();

    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->write(path.data());
    }
}

void MainWindow::loadProject(std::shared_ptr<SceneProject3D> _3dProject)
{
    string name = PathRef::DirOrPathGetName(_3dProject->getFileName());

    OSGShowWidget *tabWidget = new OSGShowWidget(ui->centralTabWidget);
    if(ui->centralTabWidget->count()==0)
    {
        tabWidget->setMinimumSize(QSize(100, 100));
    }

    ui->centralTabWidget->addTab(tabWidget, QString::fromLocal8Bit(name.c_str()));
    ui->centralTabWidget->setCurrentIndex(ui->centralTabWidget->indexOf(tabWidget));
    tabWidget->load3DProject(_3dProject);

    QDockWidgetEx *projectDock = new QDockWidgetEx(QString::fromLocal8Bit(name.c_str()), this);
    projectDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    Project3DForm *project3DForm = new Project3DForm(projectDock);
    project3DForm->LoadProject3d(_3dProject);
    projectDock->setWidget(project3DForm);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);
    if(leftDockMap.size()!=0)
    {
        tabifyDockWidget(curLeftDock, projectDock);
    }

    connect(project3DForm, &Project3DForm::signalViewPoint, tabWidget, &OSGShowWidget::slotViewPoint);
    connect(projectDock, &QDockWidgetEx::signalClose, this, &MainWindow::slotCloseDock);

    curProj = std::dynamic_pointer_cast<SceneProjectBase>(_3dProject);
    curLeftDock = projectDock;
    projectMap[_3dProject->getFileName()] = curProj;
    leftDockMap[_3dProject->getFileName()] = curLeftDock;
}

void MainWindow::on_tBOpenProject_clicked()
{
    QString dir = gSettings->value("OpenFilePath").toString();
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开场景工程文件"),dir,
                                                    QString::fromLocal8Bit("场景工程文件(*.sj);;本地影像数据(*.*)"));
    if(filePath.isNull())
    {
        return;
    }
    gSettings->setValue("OpenFilePath", filePath);

    QByteArray path = filePath.toLocal8Bit();

    std::shared_ptr<SceneProject3D> _3dProject = make_shared<SceneProject3D>();
    _3dProject->read(path.data());
    loadProject(_3dProject);

    curLeftDock->setVisible(true); //要先设置可见才能显示
    curLeftDock->raise();
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

                auto pit = projectMap.find(widget->GetName());
                if(pit != projectMap.end())
                {
                    curProj = pit->second;
                }

                auto dit = leftDockMap.find(widget->GetName());
                if(dit != leftDockMap.end())
                {
                    if(curLeftDock != dit->second)
                    {
                        curLeftDock = dit->second;
                        curLeftDock->raise();
                    }
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
    ui->centralTabWidget->removeTab(index);
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
            auto dock = dit->second;
            leftDockMap.erase(dit);
            removeDockWidget(dock);
            dock->close();
        }

        if(widget->isWork())
        {
            widget->onStopTimer();
        }
    }
}

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

int MainWindow::find3DShowWidgetIndex(std::string name)
{
    for(int i = 0; i < ui->centralTabWidget->count(); i++)
    {
        OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(i));
        if(widget)
        {
            if(widget->GetName()==name)
            {
                return i;
            }
        }
    }
    return -1;
}

void MainWindow::on_MainWindow_tabifiedDockWidgetActivated(QDockWidget *dockWidget)
{
    if(curLeftDock==dockWidget)
    {
        return;
    }

    Project3DForm* form = dynamic_cast<Project3DForm *>(dockWidget->widget());
    if(form)
    {
        curLeftDock = dynamic_cast<QDockWidgetEx *>(dockWidget);
        if(!curLeftDock)
        {
            return;
        }

        auto pit = projectMap.find(form->GetName());
        if(pit != projectMap.end())
        {
            curProj = pit->second;
        }

        int index = find3DShowWidgetIndex(form->GetName());
        if(index>=0)
        {
            ui->centralTabWidget->setCurrentIndex(index);
        }
    }
}

void MainWindow::slotCloseDock(QDockWidgetEx *dockWidget)
{
    Project3DForm* form = dynamic_cast<Project3DForm *>(dockWidget->widget());
    if(form)
    {
        auto pit = projectMap.find(form->GetName());
        if(pit != projectMap.end())
        {
            projectMap.erase(pit);
        }

        auto dit = leftDockMap.find(form->GetName());
        if(dit != leftDockMap.end())
        {
            leftDockMap.erase(dit);
        }

        int index = find3DShowWidgetIndex(form->GetName());
        if(index>=0)
        {
            OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->widget(index));
            if(widget&&widget->isWork())
            {
                widget->onStopTimer();
            }
            ui->centralTabWidget->removeTab(index);
        }
    }
}

void MainWindow::on_tBNewVectorLayer_clicked()
{
    QString dir = gSettings->value("VectorPath").toString();
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开本地矢量数据"),
                                                    dir,QString::fromLocal8Bit("本地矢量数据(*.shp *.dxf);;本地矢量数据(*.*)"));
    if(filePath.isNull())
    {
        return;
    }
    gSettings->setValue("VectorPath", filePath);

    QByteArray path = filePath.toLocal8Bit();

    std::shared_ptr<SceneProject3D> proj = std::dynamic_pointer_cast<SceneProject3D>(curProj);
    if(proj)
    {
        proj->AddLocalVector(path.data());
        OSGShowWidget *widget = dynamic_cast<OSGShowWidget *>(ui->centralTabWidget->currentWidget());
        if(widget)
        {
           widget->SetNodeViewPoint(path.data());
        }

        Project3DForm* dock = dynamic_cast<Project3DForm*>(curLeftDock->widget());
        if(dock)
        {
            dock->AddVector(path.data());
        }
    }
}

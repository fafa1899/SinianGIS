#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sceneproject3d.h"

#include <QDockWidget>
#include <QMenu>

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ÉèÖÃribbon
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

    sceneProject3D = make_shared<SceneProject3D>();
    ui->showWidget->load3DProject(sceneProject3D);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show()
{
    QWidget::show();
    ui->showWidget->onStartTimer();
    ui->showWidget->show();
}

void MainWindow::on_tBNewProject_clicked()
{
    std::cout<<"ddddd";
}

void MainWindow::on_tBNewImageLayer_clicked()
{

}

void MainWindow::on_tBArcGIS_clicked()
{
}

void MainWindow::on_actionArcGISImage_triggered()
{
    sceneProject3D->AddArcGISImagery();
}

void MainWindow::on_actionArcGISTerrain_triggered()
{
    sceneProject3D->AddArcGISTerrainImagery();
}

void MainWindow::on_actionBingImage_triggered()
{
    sceneProject3D->AddBingImagery();
}

void MainWindow::on_actionBingTerrain_triggered()
{
    sceneProject3D->AddBingTerrain();
}

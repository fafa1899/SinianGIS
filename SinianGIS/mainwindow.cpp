#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    ui->ribbonTabWidget->setCurrentIndex(0);
    ui->toolBar->addWidget(ui->ribbonTabWidget);

    ui->toolBar->layout()->setMargin(0);
    ui->toolBar->layout()->setSpacing(0);
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

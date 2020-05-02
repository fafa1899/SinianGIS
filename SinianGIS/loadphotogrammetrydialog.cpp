#include "loadphotogrammetrydialog.h"
#include "ui_loadphotogrammetrydialog.h"
#include "Settings.h"

#include <QMovie>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

LoadPhotogrammetryDialog::LoadPhotogrammetryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadPhotogrammetryDialog)
{
    ui->setupUi(this);

    QMovie * move = new QMovie(this);
    move->setFileName(":/Res/photogrammetry_data.gif");
    ui->labelGIF->setMovie(move);
   //ui->label_gif->setFixedSize(200,200);
   //ui->label_gif->setScaledContents(true);
    move->start();
}

LoadPhotogrammetryDialog::~LoadPhotogrammetryDialog()
{
    delete ui;
}

void LoadPhotogrammetryDialog::on_pushButtonOK_clicked()
{
    QString dirPath = ui->lEDir->text();
    if(dirPath.isEmpty() || dirPath.isNull())
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件夹不能为空"));
        return;
    }

    QByteArray path = dirPath.toLocal8Bit();
    dataDir = path.data();
    QDialog::accept();
}

void LoadPhotogrammetryDialog::on_pushButtonCancel_clicked()
{
    QDialog::reject();
}

void LoadPhotogrammetryDialog::on_pBDir_clicked()
{
    QString dir = gSettings->value("ObliquePath").toString();
    QString dirPath = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("选择倾斜摄影数据文件夹："), dir);
    if(dirPath.isNull())
    {
        return;
    }
    gSettings->setValue("ObliquePath", dirPath);

    ui->lEDir->setText(dirPath);
}

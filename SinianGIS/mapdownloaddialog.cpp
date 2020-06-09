#include "mapdownloaddialog.h"
#include "ui_mapdownloaddialog.h"
#include "Settings.h"
#include "GoogleMapTiles.h"

#include <QFileDialog>
#include <iostream>

using namespace std;

MapDownloadDialog::MapDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapDownloadDialog)
{
    ui->setupUi(this);
    ui->lELeft->setText(gSettings->value("Left").toString());
    ui->lETop->setText(gSettings->value("Top").toString());
    ui->lERight->setText(gSettings->value("Right").toString());
    ui->lEBottom->setText(gSettings->value("Bottom").toString());
    ui->spinBoxDownLoadLevel->setValue(gSettings->value("DownLoadLevel").toInt());
    ui->lineEditDownLoadProxy->setText(gSettings->value("DownLoadProxy").toString());
}

MapDownloadDialog::~MapDownloadDialog()
{
    delete ui;
}

void MapDownloadDialog::SaveIniConfig()
{
    gSettings->setValue("Left", ui->lELeft->text());
    gSettings->setValue("Top", ui->lETop->text());
    gSettings->setValue("Right", ui->lERight->text());
    gSettings->setValue("Bottom", ui->lEBottom->text());
    gSettings->setValue("DownLoadLevel", ui->spinBoxDownLoadLevel->value());
    gSettings->setValue("DownLoadProxy", ui->lineEditDownLoadProxy->text());
}

void MapDownloadDialog::on_pBOK_clicked()
{
    SaveIniConfig();

    WebTilesClass *webTiles = new GoogleMapTiles();

    double l = ui->lELeft->text().toDouble();
    double t = ui->lETop->text().toDouble();
    double r = ui->lERight->text().toDouble();
    double b = ui->lEBottom->text().toDouble();
    int z = ui->spinBoxDownLoadLevel->value();
    auto proxy = ui->lineEditDownLoadProxy->text().toLocal8Bit();
    auto outputDir = ui->lEOutDir->text().toLocal8Bit();

    vector<OneTile> tiles;
    webTiles->GetTiles(l, t, r, b, z, tiles);

    //size_t i = 0;
    for (size_t i = 0; i < tiles.size(); i++)
    {
        cout << (float)(i + 1) / tiles.size() << endl;
        string imgName = string(outputDir.data()) + "/" + to_string(tiles[i].y) + "_" + to_string(tiles[i].x);
        string imgPath = imgName + ".jpg";
        DownloadTiles(tiles[i].uri, imgPath, proxy.data());
        webTiles->WriteGeoInfo(imgName, tiles[i]);
    }

    delete webTiles;

    QDialog::accept();
}

void MapDownloadDialog::on_pBCancel_clicked()
{
    QDialog::reject();
}

void MapDownloadDialog::on_pBOutDir_clicked()
{
    QString dir = gSettings->value("DownLoadTilesDir").toString();
    QString dirPath = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("选择下载文件夹："), dir);
    if(dirPath.isNull())
    {
        return;
    }
    gSettings->setValue("DownLoadTilesDir", dirPath);

    ui->lEOutDir->setText(dirPath);
}

#include "loadphotogrammetrydialog.h"
#include "ui_loadphotogrammetrydialog.h"

#include <QMovie>

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

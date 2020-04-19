#include "project3dform.h"
#include "ui_project3dform.h"
#include "pathref.hpp"

Project3DForm::Project3DForm(QWidget *parent) :
    QWidget(parent), sceneProject3D(nullptr),
    mainIcon(":/Res/main.png"),
    terrainIcon(":/Res/terrain.png"),
    imageIcon(":/Res/image.png"),
    vectorIcon(":/Res/vector.png"),
    ui(new Ui::Project3DForm)
{
    ui->setupUi(this);

    //
    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0, QString::fromLocal8Bit("三维场景"));
    ui->treeWidget->addTopLevelItem(root);
    root->setCheckState(0, Qt::Checked);
    root->setIcon(0, mainIcon);

    //
    terrainItem = new QTreeWidgetItem(root);
    terrainItem->setText(0, QString::fromLocal8Bit("地形"));
    root->addChild(terrainItem);
    terrainItem->setCheckState(0, Qt::Checked);
    terrainItem->setIcon(0, terrainIcon);

    //
    imageItem = new QTreeWidgetItem(root);
    imageItem->setText(0, QString::fromLocal8Bit("影像"));
    root->addChild(imageItem);
    imageItem->setCheckState(0, Qt::Checked);
    imageItem->setIcon(0, imageIcon);

    //
    vectorItem = new QTreeWidgetItem(root);
    vectorItem->setText(0, QString::fromLocal8Bit("矢量"));
    root->addChild(vectorItem);
    vectorItem->setCheckState(0, Qt::Checked);
    vectorItem->setIcon(0, vectorIcon);

//    //
//    QTreeWidgetItem* tiltingDataItem = new QTreeWidgetItem();
//    tiltingDataItem->setText(0, QString::fromLocal8Bit("场景数据(倾斜摄影)"));
//    root->addChild(tiltingDataItem);
//    tiltingDataItem->setCheckState(0, Qt::Checked);

//    //
//    QTreeWidgetItem* modelDataItem = new QTreeWidgetItem();
//    modelDataItem->setText(0, QString::fromLocal8Bit("模型数据"));
//    root->addChild(modelDataItem);
//    modelDataItem->setCheckState(0, Qt::Checked);

    //展开所有的子项目
    ui->treeWidget->expandAll(); //要在设置完子项目之后再调用才有效
}

Project3DForm::~Project3DForm()
{
    delete ui;
}

void Project3DForm::LoadProject3d(std::shared_ptr<SceneProject3D> project)
{
    sceneProject3D = project;
    for (int i = 0; i < sceneProject3D->localImageArray.size(); i++)
    {
        QJsonValue value = sceneProject3D->localImageArray.at(i);
        if (value.isString())
        {
            QByteArray v = value.toString().toLocal8Bit();

            QTreeWidgetItem *subItem = new QTreeWidgetItem(imageItem);
            string name = PathRef::DirOrPathGetName(v.data());
            subItem->setText(0, QString::fromLocal8Bit(name.c_str()));
            imageItem->addChild(subItem);
            subItem->setCheckState(0, Qt::Checked);
            QIcon imageIcon(":/Res/image.png");
            subItem->setIcon(0, imageIcon);
        }
    }
}

#ifndef PROJECT3DFORM_H
#define PROJECT3DFORM_H

#include "sceneproject3d.h"

#include <QWidget>
#include <QTreeWidgetItem>


namespace Ui {
class Project3DForm;
}

class Project3DForm : public QWidget
{
    Q_OBJECT

public:
    explicit Project3DForm(QWidget *parent = nullptr);
    ~Project3DForm();

    void LoadProject3d(std::shared_ptr<SceneProject3D> sceneProject3D);

    void AddTerrain(std::string fileName);
    void AddImage(std::string fileName);
    void AddVector();
    void AddTiltingData(std::string fileName);

signals:
    void signalViewPoint(std::string name);
    void signalTerrainViewPoint();
    void signalImageViewPoint();
    void signalTiltingDataViewPoint();

protected:
    QTreeWidgetItem* terrainItem;
    QTreeWidgetItem* imageItem;
    QTreeWidgetItem* vectorItem;
    QTreeWidgetItem* tiltingDataItem;

    std::shared_ptr<SceneProject3D> sceneProject3D;

    QIcon mainIcon;
    QIcon terrainIcon;
    QIcon imageIcon;
    QIcon vectorIcon;
    QIcon tiltingDataIcon;

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:

    Ui::Project3DForm *ui;
};

#endif // PROJECT3DFORM_H

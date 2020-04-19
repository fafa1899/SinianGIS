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

protected:
    QTreeWidgetItem* terrainItem;
    QTreeWidgetItem* imageItem;
    QTreeWidgetItem* vectorItem;

    std::shared_ptr<SceneProject3D> sceneProject3D;

    QIcon mainIcon;
    QIcon terrainIcon;
    QIcon imageIcon;
    QIcon vectorIcon;

private:
    Ui::Project3DForm *ui;
};

#endif // PROJECT3DFORM_H

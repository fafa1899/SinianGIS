#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sceneproject3d.h"

#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void show();

protected:
    std::shared_ptr<SceneProject3D> sceneProject3D;

private slots:
    void on_tBNewProject_clicked();

    void on_tBNewImageLayer_clicked();

    void on_tBArcGIS_clicked();

    void on_actionArcGISImage_triggered();

    void on_actionArcGISTerrain_triggered();

    void on_actionBingImage_triggered();

    void on_actionBingTerrain_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

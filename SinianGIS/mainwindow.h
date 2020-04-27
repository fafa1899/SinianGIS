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

signals:
    void signalDockRaise();

protected:
    void closeEvent(QCloseEvent *e);

    std::map<std::string, std::shared_ptr<SceneProjectBase>> projectMap;
    std::map<std::string, QDockWidget *> leftDockMap;

    std::shared_ptr<SceneProjectBase> curProj;
    QDockWidget *curLeftDock;

    bool initWindow;

private slots:
    void on_tBNewProject_clicked();

    void on_tBNewImageLayer_clicked();

    void on_actionArcGISImage_triggered();

    void on_actionArcGISTerrain_triggered();

    void on_actionBingImage_triggered();

    void on_actionBingTerrain_triggered();

    void on_tBNewTerrainLayer_clicked();

    void on_tBSaveProject_clicked();

    void on_tBOpenProject_clicked();

    void on_centralTabWidget_currentChanged(int index);

    void on_centralTabWidget_tabCloseRequested(int index);

    void on_tBNewPhotogrammetry_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

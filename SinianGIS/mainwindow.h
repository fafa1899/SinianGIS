#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sceneproject3d.h"
#include "qdockwidgetex.h"

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

    void loadProject(std::shared_ptr<SceneProject3D> _3dProject);

    int find3DShowWidgetIndex(std::string name);

    std::map<std::string, std::shared_ptr<SceneProjectBase>> projectMap;
    std::map<std::string, QDockWidgetEx *> leftDockMap;

    std::shared_ptr<SceneProjectBase> curProj;
    QDockWidgetEx *curLeftDock;

    bool initWindow;

private slots:
    void on_tBNewProject_clicked();

    //加载影像数据
    void on_tBNewImageLayer_clicked();

    void on_actionArcGISImage_triggered();

    void on_actionArcGISTerrain_triggered();

    void on_actionBingImage_triggered();

    void on_actionBingTerrain_triggered();

    //加载地形数据
    void on_tBNewTerrainLayer_clicked();

    void on_tBSaveProject_clicked();

    //打开数据
    void on_tBOpenProject_clicked();

    void on_centralTabWidget_currentChanged(int index);

    void on_centralTabWidget_tabCloseRequested(int index);

    //加载倾斜摄影数据
    void on_tBNewPhotogrammetry_clicked();

    //堆叠的停靠窗口被激活
    void on_MainWindow_tabifiedDockWidgetActivated(QDockWidget *dockWidget);

    void slotCloseDock(QDockWidgetEx *dockWidget);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

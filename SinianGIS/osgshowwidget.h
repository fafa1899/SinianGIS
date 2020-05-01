#ifndef OSGSHOWWIDGET_H
#define OSGSHOWWIDGET_H

#include "sceneproject3d.h"

#include <map>

#include <QWidget>
#include <QTimer>

#include <osgViewer/CompositeViewer>
#include <osgEarthUtil/EarthManipulator>

class OSGShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OSGShowWidget(QWidget *parent = nullptr);
    ~OSGShowWidget();

    void destory();

    bool load3DProject(std::shared_ptr<SceneProject3D> project);

    void SetTerrainLayerViewPoint(std::string name);
    void SetNodeViewPoint(std::string name);

    //启动定时器绘制
    void onStartTimer();
    //关闭定时器绘制
    void onStopTimer();

    bool isWork(){return bWork;}

    std::string GetName(){return sceneProject3D?sceneProject3D->getFileName():"";}

protected:
    //virtual void paintEvent(QPaintEvent* e);
    virtual void timerEvent(QTimerEvent* );

    void addView();

    bool CalViewPointGeoExtend(const osgEarth::GeoExtent& extent, std::shared_ptr<osgEarth::Viewpoint> out);
    bool CalViewPointNode(osg::ref_ptr<osg::Node> node, std::shared_ptr<osgEarth::Viewpoint> out);

    //QTimer                     _timer;
    int _timerID;               //定时器ID
    osgViewer::CompositeViewer _viewer;

    bool bWork;

    osg::ref_ptr<osgViewer::View> view;
    std::shared_ptr<SceneProject3D> sceneProject3D;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mainManipulator;

    QWidget* viewWidget;

signals:

public slots:
    void slotViewPoint(std::string name);
};

#endif // OSGSHOWWIDGET_H

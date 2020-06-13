#include "osgshowwidget.h"
#include "ViewWidget"
#include "pathref.hpp"

#include <QLayout>
#include <QMessageBox>

#include <osgEarth/ModelLayer>
#include <osgEarth/GLUtils>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgEarthUtil/ViewFitter>
#include <osgEarthUtil/AutoClipPlaneHandler>

using namespace std;

OSGShowWidget::OSGShowWidget(QWidget *parent) : QWidget(parent)
{
    viewWidget = nullptr;
    bWork = false;

    _viewer.setThreadingModel(_viewer.SingleThreaded);

    // we have to add a starting view, otherwise the CV will automatically
    // mark itself as done :/
    addView();

    // timer fires a paint event.
    //    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    //    _timer.start(10);

    //sceneProject3D = nullptr;
}

OSGShowWidget::~OSGShowWidget()
{
    if(viewWidget)
    {
        delete viewWidget;
        viewWidget = nullptr;
    }
}

//bool OSGShowWidget::load3DProject(std::shared_ptr<SceneProject3D> project)
//{
//    this->sceneProject3D = project;

//    view->setSceneData(sceneProject3D->GetRootNode());
//    mainManipulator->setViewpoint(*(sceneProject3D->GetHomeViewPoint()));
//    mainManipulator->setHomeViewpoint(*(sceneProject3D->GetHomeViewPoint()));

//    view->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(sceneProject3D->GetMapNode()));           //加上自动裁剪。否则显示会出现遮挡

//    return true;
//}

//void OSGShowWidget::paintEvent(QPaintEvent* e)
//{
// refresh all the views.
//    if (_viewer.getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS || _viewer.checkNeedToDoFrame() )
//    {
//        _viewer.frame();
//    }
//}

//启动定时器绘制
void OSGShowWidget::onStartTimer()
{
    _timerID = startTimer(10);
    bWork = true;
}

//关闭定时器绘制
void OSGShowWidget::onStopTimer()
{
    killTimer(_timerID);
    bWork = false;
}

//定时器事件
void OSGShowWidget::timerEvent(QTimerEvent* )
{
    // refresh all the views.
    if (_viewer.getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS || _viewer.checkNeedToDoFrame() )
    {
        _viewer.frame();
    }
}

void OSGShowWidget::addView()
{
    // the new View we want to add:
    view = new osgViewer::View();

    // a widget to hold our view:
    viewWidget = new osgEarth::QtGui::ViewWidget(view);

    setLayout(new QHBoxLayout());
    layout()->setMargin(1);
    layout()->addWidget(viewWidget);

    // set up the view
    //mainManipulator = new osgEarth::Util::EarthManipulator;
    //view->setCameraManipulator(mainManipulator);
    //view->setSceneData( root );
    view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,false);

    view->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);            //LabelNode需要
    osgEarth::GLUtils::setGlobalDefaults(view->getCamera()->getOrCreateStateSet());           //FeatureNode需要
    //view->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(node));           //加上自动裁剪。否则显示会出现遮挡

    //osgEarth::Util::MapNodeHelper().configureView(view);         // add some stock OSG handlers:
    view->addEventHandler(new osgViewer::StatsHandler());
    view->addEventHandler(new osgViewer::HelpHandler);

    // add it to the composite viewer.
    _viewer.addView( view );
}

//void OSGShowWidget::SetTerrainLayerViewPoint(std::string name)
//{
//    auto layer = sceneProject3D->GetMap()->getLayerByName<osgEarth::TerrainLayer>(name);
//    if (!layer)
//    {
//        QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("找不到合适的视点"), QMessageBox::Ok);
//        return;
//    }

//    //获取视点位置
//    std::shared_ptr<osgEarth::Viewpoint> vp =  std::make_shared<osgEarth::Viewpoint>();
//    if (!CalViewPointGeoExtend(layer->getDataExtentsUnion(), vp))
//    {
//        QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("找不到合适的视点"), QMessageBox::Ok);
//        auto cp = layer->getDataExtentsUnion().getCentroid();
//        osgEarth::GeoPoint point(layer->getProfile()->getSRS(), cp.x(), cp.y(), cp.z());
//        osgEarth::GeoPoint newPoint = point.transform(sceneProject3D->GetMap()->getSRS());
//        vp->focalPoint() = newPoint;
//        vp->heading() = 0;
//        vp->pitch() = -90;
//        vp->range() = 16000;
//    }

//    sceneProject3D->insertViewPoint(name, vp);
//    mainManipulator->setViewpoint(*vp);
//}

//void OSGShowWidget::SetNodeViewPoint(std::string name)
//{
//    auto layer = sceneProject3D->GetMap()->getLayerByName<osgEarth::VisibleLayer>(name);
//    if (!layer)
//    {
//        QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("找不到合适的视点"), QMessageBox::Ok);
//        return;
//    }

//    //计算视点
//    std::shared_ptr<osgEarth::Viewpoint> vp =  std::make_shared<osgEarth::Viewpoint>();
//    if(!layer->getNode() || !CalViewPointNode(layer->getNode(), vp))
//    {
//        QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("找不到合适的视点"), QMessageBox::Ok);
//        //        out->focalPoint() = point;
//        //        out->heading() = 0;
//        //        out->pitch() = -90;
//        //        out->range() = 16000;
//    }

//    sceneProject3D->insertViewPoint(name, vp);
//    mainManipulator->setViewpoint(*vp);
//}

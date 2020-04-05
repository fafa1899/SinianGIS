#include "osgshowwidget.h"
#include "ViewWidget"

#include <QLayout>

#include <osgViewer/Viewer>

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

#include <osgEarthUtil/EarthManipulator>
//#include <osgEarthUtil/ViewFitter>
//#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/ExampleResources>
//#include <osgEarthUtil/RTTPicker>
//#include <osgEarthUtil/Shadowing>

using namespace std;

OSGShowWidget::OSGShowWidget(QWidget *parent) : QWidget(parent)
{
    _viewer.setThreadingModel(_viewer.SingleThreaded);

    std::string path = "D:/Work/OSGNewBuild/osgearth-2.10.1/tests/simple.earth";
    //std::string path = "D:/Data/hhsx/Data/MultiFoderReader.osgb";

    // load something
    _scene = osgDB::readNodeFile(path);
    if (!_scene.valid())
    {
        printf("dddd\n");
    }

    // we have to add a starting view, otherwise the CV will automatically
    // mark itself as done :/
    addView();

    // timer fires a paint event.
//    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
//    _timer.start(10);
}

bool OSGShowWidget::loadData(std::string path)
{
    return true;
}

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
}

//关闭定时器绘制
void OSGShowWidget::onStopTimer()
{
    killTimer(_timerID);
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
    osgViewer::View* view = new osgViewer::View();

    // a widget to hold our view:
    QWidget* viewWidget = new osgEarth::QtGui::ViewWidget(view);

    setLayout(new QHBoxLayout());
    layout()->setMargin(1);
    layout()->addWidget(viewWidget);

    // set up the view
    view->setCameraManipulator( new osgEarth::Util::EarthManipulator );
    //view->setCameraManipulator( new osgGA::TrackballManipulator );
    view->setSceneData( _scene );
    view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,false);

    //osgEarth::Util::MapNodeHelper().configureView(view);         // add some stock OSG handlers:
    view->addEventHandler(new osgViewer::StatsHandler());
    view->addEventHandler(new osgViewer::HelpHandler);


    // add it to the composite viewer.
    _viewer.addView( view );
}

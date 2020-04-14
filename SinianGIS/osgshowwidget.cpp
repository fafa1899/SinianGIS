#include "osgshowwidget.h"
#include "ViewWidget"

#include <QLayout>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

using namespace std;

OSGShowWidget::OSGShowWidget(QWidget *parent) : QWidget(parent)
{
    _viewer.setThreadingModel(_viewer.SingleThreaded);

    // we have to add a starting view, otherwise the CV will automatically
    // mark itself as done :/
    addView();

    // timer fires a paint event.
    //    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    //    _timer.start(10);

    sceneProject3D = nullptr;
}

bool OSGShowWidget::load3DProject(std::shared_ptr<SceneProject3D> project)
{
    this->sceneProject3D = project;

    view->setSceneData(sceneProject3D->GetRootNode());
    mainManipulator->setViewpoint(*(sceneProject3D->GetHomeViewPoint()));
    mainManipulator->setHomeViewpoint(*(sceneProject3D->GetHomeViewPoint()));

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
    view = new osgViewer::View();

    // a widget to hold our view:
    QWidget* viewWidget = new osgEarth::QtGui::ViewWidget(view);

    setLayout(new QHBoxLayout());
    layout()->setMargin(1);
    layout()->addWidget(viewWidget);

    // set up the view
    mainManipulator = new osgEarth::Util::EarthManipulator;
    view->setCameraManipulator(mainManipulator);
    //view->setSceneData( root );
    view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,false);

    //osgEarth::Util::MapNodeHelper().configureView(view);         // add some stock OSG handlers:
    view->addEventHandler(new osgViewer::StatsHandler());
    view->addEventHandler(new osgViewer::HelpHandler);

    // add it to the composite viewer.
    _viewer.addView( view );
}

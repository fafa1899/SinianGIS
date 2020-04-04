#include "mainwindow.h"
#include "osgshowwidget.h"

#include <QApplication>
#include <QStatusBar>

#include "ViewWidget"

#include <osg/Notify>
#include <osgViewer/CompositeViewer>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/Random>
#include <osgEarth/FileUtils>
#include <QApplication>
#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QLayout>


using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::QtGui;

//------------------------------------------------------------------

int
usage( const std::string& msg, osg::ArgumentParser& args )
{
    OE_NOTICE << msg << std::endl << std::endl;
    OE_NOTICE << "USAGE: " << args[0] << " file.earth" << std::endl;

    return -1;
}

//------------------------------------------------------------------

struct ViewController
{
    virtual void addView() =0;
};


struct MyMainWindow : public QMainWindow, public ViewController
{
    QTimer                     _timer;
    osgViewer::CompositeViewer _viewer;
    osg::ref_ptr<osg::Node>    _scene;

    MyMainWindow(osg::ArgumentParser& args, osg::Node* scene)
        : _viewer(args), _scene(scene)
    {
        _viewer.setThreadingModel(_viewer.SingleThreaded);

        // we have to add a starting view, otherwise the CV will automatically
        // mark itself as done :/
        addView();

        // timer fires a paint event.
        connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
        _timer.start(20);
    }

    void paintEvent(QPaintEvent* e)
    {
        // refresh all the views.
        if (_viewer.getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS ||
            _viewer.checkNeedToDoFrame() )
        {
            _viewer.frame();
        }
    }

    void addView()
    {
        // the new View we want to add:
        osgViewer::View* view = new osgViewer::View();

        // a widget to hold our view:
        QWidget* viewWidget = new osgEarth::QtGui::ViewWidget(view);

        // a dialog to hold the view widget:
        QDialog* win = new QDialog(this);
        win->setModal( false );
        win->setLayout( new QHBoxLayout() );
        win->layout()->addWidget( viewWidget );
        int x = osgEarth::Random().next( 1024 );
        int y = osgEarth::Random().next( 768 );
        win->setGeometry( x, y, 640, 480 );
        win->show();

        // set up the view
        view->setCameraManipulator( new osgEarth::Util::EarthManipulator );
        view->setSceneData( _scene.get() );
        view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,false);

        // add it to the composite viewer.
        _viewer.addView( view );
    }
};

//------------------------------------------------------------------

struct AddButton : public QPushButton
{
    ViewController* _vc;

    AddButton(ViewController* vc, const char* text)
        : QPushButton(text), _vc(vc) { }

    void mousePressEvent( QMouseEvent* e )
    {
        _vc->addView();
    }
};


int main(int argc, char *argv[])
{

    osg::ArgumentParser args(&argc,argv);
    if ( args.find("--help") >= 0)
        return usage("Help", args);

    // load something
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFiles( args );
    if (!node.valid())
        return usage("Can't load a scene!", args);


#ifdef Q_WS_X11
    // required for multi-threaded viewer on linux:
    XInitThreads();
#endif

    // Qt setup:
    QApplication q(argc, argv);

    // fire up our controller:
    MyMainWindow win( args, node.get() );

    // A button for adding new views:
    QPushButton* addButton = new AddButton( &win, "Add a view" );

    // Main window for the button.
    //QMainWindow win;
    win.setCentralWidget( addButton );
    win.setGeometry( 100, 100, 200, 100 );
    win.show();

    q.exec();


    /*
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();


    osgViewer::Viewer viewer;
    viewer.setThreadingModel( viewer.SingleThreaded );
    viewer.setRunFrameScheme( viewer.ON_DEMAND );
    //viewer.setCameraManipulator( new EarthManipulator() );

    //    // load an earth file
    //    osg::Node* node = MapNodeHelper().load(arguments, &viewer);
    //    if ( !node )
    //        return usage( "Failed to load earth file." );

    std::string osgPath = "D:/Data/hhsx/Data/MultiFoderReader.osgb";
        //string osgPath = "D:/Data/hhsx/Data/Tile_+013_+012/Tile_+013_+012.osgb";
    osg::Node * node = osgDB::readNodeFile(osgPath);
    viewer.setSceneData( node );

    QWidget* viewerWidget = new osgEarth::QtGui::ViewWidget(&viewer);
    viewerWidget->show();

    QMainWindow win;
    win.setCentralWidget( viewerWidget );
    win.setGeometry(100, 100, 1024, 800);

    win.show();


//    OSGShowWidget* viewWidget = new OSGShowWidget();
//    viewWidget->setGeometry( 100, 100, 800, 600 );
//    viewWidget->show();

    return a.exec();  */
}


#include "osg3dobjectshowwidget.h"

#include <osgGA/TrackballManipulator>

osg::ref_ptr<osgGA::TrackballManipulator> mainManipulator;

OSG3DObjectShowWidget::OSG3DObjectShowWidget(QWidget *parent) : OSGShowWidget(parent)
{
    root= new osg::Group();
    //root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

    mainManipulator = new osgGA::TrackballManipulator;
    view->setCameraManipulator(mainManipulator);

    view->getCamera()->setClearColor(osg::Vec4(0.678431f, 0.847058f, 0.901961f, 1.0f));
}

OSG3DObjectShowWidget::~OSG3DObjectShowWidget()
{

}

void OSG3DObjectShowWidget::load3DObject(std::string filePath)
{
    osg::Node * node = osgDB::readNodeFile(filePath);
    root->addChild(node);
    view->setSceneData(root);
}

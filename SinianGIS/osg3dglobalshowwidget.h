#ifndef OSG3DGLOBALSHOWWIDGET_H
#define OSG3DGLOBALSHOWWIDGET_H

#include "osgshowwidget.h"
#include "sceneproject3d.h"

#include <osgViewer/CompositeViewer>
#include <osgEarthUtil/EarthManipulator>

class OSG3DGlobalShowWidget : public OSGShowWidget
{
    Q_OBJECT
public:
    explicit OSG3DGlobalShowWidget(QWidget *parent = nullptr);
    virtual ~OSG3DGlobalShowWidget();

    bool load3DProject(std::shared_ptr<SceneProject3D> project);

    void SetTerrainLayerViewPoint(std::string name);
    void SetNodeViewPoint(std::string name);

signals:

public slots:
    void slotViewPoint(std::string name);

protected:
    std::shared_ptr<SceneProject3D> sceneProject3D;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mainManipulator;

    bool CalViewPointGeoExtend(const osgEarth::GeoExtent& extent, std::shared_ptr<osgEarth::Viewpoint> out);
    bool CalViewPointNode(osg::ref_ptr<osg::Node> node, std::shared_ptr<osgEarth::Viewpoint> out);


};

#endif // OSG3DGLOBALSHOWWIDGET_H

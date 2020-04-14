#ifndef SCENEPROJECT3D_H
#define SCENEPROJECT3D_H

#include "sceneprojectbase.h"

#include <osg/Group>

#include <osgEarth/MapNode>
#include <osgEarth/Viewpoint>

class SceneProject3D : public SceneProjectBase
{
public:
    SceneProject3D();

    virtual void create() override;
    virtual void read(std::string path) override;
    virtual void write(std::string path) override;

    osg::ref_ptr<osg::Group> GetRootNode(){return root;}
    osgEarth::Viewpoint *GetHomeViewPoint(){return &homeViewPoint;}

    void AddArcGISImagery();
    void AddArcGISTerrainImagery();

    void AddBingImagery();
    void AddBingTerrain();

protected:
    void InitEarthMapNode();
    void InitViewPoint();

    void AddArcGISDrivers(std::string name, std::string url);

    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osgEarth::Map> map;
    osg::ref_ptr<osgEarth::MapNode> mapNode;

    osgEarth::Viewpoint homeViewPoint;
};

#endif // SCENEPROJECT3D_H

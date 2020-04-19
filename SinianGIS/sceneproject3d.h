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
    osg::ref_ptr<osgEarth::Map> GetMap(){return map;}
    QJsonArray& GetImageArray(){return localImageArray;}

    void AddLocalImage(std::string filePath);
    void AddLocalTerrain(std::string filePath);
    void AddPhotogrammetry();

    void AddArcGISImagery();
    void AddArcGISTerrainImagery();

    void AddBingImagery();
    void AddBingTerrain();

    void insertViewPoint(std::string name, std::shared_ptr<osgEarth::Viewpoint> vp);

    //
    QJsonArray localImageArray;
    QJsonArray localTerrainArray;

protected:
    void InitEarthMapNode();
    void InitViewPoint();

    void AddArcGISDrivers(std::string name, std::string url);
    bool CalViewPointGeoExtend(const osgEarth::GeoExtent& extent, std::shared_ptr<osgEarth::Viewpoint> out);

    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osgEarth::Map> map;
    osg::ref_ptr<osgEarth::MapNode> mapNode;

    osgEarth::Viewpoint homeViewPoint;
    std::map<std::string, std::shared_ptr<osgEarth::Viewpoint>> viewPointMap;
};

#endif // SCENEPROJECT3D_H

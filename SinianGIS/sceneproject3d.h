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
    osg::ref_ptr<osgEarth::MapNode> GetMapNode(){return mapNode;}
    QJsonArray& GetImageArray(){return localImageArray;}

    void AddLocalImage(std::string filePath);
    void AddLocalTerrain(std::string filePath);
    bool AddPhotogrammetry(std::string fileDir);

    void AddArcGISImagery();
    void AddArcGISTerrainImagery();

    void AddBingImagery();
    void AddBingTerrain();

    void insertViewPoint(std::string name, std::shared_ptr<osgEarth::Viewpoint> vp);
    std::shared_ptr<osgEarth::Viewpoint> getViewPoint(std::string name);

    void AddSkyBox();

    //
    QJsonArray localImageArray;
    QJsonArray localTerrainArray;
    QJsonArray obliquePhotographyArray;

//signals:

protected:
    void InitEarthMapNode();
    void InitViewPoint();

    void AddArcGISDrivers(std::string name, std::string url);
    bool CalViewPointGeoExtend(const osgEarth::GeoExtent& extent, std::shared_ptr<osgEarth::Viewpoint> out);

    void createObliqueIndexes(std::string fileDir);      //读取倾斜摄影数据的元数据
    bool readObliqueXML(std::string metaXML, double& x, double& y, double &z);      //读取倾斜摄影数据的元数据

    void ReadViewPoint();
    void SaveViewPoint();

    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osgEarth::Map> map;
    osg::ref_ptr<osgEarth::MapNode> mapNode;

    osgEarth::Viewpoint homeViewPoint;
    std::map<std::string, std::shared_ptr<osgEarth::Viewpoint>> viewPointMap;
};

#endif // SCENEPROJECT3D_H

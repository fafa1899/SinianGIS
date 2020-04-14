#include "sceneproject3d.h"

#include <string>

#include <osgEarth/ImageLayer>

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
//#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
//#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/bing/BingOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
//#include <osgEarthDrivers/tms/TMSOptions>
//#include <osgEarthDrivers/tilecache/TileCacheOptions>

#include <osgEarthUtil/EarthManipulator>
//#include <osgEarthUtil/ViewFitter>
//#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/ExampleResources>
//#include <osgEarthUtil/RTTPicker>
//#include <osgEarthUtil/Shadowing>

using namespace std;

SceneProject3D::SceneProject3D()
{
    root = new osg::Group();
    InitEarthMapNode();
    InitViewPoint();
}

void SceneProject3D::create()
{
}

void SceneProject3D::read(std::string path)
{
    this->projectFilePath = path;
}

void SceneProject3D::write(std::string path)
{
    this->projectFilePath = path;
}

void SceneProject3D::InitEarthMapNode()
{
    osgEarth::ProfileOptions profileOpts;
    //profileOpts.srsString() = wktString;//"EPSG:4547";
    //osgEarth::Bounds bs(535139, 3365107, 545139, 3375107);
    //osgEarth::Bounds bs(minX, minY, maxX, maxY);
    //profileOpts.bounds() = bs;

    //地图配置：设置缓存目录
    osgEarth::Drivers::FileSystemCacheOptions cacheOpts;
    string cacheDir = appDir + "/tmp";
    cacheOpts.rootPath() = cacheDir;

    osgEarth::MapOptions mapOpts;
    mapOpts.cache() = cacheOpts;
    //mapOpts.coordSysType() = MapOptions::CSTYPE_PROJECTED;

    mapOpts.profile() = profileOpts;

    //创建地图节点
    map = new osgEarth::Map(mapOpts);
    mapNode = new osgEarth::MapNode(map);
    root->addChild(mapNode);

    osgEarth::Drivers::GDALOptions gdal;
    gdal.url() = appDir + "/Resource/BlueMarbleNASA.jpg";
    osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("BlueMarble", gdal);
    map->addLayer(layer);
}

//设置初始视点
void SceneProject3D::InitViewPoint()
{
    double cx = 0;
    double cy = 0;
    map->getProfile()->getExtent().getCentroid(cx, cy);
    if (cx<abs(1e-6) && cy<abs(1e-6))                  //中心位置为0说明可能是全地图显示
    {
        cx = 112.637113;
        cy = 26.939324;
    }
    osgEarth::GeoPoint newPoint(map->getSRS(), cx, cy, 0);

    homeViewPoint.focalPoint() = newPoint;
    homeViewPoint.heading() = 0;
    homeViewPoint.pitch() = -90;
    homeViewPoint.range() = 20000000;
}

void SceneProject3D::AddArcGISDrivers(std::string name, std::string url)
{
    osgEarth::Drivers::ArcGISOptions imagery;
    imagery.url() = url;
    osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("ArcGISTerrainImagery", imagery);
    layer->options().cacheId() = name;
    layer->options().cachePolicy() = osgEarth::CachePolicy::USAGE_READ_WRITE;
    map->addLayer(layer);
}

void SceneProject3D::AddArcGISImagery()
{    
    AddArcGISDrivers("ArcGISImagery", "http://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer");
    //AddArcGISDrivers("arcgis-reference-overlay", "http://services.arcgisonline.com/ArcGIS/rest/services/Reference/World_Boundaries_and_Places_Alternate/MapServer");
}

void SceneProject3D::AddArcGISTerrainImagery()
{
    AddArcGISDrivers("ArcGISTerrainImagery", "http://services.arcgisonline.com/arcgis/rest/services/World_Terrain_Base/MapServer");
}

void SceneProject3D::AddBingImagery()
{
    osgEarth::Drivers::BingOptions bing;
    bing.apiKey() = "As--nt_Uo7NaGON-IJ0jYcW3LmKsx2BEChizSNA7FRXF8I_Z1GJ37-3CS2qzyiyD";
    bing.imagerySet() = "Aerial";
    osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("BingImagery", bing);
    layer->options().cacheId() = "BingImagery";
    layer->options().cachePolicy() = osgEarth::CachePolicy::USAGE_READ_WRITE;
    map->addLayer(layer);
}

void SceneProject3D::AddBingTerrain()
{
    osgEarth::Drivers::BingOptions bing;
    bing.apiKey() = "As--nt_Uo7NaGON-IJ0jYcW3LmKsx2BEChizSNA7FRXF8I_Z1GJ37-3CS2qzyiyD";

    osg::ref_ptr<osgEarth::ElevationLayer> layer = new osgEarth::ElevationLayer("BingTerrain", bing);
    layer->options().cacheId() = "BingTerrain";
    layer->options().cachePolicy() = osgEarth::CachePolicy::USAGE_READ_WRITE;
    map->addLayer(layer);
}

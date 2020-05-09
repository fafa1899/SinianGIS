#include "sceneproject3d.h"
#include "pathref.hpp"
#include "tinyxml/tinyxml.h"
#include "StdStringEx.hpp"

#include <QDebug>
#include <QMessageBox>
#include <string>
#include <direct.h>

#include <osg/Program>
#include <osgDB/WriteFile>

#include <osgEarth/ImageLayer>
#include <osgEarth/ModelLayer>
#include <osgEarth/SpatialReference>
#include <osgEarth/GeoTransform>

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <osgEarthDrivers/bing/BingOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/sky_simple/SimpleSkyOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
//#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>

//#include <osgEarthDrivers/tms/TMSOptions>
//#include <osgEarthDrivers/tilecache/TileCacheOptions>

#include <osgEarthFeatures/FeatureSourceLayer>
#include <osgEarthFeatures/FeatureModelLayer>

#include <osgEarthUtil/EarthManipulator>
//#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/ExampleResources>
//#include <osgEarthUtil/RTTPicker>
//#include <osgEarthUtil/Shadowing>

using namespace std;

SceneProject3D::SceneProject3D()
{
    root = new osg::Group();
    InitEarthMapNode();
    AddSkyBox();
    InitViewPoint();
}

void SceneProject3D::create()
{
}

void SceneProject3D::read(std::string path)
{
    this->projectFilePath = path;

    SceneProjectBase::read(path);

    if (projectJSON.contains("LocalImage"))
    {
        QJsonValue key_value = projectJSON.take("LocalImage");
        if (key_value.isArray())
        {
            QJsonArray array = key_value.toArray();
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue value = array.at(i);
                if (value.isString())
                {
                    QByteArray v = value.toString().toLocal8Bit();
                    AddLocalImage(v.data());
                }
            }
        }
    }

    if (projectJSON.contains("LocalTerrain"))
    {
        QJsonValue key_value = projectJSON.take("LocalTerrain");
        if (key_value.isArray())
        {
            QJsonArray array = key_value.toArray();
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue value = array.at(i);
                if (value.isString())
                {
                    QByteArray v = value.toString().toLocal8Bit();
                    AddLocalTerrain(v.data());
                }
            }
        }
    }

    if(projectJSON.contains("ObliquePhotography"))
    {
        QJsonValue key_value = projectJSON.take("ObliquePhotography");
        if (key_value.isArray())
        {
            QJsonArray array = key_value.toArray();
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue value = array.at(i);
                if (value.isString())
                {
                    QByteArray v = value.toString().toLocal8Bit();
                    AddPhotogrammetry(v.data());
                }
            }
        }
    }

    if(projectJSON.contains("LocalVector"))
    {
        QJsonValue key_value = projectJSON.take("LocalVector");
        if (key_value.isArray())
        {
            QJsonArray array = key_value.toArray();
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue value = array.at(i);
                if (value.isString())
                {
                    QByteArray v = value.toString().toLocal8Bit();
                    AddLocalVector(v.data());
                }
            }
        }
    }

    if(projectJSON.contains("ViewPoints"))
    {
        ReadViewPoint();
    }
}

void SceneProject3D::ReadViewPoint()
{
    QJsonValue viewPointJSON = projectJSON.take("ViewPoints");
    if(viewPointJSON.isArray())
    {
        QJsonArray array = viewPointJSON.toArray();
        for (int i = 0; i < array.size(); i++)
        {
            QJsonValue value = array.at(i);
            if (value.isObject())
            {
                QJsonObject object = value.toObject();
                QJsonValue name = object.value(QString::fromLocal8Bit("name"));
                if(!name.isString())
                {
                    continue;
                }

                std::shared_ptr<osgEarth::Viewpoint> oe_VP =  std::make_shared<osgEarth::Viewpoint>();
                QJsonValue vp = object.value("vp");
                if(vp.isObject())
                {
                    QJsonObject k_v = vp.toObject();
                    QJsonValue focalPointX = k_v.value(QString::fromLocal8Bit("focalPointX"));
                    QJsonValue focalPointY = k_v.value(QString::fromLocal8Bit("focalPointY"));
                    QJsonValue focalPointZ = k_v.value(QString::fromLocal8Bit("focalPointZ"));
                    QJsonValue heading = k_v.value(QString::fromLocal8Bit("heading"));
                    QJsonValue pitch = k_v.value(QString::fromLocal8Bit("pitch"));
                    QJsonValue range = k_v.value(QString::fromLocal8Bit("range"));


                    osgEarth::GeoPoint focalPoint(map->getSRS(), focalPointX.toDouble(), focalPointY.toDouble(), focalPointZ.toDouble());
                    oe_VP->focalPoint() = focalPoint;
                    oe_VP->heading() = heading.toDouble();
                    oe_VP->pitch() = pitch.toDouble();
                    oe_VP->range() = range.toDouble();
                }

                QByteArray N = name.toString().toLocal8Bit();
                viewPointMap.insert(make_pair(N.data(),oe_VP));
            }
        }
    }
}

void SceneProject3D::write(std::string path)
{
    this->projectFilePath = path;

    projectJSON.insert("LocalImage", localImageArray);
    projectJSON.insert("LocalTerrain", localTerrainArray);
    projectJSON.insert("ObliquePhotography", obliquePhotographyArray);
    projectJSON.insert("LocalVector", localVectorArray);

    SaveViewPoint();

    SceneProjectBase::write(path);
}

void SceneProject3D::SaveViewPoint()
{
    QJsonArray viewPointJSON;
    for(auto &it : viewPointMap)
    {
        QJsonObject vp;
        QJsonValue nameValue(QString::fromLocal8Bit(it.first.c_str()));
        vp.insert("name", nameValue);

        QJsonObject key_value;
        key_value.insert("focalPointX", it.second->focalPoint()->x());
        key_value.insert("focalPointY", it.second->focalPoint()->y());
        key_value.insert("focalPointZ", it.second->focalPoint()->z());
        key_value.insert("heading", it.second->heading()->getValue());
        key_value.insert("pitch", it.second->pitch()->getValue());
        key_value.insert("range", it.second->range()->getValue());
        vp.insert("vp", key_value);

        viewPointJSON.push_back(vp);
    }

    projectJSON.insert("ViewPoints", viewPointJSON);
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
    //root->addChild(mapNode);

    osgEarth::Drivers::GDALOptions gdal;
    //gdal.url() = appDir + "/Resource/BlueMarbleNASA.jpg";
    //gdal.url() = appDir + "/Resource/World_e-Atlas_Bright-BMNG-200404.tiff";
    gdal.url() = appDir + "/Resource/baseMap.jpg";
    osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("BlueMarble", gdal);
    map->addLayer(layer);
}

void SceneProject3D::AddSkyBox()
{
    //
    osgEarth::SimpleSky::SimpleSkyOptions options;
    options.atmosphereVisible() = true;
    options.atmosphericLighting() = false;
    options.sunVisible() = true;
    options.moonVisible() = true;
    options.starsVisible() = true;
    string moonImage = PathRef::GetAppDir() + "/Resource/moon_1024x512.jpg";
    options.moonImageURI() = moonImage;
    //options.ambient() = 0.25f;

    //
    osg::ref_ptr<osgEarth::SimpleSky::SkyNode> skyNode = osgEarth::SimpleSky::SkyNode::create(options, mapNode);
    root->addChild(skyNode);
    skyNode->addChild(mapNode);

    osgEarth::DateTime d = skyNode->getDateTime();
    skyNode->setDateTime(osgEarth::DateTime(d.year(), d.month(), d.day(), 4.0)); // 格林尼治，时差8小时
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

void SceneProject3D::AddLocalImage(string filePath)
{
    osgEarth::Drivers::GDALOptions gdal;
    gdal.url() = filePath;
    osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(filePath, gdal);
    map->addLayer(layer);

    QJsonValue value(QString::fromLocal8Bit(filePath.c_str()));
    localImageArray.push_back(value);
}

void SceneProject3D::AddLocalTerrain(std::string filePath)
{
    osgEarth::Drivers::GDALOptions gdal;
    gdal.url() = filePath;
    osg::ref_ptr<osgEarth::ElevationLayer> layer = new osgEarth::ElevationLayer(filePath, gdal);
    map->addLayer(layer);

    QJsonValue value(QString::fromLocal8Bit(filePath.c_str()));
    localTerrainArray.push_back(value);
}

//加入矢量数据
bool SceneProject3D::AddLocalVector(std::string filePath)
{
    //
    osgEarth::Drivers::OGRFeatureOptions featureData;
    featureData.url() = filePath;

    //    ifstream infile("C:/Data/vector/hs/23.prj");
    //    string line;
    //    getline(infile, line);
    //    featureData.profile()->srsString() = line;

    // Make a feature source layer and add it to the Map:
    osgEarth::Features::FeatureSourceLayerOptions ogrLayer;
    ogrLayer.name() = filePath + "_source";
    ogrLayer.featureSource() = featureData;
    osgEarth::Features::FeatureSourceLayer*  featureSourceLayer = new osgEarth::Features::FeatureSourceLayer(ogrLayer);
    map->addLayer(featureSourceLayer);
    osgEarth::Features::FeatureSource *features = featureSourceLayer->getFeatureSource();
    if (!features)
    {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("无法打开该矢量文件！"), QMessageBox::Ok);
        return false;
    }

    osgEarth::Symbology::Geometry::Type type = features->getGeometryType();
    if (osgEarth::Symbology::Geometry::TYPE_UNKNOWN == type)
    {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("该矢量文件的类型未知，或者含有多种类型的特征。"), QMessageBox::Ok);
        return false;
    }

    osgEarth::Symbology::Style style;

    //高度设置
    osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
    int altMode = 1;
    switch (altMode)
    {
    case 2:
        alt->clamping() = alt->CLAMP_RELATIVE_TO_TERRAIN;
        break;
    case 3:
        alt->clamping() = alt->CLAMP_ABSOLUTE;
        break;
    case 1:
    default:
        alt->clamping() = alt->CLAMP_TO_TERRAIN;
        alt->technique() = alt->TECHNIQUE_DRAPE;
        break;
    }

    if(osgEarth::Symbology::Geometry::TYPE_POINTSET == type)
    {
        osgEarth::Symbology::PointSymbol *ps = style.getOrCreateSymbol<osgEarth::Symbology::PointSymbol>();
        ps->size() = 5;
        ps->fill()->color() = osgEarth::Symbology::Color("#EE6363");
        ps->smooth() = true;
    }
    else if(osgEarth::Symbology::Geometry::TYPE_LINESTRING == type || osgEarth::Symbology::Geometry::TYPE_RING == type)
    {
        osgEarth::Symbology::LineSymbol* ls = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
        ls->stroke()->color() = osgEarth::Symbology::Color("#436EEE");
        ls->stroke()->width() = 1.0;
        ls->tessellationSize()->set(100, osgEarth::Units::KILOMETERS);
    }
    else if(osgEarth::Symbology::Geometry::TYPE_POLYGON == type)
    {
        osgEarth::Symbology::LineSymbol* ls = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
        ls->stroke()->color() = osgEarth::Symbology::Color("#9370DB");
        ls->stroke()->width() = 1.0;
        ls->tessellationSize()->set(100, osgEarth::Units::KILOMETERS);

        osgEarth::Symbology::PolygonSymbol *polygonSymbol = style.getOrCreateSymbol<osgEarth::Symbology::PolygonSymbol>();
        polygonSymbol->fill()->color() = osgEarth::Symbology::Color(152.0f/255, 251.0f/255, 152.0f/255, 0.5f); //238 230 133
        polygonSymbol->outline() = true;
    }

    //可见性
    osgEarth::Symbology::RenderSymbol* rs = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
    rs->depthTest() = false;

    //
    osgEarth::Features::FeatureModelLayerOptions fmlOpt;
    fmlOpt.name() = filePath;
    fmlOpt.featureSourceLayer() = filePath + "_source";
    fmlOpt.enableLighting() = false;
    fmlOpt.styles() = new osgEarth::Symbology::StyleSheet();
    fmlOpt.styles()->addStyle(style);

    osg::ref_ptr<osgEarth::Features::FeatureModelLayer> fml = new osgEarth::Features::FeatureModelLayer(fmlOpt);
    map->addLayer(fml);

    /*
    if (!config.fieldName.empty())
    {
        Style labelStyle;

        TextSymbol* text = labelStyle.getOrCreateSymbol<TextSymbol>();
        //string name = QString::fromLocal8Bit("[座落地址]").toUtf8().data();
        //string name = "[宗地代码]";
        //string name = "[Name]";
        string name = string("[") + QString::fromLocal8Bit(config.fieldName.c_str()).toUtf8().data() + "]";
        text->content() = StringExpression(name);
        //text->priority() = NumericExpression( "[pop_cntry]" );
        text->size() = 16.0f;
        text->alignment() = TextSymbol::ALIGN_CENTER_CENTER;
        text->fill()->color() = Color::White;
        text->halo()->color() = Color::Red;
        text->encoding() = TextSymbol::ENCODING_UTF8;
        string fontFile = PathRef::GetAppDir() + "/fonts/SourceHanSansCN-Regular.ttf";
        text->font() = fontFile;

        // and configure a model layer:
        FeatureModelLayerOptions fmlOpt;
        fmlOpt.name() = config.name + "_labels";
        fmlOpt.featureSourceLayer() = config.name + "_source";
        fmlOpt.styles() = new StyleSheet();
        fmlOpt.styles()->addStyle(labelStyle);

        ref_ptr<FeatureModelLayer> fml = new FeatureModelLayer(fmlOpt);
        //fml->setFeatureSource(features);
        earthMap->addLayer(fml);
    }*/

    /*
    Style newStyle;
    newStyle = *style;
    newStyle.setName("test");
    //styleSheet->addStyle(newStyle);

    LineSymbol* ls = newStyle.getOrCreateSymbol<LineSymbol>();
    ls->stroke()->color() = Color::Yellow;
    ls->stroke()->width() = 1.0f;
    ls->tessellationSize()->set(100, Units::KILOMETERS);

    StyleSelector styleSelector;
    styleSelector.styleName() = "test";
    styleSelector.query()->expression() = "fid < 50";
    //styleSheet->selectors().push_back(styleSelector);

    //fmlOpt.styles()->addStyle(newStyle);
    //fmlOpt.styles()->selectors().push_back(styleSelector);
     */

    QJsonValue value(QString::fromLocal8Bit(filePath.c_str()));
    localVectorArray.push_back(value);

    return true;
}

//读取3d场景的元数据
bool SceneProject3D::readObliqueXML(std::string metaXML, double& x, double& y,  double &z)
{
    string strSRS;
    string strSRSOrigin;

    //打开XML工程文档
    TiXmlDocument myDocument(metaXML);
    if (!myDocument.LoadFile())
    {
        QString str = QString::fromLocal8Bit("不能打开XML文件：%1\n").arg(metaXML.c_str());
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("警告"), str);
        return false;
    }

    //根节点
    TiXmlElement *RootElement = myDocument.RootElement();
    TiXmlElement *ChildElement = RootElement->FirstChildElement();
    while(ChildElement)
    {
        if (_stricmp(ChildElement->Value(), "SRS") == 0)
        {
            strSRS = ChildElement->GetText();
        }
        if(_stricmp(ChildElement->Value(), "SRSOrigin") == 0)
        {
            strSRSOrigin = ChildElement->GetText();
        }

        ChildElement = ChildElement->NextSiblingElement();
    }

    if(strSRS.length()<4)
    {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("坐标参考不正确！"), QMessageBox::Ok);
        return false;
    }

    const osgEarth::SpatialReference* srs = osgEarth::SpatialReference::get(strSRS);
    if(srs)             //如果是正确的坐标系
    {
        vector<string> subline;
        StdStringEx::CutString(strSRSOrigin, subline, ',');
        if(subline.size() != 3)
        {
            QMessageBox::critical(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("strSRSOrigin格式不正确！"), QMessageBox::Ok);
            return false;
        }

        x = atof(subline[0].c_str());
        y = atof(subline[1].c_str());
        z = atof(subline[2].c_str());

        //判断是否相等
        if(!map->getSRS()->isEquivalentTo(srs))
        {
            osgEarth::GeoPoint point(srs, x, y, z);
            osgEarth::GeoPoint newPoint;
            if(!point.transform(map->getSRS(), newPoint))
            {
                QMessageBox::critical(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("不支持的空间参考转换！"), QMessageBox::Ok);
                return false;
            }
            x = newPoint.x();
            y = newPoint.y();
            z = newPoint.z();
        }
    }
    else
    {
        string tmp = strSRS.substr(0, 4);
        if(_stricmp(tmp.c_str(), "ENU:") != 0)
        {
            QMessageBox::critical(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("坐标参考不一致！"), QMessageBox::Ok);
            return false;
        }

        strSRS = strSRS.substr(4);
        vector<string> subline;
        StdStringEx::CutString(strSRS, subline, ',');
        if(subline.size() != 2)
        {
            QMessageBox::critical(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("坐标参考格式不正确！"), QMessageBox::Ok);
            return false;
        }

        y = atof(subline[0].c_str());
        x = atof(subline[1].c_str());
    }

    return true;
}

void SceneProject3D::createObliqueIndexes(std::string fileDir)
{
    string dataDir = fileDir + "/Data";

    osg::ref_ptr<osg::Group> group = new osg::Group();
    vector<string> subDirs;
    PathRef::findDir(dataDir, subDirs);

    for(size_t i = 0; i<subDirs.size(); i++)
    {
        string name = PathRef::DirOrPathGetName(subDirs[i]);
        string path = subDirs[i] + "/" + name + ".osgb";

        osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);
        osg::ref_ptr<osg::PagedLOD> lod = new osg::PagedLOD();

        auto bs = node->getBound();
        auto c = bs.center();
        auto r = bs.radius();
        lod->setCenter(c);
        lod->setRadius(r);
        lod->setRangeMode(osg::LOD::RangeMode::PIXEL_SIZE_ON_SCREEN);
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->getOrCreateStateSet();
        lod->addChild(geode.get());

        std::string relativeFilePath = "./Data/" + name + "/" + name + ".osgb";  //相对路径

        lod->setFileName(0, "");
        lod->setFileName(1, relativeFilePath);

        lod->setRange(0, 0, 1.0);																							//第一层不可见
        lod->setRange(1, 1.0, FLT_MAX);

        lod->setDatabasePath("");

        group->addChild(lod);
    }
    std::string outputLodFile = fileDir + "/Data.osgb";
    osgDB::writeNodeFile(*group, outputLodFile);
}

bool SceneProject3D::AddPhotogrammetry(std::string fileDir)
{
    string obliqueIndexesFile = fileDir + "/Data.osgb";
    if (!PathRef::isDirExist(obliqueIndexesFile))
    {
        createObliqueIndexes(fileDir);
    }

    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(obliqueIndexesFile);
    if(!node)
    {
        return false;
    }

    //
    string xmlPath = fileDir + "/metadata.xml";
    double cx = 0;
    double cy = 0;
    double cz = 0;
    if(!readObliqueXML(xmlPath, cx, cy, cz))
    {
        return false;
    }

    //
    osg::ref_ptr<osgEarth::GeoTransform> xform = new osgEarth::GeoTransform();
    xform->addChild(node);
    xform->setTerrain(mapNode->getTerrain());
    osgEarth::GeoPoint point(map->getSRS(), cx, cy, cz);                 //使用绝对高，正高
    xform->setPosition(point);

    //
    osg::ref_ptr<osgEarth::ModelLayer> layer = new osgEarth::ModelLayer(fileDir, xform);
    map->addLayer(layer);

    const char * vertexShader = {
        "void main(void ){\n"
        "   gl_TexCoord[0] = gl_MultiTexCoord0;\n"
        "   gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;\n"
        "}\n"
    };

    const char * fragShader = {
        "uniform sampler2D baseTex;\n"
        "void main(void){\n"
        "   vec2 coord = gl_TexCoord[0].xy;\n"
        "   vec4 C = texture2D(baseTex, coord)\n;"
        "	gl_FragColor = C;\n"
        "}\n"
    };

    osg::ref_ptr<osg::Program> program = new osg::Program();
    program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragShader));
    program->addShader(new osg::Shader(osg::Shader::VERTEX, vertexShader));
    osg::StateSet * ss = node->getOrCreateStateSet();

    ss->setAttributeAndModes(program, osg::StateAttribute::ON);

    //osg::StateAttribute* attr = ss->getAttribute(osg::StateAttribute::PROGRAM);
    //cout<<program->getType()<<endl;
    //ss->removeAttribute(attr);

    QJsonValue value(QString::fromLocal8Bit(fileDir.c_str()));
    obliquePhotographyArray.push_back(value);

    return true;
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

void SceneProject3D::insertViewPoint(std::string name, std::shared_ptr<osgEarth::Viewpoint> vp)
{
    viewPointMap[name] = vp;
}

std::shared_ptr<osgEarth::Viewpoint> SceneProject3D::getViewPoint(std::string name)
{
    auto iter = viewPointMap.find(name);
    if(iter == viewPointMap.end())
    {
        return nullptr;
    }
    return iter->second;
}

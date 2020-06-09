# SinianGIS
个人学习作品：一个三维数字地球软件。

# 环境依赖
该项目全部基于开源项目开发：

1. 图标素材FlatWoken: https://github.com/alecive/FlatWoken
2. 界面库Qt: https://www.qt.io/
3. OpenSceneGraph: https://github.com/openscenegraph/OpenSceneGraph
4. osgEarth: https://github.com/gwaldron/osgearth
5. osgQt: https://github.com/openscenegraph/osgQt
6. XML数据格式读写库：tinyXML

# 环境搭建
1. 安装Visual Studio 2017。
2. 安装Qt环境。
3. 编译GEOS：[VS2015编译GEOS的debug和release版本](https://blog.csdn.net/charlee44/article/details/86744035)。
4. 编译GDAL：[Win64下编译集成GEOS和Proj4的GDAL](https://blog.csdn.net/charlee44/article/details/86749175)。
5. KML支持：[GDAL集成对KML文件的支持](https://blog.csdn.net/charlee44/article/details/102731757)。
6. 编译OpenSceneGraph：[Win64下编译OSG详细过程（Win10+VS2015+OSG3.6.3）](https://blog.csdn.net/charlee44/article/details/86761461)。
7. 编译osgEarth：[Windows下OSGEarth的编译过程](https://blog.csdn.net/charlee44/article/details/94299746)。
8. 编译osgQt: [OSG嵌入QT的简明总结](https://blog.csdn.net/charlee44/article/details/92400068)。

# 目前特性
1.主界面显示了一个数字地球      
![imglink1]

2.加载显示本地的地形、影像数据
![imglink2]

3.加载显示倾斜摄影模型
![imglink3]

4.加载显示矢量数据
![imglink4]

5.下载谷歌地图影像瓦片数据
![imglink5]
![imglink6]

6.ArcGIS、bing在线数据
   
7.数据保存为json工程文件，方便下次加载。

# 计划特性
1. 在线地图支持
2. 分屏显示不同工程数据
3. 支持二维数据工程
4. 支持二三维数据联动
5. 三维卷帘与二维卷帘

# update预告
1. 文件关联
2. 图层可见性调整

[imglink1]:https://raw.githubusercontent.com/fafa1899/SinianGIS/master/README/1.PNG
[imglink2]:https://raw.githubusercontent.com/fafa1899/SinianGIS/master/README/2.PNG
[imglink3]:https://raw.githubusercontent.com/fafa1899/SinianGIS/master/README/3.PNG
[imglink4]:https://raw.githubusercontent.com/fafa1899/SinianGIS/master/README/4.PNG
[imglink5]:https://raw.githubusercontent.com/fafa1899/SinianGIS/master/README/5.PNG
[imglink6]:https://raw.githubusercontent.com/fafa1899/SinianGIS/master/README/6.PNG
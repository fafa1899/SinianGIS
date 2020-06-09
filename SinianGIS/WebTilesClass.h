#pragma once

#include <vector>


struct OneLevel 
{
	int tilesNumX;
	int tilesNumY;
	int pixelSizeX;
	int pixelSizeY;
	double dX;
	double dY;
};

struct OneTile 
{
	size_t x;
	size_t y;
	int z;
	std::string uri;
};

class WebTilesClass
{
public:
	WebTilesClass();
	virtual ~WebTilesClass();

	virtual bool  GetTiles(double left, double top, double right, double bottom, int level, std::vector<OneTile> &tiles);

	virtual bool WriteGeoInfo(std::string imgName, OneTile& tile);
	
protected:	
	//地图左上角的地理起始位置，位于左上角
	double geoStartX;		//起始位置X		
	double geoStartY;		//起始位置Y
	double geoEndX;			//终点位置X
	double geoEndY;			//终点位置Y

	int tilePixelSizeX;
	int tilePixelSizeY;

	int levelNum;
	std::vector<OneLevel> levelList;	
};

int DownloadTiles(std::string address, std::string outfile, std::string proxy = "");

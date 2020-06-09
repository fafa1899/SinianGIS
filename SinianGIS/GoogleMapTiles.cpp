#include "GoogleMapTiles.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

#include <ogr_spatialref.h>

using namespace std;

GoogleMapTiles::GoogleMapTiles()
{
	const double radius = 6378137;
	const double PI = 3.14159265358979323846;
	double halfSide = radius * PI;

	geoStartX = -halfSide;
	geoStartY = -halfSide;
	geoEndX = halfSide;
	geoEndY = halfSide;

	tilePixelSizeX = 256;
	tilePixelSizeY = 256;

	levelNum = 22;
	for (int i = 0; i < levelNum; i++)
	{
		OneLevel oneLevel;
		oneLevel.tilesNumX = (int)pow(2, i);
		oneLevel.tilesNumY = (int)pow(2, i);
		oneLevel.pixelSizeX = oneLevel.tilesNumX * tilePixelSizeX;
		oneLevel.pixelSizeY = oneLevel.tilesNumY * tilePixelSizeY;
		oneLevel.dX = (geoEndX - geoStartX) / oneLevel.pixelSizeX;
		oneLevel.dY = (geoEndY - geoStartY) / oneLevel.pixelSizeY;
		levelList.push_back(oneLevel);
	}
}

GoogleMapTiles::~GoogleMapTiles()
{	
}

bool GoogleMapTiles::GetTiles(double left, double top, double right, double bottom, int li, vector<OneTile> &tiles)
{
	static int id = 0;
	if (id > 3)
	{
		id = 0;
	}
	uri = "http://khms" + to_string(id) + ".google.com/kh/v=865?x=%zu&y=%zu&z=%d";
	id++;

	double xArray[4] = { left, left, right, right };
	double yArray[4] = { top, bottom, bottom, top };

	OGRSpatialReference webMercator;
	webMercator.importFromEPSG(3857);

	OGRSpatialReference wgs84;
	wgs84.importFromEPSG(4326);

	OGRCoordinateTransformation* LonLat2XY = OGRCreateCoordinateTransformation(&wgs84, &webMercator);
	if (!LonLat2XY)
	{
		printf("GDAL Error!");
		return false;
	}

	if (!LonLat2XY->Transform(4, xArray, yArray))
	{
		printf("GDAL Error!");
		return false;
	}
		   
	OGRCoordinateTransformation::DestroyCT(LonLat2XY);
	LonLat2XY = nullptr;
	 	
	double minX = geoEndX + 10000;
	double minY = geoEndY + 10000;
	double maxX = geoStartX - 10000;
	double maxY = geoStartY - 10000;
	for (int i = 0; i < 4; i++)
	{
		minX = (std::min)(minX, xArray[i]);
		minY = (std::min)(minY, yArray[i]);
		maxX = (std::max)(maxX, xArray[i]);
		maxY = (std::max)(maxY, yArray[i]);
	}
	   	
	double pixelStartX = (minX - geoStartX) / levelList[li].dX;
	double pixelStartY = (minY - geoStartY) / levelList[li].dY;	
	double pixelEndX = (maxX - geoStartX) / levelList[li].dX;
	double pixelEndY = (maxY - geoStartY) / levelList[li].dY;
		   
	size_t sx = (int)(pixelStartX / tilePixelSizeX);
	size_t sy = (int)(pixelStartY / tilePixelSizeY);
	size_t ex = (int)(pixelEndX / tilePixelSizeX);
	size_t ey = (int)(pixelEndY / tilePixelSizeY);
	   
	sy = levelList[li].tilesNumY - 1 - sy;
	ey = levelList[li].tilesNumY - 1 - ey;
	std::swap(sy, ey);
	
	for (size_t yi = sy; yi <= ey; yi++)
	{
		for (size_t xi = sx; xi <= ex; xi++)
		{
			char address[512] = "";
			sprintf_s(address, 512, uri.c_str(), xi, yi, li);
			
			OneTile tile;
			tile.uri = address;
			tile.x = xi;
			tile.y = yi;
			tile.z = li;
			tiles.push_back(tile);
		}
	}

	return true;
}

bool GoogleMapTiles::WriteGeoInfo(string imgName, OneTile& tile)
{
	//
	string spRefFile = imgName + ".prj";
	ofstream outfile(spRefFile);
	if (!outfile)
	{
		printf("Can't Open %s\n", spRefFile.c_str());
		return false;
	}

	OGRSpatialReference webMercator;
	webMercator.importFromEPSG(3857);

	char *pszWKT = nullptr;
	if (OGRERR_NONE != webMercator.exportToWkt(&pszWKT))
	{
		printf("Get Spatial Reference Error!\n");
		return false;
	}

	outfile << pszWKT;

	CPLFree(pszWKT);
	pszWKT = nullptr;
	outfile.close();

	//
	int tile_x = tile.x;
	int tile_y = levelList[tile.z].tilesNumY - 1 - tile.y;
	int pixel_x = tile_x * tilePixelSizeX;
	int pixel_y = tile_y * tilePixelSizeY;
	
	double dx = levelList[tile.z].dX;
	double dy = -levelList[tile.z].dY;
	double tileStartX = geoStartX + pixel_x * levelList[tile.z].dX + 0.5 * levelList[tile.z].dX;
	double tileStartY = geoStartY + pixel_y * levelList[tile.z].dY + (tilePixelSizeY - 0.5)* levelList[tile.z].dY;

	string tfwFile = imgName + ".jgw";
	FILE *pFile = nullptr;
	if (fopen_s(&pFile, tfwFile.c_str(), "wt") != 0)
	{
		printf("Can't Create JGW:%s", tfwFile.c_str());
		return false;
	}

	fprintf(pFile, "%.10lf\n", dx);
	fprintf(pFile, "%.10lf\n", 0.0);
	fprintf(pFile, "%.10lf\n", 0.0);
	fprintf(pFile, "%.10lf\n", dy);
	fprintf(pFile, "%.10lf\n", tileStartX);
	fprintf(pFile, "%.10lf\n", tileStartY);

	fclose(pFile);

	return true;
}

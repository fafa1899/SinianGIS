#pragma once
#include "WebTilesClass.h"

class GoogleMapTiles :
	public WebTilesClass
{
public:
	GoogleMapTiles();
	virtual ~GoogleMapTiles();

	virtual bool  GetTiles(double left, double top, double right, double bottom, int li, std::vector<OneTile> &tiles);

	virtual bool WriteGeoInfo(std::string imgName, OneTile& tile);
	
protected:
	std::string uri;
};


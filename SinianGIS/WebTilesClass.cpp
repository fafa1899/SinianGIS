#include "WebTilesClass.h"

#include <curl/curl.h>
#include <iostream>
#include <string>

using namespace std;

WebTilesClass::WebTilesClass()
{
}

WebTilesClass::~WebTilesClass()
{	
}

bool  WebTilesClass::GetTiles(double left, double top, double right, double bottom, int level, vector<OneTile> &tiles)
{
	return false;
}

bool WebTilesClass::WriteGeoInfo(string imgName, OneTile& tile)
{
	return false;
}

int DownloadTiles(std::string address, std::string outfile, std::string proxy)
{
	//cout << address << endl;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl = curl_easy_init();

	//设置代理
	if (!proxy.empty()) 
	{
		curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
	}

	curl_easy_setopt(curl, CURLOPT_URL, address.c_str());

	//设置useragent
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");
	
	FILE *fp = nullptr;
	if (fopen_s(&fp, outfile.c_str(), "wb") != 0)
	{
		curl_easy_cleanup(curl);
		return CURL_LAST;
	}

	//CURLOPT_WRITEFUNCTION 将后继的动作交给write_data函数处理
	//std::cout << curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data) << std::endl;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	//curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
	//curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
	////curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, nullptr);

	CURLcode error =  curl_easy_perform(curl);

	fclose(fp);  
	curl_easy_cleanup(curl);

	return error;
}


#pragma once

#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;

//std::string的强化函数类
class StdStringEx
{
public:
	//根据字符切分string，兼容最前最后存在字符
	static void CutString(string line, vector<string> &subline, char a)
	{
		//首字母为a，剔除首字母
		if (line.size() < 1)
		{
			return;
		}
		if (line[0] == a)
		{
			line.erase(0, 1);
		}

		size_t pos = 0;
		while (pos < line.length())
		{
			size_t curpos = pos;
			pos = line.find(a, curpos);
			if (pos == string::npos)
			{
				pos = line.length();
			}
			subline.push_back(line.substr(curpos, pos - curpos));
			pos++;
		}

		return;
	}

	//根据空截断字符串
	static void ChopStringLineEx(string line, vector<string> &substring)
	{
		std::stringstream linestream(line);
		string sub;

		while (linestream >> sub)
		{
			substring.push_back(sub);
		}
	}


};


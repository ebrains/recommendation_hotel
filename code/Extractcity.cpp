// Extractcity.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main()
{
	ifstream fin("E://dataset/hotel_price_location");
	if (!fin)
		return 0;//打开文件失败

	int k = 0;
	int count = 0;
	int citynum = 0;
	string line;
	string cityname[20000];
	while (getline(fin, line)){//读取一行
		if (true)
		{
			string s1, s2, s3;

			fin >> s1 >> s2 >> s3;
			
			cityname[k] = s3;
			k = k + 1;
			count += 1;
			
		}
	}

	map<string, int>city;
	for (int i = 0; i < count; i++)
	{
		if (city.find(cityname[i]) == city.end())
		{
			city[cityname[i]] = 1;
		}
		else
		{
			city[cityname[i]]++;
		}
	}

	int citycount = 0;

	for (map<string, int>::iterator it = city.begin(); it != city.end(); it++)
	{
		cout <<"cityname:  "<< it->first << "  " <<"number:  "<< it->second << endl;
		citycount += 1;
	}
	
	cout << "citynumber: " << citycount << endl;


	fin.close();

	return 0;
}
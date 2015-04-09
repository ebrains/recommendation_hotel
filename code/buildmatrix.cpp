//
//  main.cpp
//  readfile
//
//  Created by Yanqiao Zhan on 4/2/15.
//  Copyright (c) 2015 Yanqiao Zhan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <dirent.h>
#include <cmath>
using namespace std;

unordered_map<string,vector<pair<int,string>>>ranking;//the key is user, the value is hotels that this user has rated.
unordered_map<string, int>title;//Map hotel to 2d matrix index.
unordered_map<string, vector<pair<double,string>>>cityhotel;//key is city, value is hotel's price and hotel's name

void readByLine(string filename,int titleindex)//
{
    ifstream fin("./Review_Texts/"+filename);
    string s;
    string authorschema="<Author>";
    string valueschema="<Overall>";
    string author;
    string hotelname=filename.substr(filename.find("_")+1,filename.find(".")-filename.find("_")-1);
    title[hotelname]=titleindex;
    int overall;
    while( getline(fin,s) )
    {
        if(s.substr(0,authorschema.size())==authorschema)
        {
            author=s.substr(authorschema.size(),s.size()-authorschema.size()-1);
        }
        else if(s.substr(0,valueschema.size())==valueschema)
        {
            overall=atoi(s.substr(valueschema.size()).c_str());
            ranking[author].push_back(pair<int,string>(overall,hotelname));
        }
    }
}
void listallfile()//only find hotels in the given city
{
    DIR *dir;
    struct dirent *ent;
    int titleindex=0;
    if ((dir = opendir ("./Review_Texts")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {//loop all file in directory
            readByLine(ent->d_name,titleindex);//ent->d_name is the file name
            titleindex++;
        }
        closedir (dir);
    }
}



//hotel hashmap unordered_map<string,int>; find index by hotel name
//new a vector<vector<int>>
//keep the vector vector the same title with city vector

vector<vector<int>> map2vectorvector(int limit)//use who evaluates less than limit will be ignore
{
    for(unordered_map<string, vector<pair<int, string>>>::iterator it=ranking.begin();it!=ranking.end();)
    {
        if(it->second.size()<limit)
        {
            it=ranking.erase(it);
        }
        else it++;
    }
    vector<vector<int>>result;
    for(unordered_map<string, vector<pair<int,string>>>::iterator it=ranking.begin();it!=ranking.end();it++)
    {
        vector<int>newline=vector<int>(title.size(),0);
        for(int i=0;i<it->second.size();i++)
        {
            string hotlename=it->second[i].second;
            int totalvalue=it->second[i].first;
            newline[title[hotlename]]=totalvalue;
        }
        result.push_back(newline);
    }
    return result;
}


int main(int argc, const char * argv[]) {
    ofstream myfile;
    myfile.open("rate.csv");
    cout<<"Please wait for couple of seconds, busy running backend..."<<endl;
    listallfile();
    vector<vector<int>>newma=map2vectorvector(4);
    for(int i=0;i<newma.size();i++)
    {
        for(int j=0;j<newma[i].size()-1;j++)
        {
            myfile<<newma[i][j]<<",";
        }
        myfile<<newma[i][newma[i].size()-1]<<endl;
    }
    return 0;
}

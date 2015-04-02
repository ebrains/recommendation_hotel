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
#include <map>
#include <dirent.h>
using namespace std;

unordered_map<string,vector<pair<int,string>>>ranking;
void testByLine(string filename)//
{
    ifstream fin("./Review_Texts/"+filename);
    string s;
    string authorschema="<Author>";
    string valueschema="<Overall>";
    string author;
    string hotelname=filename.substr(filename.find("_")+1,filename.find(".")-filename.find("_")-1);
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
void listallfile()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("./Review_Texts")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {//loop all file in directory
            testByLine(ent->d_name);//ent->d_name is the file name
        }
        closedir (dir);
    }
}

//hotel hashmap unordered_map<string,int>; find index by hotel name
//new a vector<vector<int>>
//keep the vector vector the same title with city vector
vector<vector<int>> map2vectorvector()
{
    vector<vector<int>>result;
    
    return result;
}
int main(int argc, const char * argv[]) {
    listallfile();
    int three=0;
    for(unordered_map<string,vector<pair<int,string>>>::iterator it=ranking.begin();it!=ranking.end();it++)
    {
        
        if(it->second.size()>=6){
            cout<<it->first<<endl;
        for(int i=0;i<it->second.size();i++)
        {
            cout<<it->second[i].first<<"  "<<it->second[i].second<<endl;
        }
        cout<<"-----------"<<endl;
            three++;
        }
    }
    
    cout<<ranking.size()<<"   "<<three<<endl;
    return 0;
}

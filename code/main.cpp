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
#include <unordered_set>
#include <dirent.h>
#include <cmath>
using namespace std;

unordered_map<string,vector<pair<int,string>>>ranking;//the key is user, the value is hotels that this user has rated.
unordered_map<string, int>title;//Map hotel to 2d matrix index.
unordered_map<string, vector<pair<double,string>>>cityhotel;//key is city, value is hotel's price and hotel's name
unordered_set<string>city;//List all cities
vector<string>recommendhotel;//recommend hotel list, from most to least.
unordered_map<string , vector<int>>cityhotelrate;//key is hotel's name, value is hotels' rate, don't consider who rate it.
void testByLine(string filename,int titleindex)//
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
void listallfile()
{
    DIR *dir;
    struct dirent *ent;
    int titleindex=0;
    if ((dir = opendir ("./Review_Texts")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {//loop all file in directory
            testByLine(ent->d_name,titleindex);//ent->d_name is the file name
            titleindex++;
        }
        closedir (dir);
    }
}
void cityandhotel(string filename)
{
    ifstream fin(filename);
    string hotelname;
    string cityname;
    string price;
    string s;
    while(getline(fin,s))
    {
        fin>>hotelname>>price>>cityname;
        if(city.find(cityname)==city.end())
        {
            city.insert(cityname);
        }
        hotelname=hotelname.substr(hotelname.find("_")+1,hotelname.find("parsed")-hotelname.find("_")-2);
        double priced=atoi(price.c_str());
        cityhotel[cityname].push_back(pair<double,string>(priced,hotelname));
//        cityhotel[cityname].push_back(map<string,double>(hotelname,priced));
    }
}
//hotel hashmap unordered_map<string,int>; find index by hotel name
//new a vector<vector<int>>
//keep the vector vector the same title with city vector

vector<vector<int>> map2vectorvector(int limit)//use who evaluates less than limit will be ignore
{
    cout<<ranking.size()<<endl;
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
        vector<int>newline=vector<int>(title.size(),-1);
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
void showcity()
{
    for(unordered_set<string>::iterator it=city.begin();it!=city.end();it++)
    {
        cout<<*it<<endl;
    }
}
void buildhotelcityratesub(string hotel)
{
    ifstream fin("./Review_Texts/hotel_"+hotel+".dat");
    string s;
    string authorschema="<Author>";
    string valueschema="<Overall>";
    string author;
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
            cityhotelrate[hotel].push_back(overall);//unordered_map<string , vector<int>>cityhotelrate
        }
    }
}
void buildhotelcityrate(vector<pair<double,string>>hotel)
{
    for(int i=0;i<hotel.size();i++)
    {
        buildhotelcityratesub(hotel[i].second);
    }
    
}
double average(vector<int>rate)
{
    if(rate.size()==0)return 0;
    double sum=0;
    for(int i=0;i<rate.size();i++)
    {
        sum+=rate[i];
    }
    return sum/rate.size();
}
double avriance(vector<int>rate,double tempaverage)
{
    double sum=0;
    for(int i=0;i<rate.size();i++)
    {
        sum+=(rate[i]-tempaverage)*(rate[i]-tempaverage);
    }
    sum/=rate.size();
    return sqrt(sum);
}
void sort(vector<pair<string,double>>&aarate)
{
    for(int i=0;i<aarate.size();i++)
    {
        for(int j=i+1;j<aarate.size();j++)
        {
            if(aarate[i].second<aarate[j].second)
            {
                swap(aarate[i],aarate[j]);
            }
        }
    }
}
void recommendmostpopular(string city,map<string,double>hotelprice)
{
    vector<pair<string,double>>aarate;
    buildhotelcityrate(cityhotel[city]);
    for(unordered_map<string, vector<int>>::iterator it=cityhotelrate.begin();it!=cityhotelrate.end();it++)
    {
        double tempaverage=average(it->second);
//        double tempavriance=avriance(it->second, tempaverage);
          aarate.push_back(pair<string,double>(it->first,tempaverage));
    }
    sort(aarate);
    cout<<"We recommend the following hotels for you!"<<endl;
    cout<<"hotel name  |  average rate   |  price "<<endl;
    for(int i=0;i<aarate.size();i++)
    {
        cout<<aarate[i].first<<"  "<<aarate[i].second<<"  "<<hotelprice[aarate[i].first]<<endl;
    }
    cout<<aarate.size()<<" accept hotels in total"<<endl;
}
int main(int argc, const char * argv[]) {
    double maxprice=INT_MAX,minprice=0;
        string destination;
        listallfile();
    map2vectorvector(5);
    cityandhotel("hotel_price_location");

    showcity();
    do{
    cout<<"select the destination city"<<endl;
    cin>>destination;
    }while(city.find(destination)==city.end());
    
    do{
    cout<<"Enter the highest price you accept: ";
    cin>>maxprice;
    cout<<"Enter the lowest price you accept: ";
    cin>>minprice;
    }while(maxprice<minprice);
    
    map<string,double>hotelprice;
    for(vector<pair<double,string>>::iterator it=cityhotel[destination].begin();it!=cityhotel[destination].end();)
    {
        if(it->first<=maxprice&&it->first>=minprice)
        {
            hotelprice[it->second]=it->first;
            it++;
        }
        else
        {
            it=cityhotel[destination].erase(it);
        }
    }
    recommendmostpopular(destination,hotelprice);

    return 0;
}

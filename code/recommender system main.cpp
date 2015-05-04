//
//  main.cpp
//  recommender system for hotel
//
//  Created by Yanqiao Zhan on 4/7/15.
//  Copyright (c) 2015 Yanqiao Zhan. All rights reserved.
//

#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <dirent.h>
using namespace std;
int columnum;
int rownum;


vector<double>averageitems;

template <typename T>
vector<vector<T>>txtRead(string filepath,int row,int col)
{
    vector<vector<T>>m(row,vector<T>(col,0));
    ifstream my;
    my.open(filepath);
    if(!my.is_open())
    {
        cerr<<"File can't open!"<<endl;
        exit(1);
    }
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            my>>m[i][j];
        }
    }
    my.close();
    return m;
}
template <typename T>
void writetotxt(vector<vector<T>>&user,vector<vector<T>>&item,string outputpath1,string outputpath2)
{
    ofstream u(outputpath1);
    ofstream it(outputpath2);
    for(int i=0;i<user.size();i++)
    {
        for(int j=0;j<user[i].size();j++)
        {
            u<<user[i][j]<<" ";
        }
        u<<endl;
    }
    for(int i=0;i<item.size();i++)
    {
        for(int j=0;j<item[i].size();j++)
        {
            it<<user[i][j]<<" ";
        }
        it<<endl;
    }
}

template <typename T>
void writetotxt(vector<vector<T>>&user,string outputpath)
{
    ofstream u(outputpath);

    for(int i=0;i<user.size();i++)
    {
        for(int j=0;j<user[i].size();j++)
        {
            u<<user[i][j]<<" ";
        }
        u<<endl;
    }

}
template <typename T>
void writetotxt(vector<T>&user,double i,string outputpath)
{
    ofstream u(outputpath,std::fstream::app);
    u<<i<<",";
    for(int i=0;i<user.size();i++)
    {
            u<<user[i]<<",";
    }
    u<<endl;
}
template <typename T>
void writetocsv(vector<vector<T>>&matrix,string outputpath)
{
    ofstream u(outputpath);
    
    for(int i=0;i<matrix.size();i++)
    {
        for(int j=0;j<matrix[i].size();j++)
        {
            u<<matrix[i][j]<<",";
        }
        u<<endl;
    }
}

/*
Start to build a matrix
 */

unordered_map<string,vector<pair<int,string>>>ranking;//the key is user, the value is hotels that this user has rated.
unordered_map<string, int>title;//Map hotel to 2d matrix index.
unordered_set<string>city;
unordered_map<int, string>indextoid;
unordered_set<string>cityhotel;
int amountvalue=0;

string projectpath="/Users/yanqiaozhan/Code/C++/recommendation/buildmatrix/buildmatrix";
bool readByLine(string filename,int titleindex)//
{
    ifstream fin(projectpath+"/Review_Texts/"+filename);
    string s;
    string authorschema="<Author>";
    string valueschema="<Overall>";
    string author;
    string hotelname=filename.substr(filename.find("_")+1,filename.find(".")-filename.find("_")-1);

    if(cityhotel.find(hotelname)==cityhotel.end()){return false;}
    
    title[hotelname]=titleindex;
    int overall;
    bool skip=false;
    while( getline(fin,s) )
    {
        if(s.substr(0,authorschema.size())==authorschema)
        {
            author=s.substr(authorschema.size(),s.size()-authorschema.size()-1);
            if(author=="A TripAdvisor Member"||author=="lass=")
                skip=true;
        }
        else if(s.substr(0,valueschema.size())==valueschema)
        {
            if(skip==true)skip=false;
            else{
                overall=atoi(s.substr(valueschema.size()).c_str());
                ranking[author].push_back(pair<int,string>(overall,hotelname));
            }
        }
    }
    return true;
}
void listallfile()//only find hotels in the given city
{
    DIR *dir;
    struct dirent *ent;
    int titleindex=0;
    if ((dir = opendir ("/Users/yanqiaozhan/Code/C++/recommendation/buildmatrix/buildmatrix/Review_Texts")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {//loop all file in directory
            if(readByLine(ent->d_name,titleindex))//ent->d_name is the file name
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
    while(getline(fin,hotelname,','))
    {
        
        getline(fin,cityname,'\r');
        if(city.find(cityname)==city.end())
        {
            city.insert(cityname);
        }
        cityhotel.insert(hotelname);
    }
}
void cityandhotel(string filename,string particularcity)
{
    ifstream fin(filename);
    string hotelname;
    string cityname;
    string price;
    city.insert(particularcity);
    while(getline(fin,hotelname,','))
    {
        getline(fin,cityname,'\r');
        if(cityname==particularcity){
            cityhotel.insert(hotelname);
        }
    }
}
//Boston_Massachusetts

//hotel hashmap unordered_map<string,int>; find index by hotel name
//new a vector<vector<int>>
//keep the vector vector the same title with city vector

vector<vector<double>> map2vectorvector(int limit)//use who evaluates less than limit will be ignore
{
    for(unordered_map<string, vector<pair<int, string>>>::iterator it=ranking.begin();it!=ranking.end();)
    {
        if(it->second.size()<limit)
        {
            it=ranking.erase(it);
        }
        else it++;
    }
    vector<vector<double>>result;
    for(unordered_map<string, vector<pair<int,string>>>::iterator it=ranking.begin();it!=ranking.end();it++)
    {
        vector<double>newline=vector<double>(title.size(),0);
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
/*
 end for build matrix
 */

double average(vector<double>&v)
{
    double sum=0;
    int count=0;
    for(int i=0;i<v.size();i++)
    {
        if(v[i]!=0)
        {
            sum+=v[i];
            count++;
        }
    }
    return sum/count;
}

double simsub(vector<vector<double>>&matrix,vector<double>&useraverage,int a,int b)
{
    double ret=0;
    double fenzi=0;
    double fenmu1=0;
    double fenmu2=0;
    double fenmu;
    for(int i=0;i<matrix.size();i++)
    {
        fenzi+=matrix[i][a]*matrix[i][b];
        fenmu1+=matrix[i][a]*matrix[i][a];
        fenmu2+=matrix[i][b]*matrix[i][b];
    }
    fenmu=sqrt(fenmu1)*sqrt(fenmu2);
    if(fenmu!=0)
        ret=fenzi/fenmu;
    else ret=0;
    return ret;
}
void display(vector<vector<double>>&matrix)
{
    for(int i=0;i<matrix.size();i++)
    {
        for(int j=0;j<matrix[i].size();j++)
        {
            cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}


vector<vector<double>> sim(vector<vector<double>>&matrix)
{
    vector<vector<double>>ret;
    vector<double>useraverage;
    for(int i=0;i<matrix.size();i++)
    {
        useraverage.push_back(average(matrix[i]));
    }
    
    int itemcount=matrix[0].size();
    for(int i=0;i<itemcount;i++)
    {
        vector<double>temp;
        for(int j=0;j<=i;j++)
        {
            if(i==j)temp.push_back(0);
            else
                temp.push_back(simsub(matrix, useraverage, i, j));
        }
        cout<<"line"<<i<<"finished   "<<itemcount-i<<"left"<<endl;
        ret.push_back(temp);
    }
    for(int i=0;i<itemcount;i++)
    {
        for(int j=i+1;j<itemcount;j++)
        {
            ret[i].push_back(ret[j][i]);
        }
    }
    return ret;
}

template <typename T>
vector<vector<double>> noramlizing(vector<vector<T>>matrix)
{
    for(int j=0;j<matrix[0].size();j++)
    {
        double sum=0;
        int count=0;
        for(int i=0;i<matrix.size();i++)
        {
            if(matrix[i][j]!=0)
            {
                sum+=matrix[i][j];
                count++;
            }
        }
        
        double aver=sum/count;
        for(int i=0;i<matrix.size();i++)
        {
            if(matrix[i][j]!=0)
                matrix[i][j]-=aver;
        }
    }

    for(int i=0;i<matrix.size();i++)
    {
        double aver=average(matrix[i]);

        for(int j=0;j<matrix[i].size();j++)
        {
            if(matrix[i][j]!=0)
                matrix[i][j]-=aver;
        }
    }
    return matrix;
}

void itemaverage(vector<vector<double>>&matrix)
{
    
    for(int j=0;j<matrix[0].size();j++)
    {
        double sum=0;
        int count=0;
        for(int i=0;i<matrix.size();i++)
        {
            if(matrix[i][j]!=0)
            {
                sum+=matrix[i][j];
                count++;
            }
        }
        averageitems.push_back(sum/count);
    }
}
double subpredict(vector<vector<double>>&relationship,unordered_map<int, double>&newrate,int hotelid,double lowerlimit)
{
    double fenzi=0,fenmu=0;
    
    for(unordered_map<int, double>::iterator it=newrate.begin();it!=newrate.end();it++)
    {

        if(relationship[it->first][hotelid]>lowerlimit)//if some problem occurs,
            //please check variable rate in main function
        {
            fenzi+=relationship[it->first][hotelid]*(it->second-averageitems[it->first]);
            fenmu+=relationship[it->first][hotelid];

        }
    }
    if(fenmu!=0)
    {
        double r=averageitems[hotelid]+fenzi/fenmu;
        if(r>5)return 5;
        else return r;
    }
    else return 0;
    
}

vector<pair<int, double>>predict(vector<vector<double>>&relationship,unordered_map<int, double>&rate,double lowerlimit,int topk,vector<vector<double>>&matrix)
{
    
    unordered_map<int, double>ret;
    itemaverage(matrix);
    for(int i=0;i<relationship.size();i++)
    {
        if(rate.find(i)==rate.end())
        {
            ret[i]=subpredict(relationship, rate, i, lowerlimit);

        }
    }
    vector<pair<int, double>>finalret(topk,pair<int,double>(0,0));
    for(unordered_map<int, double>::iterator it=ret.begin();it!=ret.end();it++)
    {
        if(it->second>finalret[topk-1].second)
        {
            for(int i=0;i<topk;i++)
            {
                if(it->second>finalret[i].second)
                {
                    finalret.insert(finalret.begin()+i, pair<int,double>(it->first,it->second));
                    break;
                }
            }
            finalret.pop_back();
        }
    }
    
    for(vector<pair<int, double>>::iterator it=finalret.begin();it!=finalret.end();)//handle the zero in tail
    {
        if(it->second==0)
        {
            finalret.erase(it);
        }
        else it++;
    }
    return finalret;
    
}
vector<pair<int, double>>mostpopular(int topk)
{
    vector<pair<int, double>>ret(topk,pair<int, double>(-1,-1));
    for(int i=0;i<averageitems.size();i++)
    {
        if(ret[topk-1].second<averageitems[i])
        {
            for(int j=0;j<topk;j++)
            {
                if(ret[j].second<averageitems[i])
                {
                    ret[j]=pair<int, double>(i,averageitems[i]);
                    break;
                }
            }
        }
    }
    return ret;
}


struct myclass {
    bool operator() (pair<int, double> i,pair<int, double> j) { return (i.second>j.second);}
} myobject;

int main()
{
    

    
    cout<<"Please select which model ( 1 -> all cities/  2 -> Boston)"<<endl;
    int select=0;
    while(select!=1&&select!=2)
        cin>>select;
    string FilePath;
    vector<vector<double>>ret;
    vector<vector<double>>train;
    vector<pair<int, double>>p;
    unordered_map<int, double>rate;
    rate[0]=1;
    rate[10]=3;
    rate[12]=4;
    rate[37]=1;
    rate[41]=4;
    rate[33]=3;
    rate[24]=2;
    
    
    int row,col;
    if(select==1)//all cities
    {
        cityandhotel("/Users/yanqiaozhan/Code/C++/recommendation/buildmatrix/buildmatrix/hotel.csv");
        listallfile();
        cout<<"user "<<ranking.size()<<endl;
        vector<vector<double>>train=map2vectorvector(4);
        vector<vector<double>>ntrain=noramlizing(train);
        ret=sim(ntrain);
        p=predict(ret, rate, 0, 10,train);
/*        FilePath="/Users/yanqiaozhan/Code/C++/recommendation/rmse/rmse/rate.txt";
        rownum=2207;
        columnum=1700;
        row = rownum;
        col = columnum;
        ret=txtRead<double>("/Users/yanqiaozhan/Code/C++/recommendation/rmse/rmse/relationshiptemp_noramlize.txt", columnum, columnum);
        train = txtRead<double>(FilePath, row, col);
        p=predict(ret, rate, 0, 10,train);
 */
    }
    else//Boston
    {
        cityandhotel("/Users/yanqiaozhan/Code/C++/recommendation/buildmatrix/buildmatrix/hotel.csv","Hong_Kong_Hong_Kong_");
        cout<<"hotels "<<cityhotel.size()<<endl;
        listallfile();
        cout<<"user "<<ranking.size()<<endl;
        vector<vector<double>>train=map2vectorvector(1);
        
        
        /*FilePath="/Users/yanqiaozhan/Code/C++/recommendation/buildmatrix/buildmatrix/rate.txt";
        rownum=17;
        columnum=42;
        row = rownum;
        col = columnum;
        train = txtRead<double>(FilePath, row, col);//create the user-item matrix
         
         */
        vector<vector<double>>ntrain=noramlizing(train);
        ret=sim(ntrain);
        p=predict(ret, rate, 0, 10,train);
        if(10-p.size()>0){
            vector<pair<int, double>>p1=mostpopular(10-p.size());
            p.insert(p.end(),p1.begin(),p1.end());
            sort(p.begin(), p.end(), myobject);
        }
        
        

        
    }
    for(unordered_map<string, int>::iterator it=title.begin();it!=title.end();it++)
    {
        indextoid[it->second]=it->first;
    }
    
    cout<<endl;
    cout<<"**********----result-----********"<<endl;
    cout<<endl;
    for(int i=0;i<p.size();i++)
    {
        cout<<"rank: "<< i<<"|    hotel id: "<<indextoid[p[i].first]<<"|    predict value: "<<p[i].second<<endl;
    }

    cout<<endl<<endl<<"finished"<<endl;
    return 0;
}

//
//  main.cpp
//  recommendation
//
//  Created by Yanqiao Zhan on 3/30/15.
//  Copyright (c) 2015 Yanqiao Zhan. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
double average(vector<int>v)
{
    double sum=0;
    int count=0;
    for(int i=0;i<v.size();i++)
    {
        if(v[i]!=-1)
        {
            sum+=v[i];
            count++;
        }
    }
    return sum/count;
}
vector<double> subsim(vector<vector<int>>matrix,int finished,int current)
{
    vector<double> result;
    vector<double>averageu;
    for(int i=0;i<matrix.size();i++)
        averageu.push_back(average(matrix[i]));
    for(int i=0;i<finished;i++)
    {
        double tempresult1=0,tempresult2=0,tempresult3=0;
        vector<int>v1,v2;
        for(int j=0;j<matrix.size();j++)
        {
            if(matrix[j][i]!=-1&&matrix[j][current]!=-1)
            {
                tempresult1+=(matrix[j][i]-averageu[j])*(matrix[j][current]-averageu[j]);
                tempresult2+=(matrix[j][i]-averageu[j])*(matrix[j][i]-averageu[j]);
                tempresult3+=(matrix[j][current]-averageu[j])*(matrix[j][current]-averageu[j]);
            }
        }
        double tempresult=tempresult1/(sqrt(tempresult2)*sqrt(tempresult3));
        
        result.push_back(tempresult);
    }
    return result;
}
double pred(vector<double>sim,vector<int>scoretable,double threshold)
{
    double result=0;
    double numerator=0,denominator=0;
    for(int i=0;i<sim.size();i++)
    {
        if(sim[i]>=threshold&&scoretable[i]!=-1){
            numerator+=sim[i]*(scoretable[i]);
            denominator+=sim[i];
        }
    }
    if(numerator!=NAN&&numerator!=0&&denominator!=NAN&&denominator!=0)
        result=numerator/denominator;
    return result;
}
int main(int argc, const char * argv[]) {

    vector<vector<int>>matrix={{1,1,-1,4,5,-1,1,0},{-1,4,2,0,5,-1,1,2},{5,2,-1,0,3,1,-1,4}};
    vector<int>input={4,5,3,4};
    vector<double>output;
    int maxindex=-1;
    double maxvalue=-1;
    for(int i=4;i<8;i++)
    {
        vector<double>output=subsim(matrix, 4,i);
        double temp=pred(output, input, 0);
        if(temp>maxvalue)
        {
            maxvalue=temp;
            maxindex=i;
        }
        output.push_back(temp);
        cout<<temp<<endl;
    }
    cout<<"should select "<<maxindex<<" value:"<<maxvalue<<endl;
    return 0;
}

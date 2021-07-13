#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <math.h>
#include <limits>
#include <bits/stdc++.h>
#include<set>
#include <fstream>
#include <cstdio>
#include <ctime>
#define NODE 4663

#include <chrono>
#include <thread>

using namespace std;
class city{
public:
    int no;
    double x,y;
    city(){
        this->no=0;
        this->x=0.0;
        this->y=0.0;
    }
    ~city(){}
};

class twoCities{
public:
    int city1;
    int city2;
    double dist;
    twoCities(int c1,int c2,double d){
        this->city1=c1;
        this->city2=c2;
        this->dist=d;
    }
    ~twoCities(){}

};

void readCities(city X[]){
    string line;
    ifstream myfile ("ca4663.tsp");
    int no;
    double x,y;
    if (myfile.is_open()){
        for(int i=0;i<7;i++){
            getline (myfile,line);
        }
        while ( getline (myfile,line) ){
            if(line == "EOF"){
                break;
            }
            int i=0;
            string temp;
            while(line[i] != ' '){
                temp+=line[i];
                i++;
            }
            no=atoi(temp.c_str());
            i++;
            temp="";
            while(line[i] != ' '){
                temp+=line[i];
                i++;
            }
            x=stold(temp);
            i++;
            temp="";
            for(unsigned int k=i;k<line.size();k++){
                temp += line[k];
            }
            y=stold(temp);
            X[no-1].no=no;
            X[no-1].x=x;
            X[no-1].y=y;
        }
    myfile.close();
    }
    else
        cout<<"File cannot find."<<endl;
}

void distanceGraph(city X[],vector<double> citiesGraph[]){
    double distance;
    for(unsigned int i=0;i<citiesGraph[0].size();i++){
        for(unsigned int j=0;j<citiesGraph[0].size();j++){
            distance=sqrt((X[i].x-X[j].x)*(X[i].x-X[j].x) + (X[i].y-X[j].y)*(X[i].y-X[j].y));
            citiesGraph[i][j]=distance;
        }
    }
}


bool dfs(vector<int> tsp[],int vertex, set<int>&visited, int parent) {
   visited.insert(vertex);
   for(unsigned int v = 0; v<tsp[vertex].size(); v++) {
         if(tsp[vertex][v] == parent)    //if v is the parent not move that direction
            continue;
         if(visited.find(tsp[vertex][v]) != visited.end())    //if v is already visited
            return true;
         if(dfs(tsp,tsp[vertex][v], visited, vertex))
            return true;

   }
   return false;
}

bool hasCycle(vector<int> tsp[]) {
   set<int> visited;       //visited set
   for(int v = 0; v<NODE; v++) {
      if(visited.find(v) != visited.end())    //when visited holds v, jump to next iteration
         continue;
      if(dfs(tsp,v, visited, -1)) {    //-1 as no parent of starting vertex
         return true;
      }
   }
   return false;
}

void nna(vector<double> citiesGraph[],city Cities[]){
    std::clock_t start;//this to calculate the time
    double duration;
    start = std::clock();

    int cityNumber=citiesGraph[0].size();
    vector<int>visited(cityNumber,0);
    vector<int>cityOrder;
    int currentCity=0;
    int tempCity;
    double minDistance=numeric_limits<double>::max();
    visited[0]=1;
    cityOrder.push_back(currentCity);
    for(int i=0;i<cityNumber-1;i++){
        for(int j=0;j<cityNumber;j++){
            if(currentCity != j && visited[j] == 0){
                if(minDistance>citiesGraph[currentCity][j]){
                    tempCity=j;
                    minDistance=citiesGraph[currentCity][j];
                }
            }
        }
        cityOrder.push_back(tempCity);
        currentCity=tempCity;
        visited[currentCity]=1;
        minDistance=numeric_limits<double>::max();
    }
    //cout<<cityOrder.size()<<endl;
    //for(unsigned int i=0;i<cityOrder.size();i++)
      //  cout<<cityOrder[i]<<endl;
    ofstream myfile;
    myfile.open("nearestNeighbor.csv");
    myfile<<fixed;
    myfile<<setprecision(4);
    double length=0.0;
    for(int i=0;i<4663;i++){//write data to csv file called greedy.csv and finding length
        myfile<<Cities[cityOrder[i]].no<<","<<Cities[cityOrder[i]].x<<","<<Cities[cityOrder[i]].y<<endl;
        length += citiesGraph[cityOrder[i]][cityOrder[(i+1)%4663]];
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout<<"Nearest neighbor's data was written in nearestNeighbor.csv file"<<endl;
    cout<<"Length of nearest neighbor = "<<int(length)<<endl;
    cout<<setprecision(3);
    cout<<"Nearest neighbor's duration = "<<duration<<" second"<<endl;
    cout<<setprecision(8);
    cout<<endl;
    myfile.close();
}



void greedy(vector<double> citiesGraph[],city Cities[]){
    std::clock_t start;//this to calculate the time
    double duration;
    start = std::clock();

    vector<twoCities>sortingDistance;// put all distances of two cities
    vector<int> visitedGraph[4663];
    vector<int> edgeList(4663,0);//two edge test
    int i=0;
    int j=0;
    while(4663*4662/2>sortingDistance.size()){
        if(i>j){
            twoCities temp(i,j,citiesGraph[i][j]);
            sortingDistance.push_back(temp);
            j++;
        }
        else{
            i++;
            j=0;
        }
    }
    sort(sortingDistance.begin(), sortingDistance.end(), [](const twoCities& lhs, const twoCities& rhs) {
      return lhs.dist < rhs.dist;
    });//sort the distances

    i=0;
    int counter=0;
    int c1,c2;
    while(counter<4663){
        c1=sortingDistance[i].city1;
        c2=sortingDistance[i].city2;
        if(edgeList[c1]<2 && edgeList[c2]<2){
            edgeList[c1]++;
            edgeList[c2]++;
            visitedGraph[c1].push_back(c2);
            visitedGraph[c2].push_back(c1);
            counter++;
            if(counter<4663 && hasCycle(visitedGraph)){
                edgeList[c1]--;
                edgeList[c2]--;
                visitedGraph[c1].pop_back();
                visitedGraph[c2].pop_back();
                counter--;
            }
        }
        i++;
    }

    //put results cityOrder array
    int cityOrderGreedy[4663];
    cityOrderGreedy[0]=0;
    int previous=0;
    cityOrderGreedy[1]=visitedGraph[0][0];
    int current=cityOrderGreedy[1];
    for(int i=2;i<4663;i++){
        if(visitedGraph[current][0] == previous){
            previous=current;
            current=visitedGraph[current][1];
            cityOrderGreedy[i]=current;
            //cout<<cityOrderGreedy[i-1]<<endl;
        }
        else{
            previous=current;
            current=visitedGraph[current][0];
            cityOrderGreedy[i]=current;
            //cout<<cityOrderGreedy[i-1]<<endl;
        }
    }
    // end of greedy
    ofstream myfile;
    myfile.open("greedy.csv");
    myfile<<fixed;
    myfile<<setprecision(4);
    double length=0.0;
    for(int i=0;i<4663;i++){//write data to csv file called greedy.csv and finding length
        myfile<<Cities[cityOrderGreedy[i]].no<<","<<Cities[cityOrderGreedy[i]].x<<","<<Cities[cityOrderGreedy[i]].y<<endl;
        length += citiesGraph[cityOrderGreedy[i]][cityOrderGreedy[(i+1)%4663]];
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout<<"Greedy's data was written in greedy.csv file"<<endl;
    cout<<"Length of greedy = "<<int(length)<<endl;
    cout<<setprecision(3);
    cout<<"Greedy's duration = "<<duration<<" second"<<endl;
    cout<<setprecision(8);
    cout<<endl;
    myfile.close();


    /*for(int i=0;i<4663;i++){//display adj. 2d list(vector)
        cout<<i<<" -> ";
        for(unsigned int j=0;j<visitedGraph[i].size();j++){
            cout<<visitedGraph[i][j]<<" ";
        }
        cout<<endl;
    }*/

    /*for(int i=0;i<4663;i++){// cityOrder display
        cout<<cityOrderGreedy[i]<<endl;
    }*/


}



int main(){
    cout<<fixed;
    cout<<setprecision(8);
    city Cities[4663];
    vector<double> citiesGraph[4663];
    vector<double> initialZero(4663,0);
    for(int i=0;i<4663;i++){
        citiesGraph[i]=initialZero;
    }
    readCities(Cities);
    distanceGraph(Cities,citiesGraph);
    nna(citiesGraph,Cities);
    greedy(citiesGraph,Cities);




    return 0;
}

// Mike Adams
// Andy Wu
// Daniel Choi


#include <algorithm> 
#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include "Process.h"
using namespace std;


  Process::Process(char  n, int mysize, std::vector<std::vector< int > > runs) {
    name = n;
    memsize = mysize;
    
    for (unsigned int i =0; i < runs.size(); i++)
    {
        vector<int > tmp;
        for (unsigned int j = 0; j < runs[i].size(); j++){
            tmp.push_back(runs[i][j]);
        }
        runtimes.push_back(tmp);
    }

  }

  
  Process::Process( Process & p) {

    name = p.getName();
    memsize = p.getSize();
    runtimes = p.getRuntimes();
  }
  Process::Process(){

    name = ' ';
    memsize = 0;
  }

   Process Process::operator=( Process p ){
    runtimes = p.getRuntimes();
    memsize = p.getSize();
    name = p.getName();
    return *this;
   }

  Process::Process(const Process & p){

  }
    
  char Process::getName() {return name;}
  int Process::getSize() {return memsize;}
  std::vector<std::vector< int > > Process::getRuntimes(){return runtimes;}
  bool Process::runNow(int t ){
    //cout << "ya" << t << " " <<runtimes.size() << endl;
    for (unsigned int i =0; i < runtimes.size(); i++)
    {
        //cout << "ya" << t << " " <<runtimes[i][0];
        if (runtimes[i][0] == t)
            return true;
    }
    return false;

  }
  bool Process::remNow(int t ){
    //cout << "ya" << t << " " <<runtimes.size() << endl;
    for (unsigned int i =0; i < runtimes.size(); i++)
    {
        //cout << "ya" << t << " " <<runtimes[i][0];
        if (runtimes[i][0] + runtimes[i][1] == t)
            return true;
    }
    return false;

  }

  void Process::add(int t, int add){
    for (unsigned int i =0; i < runtimes.size(); i++)
    {
        //cout << "ya" << t << " " <<  add << " " <<runtimes[i][0] << endl;
        
        runtimes[i][0] = runtimes[i][0] +  add;
        //cout << "now " <<  " " << runtimes[i][0] << endl;;
    }
  }
  void Process::set(int t){
    for (unsigned int i =0; i < runtimes.size(); i++)
    {
        if (runtimes[i][0] == t){
          runtimes[i][1] = 0;
        }
    }
  }

  bool Process::isDone(int t){
    for (unsigned int i =0; i < runtimes.size(); i++)
    {
       
        if (runtimes[i][0] + runtimes[i][1] > t){
          return false;
        }
    }
    return true;

  }







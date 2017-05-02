#include <algorithm> 
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "Memory.h"
#include "Process.h"

using namespace std;

int addNCProcess(int size, char name, vector<vector<char> > &v, int &dots);
void removeNCProcess(char name, vector<vector<char> > &v, int &dots);
void printNC(vector<vector<char> > v);

int main(int argc, char* argv[]) {

  if (argc != 2) {
    cerr << "ERROR: Invalid arguments\n";
    exit(EXIT_FAILURE);
  }
  
  ifstream in_str(argv[1]); // opens input stream 
  
  if (!in_str.good()) // in case of error 
  {
    cerr << "ERROR: Can't open " << argv[1] << " to read\n";
    exit(EXIT_FAILURE);
  }
  int numProcesses = 0;
  in_str >> numProcesses;
  int count = 0;
  string mystring;
  vector<Process * > myProcess;
  vector<Process * > myProcess2;
  vector<Process * > myProcess3;
  vector<Process * > myProcess4;
  char processname = '!';
  while (count < numProcesses) {
    vector<vector<int> > runs;
    int framesize = 0;

    if (processname == '!'){
      in_str >> processname;
    }
    in_str >> framesize;
    char pastprocess = processname;
    while(true)
    {
     string a;

     in_str >> a;
     if ('0' <= a[0] && a[0] <= '9'){
       std::stringstream  data2(a);

      std::string line2;
      vector<int> tmp;
      while(std::getline(data2,line2,'/')){
        tmp.push_back(atoi(line2.c_str()));

      }
      runs.push_back(tmp);
    }
    else{
      processname = a[0];
      break;
    }
    }
    Process * p1 = new Process(pastprocess , framesize , runs);
    Process * p2 = new Process(pastprocess , framesize , runs);
    Process * p3 = new Process(pastprocess , framesize , runs);
    Process * p4 = new Process(pastprocess , framesize , runs);
    myProcess.push_back(p1);
    myProcess2.push_back(p2);
    myProcess3.push_back(p3);
    myProcess4.push_back(p4);

    count +=1;
  }

  Memory * myMem = new Memory();
  int t3 =0;
  cout << "time 0ms: Simulator started (Contiguous -- Next-Fit)" << endl;
  
  int loc = 0;
  int next_loc = 0;
  bool  skip = false;
  vector<char> np;
  while (true)
  {
    for (unsigned int i = 0; i < myProcess3.size(); i++)
    {
      if (myProcess3[i]->remNow(t3)){
        cout << "time " << t3 << "ms: Process " << myProcess3[i]->getName() << " removed:" << endl;
        myMem->removeProcess(myProcess3[i]->getName(), loc);
        myMem->print();
      }
    }
    for (unsigned int i = 0; i < myProcess3.size(); i++)
    {
      
      if (myProcess3[i]->runNow(t3)){
        if (not skip){
          cout << "time "<< t3 <<"ms: Process "<< myProcess3[i]->getName() <<" arrived (requires "<< myProcess3[i]->getSize() << " frames)" << endl;

        }
        skip = false;
        if (myMem->addProcessNext(myProcess3[i]->getSize(), myProcess3[i]->getName() ,loc,next_loc) == 0){
          loc = next_loc;
          cout<<  "time " << t3 << "ms: Placed process " << myProcess3[i]->getName() << ":" << endl;
          myMem->print();


        }
        else if (myMem->addProcessNext(myProcess3[i]->getSize(), myProcess3[i]->getName(),loc,next_loc) == -1)
        {
          loc = next_loc;
          cout << "time "<< t3 << "ms: Cannot place process " << myProcess3[i]->getName() << " -- skipped!" << endl;
          myProcess3[i]->set(t3);
        }
        else{
          for (unsigned int j= 0; j < myProcess3.size(); j++)
          {
            myProcess3[j]->add(t3, myMem->used(np));
          }
          cout << "time "<< t3 << "ms: Cannot place process " << myProcess[i]->getName() << " -- starting defragmentation" << endl;
          int added = t3;
          vector<char> spots;
          t3+= myMem->used(spots) -1;
          added = t3 - added;

          myMem->defrag();
          
          skip = true;
          // after a remove the nextspotstart is changed
          
          cout << "time " << t3 +1 <<"ms: Defragmentation complete (moved " << added +1 << " frames: " << spots[0];
          for (unsigned int k=1; k < spots.size(); k++){
            cout << ", " << spots[k];
          }
          cout << ")" << endl;
          myMem->print();
          
        }
      }
    }
    
    bool finished = true;
    for (unsigned int a =0; a < myProcess3.size(); a++){
      if (not myProcess3[a]->isDone(t3)){
        finished = false;
        break;
      }
    }
    if (finished){
      cout << "time " << t3<< "ms: Simulator ended (Contiguous -- Next-Fit)" << endl;
      break;
    }
    t3+=1;

  }

  

  int t = 0;
  cout <<endl << "time 0ms: Simulator started (Contiguous -- Best-Fit)" << endl;
  
  myMem = new Memory();
  int v= 0;
  skip = false;

  
  while (true)
  {
    for (unsigned int i = 0; i < myProcess.size(); i++){
      if (myProcess[i]->remNow(t)){
          cout << "time " << t << "ms: Process " << myProcess[i]->getName() << " removed:" << endl;
          myMem->removeProcess(myProcess[i]->getName(),v);
          myMem->print();
        }
      }
    for (unsigned int i = 0; i < myProcess.size(); i++)
    {
      
      
      if (myProcess[i]->runNow(t)){
        if (not skip){
          cout << "time "<< t<<"ms: Process "<< myProcess[i]->getName() <<" arrived (requires "<< myProcess[i]->getSize() << " frames)" << endl;
        }
        skip = false;
        if (myMem->addProcess(myProcess[i]->getSize(), myProcess[i]->getName()) == 0){
          cout<<  "time " << t << "ms: Placed process " << myProcess[i]->getName() << ":" << endl;
          myMem->print();

        }
        else if (myMem->addProcess(myProcess[i]->getSize(), myProcess[i]->getName()) == -1)
        {
          cout << "time "<< t << "ms: Cannot place process " << myProcess[i]->getName() << " -- skipped!" << endl;
          myProcess[i]->set(t);
        }
        else{
          for (unsigned int j= 0; j < myProcess.size(); j++)
          {
            myProcess[j]->add(t, myMem->used(np));
          }
          cout << "time "<< t << "ms: Cannot place process " << myProcess[i]->getName() << " -- starting defragmentation" << endl;
          int add = t;
          vector<char> spots;
          t+= myMem->used(spots) -1;
          add = t - add;
          myMem->defrag();
          skip = true;
          cout << "time " << t+1  <<"ms: Defragmentation complete (moved " << add +1 << " frames: " << spots[0];
          for (unsigned int k=1; k < spots.size(); k++){
            cout << ", " << spots[k];
          }
          cout << ")" << endl;
          myMem->print();
          
        }
      }
    }
    
    bool finished = true;
    for (unsigned int a =0; a < myProcess.size(); a++){
      if (not myProcess[a]->isDone(t)){
        finished = false;
        break;
      }
    }
    if (finished){
      cout << "time " << t << "ms: Simulator ended (Contiguous -- Best-Fit)" << endl;
      break;
    }

    t+=1;

  }

  cout << endl;
  




  int t2 =0;
  v= 0;
  skip = false;
  cout << "time 0ms: Simulator started (Contiguous -- Worst-Fit)" << endl;
  myMem = new Memory();
  while (true)
  {
    for (unsigned int i = 0; i < myProcess2.size(); i++)
    {
      if (myProcess2[i]->remNow(t2)){
        cout << "time " << t2 << "ms: Process " << myProcess2[i]->getName() << " removed:" << endl;
        myMem->removeProcess(myProcess2[i]->getName(),v);
        myMem->print();
      }
    }
    for (unsigned int i = 0; i < myProcess2.size(); i++)
    {
      
      if (myProcess2[i]->runNow(t2)){
        if (not skip){
          cout << "time "<< t2<<"ms: Process "<< myProcess2[i]->getName() <<" arrived (requires "<< myProcess2[i]->getSize() << " frames)" << endl;
        }
        skip = false;
        if (myMem->addProcessWorst(myProcess2[i]->getSize(), myProcess2[i]->getName()) == 0){
          cout<<  "time " << t2 << "ms: Placed process " << myProcess2[i]->getName() << ":" << endl;
          myMem->print();

        }
        else if (myMem->addProcessWorst(myProcess2[i]->getSize(), myProcess2[i]->getName()) == -1)
        {
          cout << "time "<< t2 << "ms: Cannot place process " << myProcess2[i]->getName() << " -- skipped!" << endl;
          myProcess2[i]->set(t2);
        }
        else{
          for (unsigned int j= 0; j < myProcess2.size(); j++)
          {
            myProcess2[j]->add(t2, myMem->used(np));
          }
          int add = t2;
          vector<char> spots;
          cout << "time "<< t2 << "ms: Cannot place process " << myProcess2[i]->getName() << " -- starting defragmentation" << endl;
          t2+= myMem->used(spots) -1;
          add = t2 - add;
          myMem->defrag();
          skip = true;
          cout << "time " << t2+1 <<"ms: Defragmentation complete (moved " << add + 1 << " frames: "<< spots[0];
          for (unsigned int k=1; k < spots.size(); k++){
            cout << ", " << spots[k];
          }
          cout << ")" << endl;
          myMem->print();
          
        }
      }
    }
    
    bool finished = true;
    for (unsigned int a =0; a < myProcess2.size(); a++){
      if (not myProcess2[a]->isDone(t2)){
        finished = false;
        break;
      }
    }
    if (finished){
      cout << "time " << t2 << "ms: Simulator ended (Contiguous -- Worst-Fit)" << endl;
      break;
    }

    t2+=1;

  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  int t4 = 0;
  int dots = 256;
  skip = false;
  vector<vector<char> > nc;
  for (int i = 0; i < 8; i++) {
    vector<char> row;
    for (int j = 0; j < 32; j++) {
      row.push_back('.');
    }
    nc.push_back(row);
  }
  cout <<endl << "time 0ms: Simulator started (Non-contiguous)" << endl;
  while (true) {
    for (int i = 0; i < myProcess4.size(); i++) {
      if (myProcess4[i]->remNow(t4)) {
        cout << "time " << t4 << "ms: Process " << myProcess4[i]->getName() << " removed:" << endl;
        removeNCProcess(myProcess4[i]->getName(), nc, dots);
        printNC(nc);
      }
    }
    for (int i = 0; i < myProcess4.size(); i++) {
      if (myProcess4[i]->runNow(t4)){
        if (not skip){
          cout << "time "<< t4<<"ms: Process "<< myProcess4[i]->getName() <<" arrived (requires "<< myProcess4[i]->getSize() << " frames)" << endl;
        }
        skip = false;
        if (addNCProcess(myProcess4[i]->getSize(), myProcess4[i]->getName(), nc, dots) == 0){
          cout<<  "time " << t4 << "ms: Placed process " << myProcess4[i]->getName() << ":" << endl;
          printNC(nc);
        }
        else if (addNCProcess(myProcess4[i]->getSize(), myProcess4[i]->getName(), nc, dots) == -1)
        {
          cout << "time "<< t4 << "ms: Cannot place process " << myProcess4[i]->getName() << " -- skipped!" << endl;
          myProcess4[i]->set(t4);
        }
      }
    }

    bool finished = true;
    for (int a =0; a < myProcess4.size(); a++){
      if (not myProcess4[a]->isDone(t4)){
        finished = false;
        break;
      }
    }
    if (finished){
      cout << "time " << t4 << "ms: Simulator ended (Non-contiguous)" << endl;
      break;
    }

    t4+=1;
  }


  return 0;
}

int addNCProcess(int size, char name, vector<vector<char> > &v, int &dots) {
  if (size > dots) {
    return -1;
  }
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) {
      if (v[i][j] == '.' && size > 0) {
        v[i][j] = name;
        dots--;
        size--;
      }
    }
  }
  return 0;
}
void removeNCProcess(char name, vector<vector<char> > &v, int &dots) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) {
      if (v[i][j] == name) {
        v[i][j] = '.';
        dots++;
      }
    }
  }
}
void printNC(vector<vector<char> > v) {
  cout << std::string(32, '=') << endl;
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) {
      cout << v[i][j];
    }
    cout << endl;
  }
  cout << std::string(32, '=') << endl;
}

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

#include "Memory.h"
using namespace std;
int framesPerLine = 32;
int frameSize = 256;


  Memory::Memory() {
    mostrecent = 0;
    totalbytes = 256;
    freebytes = 256;
    vector<int > tmp; 
    tmp.push_back(0);
    tmp.push_back(freebytes);
    freepartitions.push_back(tmp);

    
  }
  
  Memory::Memory( Memory & p) {
    totalbytes = 256;
    freebytes = 256;
  }

   Memory Memory::operator=( Memory p ){
    totalbytes = 256;
    freebytes = 256;
    return *this;
   }

  Memory::Memory(const Memory & p){
    totalbytes = 256;
    freebytes = 256;
    
  }

 void Memory::print(){
  int curpart= 0;
  int used =0;
  cout << std::string(framesPerLine, '=') << endl;
  while(true){
    if (used % framesPerLine==0 && used != 0){ cout << endl;}
    if ( curpart < usedpartitions.size() && used >= usedpartitions[curpart][0] && used <= usedpartitions[curpart][1])
    {
      std::map<char,int>::const_iterator itr;
      char key = '.';

      for (itr = locations.begin(); itr != locations.end(); itr++){
        if (itr->second == usedpartitions[curpart][0])
        {
          key = itr->first;
          break;
        }
      }
      cout << key ;

      if (used == usedpartitions[curpart][1]){
        curpart+=1;
      }
    }
    else
    {
      cout << '.';
    }
   
    if (used >= frameSize-1) {break;}
    used +=1;

  }

  cout << endl << std::string(framesPerLine, '=') << endl;

 }

 void Memory::defrag(){
   freepartitions.clear();
   vector<int> tmp;
   tmp.push_back(totalbytes - freebytes);
   tmp.push_back(totalbytes);
   freepartitions.push_back(tmp);
   int nextfree =0;
   
   vector< vector < int > > ret;

   for (int i =0; i < usedpartitions.size(); i++)
   {
    vector<int > tmp2;

    tmp2.push_back(nextfree);
    tmp2.push_back(usedpartitions[i][1] - usedpartitions[i][0] + nextfree);
    nextfree += 1 + usedpartitions[i][1] - usedpartitions[i][0];
    ret.push_back(tmp2);

    std::map<char,int>::const_iterator itr;
    char key ='.';

    for (itr = locations.begin(); itr != locations.end(); itr++){
      if (itr->second == usedpartitions[i][0])
      {
        key = itr->first;
        break;
      }
    }

    locations[key] = nextfree -  (1+ usedpartitions[i][1] - usedpartitions[i][0]);


   }

   usedpartitions = ret;

 }

 void Memory::removeProcess(char tag, int & index){
  int loc = locations[tag];
  std::map<char,int>::iterator itr;
  itr=locations.find(tag);
  locations.erase(itr); 
  int ending = 0;
  for (int i =0; i < usedpartitions.size(); i++)
  {
    if (usedpartitions[i][0] == loc){
      ending = usedpartitions[i][1];
      freebytes += ending - loc + 1;
      usedpartitions.erase(usedpartitions.begin() + i);
      break;
    }

  }
  bool hit = false;
  //cout << "Ending " << ending << " start: " << loc << " " << tag << endl;
  for (int i =0; i < freepartitions.size(); i++)
  {
    //cout << freepartitions[i][0] << " ----" << freepartitions[i][1] <<  endl;
    if (freepartitions[i][1] == loc-1){
      hit = true;
      if (i+1 < freepartitions.size() && freepartitions[i+1][0] == ending+1)
      {
        //cout << "yayayayay" << endl;
        freepartitions[i][1] = freepartitions[i+1][1];
        freepartitions.erase(freepartitions.begin() + i+1);
        //cout << freepartitions[i][0] << " -->---" << freepartitions[i][1] << endl;
        break;
        
      }
      else{

        freepartitions[i][1] = ending;
      }
    }
    else if ( freepartitions[i][0] == ending+1){
      freepartitions[i][0] = loc;
      hit = true;
    }



  }
  int count = 0;
  bool adder = false;
  if (!hit){

    vector<int> tmp;
    tmp.push_back(loc);
    tmp.push_back(ending);
    vector<vector<int> >::iterator itr;
    for (itr = freepartitions.begin(); itr != freepartitions.end(); itr++){
      if ((*itr)[0] > loc){
        if (index <= count)
        {
          index +=1;
        }
        freepartitions.insert(itr,tmp);
        adder = true;
        break;
      }
    } 
    if (!adder){
      freepartitions.push_back(tmp);
    }
    count+=1;

    
  }




 }

 int Memory::used(vector<char> & spots){
  int counter = 0; 
  int loc =0;
  for (int i =0; i < usedpartitions.size(); i++)
  {
    if (usedpartitions[i][0] == loc)
    {
      loc = usedpartitions[i][1];
      counter += usedpartitions[i][1] - usedpartitions[i][0] +1;
    }
    else{
      std::map<char,int>::const_iterator itr;
      char key = '.';

      for (itr = locations.begin(); itr != locations.end(); itr++){
        if (itr->second == usedpartitions[i][0])
        {
          key = itr->first;
          break;
        }
      }
      spots.push_back(key);
    }

  }
  return totalbytes - freebytes - counter;
 }
    
 int Memory::addProcess(int memsize, char tag){

     if (memsize > freebytes){
      return -1;
     }
     int min = 1000000;
     int loc =0;
     int ending = 0;
     bool found = false;
     int spot =0;
     for (int i=0; i < freepartitions.size(); i++)
     {
      if (freepartitions[i][1] - freepartitions[i][0] +1 >= memsize){      // need to handle start to finish 
        if (min >= freepartitions[i][1] - freepartitions[i][0]+1) 
        {
          found = true;
          min = freepartitions[i][1] - freepartitions[i][0]+1;
          spot = freepartitions[i][0];
          ending = spot + memsize-1;
          loc =i;
        }
      }

     }
     if (found){
       if (min == memsize){
        freepartitions.erase(freepartitions.begin() + loc);
       }
       else
       {
        freepartitions[loc][0] += memsize;
       }
       vector<int > tmp;
       tmp.push_back(spot);
       tmp.push_back(ending);
       vector < vector < int > > tmp2;
       bool added = false;
       for (int k =0; k < usedpartitions.size(); k++)
       {
        if (!added && tmp[1] < usedpartitions[k][0])
        { 
          added = true;
          tmp2.push_back(tmp);
        }
        tmp2.push_back(usedpartitions[k]);
       }
       if (!added){ tmp2.push_back(tmp);}
       usedpartitions = tmp2;

       freebytes -= memsize;
       locations[tag] = spot;
       return 0;
     }
     else
     {
      return -2;
     }
   }



  int Memory::addProcessNext(int memsize, char tag , int start, int & placed){

     if (memsize > freebytes){
      return -1;
     }
     int min = 1000000;
     int loc =0;
     int ending = 0;
     bool found = false;
     int spot =0;
     for (int i=start; i < freepartitions.size(); i++)
     {
      if (freepartitions[i][1] - freepartitions[i][0] +1 >= memsize){      // need to handle start to finish 


        if (i==start){
          if (mostrecent > freepartitions[i][0] && mostrecent < freepartitions[i][1]){
            if (freepartitions[i][1] - mostrecent  +1 >= memsize){
               vector<int > tmp;
               tmp.push_back(mostrecent);
               tmp.push_back(mostrecent + memsize -1 );
               vector < vector < int > > tmp2;
               bool added = false;
               for (int k =0; k < usedpartitions.size(); k++)
               {
                if (!added && tmp[1] < usedpartitions[k][0])
                { 
                  added = true;
                  tmp2.push_back(tmp);
                }
                tmp2.push_back(usedpartitions[k]);
               }
               if (!added){ tmp2.push_back(tmp);}
               usedpartitions = tmp2;

               freebytes -= memsize;
               locations[tag] = mostrecent;
               freepartitions[i][1] = mostrecent -1;
               vector<vector<int > > tmpa;
               for (int c = 0; c < i; c++){
                tmpa.push_back(freepartitions[c]);
               }
               vector<int> sub;
               sub.push_back(freepartitions[i][0]);
               sub.push_back(mostrecent -1);
               tmpa.push_back(sub);
               for (int c = i+1; c < freepartitions.size(); c++){
                tmpa.push_back(freepartitions[c]);
               }
               freepartitions = tmpa;
               return 0;

            }
          }
        }

        found = true;
        min = freepartitions[i][1] - freepartitions[i][0]+1;
        spot = freepartitions[i][0];
        ending = spot + memsize-1;
        loc =i;
        mostrecent = ending +1;
        break;

      }

     }
     if (not found){

      for (int i=0; i <= start; i++)
      {
      if (freepartitions[i][1] - freepartitions[i][0] +1 >= memsize){      // need to handle start to finish 
        
        
        found = true;
        min = freepartitions[i][1] - freepartitions[i][0]+1;
        spot = freepartitions[i][0];
        ending = spot + memsize-1;
        loc =i;
        mostrecent = ending +1;
        break;
      
      }

      }

     }


     if (found){
      placed =loc ;
       if (min == memsize){
        freepartitions.erase(freepartitions.begin() + loc);
       }
       else
       {
        freepartitions[loc][0] += memsize;
       }
       vector<int > tmp;
       tmp.push_back(spot);
       tmp.push_back(ending);
       vector < vector < int > > tmp2;
       bool added = false;
       for (int k =0; k < usedpartitions.size(); k++)
       {
        if (!added && tmp[1] < usedpartitions[k][0])
        { 
          added = true;
          tmp2.push_back(tmp);
        }
        tmp2.push_back(usedpartitions[k]);
       }
       if (!added){ tmp2.push_back(tmp);}
       usedpartitions = tmp2;

       freebytes -= memsize;
       locations[tag] = spot;
       return 0;
     }
     else
     {
      return -2;
     }
   }


int Memory::addProcessWorst(int memsize, char tag){

     if (memsize > freebytes){
      return -1;
     }
     int max = -1000000;
     int loc =0;
     int ending = 0;
     bool found = false;
     int spot =0;
     for (int i=0; i < freepartitions.size(); i++)
     {
      if (freepartitions[i][1] - freepartitions[i][0] +1 >= memsize){      // need to handle start to finish 
        if (max <= freepartitions[i][1] - freepartitions[i][0]+1) 
        {
          found = true;
          max = freepartitions[i][1] - freepartitions[i][0]+1;
          spot = freepartitions[i][0];
          ending = spot + memsize-1;
          loc =i;
        }
      }

     }
     if (found){
       if (max == memsize){
        freepartitions.erase(freepartitions.begin() + loc);
       }
       else
       {
        freepartitions[loc][0] += memsize;
       }
       vector<int > tmp;
       tmp.push_back(spot);
       tmp.push_back(ending);
       vector < vector < int > > tmp2;
       bool added = false;
       for (int k =0; k < usedpartitions.size(); k++)
       {
        if (!added && tmp[1] < usedpartitions[k][0])
        { 
          added = true;
          tmp2.push_back(tmp);
        }
        tmp2.push_back(usedpartitions[k]);
       }
       if (!added){ tmp2.push_back(tmp);}
       usedpartitions = tmp2;

       freebytes -= memsize;
       locations[tag] = spot;
       return 0;
     }
     else
     {
      return -2;
     }

  }





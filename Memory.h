// Mike Adams
// Andy Wu
// Daniel Choi
#ifndef __mem_h
#define __mem_h

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <map>


//============================================================//
//================== HEADER FOR MEMORY CLASS ================//
//============================================================//
class Memory {
  private:
    int freebytes;
    int totalbytes;
    std::vector< std::vector < int > >freepartitions;
    std::vector<std::vector < int > >usedpartitions;

    std::map<char,int> locations;


  public:
    Memory();
    Memory( Memory &  p);
    Memory( const Memory &  p);
    Memory operator=( Memory p );

    int addProcess(int memsize, char tag);
    int addProcessWorst(int memsize, char tag);
    int addProcessNext(int memsize, char tag, int loc, int & nextloc);
    void removeProcess(char c, int & loc);
    void print();
    void defrag();
    int used(std::vector<char> & spots);
    
    

  
};

#endif

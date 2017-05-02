// Mike Adams adamsm6
// Andy Wu    wua6
// Daniel Choi choid5
#ifndef __process_h
// Mike Adams
// Andy Wu
// Daniel Choi

#define __process_h

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

//============================================================//
//================== HEADER FOR PROCESS CLASS ================//
//============================================================//
class Process {
  private:
    char name;
    int memsize;
    std::vector< std::vector < int > > runtimes;
  public:
    Process(char n, int size, std::vector<std::vector<int > > runtimes );
    Process( Process &  p);
    Process( const Process &  p);
    Process();
    Process operator=( Process p );
    
    char getName();
    std::vector<std::vector < int > > getRuntimes();
    int getSize();
    bool runNow(int t);
    bool remNow(int t);
    void add(int t, int add);
    void set(int t);
    bool isDone(int t);

  
};

#endif
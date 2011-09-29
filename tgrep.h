// 2/21/2011
// Authored by Mike Yoffa
// affoyekim@gmail.com

#ifndef _TGREP_H_
#define _TGREP_H_

#include <cstdlib>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int getTime(string line);

int getParsedTime(string line, int& hr, int& min, int& sec);

string getDate(string line);

int readStamp(ifstream& log, string& date, int& time);

int toStart(ifstream& log);

int printInterval(ifstream& log, int startTime, int endTime, int logStart);

int getTimeDif(int time1, int time2);

int newSearch(ifstream& log, int first, int last, int key, string startDate, int startTime, int endTime);

int dealWithParams(int argc, char *argv[], ifstream& log, int& startGoal, int& endGoal);

#endif
// 2/21/2011
// Authored by Mike Yoffa
// affoyekim@gmail.com

#include <cstdlib>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//parameter stuff. addmittedly not very elegent, but I did this last and it was feeling tedious..
int dealWithParams(int argc, char *argv[], ifstream& log, int& startGoal, int& endGoal)
{
  if(argc != 2 && argc != 3)
  {
    cout << "Incorrect number of parameters!! Should be 1 or 2." << endl;
	return 1;
  }
  string time(argv[1]);
  if(argc == 3)
  {
    log.open(argv[2]);
	if (!log.is_open())
	{
	  log.open(argv[1]);
	  time.assign(argv[2]);
	}
  }
  else
    log.open("/logs/haproxy.log");
  if (!log.is_open())
  {
    cout << "Unable to find logfile!!" << endl;
    log.close();
    return 1;
  }
  int pos = time.find('-');
  if(pos == -1)
  {
    pos = time.find(':');
    time.erase(pos, 1);
    pos = time.find(':');
    if(pos == -1)
    {
      startGoal = atoi(time.append("00").c_str());
      endGoal = atoi(time.replace(time.size()-2, 2, "59").c_str());
    }
    else
    {
      time.erase(pos, 1);
	  startGoal = atoi(time.c_str());
      endGoal = startGoal;
    }
  }
  else
  {
    string start = time.substr(0,pos);
	string end = time.substr(pos+1);
	pos = start.find(':');
    start.erase(pos, 1);
    pos = start.find(':');
    if(pos == -1)
      startGoal = atoi(start.append("00").c_str());
    else
	  startGoal = atoi(start.erase(pos, 1).c_str());
	pos = end.find(':');
	end.erase(pos, 1);
    pos = end.find(':');
    if(pos == -1)
      endGoal = atoi(end.append("59").c_str());
    else
	  endGoal = atoi(end.erase(pos, 1).c_str());
  }
  if(startGoal > 245959 || endGoal > 245959)
  {
    cout << "Invalid time parameter!!" << endl;
	return 1;
  }
  if(endGoal < startGoal)
    endGoal += 240000;
  return 0;
}
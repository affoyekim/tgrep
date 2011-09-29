// 2/21/2011
// Authored by Mike Yoffa
// affoyekim@gmail.com

#include <cstdlib>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Takes a line beginning with a timestamp and returns the time as an int
int getTime(string line)
{
  string timestamp = line.substr(7,8);
  timestamp.erase(5,1);
  timestamp.erase(2,1);
  return atoi(timestamp.c_str());
}

//Similar, except it just read the stamp right off the stream and also gives the date
//Assumes that the ifstream pointer is currently at the beginning of a line
int readStamp(ifstream& log, string& date, int& time)
{
  char datebuf[6];
  log >> datebuf;
  date = string(datebuf);
  date += " ";
  log >> datebuf;
  if(atoi(datebuf) < 10)
    date += " ";
  date += string(datebuf);
  char timebuf[8];
  log >> timebuf;
  string timestamp(timebuf);
  timestamp.erase(5,1);
  timestamp.erase(2,1);
  time = atoi(timestamp.c_str());
  return 0;
}

//This will send the ifstram pointer to the beginning of whatever line it's on
//If the pointer is already at the beginning, it will go to that of the one before it
int toStart(ifstream& log)
{
  if(log.tellg() == 0)
    return 0;
  log.unget();
  log.unget();
  while(log.peek() != '\n' && log.tellg() > 0)
  {
    log.unget();
  }
  if(log.tellg() != 0)
    log.get();
  return 0;
}

//Prints out all lines from the requested interval
//Obviously much faster when the pointer is already placed near the beginning
int printInterval(ifstream& log, int startTime, int endTime, int logStart)
{
  int curTime = startTime;
  string curDate;
  int addFlag = 0;
  if(startTime > 240000)
    addFlag++;
  string line;
  getline(log, line);
  if(log.tellg() == -1)
    return 0;
  curTime = getTime(line);
  if(addFlag > 0)
    curTime += 240000;
  //First backs up from the current point until it finds the beginning
  while(curTime >= startTime && log.tellg() != 0)
  {
    toStart(log);
    readStamp(log, curDate, curTime);
	if(addFlag > 0)
	  curTime += 240000;
    toStart(log);
  }
  line = "";
  //then goes til the end
  while(curTime <= endTime && log.tellg() != -1)
  {
    if(curTime >= startTime)
      cout << line;
    getline(log, line);
	if(log.tellg() == -1)
	  return 0;
	curTime = getTime(line);
	line += "\n";
	if(curTime < logStart)
	  addFlag++;
	if(addFlag > 0)
	  curTime += 240000;
  }
  return 0;
}

//Returns the difference, in seconds, between two times
int getTimeDif(int time1, int time2)
{
  int sec1 = time1 % 100;
  time1 = (time1 - sec1) / 100;
  int min1 = time1 % 100;
  time1 = (time1 - min1) / 100;
  int hr1 = time1;
  
  int sec2 = time2 % 100;
  time2 = (time2 - sec2) / 100;
  int min2 = time2 % 100;
  time2 = (time2 - min2) / 100;
  int hr2 = time2;
  
  int sec = sec1 - sec2;
  int min = min1 - min2;
  int hr = hr1 - hr2;
  
  return (hr * 60 + min) * 60 + sec;
}

//The search algorithm
//This search is kind of like a binary search, except instead of just searching in the middle everytime,
//  it looks at how far off the last guess was and uses the character-to-time ratio to make an educated
//  guess on where the value might be
int newSearch(ifstream& log, int first, int last, int key, string startdate, int startTime, int endTime)
{
  int firsttime = startTime;
  int lasttime = endTime;
  int pos;
  int oldPos;
  int curTime;
  
  while (first <= last)
  {
	//Save the old position for timeout purposes
	oldPos = pos;
	//Get how many characters the search interval spans
	int offsetDif = last - first;
	//Get how much time the search interval spans
	int timeDif = getTimeDif(lasttime, firsttime);
    //Get how much time the last guess was off by
	int keyDif = getTimeDif(key, firsttime);
    //Get the new position
	pos = offsetDif * keyDif / timeDif;
    pos += first;
	
	//The following line is all you would need to switch to binary search
	//  pos = (first - last) / 2
	
	//If it gets a position less than 0, just stop. It's cleary not there.
	if(pos < 0)
	  break;
	
	//Go to the position and go to the start of the next line
    log.seekg(pos);
    string line;
    getline(log, line);
	//Set the position to the start of a line
	pos = log.tellg();
	//If it got the same position, timeout
	if(pos == oldPos)
	  break;
	//Find what time we're currently at and adjust for date accordingly
	string curDate;
	readStamp(log, curDate, curTime);
	cout << curTime << endl;
	if (curDate.compare(startdate) != 0)
      curTime = curTime + 240000;
	cout << curTime << endl;
	//Just like binary search
	if (key > curTime)
	{
	  first = pos;
	  firsttime = curTime;
	}
	else if (key < curTime)
	{
	  last = pos;
	  lasttime = curTime;
	}
	else
	  return pos;
  }
  if(pos > 0)
    return pos;
  return -1;
}

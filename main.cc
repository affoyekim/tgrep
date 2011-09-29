// 2/21/2011
// Authored by Mike Yoffa
// affoyekim@gmail.com

#include "tgrep.h"
using namespace std;

int main (int argc, char *argv[])
{
  //start/end of the interval to print
  int startGoal;
  int endGoal;
  ifstream log;
  if(dealWithParams(argc, argv, log, startGoal, endGoal) == 1)
    return 1;
  
  //Get the start date/time
  string line;
  getline(log, line);
  string startDate;
  int startTime;
  readStamp(log, startDate, startTime);

  //Find out how long the log is and get the end date/time
  log.seekg(0, ios_base::end);
  int logSize = log.tellg();
  log.seekg(logSize-1);
  toStart(log);
  int endTime;
  string endDate;
  readStamp(log, endDate, endTime);
  //Times on the next day are just considered +240000
  endTime += 240000;
  
  //Search for the times on the first day
  //If the end of the interval is before the startTime, don't even bother
  int pos;
  if(endGoal >= startTime)
  {
    //If the interval begins at or before the startTime, just go to the beginning
    if(startGoal <= startTime)
    {
      log.seekg(0);
	  printInterval(log, startTime, endGoal, startTime);
    }
	//Otherwise, go find it!
    else
    {  
      pos = newSearch(log, 0, logSize, startGoal, startDate, startTime, endTime);
	  cout << "POS: " << pos << endl;
      if (pos != -1) {
        log.seekg(pos);
		cout << "POS: " << pos << endl;
        printInterval(log, startGoal, endGoal, startTime);
      }
    }
  }
  
  //Now look for the same interval on the next day
  startGoal += 240000;
  endGoal += 240000;
  
  //If the start of the interval is after the endTime, don't even bother
  if(startGoal <= endTime)
  {
    //If the interval ends after the end time, shorten it to the endTime
	if(endGoal > endTime)
	  endGoal = endTime;
	//If the interval happens to begin right at the end time...sure, just print that out
	if(startGoal == endTime)
	  printInterval(log, startGoal, endGoal, startTime);
	else
	{
	  //Otherwise, go find it!
	  pos = newSearch(log, 0, logSize, startGoal, startDate, startTime, endTime);
	  if (pos != -1) {
	    log.seekg(pos);
	    printInterval(log, startGoal, endGoal, startTime);
	  }
	}
  }
	
  log.close();
  
  return 0;
}
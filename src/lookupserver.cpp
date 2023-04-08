// lookupserver.cpp

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include "params.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <unordered_map>

using namespace std;

#define logging
#define SYSTEM_NAME "server"
#define LOG_FILENAME "/home/class/csc3004/tmp/josleskey-lookupserver.log"
#include "logfile.h"

typedef unordered_map<string, Bible*> BibleMap;

Bible* loadBible(BibleMap &cache, string version, LookupResult &result);
QueryMap extractParams(string queryString);
void sendResponses(Bible &bible, Ref ref, int n);

const string receive_pipe = "WBrequest";
const string send_pipe = "WBreply";

int main()
{
  LookupResult result;

  BibleMap bibles; 

  #ifdef logging
    logFile.open(logFilename, ios::out);
  #endif

  // Connect pipes
  log ("Create request pipe");
  Fifo recfifo(receive_pipe);

  while (true)
  {
    log("Open request pipe");
    recfifo.openread();

    log("Listen for requests");
    string query = recfifo.recv();

    log("Received request: " + query);

    log("Close request pipe");
    recfifo.fifoclose();

    cout << "Received request: " << query << endl;

    QueryMap params = extractParams(query);

    string version = "web";
    int verseCount = 1;
    Ref ref;

    if (params.find("version") != params.end()
        && params.find("reference") != params.end()) {
      version = params["version"];
      ref = Ref(params["reference"]);
      verseCount = toInteger(params["num_verses"], 1);
    }

    Bible *bible = loadBible(bibles, version, result);

    sendResponses(*bible, ref, verseCount);
  }
}

Bible* loadBible(BibleMap &cache, string version, LookupResult &result)
{
  log("Load Bible file");

  string versionPath = "web";
  if (version == "kjv" || version == "dby" || version == "ylt" || version == "webster")
    versionPath = version;

  if (cache.find(versionPath) == cache.end()) {
    cache[versionPath] = new Bible("/home/class/csc3004/Bibles/" + versionPath + "-complete", result);
    cout << "Adding new version to cache: " << versionPath << endl;
  } else {
    cout << "Using cached version: " << versionPath << endl;
  }

  cout << endl;

  Bible *bible = cache[versionPath];

  if (result == SUCCESS)
    log("Indexed Bible file");
  else if (result == FILE_ERROR) {
    log("Failed to load Bible file");
  }
  
  return bible;
}

void sendResponses(Bible &bible, Ref ref, int n)
{
  log("Create response pipe");
  Fifo sendfifo(send_pipe);

  log("Open response pipe");
  sendfifo.openwrite();

  LookupResult result;
  Verse outVerse;
  string response;

  for (int i = 0; i < n && result != REACHED_END; i++) {
    if (i == 0)
      outVerse = bible.lookup(ref, result);
    else
      outVerse = bible.nextVerse(result);

    string reportedStatus;

    if (i == n - 1)
      reportedStatus = to_string(result);
    else
      reportedStatus = result == SUCCESS ? to_string(IN_TRANSIT) : to_string(result);

    response = "status=" + reportedStatus + "&body=";

    log(result);

    if (result == SUCCESS) {
      response.append(outVerse.toString());
    }
    else if (result != REACHED_END) {
      log("Error: " + bible.error(result));
      response.append(bible.error(result));
    }

    log("Send response (" + to_string(i + 1) + "): " + response);
    sendfifo.send(response);

    cout << "Sent response: " << response << endl << endl;
  }

  log("Close response pipe");
  sendfifo.fifoclose();
}

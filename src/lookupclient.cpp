// lookupclient.cpp

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "fifo.h"
#include "params.h"
#include "Bible.h"
#include "Verse.h"

#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

#define logging
#define SYSTEM_NAME "client"
#define LOG_FILENAME "/home/class/csc3004/tmp/josleskey-lookupclient.log"
#include "logfile.h"

void displayResponse(string version, string reference, string n);

const string receive_pipe = "WBreply";
const string send_pipe  = "WBrequest";

int main()
{
  #ifdef logging
    logFile.open(logFilename.c_str(), ios::out);
  #endif

  log("Send response header");
  cout << "Content-Type: text/plain\n\n" << endl;

  Cgicc cgi;
  form_iterator versionField = cgi.getElement("version");
  form_iterator referenceField = cgi.getElement("reference");
  form_iterator nField = cgi.getElement("num_verse");
  string version = **versionField;
  string reference = **referenceField;
  string num_verse = **nField;

  displayResponse(version, reference, num_verse);

  return 0;
}

void displayResponse(string version, string reference, string num_verse)
{
  string request = "version=" + version + "&reference=" + reference + "&num_verses=" + num_verse;

  QueryMap params;

  log("Request: " + request);

  log("Create request pipe");
  Fifo sendfifo(send_pipe);

  log("Open request pipe");
  sendfifo.openwrite();

  log("Send request: " + request);
  sendfifo.send(request);

  log("Close request pipe");
  sendfifo.fifoclose();

  log("Create response pipe");
  Fifo recfifo(receive_pipe);

  log("Open response pipe");
  recfifo.openread();

  int i = -1;

  LookupResult status = IN_TRANSIT;

  while (status == IN_TRANSIT) {
    i++;

    log("Listen for response");
    string response = recfifo.recv();

    log("Received response: " + response);

    params = extractParams(response);
    status = params.find("status") != params.end() ? static_cast<LookupResult>(toInteger(params["status"], OTHER)) : OTHER;
    if (status == IN_TRANSIT || status == SUCCESS) {
      if (params.find("body") != params.end()) {
        Verse verse = Verse(params["body"]);
        log("Extracted verse: " + verse.toString());
        if (i == 0)
          verse.displayFlowing(true);
        else
          verse.displayFlowing(false);
      } else {
        status = OTHER;
        log("No body in response");
      }
    } else if (status != REACHED_END) {
      if (params.find("body") != params.end()) {
        log("Error: " + params["body"]);
        cout << params["body"] << endl;
      }
      else
        log("Error code " + status);
    }
  }

  log("Close response pipe");
  recfifo.fifoclose();
}

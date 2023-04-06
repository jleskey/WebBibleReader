// lookupserver.cpp

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

using namespace std;

#define logging
#define LOG_FILENAME "/home/class/csc3004/tmp/josleskey-WebBibleReader.log"
#include "logfile.h"

string buildResponse(Bible &bible, Ref ref);

const string receive_pipe  = "WBrequest";
const string send_pipe = "WBreply";

int main()
{
  LookupResult result;

  #ifdef logging
    logFile.open(LOG_FILENAME, ios::out);
  #endif

  Bible webBible("/home/class/csc3004/Bibles/web-complete", result);

  if (result == FILE_ERROR)
    return 1;

  cout << endl
       << "               Completed indexing." << endl
       << "=================================================" << endl
       << "References: " << webBible.getRefCount()
       << "\tLast byte offset: " << webBible.getOffsetByIndex(-1) << endl << endl;

  // Connect pipes
  Fifo recfifo(receive_pipe);
  Fifo sendfifo(send_pipe);

  cout << endl;

  while (true) {
    recfifo.openread();
    string refQuery = recfifo.recv();
    recfifo.fifoclose();

    // reply placeholder
    //string reply = "status=OTHER&body=" + refQuery.substr(refQuery.find("reference=") + 10) + " [verse text]";

    Ref ref(refQuery.substr(refQuery.find("reference=") + 10));
    string reply = "status=OTHER&body=" + buildResponse(webBible, ref);
    
    cout << "Server received request: " << refQuery << endl << flush;

    sendfifo.openwrite();
    sendfifo.send(reply);
    sendfifo.fifoclose();

    cout << "Client received response: " << reply << endl << flush;
  }
}

string buildResponse(Bible &bible, Ref ref)
{
  LookupResult result;
  Verse verse;

  ostringstream out;

  verse = bible.lookup(ref, result);

  if (result == SUCCESS)
  {
    out << GetBookName(ref.getBook()) << " " << ref.getChap() << ":" << ref.getVerse() << " " << verse.getVerse();
    out << endl;
  }
  else if (result != REACHED_END)
  {
    out << bible.error(result) << endl;
    exit(2);
  }

  return out.str();
}

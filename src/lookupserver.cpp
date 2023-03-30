// lookupserver.cpp

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

// Create Bible object to process the raw text file

const string receive_pipe  = "WBrequest";
const string send_pipe = "WBreply";

int main()
{
  LookupResult result;

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

    string reply = refQuery + " [verse text]";

    cout << "Server received request: " << refQuery << endl << flush;

    sendfifo.openwrite();
    sendfifo.send(reply);
    sendfifo.fifoclose();

    cout << "Client received response: " << reply << endl << flush;
  }
}

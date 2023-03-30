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

  cout << "Indexing complete." << endl
       << "References: " << webBible.getRefCount() << endl
       << "Last byte offset: " << webBible.getOffsetByIndex(-1) << endl;

  // Connect pipes
  Fifo recfifo(receive_pipe);
  Fifo sendfifo(send_pipe);

  recfifo.openread();

  while (true) {
    string refQuery = recfifo.recv();
    cout << "Received request: " << refQuery;
    sendfifo.openwrite();
    sendfifo.send(refQuery + " [verse text]");
    sendfifo.fifoclose();
  }
}

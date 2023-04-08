/***************************************************************************
 fifo.cpp  -  code to allow interprocess communication via a fifo, or "names pipe"
 *
* copyright : (C) 2009 by Jim Skon
*
* This code permits the creation and use of FIFOs for communication
* between processes.  
* 
* the named piped is created and used in /tmp
*
***************************************************************************/

#include "fifo.h"
#include "logfile.h"

#define SYSTEM_NAME "FIFO"

using namespace std;

Fifo::Fifo(){
  fd = 0;
}

Fifo::Fifo(string name){
  // create a named pipe (FIFO)
  // build the name string
  pipename = PATH + SIG + name;

  umask(0);
  // Create (or open) the fifo
  int result = mknod(pipename.c_str(),MODE | S_IFIFO, 0);

  if ((result == -1) && (errno != EEXIST)) {
    log("Error creating pipe: " + name);
    return;
  }
  log("Success creating pipe: " + name);
  fd = 0;
  return;

}

void Fifo::openwrite() {
  if (fd !=0) {
    log("Fifo already opened: " + pipename);
    return;
  }
  // Open the pipe
  fd = open(pipename.c_str(),O_WRONLY);

  // Check if open succeeded
  if (fd ==-1) {
	log("Error - bad input pipe: " + pipename);
	return;
  }
}
void Fifo::openread() {
  if (fd !=0) {
    log("Fifo already opened: " + pipename);
    return;
  }
  // Open the pipe
  fd = open(pipename.c_str(),O_RDONLY);

  // Check if open succeeded
  if (fd ==-1) {
	log("Error - bad input pipe: " + pipename);
	return;
  }
}
  
void Fifo::fifoclose() {
  close(fd);
  fd = 0;
  
}


// Receive a message from a FIFO (named pipe)
string Fifo::recv() {
  if (fd ==0) {
    log("Fifo not open for read: " + pipename);
    return ("");
  }

  int length, i;
  string message;
  bool done;
  int bytes;
  char inbuff;

  // clear message buffer
  message = "";
  // read until we see an end of message line
  done = false;
  i = 0;

  while (i<MaxMess && !done) {
    // Read the next character in the fifo
    bytes = read(fd, &inbuff,1);

    // -1 means something isn't working
    if (bytes ==-1) {
      log("Error - bad read on input pipe: " + pipename);
      return("");
    }
    // check if nothing was read
    if (bytes > 0) {
      // Check if end of message
      if (inbuff == MESSTERM && (i > 0)) {
	done = true;
      } else {
	i++;
	message += inbuff;
      }
    } else {
      // Nothing to read, try to open
      fifoclose();
      openread();
    }
  }
  return(message);
}

// Send a message to a FIFO (named pipe)
// Return 0 if fails, 1 if succeeds
void Fifo::send(string message) {
  if (fd ==0) {
    log("Fifo not open for send: " + pipename);
    return;
  }

  int bytes;

  // Append end of message terminator
  message = message + MESSTERM;
  bytes = write(fd, message.c_str(),message.length());
  if (bytes ==-1) {
    log("Error - bad write on output pipe: " + pipename);
    return;
  }
    if (bytes == 0) {
      log("Error - nothing written: " + pipename);
      return;
    }
  return;
}

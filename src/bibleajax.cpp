/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 * 
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching 
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

Cgicc cgi;  // create object used to access CGI request data

bool evaluateInput(form_iterator input, int &targetVar);

int main() {
  /* A CGI program must send a response header with content type
   * back to the web client before any other output.
   * For an AJAX request, our response is not a complete HTML document,
   * so the response type is just plain text to insert into the web page.
   */
  cout << "Content-Type: text/html\n\n";

  // Connect to the Bible text file
  Bible webBible("/home/class/csc3004/Bibles/web-complete");

  // Bible content variables
  int b = 0, c = 0, v = 0, n = 1;
  Verse outVerse;
  LookupResult result;
  bool validInput = true;

  // GET THE INPUT DATA
  // browser sends us a string of field name/value pairs from HTML form
  // retrieve the value for each appropriate field name
  form_iterator st = cgi.getElement("search_type");
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");

  evaluateInput(book, b);
  evaluateInput(chapter, c);
  evaluateInput(verse, v);
  evaluateInput(nv, n);

  // Create a reference from the numbers
  Ref ref(b, c, v);

  for (int i = 0; i < n && result != REACHED_END; i++) {
    if (i == 0)
      outVerse = webBible.lookup(ref, result);
    else
      outVerse = webBible.nextVerse(result);
    if (result == SUCCESS) {
      if (i == 0) outVerse.displayFlowing(true);
      else outVerse.displayFlowing(false);
    }
    else if (result != REACHED_END) {
      cout << webBible.error(result) << endl;
      exit(2);
    }
  }
}

bool evaluateInput(form_iterator input, int &targetVar) {
  if (input != cgi.getElements().end()) {
    string text = input->getStrippedValue();
    if (text != "") {
      int number = input->getIntegerValue();
      targetVar = number;
      return true;
    }
  }
  return false;
}

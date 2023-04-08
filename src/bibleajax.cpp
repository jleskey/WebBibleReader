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

Cgicc cgi; // create object used to access CGI request data

bool evaluateIntInput(form_iterator input, int &targetVar);
bool evaluateStringInput(form_iterator input, string &targetVar);
Bible connectBible(string version);

int main()
{
  /* A CGI program must send a response header with content type
   * back to the web client before any other output.
   * For an AJAX request, our response is not a complete HTML document,
   * so the response type is just plain text to insert into the web page.
   */
  cout << "Content-Type: text/html\n\n";

  // Bible content variables
  string bv;
  int b = 0, c = 0, v = 0, n = 1;
  Bible theBible;
  Verse outVerse;
  LookupResult result;
  bool validInput = true;

  // GET THE INPUT DATA
  // browser sends us a string of field name/value pairs from HTML form
  // retrieve the value for each appropriate field name
  form_iterator st = cgi.getElement("search_type");
  form_iterator version = cgi.getElement("version");
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");

  evaluateStringInput(version, bv);
  evaluateIntInput(book, b);
  evaluateIntInput(chapter, c);
  evaluateIntInput(verse, v);
  evaluateIntInput(nv, n);

  // Connect to the Bible text file
  theBible = connectBible(bv);

  // Create a reference from the numbers
  Ref ref(b, c, v);

  for (int i = 0; i < n && result != REACHED_END; i++)
  {
    if (i == 0)
      outVerse = theBible.lookup(ref, result);
    else
      outVerse = theBible.nextVerse(result);
    if (result == SUCCESS)
    {
      if (i == 0)
        outVerse.displayFlowing(true);
      else
        outVerse.displayFlowing(false);
    }
    else if (result != REACHED_END)
    {
      cout << theBible.error(result) << endl;
      exit(2);
    }
  }
}

bool evaluateIntInput(form_iterator input, int &targetVar)
{
  if (input != cgi.getElements().end())
  {
    string text = input->getStrippedValue();
    if (text != "")
    {
      int number = input->getIntegerValue();
      targetVar = number;
      return true;
    }
  }
  return false;
}

bool evaluateStringInput(form_iterator input, string &targetVar)
{
  if (input != cgi.getElements().end())
  {
    string text = input->getValue();
    targetVar = text;
    return true;
  }
  return false;
}

Bible connectBible(string version)
{
  string path = "/home/class/csc3004/Bibles/";
  if (version == "kjv" || version == "dby" || version == "ylt" || version == "webster")
    path.append(version);
  else
    path.append("web");
  path.append("-complete");
  return Bible(path);
}

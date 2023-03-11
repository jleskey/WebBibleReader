// Class Bible
// Computer Science, MVNU
//
// A Bible object represents a particular version of the Bible,
// and provides functions to lookup verses and references.
// A Bible object is constructed by giving it a file name string
// that refers to a file containing the entire text of a version.

#ifndef Bible_H
#define Bible_H

#include "Ref.h"
#include "Verse.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// status codes to be returned when looking up a reference
enum LookupResult
{
  SUCCESS,
  NO_BOOK,
  NO_CHAPTER,
  NO_VERSE,
  FILE_ERROR,
  REACHED_END,
  OTHER,
};

class Bible
{ // A class to represent a version of the bible
private:
  string infile;     // file path name
  ifstream instream; // input stream, used when file is open
  bool isOpen;       // true if file is open
  Ref lastValidRef;  // last valid reference in reading operation
  Ref currentRef;    // current reference in reading operation
  Ref targetRef;     // given point of reference
  map<Ref, int> refIndex;

public:
  Bible();               // Default constructor
  Bible(const string s); // Constructor – pass name of bible file

  // Find and return a verse in this Bible, given a reference
  Verse lookup(const Ref ref, LookupResult &status);

  // Return the next verse from the Bible file stream if the file is open.
  // If the file is not open, open the file and return the first verse.
  Verse nextVerse(LookupResult &status);

  // Return an error message string to describe status
  string error(LookupResult status);

  // Display Bible filename
  void display();

  // Generate index of Bible references
  int buildTextIndex();

  // Return number of references
  int getRefCount() { return refIndex.size(); }

  // Check if a reference exists in index
  void checkRef(Ref ref, LookupResult &status);
  bool refExists(Ref ref);

  // Return particular reference in index
  Ref getRef(int i);

  // Return particular offset in index
  int getOffsetByIndex(int i);
  int getOffsetOfRef(Ref i);

  // Return the reference before/after the given parameter ref
  Ref next(const Ref ref, LookupResult &status);
  Ref prev(const Ref ref, LookupResult &status);
};
#endif // Bible_H

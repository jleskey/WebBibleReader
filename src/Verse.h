// Class Verse
// Computer Science, MVNU
//
// A Verse object represents a complete Bible verse, including its reference
//    * Ref ref          - the reference (e.g., 1:1:1)
//    * string verseText - the actual verse text (without reference)

#ifndef Verse_H
#define Verse_H
#include <string>
#include <stdlib.h>
#include "Ref.h"
using namespace std;

class Verse
{
private:
  Ref verseRef;     // The reference for this verse.
  string verseText; // actual verse text (without reference)

public:
  Verse(); // Default constructor
  Verse(const string s); // Extract verse from line of Bible input file

  // Accessors
  Ref getRef();
  string getVerse();

  // Display functions
  void display();                          // Display ref & verse [with line breaks in needed].
  void displayFlowing(bool startsPassage); // Display ref & verse, formatted for flowing passages.
  void displayFullInline();                // Display full ref & verse inline
};

#endif // Verse_H

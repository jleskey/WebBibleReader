// Verse class function definitions
// Computer Science, MVNU

#include "Verse.h"
#include <iostream>
using namespace std;

Verse::Verse()
{ // Default constructor
  verseRef = Ref();
  verseText = "Uninitialized Verse!";
}

// Parse constructor - pass verse string from file
Verse::Verse(const string s)
{
  // use Ref constructor to create verseRef
  // store the remainder of the string in verseText

  // TODO: Make verse text extraction less hard-coded.
  // This would probably require the Ref class to consume part of the string.

  verseRef = Ref(s);
  verseText = s.length() == 0 ? "[End of the text]" : s.substr(s.find(' '), s.length());
}

string Verse::getVerse()
{
  return verseText;
}

Ref Verse::getRef()
{
  return verseRef;
}

// display reference and verse quite simply
void Verse::display()
{
  verseRef.displayMarker();
  cout << " " << verseText;
}

// display reference and verse in a flowing format
void Verse::displayFlowing(bool startsPassage)
{
  if (startsPassage || verseRef.getVerse() == 1)
  {
    cout << endl;
    verseRef.displayHeader();
    cout << endl
         << endl;
  }
  display();
  cout << endl;
}

// display reference and verse inline
void Verse::displayFullInline()
{
  verseRef.display();
  cout << " " << verseText;
}

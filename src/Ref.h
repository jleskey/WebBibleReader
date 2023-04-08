// Class Ref
// Computer Science, MVNU
//
// Ref is a class for representing a Bible reference consisting of
//    * integer book   - the book from 1 (Genesis) to 66 (Revalation)
//    * integer chap   - the chapter number >1
//    * integer verse  - he verse numner >1

#ifndef Ref_H
#define Ref_H
#include <string>
#include <stdlib.h>
using namespace std;

const string bookNames[66] = {
  "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy", "Joshua",
  "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings", "2 Kings",
  "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah", "Esther", "Job",
  "Psalms", "Proverbs", "Ecclesiastes", "Song of Solomon", "Isaiah",
  "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea", "Joel",
  "Amos", "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk", "Zephaniah",
  "Haggai", "Zechariah", "Malachi", "Matthew", "Mark", "Luke", "John",
  "Acts", "Romans", "1 Corinthians", "2 Corinthians", "Galatians",
  "Ephesians", "Philippians", "Colossians", "1 Thessalonians",
  "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon",
  "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John", "3 John",
  "Jude", "Revelation"
};

// GetBookName returns the name of the nth book of the Bible.
string GetBookName(int n);

// GetNextToken returns a token from str, which contains all characters
// up to the first character from delimiters
string GetNextToken(string &str, const string &delimiters);

class Ref
{
private:
  short book, chap, verse; // Reference information
public:
  Ref();                                // Default constructor
  Ref(string s);                        // Parse constructor - example parameter "43:3:16"
  Ref(const int, const int, const int); // Construct from three integers;

  // Accessors
  int getBook() const;  // Access book number
  int getChap() const;  // Access chapter number
  int getVerse() const; // Access verse number

  // Comparators
  bool operator==(const Ref) const;
  bool operator<(const Ref) const;
  bool operator<=(const Ref) const;

  // Display methods
  string toString();     // Return reference string
  void display();        // Print reference with basic formatting
  void displayFlowing(); // Print reference in flowing format
  void displayHeader();  // Display book name and chapter number
  void displayMarker();  // Display verse number
};

#endif // Ref_H

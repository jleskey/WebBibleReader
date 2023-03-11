// testreader.cpp

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <string> 
#include <stdio.h>
using namespace std;

void printVerseOffset(int b, int c, int v);
void printVerse(int b, int c, int v, int n);

// Create Bible object to process the raw text file
Bible webBible("/home/class/csc3004/Bibles/web-complete");

int main (int argc, char **argv) {
	cout << "Indexing complete." << endl
		 << "References: " << webBible.getRefCount() << endl
		 << "Last byte offset: " << webBible.getOffsetByIndex(-1) << endl;
	
  // Test offsets
	printVerseOffset(1, 1, 1);
	printVerseOffset(1, 2, 1);
	printVerseOffset(1, 2, 2);

  // Test verse retrieval
  printVerse(1, 1, 1, 1);
  printVerse(43, 3, 16, 1);
  printVerse(66, 5, 12, 1);
  printVerse(1, 2, 2, 3);
  printVerse(19, 19, 1, 6);
  printVerse(40, 5, 43, 10);
}

void printVerse(int b, int c, int v, int n) {
  LookupResult result;

  Ref ref = Ref(b, c, v);
  Verse verse;

  cout << endl;
  
  for (int i = 0; i < n && result != REACHED_END; i++)
  {
    if (i == 0)
      verse = webBible.lookup(ref, result);
    else
      verse = webBible.nextVerse(result);

    if (result == SUCCESS)
    {
      verse.displayFullInline();
      cout << endl;
    }
    else if (result != REACHED_END)
    {
      cout << webBible.error(result) << endl;
      exit(2);
    }
  }
}

void printVerseOffset(int b, int c, int v) {
	Ref ref = Ref(b, c, v);
	cout << "Byte offset of ";
	ref.display();
	cout << ": " << webBible.getOffsetOfRef(ref) << endl;
}

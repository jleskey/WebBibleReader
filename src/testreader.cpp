// testreader.cpp

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <string> 
#include <stdio.h>
using namespace std;

void printVerseOffset(int b, int c, int v);

// Create Bible object to process the raw text file
Bible webBible("/home/class/csc3004/Bibles/web-complete");

int main (int argc, char **argv) {
	cout << "Indexing complete." << endl
		 << "References: " << webBible.getRefCount() << endl
		 << "Last byte offset: " << webBible.getOffsetByIndex(-1) << endl;
	
	printVerseOffset(1, 1, 1);
	printVerseOffset(1, 2, 1);
	printVerseOffset(1, 2, 2);
}

void printVerseOffset(int b, int c, int v) {
	Ref ref = Ref(b, c, v);
	cout << "Byte offset of ";
	ref.display();
	cout << ": " << webBible.getOffsetOfRef(ref) << endl;
}

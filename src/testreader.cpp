// testreader.cpp

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main (int argc, char **argv) {
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");

	cout << "Indexing complete." << endl
		 << "References: " << webBible.getRefCount() << endl
		 << "Last offset: " << webBible.getOffset(-1) << endl;
	
	Verse verse;
	int b = 1, c = 1, v = 1, n = 3;
	LookupResult result;

	// Create a reference from the numbers
	Ref ref(b, c, v);

	for (int i = 0; i < n && result != REACHED_END; i++) {
		if (i == 0)
			verse = webBible.lookup(ref, result);
		else
			verse = webBible.nextVerse(result);
		if (result == SUCCESS) {
			if (i == 0) verse.displayFlowing(true);
			else verse.displayFlowing(false);
		}
		else if (result != REACHED_END) {
			cerr << webBible.error(result) << endl;
			exit(2);
		}
	}
}

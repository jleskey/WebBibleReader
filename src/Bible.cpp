// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	buildTextIndex();
}

// Constructor – pass bible filename
Bible::Bible(const string s) {
	infile = s;
	buildTextIndex();
}

// lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
    // scan the file to retrieve the line that holds ref ...
    // update the status variable

	status = OTHER;
	Verse lastValidVerse, currentVerse;

	targetRef = ref;
	do {
		currentVerse = nextVerse(status);
		currentRef = currentVerse.getRef();
		if (currentRef <= ref && status == SUCCESS) {
			lastValidVerse = currentVerse;
			lastValidRef = currentRef;
		}
	} while (lastValidRef < ref && status == SUCCESS);

	if (ref.getBook() == 0 || lastValidRef.getBook() < ref.getBook() && (ref.getChap() != 0 || ref.getVerse() != 0))
		status = NO_BOOK;
	else if (ref.getChap() == 0 || lastValidRef.getBook() == ref.getBook() && lastValidRef.getChap() < ref.getChap() && ref.getVerse() != 0)
		status = NO_CHAPTER;
	else if (ref.getVerse() == 0 || lastValidRef.getBook() == ref.getBook() && lastValidRef.getChap() == ref.getChap() && lastValidRef.getVerse() < ref.getVerse())
		status = NO_VERSE;

    return(lastValidVerse);
}



// Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
	string verseData = "";

	if (!instream.is_open())
		instream.open(infile.c_str(), ios::in);
	
	instream.unsetf(ios::skipws);

	if (instream.fail()) {
		status = FILE_ERROR;
		instream.close();
	} else if (!getline(instream, verseData)) {
		status = REACHED_END;
		instream.close();
	}
	else
		status = SUCCESS;

	return Verse(verseData);
}

string Bible::error(LookupResult status) {
	ostringstream out;

	//out << "Error: ";

	if (status == SUCCESS)
		out << "There is no error";
	else if (status == FILE_ERROR)
		out << "Could not read file";
	else if (status == REACHED_END)
		out << "Reached the end of the file";
	else if (status == OTHER)
		out << "Could not process passage";
	else {
		out << "No ";

		switch (status)
		{
			case NO_VERSE:
				out << "verse " << targetRef.getVerse() << " in ";
			case NO_CHAPTER:
				out << "chapter " << targetRef.getChap() << " in ";
			case NO_BOOK:
				if (status == NO_BOOK) out << "book " << targetRef.getBook();
				else out << GetBookName(targetRef.getBook());
				break;
			default:
				out << "explanation is forthcoming for what just happened";
		}
	}

	out << ".";
	return out.str();
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}

int Bible::buildTextIndex () {
	ifstream  instream;     /* input file descriptor */
	int position;         /* location of line in the file */
	string text, line, word;

	/* open text file */
	instream.open(infile.c_str(), ios::in);
	if (!instream) {
		cerr << "Error - can't open input file: " << infile << endl;
		return 0;  /* false, indicates error */
	}

	/* process text file */
	while (!instream.fail()) {
		/* Get the file position at beginning of line */
		position = instream.tellg();
		/* get the next line */
		if (getline(instream,line)) {
			Ref ref = Ref(line);
			refIndex[ref] = position;
		}
	}
  return 1;  /* true, indicates success */
}

Ref Bible::getRef(int index) {
	map<Ref, int>::iterator it;
	int i = 0;
	if (index < 0 && index > -getRefCount())
		for (it = refIndex.end(); index > i; it--)
			i--;
	else if (index < getRefCount())
		for (it = refIndex.begin(); i < index; it++)
			i++;
	else
		return Ref();
	return it->first;
}

int Bible::getOffset(int index) {
	map<Ref, int>::iterator it;
	int i = 0;
	if (index < 0 && index > -getRefCount())
		for (it = refIndex.end(); i > index; it--)
			i--;
	else if (index < getRefCount())
		for (it = refIndex.begin(); i < index; it++)
			i++;
	else
		return 0;
	return it->second;
}

int Bible::getOffset(Ref ref) {
  map<Ref, int>::iterator it;  // iterator for find
  it = refIndex.find(ref);
  if (it == refIndex.end()) {
      return -1;
  } else {
      return it->second;
  }
}

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

  int offset = getOffsetOfRef(ref);
  instream.seekg(offset, ios::beg);

	Verse verse = nextVerse(status);

  if (status == REACHED_END) {
    checkRef(ref, status);
    return Verse();
  }

  return verse;
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

void Bible::checkRef(Ref ref, LookupResult &status) {
  int b = ref.getBook(),
      c = ref.getChap(),
      v = ref.getVerse();
  Ref verseRef = Ref(b, c, v);
  Ref chapRef = Ref(b, c, 1);
  Ref bookRef = Ref(b, 1, 1);

  if (refExists(verseRef))
    status = SUCCESS;
  else if (refExists(chapRef))
    status = NO_VERSE;
  else if (refExists(bookRef))
    status = NO_CHAPTER;
  else
    status = NO_BOOK;
}

bool Bible::refExists(Ref ref) {
  return getOffsetOfRef(ref) != -1;
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

int Bible::getOffsetByIndex(int index) {
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

int Bible::getOffsetOfRef(Ref ref) {
  map<Ref, int>::iterator it;  // iterator for find
  it = refIndex.find(ref);
  if (it == refIndex.end()) {
      return -1;
  } else {
      return it->second;
  }
}

Ref Bible::prev(const Ref ref, LookupResult& status) {
	map<Ref, int>::iterator it;  // iterator for find
	it = refIndex.find(ref);
	if (it == refIndex.end()) {
		return Ref();
	} else {
		it--;
		if (it == refIndex.begin())
			return Ref();
		else
			return it->first;
	}
}

Ref Bible::next(const Ref ref, LookupResult& status) {
	map<Ref, int>::iterator it;  // iterator for find
	it = refIndex.find(ref);
	if (it == refIndex.end()) {
		return Ref();
	} else {
		it++;
		if (it == refIndex.end())
			return Ref();
		else
			return it->first;
	}
}

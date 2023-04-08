# lookupclient program
# Bob Kasper, MVNU Computer Science
# updated January 2020 to use
# c++11 compiler option, current paths on cs.mvnu.edu

# This is a Makefile for the Bible web app demo program.
# Copy this directory to a location within your home directory. 
# Change the USER name value below to your own user name.
# Then use "make" to build the server program,
# and deploy it to the live web server directory.
# To test the program, go to http://cs.mvnu.edu/class/csc3004/USER/
# and open the lookupclient.html link.

USER= josleskey

# Use GNU C++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

all:	lookupserver lookupclient.cgi PutCGI PutStatic

# Ref Object
Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

# Verse Object
Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

# Bible Object
Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp

# Pipe mechanism
fifo.o: fifo.cpp fifo.h logfile.h
	$(CC) $(CFLAGS) -c fifo.cpp

# Lookup server
lookupserver.o: lookupserver.cpp fifo.h Ref.h Verse.h Bible.h logfile.h
	$(CC) $(CFLAGS) -c lookupserver.cpp
lookupserver: lookupserver.o Ref.o Verse.o Bible.o fifo.o
	$(CC) $(CFLAGS) -o lookupserver lookupserver.o Ref.o Verse.o Bible.o fifo.o

# Lookup client
lookupclient.o: lookupclient.cpp fifo.h logfile.h Verse.h Bible.h
	$(CC) $(CFLAGS) -c lookupclient.cpp
lookupclient.cgi:	lookupclient.o fifo.o Ref.o Verse.o Bible.h
		$(CC) $(CFLAGS) -o lookupclient.cgi lookupclient.o fifo.o Ref.o Verse.o Bible.o -lcgicc
			
PutCGI:	lookupclient.cgi
		chmod 755 lookupclient.cgi
		cp lookupclient.cgi /var/www/html/class/csc3004/$(USER)/cgi-bin

		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/

PutStatic:
		cp bibleindexproj3.html bibleindex.css /var/www/html/class/csc3004/$(USER)

		echo "Current contents of your HTML directory: "
		ls -l /var/www/html/class/csc3004/$(USER)

clean:		
		rm *.o lookupserver

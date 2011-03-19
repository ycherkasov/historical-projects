PROGRAM =
LIBS = 
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -O2
COMPILE = $(COMPILER) $(COMPILEFLAGS)

EXECUTABLE = udpclient udpserver
OBJFILES = udpclient.o udpsevrer.o

all: client server


client: tcpclient.cpp
	$(COMPILE) udpclient.cpp -o udpclient

server: tcpserver.cpp
	$(COMPILE) udpserver.cpp -o udpserver


# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJFILES)

.PHONY: rebuild
rebuild: clean all

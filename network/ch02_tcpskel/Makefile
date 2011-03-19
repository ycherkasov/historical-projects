PROGRAM =
LIBS = 
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -O2
COMPILE = $(COMPILER) $(COMPILEFLAGS)

EXECUTABLE = tcpclient tcpserver
OBJFILES = client.o sevrer.o

all: client server


client: tcpclient.cpp
	$(COMPILE) tcpclient.cpp -o tcpclient

server: tcpserver.cpp
	$(COMPILE) tcpserver.cpp -o tcpserver


# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJFILES)

.PHONY: rebuild
rebuild: clean all

PROGRAM =
LIBS = 
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -O2
COMPILE = $(COMPILER) $(COMPILEFLAGS)

EXECUTABLE = client server
OBJFILES = client.o sevrer.o

all: client server


client: client.cpp
	$(COMPILE) client.cpp -o client

server: server.cpp
	$(COMPILE) server.cpp -o server


# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJFILES)

.PHONY: rebuild
rebuild: clean all

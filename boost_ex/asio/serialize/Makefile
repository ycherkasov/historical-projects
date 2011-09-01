PROGRAM =  
LIBS = boost_system boost_regex boost_thread boost_serialization
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging, -O2 for release
COMPILER = g++
#COMPILEFLAGS = -O2
COMPILEFLAGS = -ggdb
COMPILE = $(COMPILER) $(COMPILEFLAGS)

CLI_FILENAME = client
CLI_SOURCES = $(CLI_FILENAME).cpp 
CLI_OBJFILE = $(CLI_FILENAME).o 

SRV_FILENAME = server
SRV_SOURCES = $(SRV_FILENAME).cpp
SRV_OBJFILE = $(SRV_FILENAME).o

all: client server

client: $(CLI_OBJFILE) stock.hpp connection.hpp
	$(COMPILE) -o $(CLI_FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(CLI_OBJFILE) $(foreach slib,$(LIBS), -l$(slib) )

server: $(SRV_OBJFILE) stock.hpp connection.hpp
	$(COMPILE) -o $(SRV_FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(SRV_OBJFILE) $(foreach slib,$(LIBS), -l$(slib) )

$(CLI_OBJFILE): $(CLI_SOURCES)
	$(COMPILE) -c $(CLI_SOURCES)

$(SRV_OBJFILE): $(SRV_SOURCES)
	$(COMPILE) -c $(SRV_SOURCES)

# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf client server $(CLI_OBJFILE) $(SRV_OBJFILE)

.PHONY: rebuild
rebuild: clean all

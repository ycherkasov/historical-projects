PROGRAM =  
LIBS = boost_serialization boost_system boost_thread boost_regex
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging -O2 for release
COMPILER = g++
COMPILEFLAGS = -ggdb
COMPILE = $(COMPILER) $(COMPILEFLAGS)

CLI_FILENAME = client
CLI_SOURCES = $(CLI_FILENAME).cpp raw_binary_archive.cpp
CLI_OBJFILE = $(CLI_FILENAME).o 

HEADERS = 

all: client

client: $(CLI_OBJFILE)
	$(COMPILE) -o $(CLI_FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(CLI_OBJFILE) $(foreach slib,$(LIBS), -l$(slib) )

$(CLI_OBJFILE): $(CLI_SOURCES) $(HEADERS)
	$(COMPILE) -c $(CLI_SOURCES)

$(SRV_OBJFILE): $(SRV_SOURCES) $(HEADERS)
	$(COMPILE) -c $(SRV_SOURCES)

# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(CLI_FILENAME) $(SRV_FILENAME) $(CLI_OBJFILE) $(SRV_OBJFILE)

.PHONY: rebuild
rebuild: clean all

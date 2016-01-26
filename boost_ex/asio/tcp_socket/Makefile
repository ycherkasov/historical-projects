PROGRAM =  
LIBS = boost_system boost_regex boost_thread
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -O2
COMPILE = $(COMPILER) $(COMPILEFLAGS)

CLI_FILENAME = client
CLI_SOURCES = $(CLI_FILENAME).cpp 
CLI_OBJFILE = $(CLI_FILENAME).o 

SRV_FILENAME = aserver
SRV_SOURCES = $(SRV_FILENAME).cpp
SRV_OBJFILE = $(SRV_FILENAME).o

all: client server

client: $(CLI_OBJFILE)
	$(COMPILE) -o $(CLI_FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(CLI_OBJFILE) $(foreach slib,$(LIBS), -l$(slib) )

server: $(SRV_OBJFILE)
	$(COMPILE) -o $(SRV_FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(SRV_OBJFILE) $(foreach slib,$(LIBS), -l$(slib) )

$(CLI_OBJFILE): $(CLI_SOURCES)
	$(COMPILE) -c $(CLI_SOURCES)

$(SRV_OBJFILE): $(SRV_SOURCES)
	$(COMPILE) -c $(SRV_SOURCES)

# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf client server $(CLI_OBJFILES) $(SRV_OBJFILES)

.PHONY: rebuild
rebuild: clean all

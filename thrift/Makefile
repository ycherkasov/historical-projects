PROGRAM = 
LIBS = boost_thread thrift
LIBDIRS = 
INCDIRS = /usr/local/include/thrift

#change to -ggdb for debugging -O2 for release
COMPILER = g++
COMPILEFLAGS = -ggdb
COMPILE = $(COMPILER) $(COMPILEFLAGS)

CLI_FILENAME = client
CLI_SOURCES = $(CLI_FILENAME).cpp 
CLI_OBJFILE = $(CLI_FILENAME).o 

SRV_FILENAME = server
SRV_SOURCES = $(SRV_FILENAME).cpp
SRV_OBJFILE = $(SRV_FILENAME).o
LIB_DIR = /usr/local/lib

IDL_FILENAME = telemetry.thrift

GEN_SRC = ./gen-cpp/SharedService.cpp ./gen-cpp/shared_types.cpp ./gen-cpp/telemetry_types.cpp ./gen-cpp/UavTelemetry.cpp

all: idl client server

idl: $(IDL_FILENAME)
	thrift -r --gen cpp telemetry.thrift 

client: $(CLI_OBJFILE)
	$(COMPILE) -o $(CLI_FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(CLI_OBJFILE) $(foreach slib,$(LIBS), -l$(slib) ) ${GEN_SRC}

server: $(SRV_OBJFILE)
	$(COMPILE) -o $(SRV_FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(SRV_OBJFILE) $(foreach slib,$(LIBS), -l$(slib) ) ${GEN_SRC}

$(CLI_OBJFILE): $(CLI_SOURCES)
	$(COMPILE) -c $(CLI_SOURCES) $(foreach incdir,$(INCDIRS),-I$(incdir))

$(SRV_OBJFILE): $(SRV_SOURCES)
	$(COMPILE) -c $(SRV_SOURCES) $(foreach incdir,$(INCDIRS),-I$(incdir)) 

# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(CLI_FILENAME) $(SRV_FILENAME) $(CLI_OBJFILE) $(SRV_OBJFILE)

.PHONY: rebuild
rebuild: clean all

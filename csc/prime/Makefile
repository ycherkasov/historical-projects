PROGRAM = prime
LIBS = ntl
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
#COMPILEFLAGS = -ggdb
COMPILEFLAGS = -O2
COMPILE = $(COMPILER) $(COMPILEFLAGS)

FILENAME = $(PROGRAM)
SOURCES = $(FILENAME).cpp 
OBJFILE = $(FILENAME).o 
INCLUDES = primetest_ntl.h primetest_simple.h

all: $(PROGRAM)

$(PROGRAM): $(OBJFILE) 
	$(COMPILE) -o $(FILENAME) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(OBJFILE) $(foreach slib,$(LIBS), -l$(slib) )

$(OBJFILE): $(SOURCES) $(INCLUDES)
	$(COMPILE) -c $(SOURCES)

# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(PROGRAM) $(OBJFILE)

.PHONY: rebuild
rebuild: clean all

PROGRAM = accum
LIBS = 
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -ggdb
COMPILE = $(COMPILER) $(COMPILEFLAGS)

FILENAME = $(PROGRAM)
SOURCES = $(FILENAME).cpp 
OBJFILE = $(FILENAME).o 
INCLUDES = 

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

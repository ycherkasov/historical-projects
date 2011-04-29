PROGRAM = pthread
LIBS = pthread
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -ggdb
COMPILE = $(COMPILER) $(COMPILEFLAGS)

FILENAME = $(PROGRAM)
SOURCES = $(FILENAME).cxx 
OBJFILES = $(FILENAME).o 

all: $(PROGRAM)

$(PROGRAM): $(OBJFILES)
	$(COMPILE) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(OBJFILES) $(foreach slib,$(LIBS), -l$(slib) ) -o $@

%.o: %.cxx
	$(COMPILE) -c $<


# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(PROGRAM) $(OBJFILES) test.txt

.PHONY: rebuild
rebuild: clean all

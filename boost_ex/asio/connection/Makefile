PROGRAM = connection
LIBS = boost_system boost_regex boost_thread
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -O2
COMPILE = $(COMPILER) $(COMPILEFLAGS)

FILENAME = $(PROGRAM)
SOURCES = $(FILENAME).cpp 
OBJFILES = $(FILENAME).o 

all: $(PROGRAM)

$(PROGRAM): $(OBJFILES)
	$(COMPILE) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(OBJFILES) $(foreach slib,$(LIBS), -l$(slib) ) -o $@

%.o: %.cpp
	$(COMPILE) -c $<


# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(PROGRAM) $(OBJFILES)

.PHONY: rebuild
rebuild: clean all

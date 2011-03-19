PROGRAM =
LIBS = 
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -O2
COMPILE = $(COMPILER) $(COMPILEFLAGS)

SOURCES = main.cpp 
OBJFILES = main.o 

all: $(PROGRAM)

$(PROGRAM): $(OBJFILES)
	$(COMPILE) $(foreach incdir,$(INCDIRS),-I$(incdir)) \
	$(OBJECTS) $(foreach slib,$(LIBS), -l$(slib) ) -o $@

%.o: %.cpp
	$(COMPILE) -c $<


# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(PROGRAM) $(OBJFILES)

.PHONY: rebuild
rebuild: clean all

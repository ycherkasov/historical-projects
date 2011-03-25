PROGRAM = serialize
LIBS = boost_serialization
LIBDIRS = 
INCDIRS = 

#change to -ggdb for debugging
COMPILER = g++
COMPILEFLAGS = -ggdb
COMPILE = $(COMPILER) $(COMPILEFLAGS)

FILENAME = demo_portable_archive
SOURCES = demo_portable_archive.cpp portable_binary_iarchive.cpp portable_binary_oarchive.cpp
OBJFILES = demo_portable_archive.o portable_binary_iarchive.o portable_binary_oarchive.o


all: $(PROGRAM)

$(PROGRAM): $(OBJFILES)
	$(COMPILE) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(OBJFILES) $(foreach slib,$(LIBS), -l$(slib) ) -o $@

demo_portable_archive.o: demo_portable_archive.cpp
	$(COMPILE) -c demo_portable_archive.cpp

portable_binary_iarchive.o: portable_binary_iarchive.cpp
	$(COMPILE) -c portable_binary_iarchive.cpp

portable_binary_oarchive.o: portable_binary_oarchive.cpp
	$(COMPILE) -c portable_binary_oarchive.cpp

# abstract (phony) targets
.PHONY: clean
clean:
	rm -rf $(PROGRAM) $(OBJFILES) *.o

.PHONY: rebuild
rebuild: clean all

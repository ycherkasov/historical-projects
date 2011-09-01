PROGRAM = netapp
LIBS = 
LIBDIRS = 
INCDIRS = 


COMPILER = g++
#change to -ggdb for debugging
#COMPILEFLAGS = -O2
COMPILEFLAGS = -ggdb
COMPILE = $(COMPILER) $(COMPILEFLAGS)

SOURCES = main.cpp app.cpp app.h net.cpp net.h network_service.cpp network_service.h server_handlers.cpp server_handlers.h
OBJFILES = main.o app.o net.o network_service.o server_handlers.o

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

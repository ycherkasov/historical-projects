echo 'PROGRAM ='
echo 'LIBS = '
echo 'LIBDIRS = '
echo 'INCDIRS = '
echo
echo '#change to -ggdb for debugging'
echo 'COMPILER = g++'
echo 'COMPILEFLAGS = -O2'
echo 'COMPILE = $(COMPILER) $(COMPILEFLAGS)'
echo
echo 'SOURCES = main.cpp '
echo 'OBJFILES = main.o '
echo
echo 'all: $(PROGRAM)'
echo
echo '$(PROGRAM): $(OBJFILES)'
echo '	$(COMPILE) $(foreach incdir,$(INCDIRS),-I$(incdir)) $(OBJFILES) $(foreach slib,$(LIBS), -l$(slib) ) -o $@'
echo
echo '%.o: %.cpp'
echo '	$(COMPILE) -c $<'
echo
echo
echo '# abstract (phony) targets'
echo '.PHONY: clean'
echo 'clean:'
echo '	rm -rf $(PROGRAM) $(OBJFILES)'
echo
echo '.PHONY: rebuild'
echo 'rebuild: clean all'


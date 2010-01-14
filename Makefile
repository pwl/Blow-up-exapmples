#			#
#	Makefile	#
#			#

export SHELL = /bin/bash
export CC = cc
export CFLAGS = -ansi -pedantic -Wall
       # -Wpointer-arith -Wcast-qual -Wcast-align\
       # -Wwrite-strings -Wnested-externs\
       # -fshort-enums -fno-common
       # -Wmissing-prototypes -Wstrict-prototypes
       # -Wconversion -Wshadow
export OFLAGS = # -O3 # left empty for debuggin reasons
export GDBFLAGS = #-ggdb
export FLAGS = $(CFLAGS) $(OFLAGS) $(GDBFLAGS)
export LIBS = -lm -lgsl -lgslcblas # -lfftw3
export ARCHIVE = $(PWD)/libyapdes.a
export MAKEFILES = $(PWD)/Makefile.common
export INCLUDES = $(PWD)/solver
DIRS = "solver"
RM = /bin/rm -f
# $(patsubst %/,%,$(wildcard */))

.PHONY : clean spectral ODE_solver harmonic_mm harmonic_mm_bubbling

project: $(DIRS)

harmonic: harmonic_mm.o $(DIRS)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_mm.o $(ARCHIVE) -o $@

run:	harmonic
	$(RM) log/{snapshot,info_1,movie}/*
	time ./harmonic

shooting2:	shooting/shooting2.c
	$(CC) $(LIBS) $(FLAGS) -o $@ $<

harmonic_bubbling: harmonic_mm_bubbling.o $(DIRS)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_mm_bubbling.o $(ARCHIVE) -o $@

harmonic_mm.o: harmonic_mm.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

harmonic_mm_bubbling.o: harmonic_mm_bubbling.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

example: example.o $(DIRS)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) example.o $(ARCHIVE) -o $@

example.o: example.c example.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

bisection.o: bisection.c bisection.h
	$(CC) $(FLAGS) -c -o $@ $<

# test: test.o $(DIRS)
# 	$(CC) $(CFLAGS) $(LIBS) $(ARCHIVE) test.o -o $@

# test.o: test.c test.h
# 	$(CC) $(CFLAGS) $(LIBS) -c -o $@ $<

#.PHONY: clean $(DIRS) #$(OBJECTS)

$(DIRS):
	$(MAKE) -weC $@ project

clean:
	@rm -f *.o libyapdes.a
	@for d in $(DIRS); do $(MAKE) -eC "$${d}" $@; done

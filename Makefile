#			#
#	Makefile	#
#			#

export SHELL = /bin/bash
export CC = cc
export CFLAGS = -fopenmp -ansi -pedantic -Wall --std=c99 -Wno-unused
       # -Wpointer-arith -Wcast-qual -Wcast-align\
       # -Wwrite-strings -Wnested-externs\
       # -fshort-enums -fno-common
       # -Wmissing-prototypes -Wstrict-prototypes
       # -Wconversion -Wshadow
# export CFLAGS =  -openmp -ansi -Wall -Wno-unused
export OFLAGS = -O3 # left empty for debuggin reasons
export GDBFLAGS = #-ggdb
export FLAGS = $(CFLAGS) $(OFLAGS) $(GDBFLAGS)
export LIBS = -lm -lgsl -lgslcblas # -lfftw3
export ARCHIVE = $(PWD)/libyapdes.a
export MAKEFILES = $(PWD)/Makefile.common
export INCLUDES = $(PWD)/solver
export SHOOTING_OBJECTS = shooting1 shooting2 shooting3 shooting4 shooting_expander shooting_shrinker_reversed
export REQUIRED = mm_distribute_points.o derivatives.o
DIRS = "solver"
RM = /bin/rm -f
# $(patsubst %/,%,$(wildcard */))

.PHONY : clean spectral ODE_solver harmonic_mm harmonic_mm_bubbling $SHOOTING_OBJECTS

project: $(DIRS)

harmonic: harmonic_mm.o $(DIRS) $(REQUIRED)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_mm.o $(ARCHIVE) $(REQUIRED) -o $@

run:	harmonic
	$(RM) log/{snapshot,movie}/*
	time ./harmonic

$(SHOOTING_OBJECTS): % : shooting/%.c shooting/%.h shooting.o
	$(CC) $(FLAGS) $(LIBS) shooting/shooting.o $< -o $@

shooting.o:	shooting/shooting.c shooting/shooting.h
	$(CC) $(FLAGS) -c -o shooting/$@ $<

harmonic_bubbling: harmonic_mm_bubbling.o $(DIRS)  $(REQUIRED)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_mm_bubbling.o $(ARCHIVE) $(REQUIRED) -o $@

harmonic_experimental: harmonic_mm_experimental.o $(DIRS)  $(REQUIRED)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_mm_experimental.o $(ARCHIVE) $(REQUIRED) -o $@

harmonic_multiple_blowup: harmonic_multiple_blowup.o $(DIRS)  $(REQUIRED)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_multiple_blowup.o $(ARCHIVE) $(REQUIRED) -o $@

harmonic_multiple_blowup_S3_to_S3: harmonic_multiple_blowup_S3_to_S3.o $(DIRS)  $(REQUIRED)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_multiple_blowup_S3_to_S3.o $(ARCHIVE) $(REQUIRED) -o $@

harmonic_multiple_blowup_R3_to_S3: harmonic_multiple_blowup_R3_to_S3.o $(DIRS) $(REQUIRED)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_multiple_blowup_R3_to_S3.o $(ARCHIVE) $(REQUIRED) -o $@

harmonic_ys_bisection: harmonic_ys_bisection.o $(DIRS) $(REQUIRED)
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) harmonic_ys_bisection.o $(ARCHIVE) -o $@

harmonic_mm.o: harmonic_mm.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

mm_distribute_points.o: mm_distribute_points.c mm_distribute_points.h derivatives.o
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

d1: d1.c derivatives.o
	$(CC) $(FLAGS) derivatives.o -o $@ $<

derivatives.o: derivatives.c derivatives.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

harmonic_mm_bubbling.o: harmonic_mm_bubbling.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

harmonic_mm_experimental.o: harmonic_mm_experimental.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

harmonic_multiple_blowup.o: harmonic_multiple_blowup.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

harmonic_multiple_blowup_S3_to_S3.o: harmonic_multiple_blowup_S3_to_S3.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

harmonic_multiple_blowup_R3_to_S3.o: harmonic_multiple_blowup_R3_to_S3.c harmonic.h
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

harmonic_ys_bisection.o: harmonic_ys_bisection.c harmonic.h
	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) -c -o $@ $<

# example: example.o $(DIRS)
# 	$(CC) $(FLAGS) $(LIBS) -I $(INCLUDES) example.o $(ARCHIVE) -o $@

# example.o: example.c example.h
# 	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

# bisection.o: bisection.c bisection.h
# 	$(CC) $(FLAGS) -c -o $@ $<

# test: test.o $(DIRS)
# 	$(CC) $(CFLAGS) $(LIBS) $(ARCHIVE) test.o -o $@

# test.o: test.c test.h
# 	$(CC) $(CFLAGS) $(LIBS) -c -o $@ $<

#.PHONY: clean $(DIRS) #$(OBJECTS)

$(DIRS):
	@$(MAKE) --no-print-directory -eC $@ project

clean:
	@rm -f *.o libyapdes.a
	@for d in $(DIRS); do $(MAKE) -eC "$${d}" $@; done

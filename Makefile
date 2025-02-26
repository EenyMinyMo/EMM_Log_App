ifeq ($(origin CC),default)
	CC = g++
endif

CFLAGS ?= -O2 -std=c++17
SHARED_FLAGS = $(CFLAGS) -fpic -shared

SRC = ./source
OBJ = ./obj
EXE = ./exe
TEMP = ./temp
INCLUDE = ./header

SUBDIRS = $(SRC) $(OBJ) $(EXE)
TEMPDIRS = $(EXE) $(TEMP)

LD_LIBRARY_PATH = LIBTARGET

LIBDIR = $(SRC)/lib
APPDIR = $(SRC)/app

LIBCPP = $(LIBDIR)/lib.cpp
APPCPP = $(APPDIR)/task.cpp $(APPDIR)/task_queue.cpp $(APPDIR)/pc_threads.cpp $(APPDIR)/main.cpp

LIBTARGET = $(EXE)/lib.l
APPTARGET = $(EXE)/app

ALLINCLUDE = $(INCLUDE)/lib/lib.hpp $(INCLUDE)/app/task.hpp $(INCLUDE)/app/task_queue.hpp $(INCLUDE)/app/pc_threads.hpp

.PHONY: app dyn_lib subdirs clean

dyn_lib: $(LIBCPP) $(INCLUDE)/lib/lib.hpp
	$(CC) $(SHARED_FLAGS) -I$(INCLUDE) $< -o $(LIBTARGET)

task.o: $(APPDIR)/task.cpp $(ALLINCLUDE)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(OBJ)/$< -o $(OBJ)/$@

task_queue.o: $(APPDIR)/task_queue.cpp $(ALLINCLUDE)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(OBJ)/$< -o $(OBJ)/$@

pc_threads.o: $(APPDIR)/pc_threads.cpp $(ALLINCLUDE)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(OBJ)/$< -o $(OBJ)/$@

app: $(APPCPP) $(ALLINCLUDE) dyn_lib
	$(CC) $(CFLAGS) -I$(INCLUDE) $(APPCPP) $(LIBTARGET) -o $(APPTARGET)

clean:
	for dir in $(TEMPDIRS); do \
		rm -rf $$dir; \
	done
	-rm -rf *.x *.o *.log *.exe

subdirs: 
	for dir in $(TEMPDIRS); do \
		mkdir $$dir; \
	done

DIRS    := src src/Win32
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJS    := $(patsubst %.cpp, %.o, $(SOURCES))
OBJS    := $(foreach o,$(OBJS),./bin/$(o))
DEPFILES:= $(patsubst %.o, %.P, $(OBJS))

INCLUDE = 
LIBS =  -lComctl32 -lDbgHelp -lgdiplus -lSetupApi -lkernel32 -luser32 -lgdi32 -lwinspool  -lcomdlg32 -ladvapi32 -lshell32  -lole32  -loleaut32  -luuid  -lodbc32  -lodbccp32 -luxtheme

CFLAGS   = -std=c++11 -Wall -O3 -fno-omit-frame-pointer -MMD -c -DWIN32 $(INCLUDE)
LFLAGS   = -static -static-libgcc -static-libstdc++ -O3 $(LIBS)

COMPILER = g++

# TODO: will always link on Windows as filename is bin/hidterm.exe
bin/hidterm: $(OBJS)
	$(COMPILER) $(OBJS) $(LFLAGS) -o bin/hidterm

#generate dependency information and compile
bin/%.o : %.cpp
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -o $@ -MF bin/$*.P $<

#remove all generated files
clean:
	rm -rf bin

#include the dependency information
-include $(DEPFILES)

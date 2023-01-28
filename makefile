linkTarget = ImageProcesser

#Flags
CFLAGS = -std=c++17 -Ofast

#Obj files
objects = main.o \
	  $(patsubst %.cpp,%.o,$(wildcard ./src/*.cpp))

# Define the rebuildables.
rebuildables = $(objects) $(linkTarget)

# Build rules
$(linkTarget): $(objects)
	g++ -g -o $(linkTarget) $(objects) $(LIBS) $(CFLAGS)

%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)

.PHONY:	clean
clean:
	-rm -rf $(rebuildables)

.PHONY:	debug
debug:	CFLAGS += -g
debug:	$(linkTarget)
	
## Makefile for crystal-builder, v 3.7.0
## 09.30.15

## get the current OS
OSTYPE    := $(shell uname -s)

## C++ compiler/linker
CC        := g++ 
LD        := g++

WARNINGS  := -Wno-comment -Wno-return-type

## Special modifiers to control the environment when using
## this application.  Simply add any of the following to 
## the ENVR variable to change the behavior

## -DDEBUGMSG   - prints A LOT of information during the gui session and rendering session 
## -DRENDERMSG  - prints updates made during the rendering scene
## -DGUIMESG    - prints updates made during the gui session
## -DWHITEBACK  - white background and black unit cell for the rendering scene
## -DBLACKBACK  - black background and white unit cell for the rendering scene, this is the default

ENVR      := -DGUIMESG -DRENDERMSG -DBLACKBACK

## Optimizations/warnings
CFLAGS    := -O3 


## Application build characteristics.  
## This app can be built in 4 ways: GUI + GL, GUI - GL, Command line + GL, Command line - GL
## Comment/Uncomment the vars GL and GTK for the desired behavior.
## GTK controls the GUI
## GL controls the 3d graphics rendering 

## Load the OpenGL library differently based on
## which OS the user is using for the application

## Mac OS X uses "frameworks"
ifeq ($(OSTYPE), Darwin)
GL = -framework GLUT -framework OpenGL -DOGL
endif
## Linux uses "libraries"
ifeq ($(OSTYPE), Linux)
GL = -lGL -lGLU -lglut -DOGL
endif

## Load the GTK.  Use pkg-config to define the proper
## CFLAGS and LD_LIB parameters
GTK=`pkg-config --cflags --libs gtk+-2.0` -DUSE_GTK


#########################################################################################
##                             Begin building the application now,                     ##
##                             DO NOT EDIT ANYTHING BELOW !!!!!!!!                     ##
#########################################################################################

## get the list of all object files to be made
OBJ_FILES := src/objlist.txt
OBJVAR=`cat $(OBJ_FILES)`

## sub-directories containing source/header files
MODULES   := main gui util structures
SRC_DIR   := $(addprefix src/,$(MODULES))

## directory for building the application
## holds all object files in their own directories
BUILD_DIR := $(addprefix build/,$(MODULES))

## define the source files to use
SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))

## define the object files to use, use pattern replacement
## of the source files
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
##                      pattern  replacement text

## define the location of all header files
## assumed that each of them is in the appropriate
## source directory
INCLUDES  := $(addprefix -I,$(SRC_DIR))

## path to source files, all source codes end in .cpp
vpath %.cpp $(SRC_DIR)


## create the object files individually and 
## place them in build/*/*.o to keep the original
## source directories nice and clean
define make-objects
$1/%.o: %.cpp
	@echo ""
	@echo ">> Now compiling $$<"
	@echo "===================="
	$(CC) $(ENVR) $(WARNINGS) $(CFLAGS) $(GTK) $(GL) $(INCLUDES) -c $$< -o $$@
endef

## force make to build these targets for each invocation of make
.PHONY: all checkdirs clean

## define what to do when we 'make all'
all: checkdirs build/crysb

## link the object files to create the final binary
## print the objects contained within the current
## build process
build/crysb: $(OBJ)
	@echo " "
	@echo ">> compilation finished"
	@echo "========================"
	@echo ">> Now linking "
	@echo "$(OBJVAR)"
	$(LD) $(ENVR) $(WARNINGS) $(CFLAGS) $(GTK) $(GL) $^ -o $@
	@echo ">>>> Finished!"

## target for making the object folders
checkdirs: $(BUILD_DIR)

## create the object folders if they do not exist
$(BUILD_DIR):
	@mkdir -p $@

## cleanup our mess
clean:
	@rm -rf $(BUILD_DIR)

## loop through each build directory and create
## the object files, leaving them in build/*/*
$(foreach bdir,$(BUILD_DIR),$(eval $(call make-objects,$(bdir))))

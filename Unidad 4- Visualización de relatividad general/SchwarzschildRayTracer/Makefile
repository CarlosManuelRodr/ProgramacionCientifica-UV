WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -std=c++11
RESINC = 
LIBDIR = 
LIB =  -lsfml-graphics -lsfml-window -lsfml-system -pthread
LDFLAGS = 

INC_RELEASE =  $(INC)
CFLAGS_RELEASE =  $(CFLAGS) -O2 
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS)
OBJDIR_RELEASE = Obj
DEP_RELEASE = 
OUT_RELEASE = Build/SchwarzschildRayTracer


OBJ_RELEASE := 
OBJ_RELEASE += \
$(OBJDIR_RELEASE)/Main.o \
$(OBJDIR_RELEASE)/RandomGen.o \


all: release

clean: clean_release


before_release: 
	@echo ""
	@echo "####################################"
	@echo "#                                  #"
	@echo "#  Building SchwarzschildRayTracer #"
	@echo "#                                  #"
	@echo "####################################"
	@echo ""
	@test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 
	@echo ""
	@echo "#############################"
	@echo "#                           #"
	@echo "#            Done.          #"
	@echo "#                           #"
	@echo "#############################"
	@echo ""

release: before_release out_release after_release

out_release: $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(OBJ_RELEASE) $(LDFLAGS_RELEASE) $(LIBDIR_RELEASE) $(LIB_RELEASE) -o $(OUT_RELEASE)


$(OBJDIR_RELEASE)/Main.o: Source/Main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Source/Main.cpp -o $(OBJDIR_RELEASE)/Main.o

$(OBJDIR_RELEASE)/RandomGen.o: Source/RandomGen.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Source/RandomGen.cpp -o $(OBJDIR_RELEASE)/RandomGen.o


clean_release: 
	@echo "######################"
	@echo "#                    #"
	@echo "#    Cleaning up     #"
	@echo "#                    #"
	@echo "######################"
	@echo ""
	@rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	@rm -fd Obj


.PHONY: before_release after_release clean_release

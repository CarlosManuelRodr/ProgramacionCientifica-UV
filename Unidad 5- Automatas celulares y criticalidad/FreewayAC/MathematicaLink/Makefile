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
LIB =  
LDFLAGS = -pthread
OBJDIR = obj
BINDIR = bin


# Shared

clean: 
	@echo "##########################"
	@echo "#                        #"
	@echo "#      Cleaning up       #"
	@echo "#                        #"
	@echo "##########################"
	@echo ""
	@rm -rf $(OBJDIR) $(BINDIR)


# Mathematica
VERSION=10.0
MLINKDIR = /opt/Mathematica/SystemFiles/Links/MathLink/DeveloperKit/Linux-x86-64
SYS = Linux-x86-64
CADDSDIR = ${MLINKDIR}/CompilerAdditions
DEFINES = MATHEMATICA
EXTRA_CFLAGS=-m64 -D ${DEFINES} $(CFLAGS_MATH)
MPREP = ${CADDSDIR}/mprep
SRCDIR = FreewayAC

INC_MATH =  $(INC) ${CADDSDIR}
CFLAGS_MATH =  $(CFLAGS) -O2 
RESINC_MATH =  $(RESINC)
RCFLAGS_MATH =  $(RCFLAGS)
LIBDIR_MATH =  $(LIBDIR) ${CADDSDIR}
LIB_MATH = $(LIB)
LDFLAGS_MATH = $(LDFLAGS)
OBJDIR_MATH = $(OBJDIR)
DEP_MATH = 
OUT_MATH = $(BINDIR)/FreewayAC


OBJ_MATH := 
OBJ_MATH += \
$(OBJDIR_MATH)/Auxiliar.o \
$(OBJDIR_MATH)/BmpWriter.o \
$(OBJDIR_MATH)/CellularAutomata.o \
$(OBJDIR_MATH)/main.o \
$(OBJDIR_MATH)/maintm.o \

before_mathematica: 
	@echo ""
	@echo "########################################"
	@echo "#                                      #"
	@echo "# Building PEntropy (for Mathematica)  #"
	@echo "#                                      #"
	@echo "########################################"
	@echo ""
	@test -d $(BINDIR) || mkdir -p $(BINDIR)
	@test -d $(OBJDIR_MATH) || mkdir -p $(OBJDIR_MATH)

after_mathematica: 
	@echo ""
	@echo "#############################"
	@echo "#                           #"
	@echo "#            Done.          #"
	@echo "#                           #"
	@echo "#############################"
	@echo ""

mathematica : before_mathematica $(BINDIR)/FreewayAC_Mathematica after_mathematica

$(BINDIR)/FreewayAC_Mathematica : $(OBJ_MATH)
	${CXX} ${EXTRA_CFLAGS} -I${INC_MATH} $(OBJ_MATH) -L${LIBDIR_MATH} -lML64i4 -lm -lpthread -lrt -lstdc++ -ldl -luuid -o $@
	
$(OBJDIR_MATH)/maintm.o : $(OBJDIR_MATH)/maintm.c
	${CXX} -c ${EXTRA_CFLAGS} -I${INC_MATH} $(OBJDIR_MATH)/maintm.c -o $@
	
$(OBJDIR_MATH)/maintm.c : main.tm
	${MPREP} $? -o $@
	
$(OBJDIR_MATH)/Auxiliar.o: ../FreewayAC/Auxiliar.cpp
	$(CXX) $(EXTRA_CFLAGS) -c ../FreewayAC/Auxiliar.cpp -o $(OBJDIR_MATH)/Auxiliar.o

$(OBJDIR_MATH)/BmpWriter.o: ../FreewayAC/BmpWriter.cpp
	$(CXX) $(EXTRA_CFLAGS) -c ../FreewayAC/BmpWriter.cpp -o $(OBJDIR_MATH)/BmpWriter.o

$(OBJDIR_MATH)/CellularAutomata.o: ../FreewayAC/CellularAutomata.cpp
	$(CXX) $(EXTRA_CFLAGS) -c ../FreewayAC/CellularAutomata.cpp -o $(OBJDIR_MATH)/CellularAutomata.o

$(OBJDIR_MATH)/main.o: main.cpp
	$(CXX) $(EXTRA_CFLAGS) -I$(INC_MATH) -c main.cpp -o $(OBJDIR_MATH)/main.o



.DEFAULT_GOAL := mathematica
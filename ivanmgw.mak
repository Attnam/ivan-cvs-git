# Iter Vehemens ad Necem makefile for MinGW environment

# Copyrights (C) Timo Kiviluoto / IvanDev 2002

AR       = ar rs
CC       = gcc -o
FeDXDIR  = FeDX
FeDXOBJ  = $(FeDXDIR)/Source/bitmap.o $(FeDXDIR)/Source/colorbit.o $(FeDXDIR)/Source/ddutil.o $(FeDXDIR)/Source/dxutil.o $(FeDXDIR)/Source/graphics.o
FeDXASM  = $(FeDXDIR)/Source/gccblit.o
FEELDIR  = FEEL
FEELOBJ  = $(FEELDIR)/Source/error.o $(FEELDIR)/Source/femain.o
FeFileDIR  = FeFile
FeFileOBJ  = $(FeFileDIR)/Source/save.o
FeIODIR  = FeIO
FeIOOBJ  = $(FeIODIR)/Source/feio.o
FELLDIR  = FELL
FELLOBJ  = $(FELLDIR)/Source/felist.o $(FELLDIR)/Source/hscore.o
FeMathDIR  = FeMath
FeMathOBJ  = $(FeMathDIR)/Source/femath.o
FeWinDIR  = FeWin
FeWinOBJ  = $(FeWinDIR)/Source/whandler.o
LTestDIR  = LibTest
LTestBIN  = LibTest.exe
LTestOBJ  = $(LTestDIR)/Source/libtest.o
IVANDIR  = Main
IVANBIN  = Ivan.exe
IVANOBJ  = $(IVANDIR)/Source/charba.o $(IVANDIR)/Source/area.o $(IVANDIR)/Source/team.o\
           $(IVANDIR)/Source/charde.o $(IVANDIR)/Source/cont.o $(IVANDIR)/Source/dungeon.o\
           $(IVANDIR)/Source/game.o $(IVANDIR)/Source/god.o $(IVANDIR)/Source/igraph.o\
           $(IVANDIR)/Source/itemba.o $(IVANDIR)/Source/itemde.o $(IVANDIR)/Source/level.o\
           $(IVANDIR)/Source/lsquare.o $(IVANDIR)/Source/lterraba.o $(IVANDIR)/Source/lterrade.o\
           $(IVANDIR)/Source/main.o $(IVANDIR)/Source/materba.o $(IVANDIR)/Source/materde.o\
           $(IVANDIR)/Source/message.o $(IVANDIR)/Source/object.o $(IVANDIR)/Source/pool.o\
           $(IVANDIR)/Source/proto.o $(IVANDIR)/Source/roomba.o  $(IVANDIR)/Source/roomde.o\
           $(IVANDIR)/Source/script.o $(IVANDIR)/Source/square.o $(IVANDIR)/Source/stack.o\
           $(IVANDIR)/Source/wsquare.o $(IVANDIR)/Source/typeable.o $(IVANDIR)/Source/worldmap.o\
           $(IVANDIR)/Source/wskill.o $(IVANDIR)/Source/wterraba.o $(IVANDIR)/Source/wterrade.o\
           $(IVANDIR)/Source/config.o $(IVANDIR)/Source/terra.o
FLAGS = -IInclude -I$(FeDXDIR)/Include -I$(FEELDIR)/Include -I$(FeFileDIR)/Include -I$(FeIODIR)/Include -I$(FELLDIR)/Include -I$(FeMathDIR)/Include -I$(FeWinDIR)/Include -s -O3 -ffast-math -W -Wall -fvtable-thunks
LIBS = -lstdc++ -lddraw -ldxguid -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lwinmm

ivan:	$(IVANBIN)
ltest:	$(LTestBIN)

$(FeDXOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeDXASM) : %.o : %.s
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FEELOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeFileOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeIOOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FELLOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeMathOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeWinOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(LTestOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(IVANOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -I$(IVANDIR)/Include -I$(IVANDIR)/Resource

$(LTestBIN) : $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(LTestOBJ)
	@echo Compiling $(LTestBIN)...
	@$(CC) $(LTestBIN) $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(LTestOBJ) $(LIBS)

$(IVANBIN) : $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(IVANOBJ)
	@echo Compiling $(IVANBIN)...
	@$(CC) $(IVANBIN) $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(IVANOBJ) $(LIBS)

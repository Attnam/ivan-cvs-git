# Iter Vehemens ad Necem makefile

# Copyrights (C) Timo Kiviluoto / Fatal Error Productions 2002

AR       = ar rs
CC       = gcc -o
FeDXDIR  = FeDX
FeDXOBJ  = $(FeDXDIR)/Object/bitmap.o $(FeDXDIR)/Object/colorbit.o $(FeDXDIR)/Object/ddutil.o $(FeDXDIR)/Object/dxutil.o $(FeDXDIR)/Object/graphics.o
FeDXASM  = $(FeDXDIR)/Object/gccblit.o
FEELDIR  = FEEL
FEELOBJ  = $(FEELDIR)/Object/error.o $(FEELDIR)/Object/femain.o
FeFileDIR  = FeFile
FeFileOBJ  = $(FeFileDIR)/Object/save.o
FeIODIR  = FeIO
FeIOOBJ  = $(FeIODIR)/Object/feio.o
FELLDIR  = FELL
FELLOBJ  = $(FELLDIR)/Object/felist.o $(FELLDIR)/Object/hscore.o
FeMathDIR  = FeMath
FeMathOBJ  = $(FeMathDIR)/Object/femath.o
FeWinDIR  = FeWin
FeWinOBJ  = $(FeWinDIR)/Object/whandler.o
LTestDIR  = LibTest
LTestBIN  = LibTest.exe
LTestOBJ  = $(LTestDIR)/Object/libtest.o
IVANDIR  = Main
IVANBIN  = Ivan.exe
IVANOBJ  = $(IVANDIR)/Object/charba.o $(IVANDIR)/Object/area.o $(IVANDIR)/Object/team.o\
           $(IVANDIR)/Object/charde.o $(IVANDIR)/Object/cont.o $(IVANDIR)/Object/dungeon.o\
           $(IVANDIR)/Object/game.o $(IVANDIR)/Object/god.o $(IVANDIR)/Object/igraph.o\
           $(IVANDIR)/Object/itemba.o $(IVANDIR)/Object/itemde.o $(IVANDIR)/Object/level.o\
           $(IVANDIR)/Object/lsquare.o $(IVANDIR)/Object/lterraba.o $(IVANDIR)/Object/lterrade.o\
           $(IVANDIR)/Object/main.o $(IVANDIR)/Object/materba.o $(IVANDIR)/Object/materde.o\
           $(IVANDIR)/Object/message.o $(IVANDIR)/Object/object.o $(IVANDIR)/Object/pool.o\
           $(IVANDIR)/Object/proto.o $(IVANDIR)/Object/roomba.o  $(IVANDIR)/Object/roomde.o\
           $(IVANDIR)/Object/script.o $(IVANDIR)/Object/square.o $(IVANDIR)/Object/stack.o\
           $(IVANDIR)/Object/wsquare.o $(IVANDIR)/Object/typeable.o $(IVANDIR)/Object/worldmap.o\
           $(IVANDIR)/Object/wskill.o $(IVANDIR)/Object/wterraba.o $(IVANDIR)/Object/wterrade.o\
           $(IVANDIR)/Object/config.o
FLAGS = -IInclude -IFeDX/Include -IFEEL/Include -IFeFile/Include -IFeIO/Include -IFELL/Include -IFeMath/Include -IFeWin/Include -O3 -ffast-math -W -Wall -fvtable-thunks
LIBS = -lstdc++ -lddraw -ldxguid -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lwinmm

ivan:	$(IVANBIN)
ltest:	$(LTestBIN)

clean:	;
	@echo Deleting object files...
	@if exist $(ROOTDIR)\Object\*.o del $(ROOTDIR)\Object\*.o
	@echo Deleting library...
	@if exist $(ROOTDIR)\$(LIBRARY) del $(ROOTDIR)\$(LIBRARY)

$(FeDXOBJ) : $(FeDXDIR)/Object/%.o : $(FeDXDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeDXASM) : $(FeDXDIR)/Object/%.o : $(FeDXDIR)/Source/%.s
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FEELOBJ) : $(FEELDIR)/Object/%.o : $(FEELDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeFileOBJ) : $(FeFileDIR)/Object/%.o : $(FeFileDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeIOOBJ) : $(FeIODIR)/Object/%.o : $(FeIODIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FELLOBJ) : $(FELLDIR)/Object/%.o : $(FELLDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeMathOBJ) : $(FeMathDIR)/Object/%.o : $(FeMathDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeWinOBJ) : $(FeWinDIR)/Object/%.o : $(FeWinDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(LTestOBJ) : $(LTestDIR)/Object/%.o : $(LTestDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(IVANOBJ) : $(IVANDIR)/Object/%.o : $(IVANDIR)/Source/%.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -IMain/Include -IMain/Resource

$(LTestBIN) : $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(LTestOBJ)
	@echo Compiling $(LTestBIN)...
	@$(CC) $(LTestBIN) $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(LTestOBJ) $(LIBS)

$(IVANBIN) : $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(IVANOBJ)
	@echo Compiling $(IVANBIN)...
	@$(CC) $(IVANBIN) $(FeDXOBJ) $(FeDXASM) $(FEELOBJ) $(FeFileOBJ) $(FeIOOBJ) $(FELLOBJ) $(FeMathOBJ) $(FeWinOBJ) $(IVANOBJ) $(LIBS)
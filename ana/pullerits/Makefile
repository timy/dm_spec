DIR_BASE	=	../..
include $(DIR_BASE)/Makefile.in

SRC		=	main.cc
OBJ		=	$(SRC:%.cc=%.o)
EXE		=	test

OBJ_CORE	=	$(join $(addsuffix $(DIR_SRC)/, $(dir $(SRC_CORE))), \
				$(notdir $(SRC_CORE:.cc=.o)))

%.o : %.cc
	$(CC) $< $(CFLAGS) -c -o $@

all : $(EXE)

$(EXE) : $(OBJ_CORE) $(OBJ)
	$(CC) $(OBJ_CORE) $(OBJ) $(LFLAGS) -o $@

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) *~

cleanall: clean
	$(RM) res/*.dat
	$(RM) fig/*.png
	$(RM) fig/*.svg

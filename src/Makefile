DIR_BASE	=	..
include $(DIR_BASE)/Makefile.in

SRC		=	main_mpi.cc
OBJ		=	$(SRC:%.cc=%.o)
EXE		=	$(DIR_BASE)/test

OBJ_CORE	=	$(join $(addsuffix $(DIR_SRC)/, $(dir $(SRC_CORE))), \
				$(notdir $(SRC_CORE:.cc=.o)))
CFLAGS	+= 	-DUSE_MPI

%.o : %.cc
	$(CC) $< $(CFLAGS) -c -o $@

all : $(EXE)

$(EXE) : $(OBJ_CORE) $(OBJ)
	$(CC) $(OBJ_CORE) $(OBJ) $(LFLAGS) -o $@

clean:
	$(RM) $(OBJ_CORE)
	$(RM) $(OBJ)
	$(RM) *~

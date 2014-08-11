SRC	=	para.cc rotation.cc coord.cc repr.cc bath.cc random.cc \
		field.cc prop.cc eom.cc polar.cc timestep.cc help.cc \
		seidner.cc \
		output.cc \
		cfg.cc mpi_cfg.cc util.cc measure.cc file.cc \
		esmb.cc mpic.cc \
		seidner_io.cc mvar.cc config.cc \
		postproc.cc main_mpi.cc

#pol_io.cc pol_io_line.cc

OBJ	=	$(SRC:%.cc=%.o)

%.o : %.cc
	$(CC) $< $(CFLAGS) -c -o $@

all : $(EXE)

$(EXE) : $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@ 

clean:
	$(RM) $(OBJ)
	$(RM) *~

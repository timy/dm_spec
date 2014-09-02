EXE =	test
SRC	=	main.cc

DIR_INC =  ../../inc
DIR_CORE = ../../src
OBJ_CORE = $(DIR_CORE)/para.o $(DIR_CORE)/spec_a.o $(DIR_CORE)/coord.o \
	$(DIR_CORE)/repr.o $(DIR_CORE)/bath.o $(DIR_CORE)/config.o \
	$(DIR_CORE)/polar.o $(DIR_CORE)/prop.o $(DIR_CORE)/eom.o \
	$(DIR_CORE)/cfg.o $(DIR_CORE)/mpi_cfg.o $(DIR_CORE)/field.o \
	$(DIR_CORE)/file.o $(DIR_CORE)/rotation.o $(DIR_CORE)/mvar.o \
	$(DIR_CORE)/esmb.o $(DIR_CORE)/util.o $(DIR_CORE)/mpic.o \
	$(DIR_CORE)/timestep.o $(DIR_CORE)/help.o \
	$(DIR_CORE)/random.o \
	$(DIR_CORE)/output.o $(DIR_CORE)/seidner.o \
	$(DIR_CORE)/seidner_io.o $(DIR_CORE)/pol_io.o $(DIR_CORE)/pol_io_line.o


# for the laptop
#SRC	=	para.cc field.cc prop.cc eom.cc polar.cc spec.cc setup.cc main_mpi.cc


# The default one for pinot
CC		=	/data1/home/tianmin/software/mpich/bin/mpicxx
CFLAGS	=	-Wall -O3 -I$(DIR_INC)
LFLAGS	=	\
	/data1/home/tianmin/software/mpich/lib/libmpich.a  \
	/data1/home/tianmin/software/fftw3/lib/libfftw3.a \
	/data1/home/tianmin/software/lapack/lib/liblapack.a \
	/data1/home/tianmin/software/lapack/lib/librefblas.a \
	/usr/lib64/gcc/x86_64-suse-linux/4.3/libgfortran.a \
	-lgsl -lblas /data1/home/tianmin/software/cblas/lib/cblas_LINUX.a \
	-lconfig

OBJ	=	$(SRC:%.cc=%.o) 

%.o : %.cc
	$(CC) $< $(CFLAGS) -c -o $@

all : $(EXE)

$(EXE) : $(OBJ)
	$(CC) $(OBJ) $(OBJ_CORE) $(LFLAGS) -o $@ 

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -f *~

cleanall: clean
	rm -f res/*.dat
	rm -f fig/*.svg
	rm -f fig/*.png

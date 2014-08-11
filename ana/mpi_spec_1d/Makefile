EXE =	test
SRC	=	main.cc

DIR_INC =  ../../inc/2
DIR_CORE = ../../src/2
OBJ_CORE = $(DIR_CORE)/para.o $(DIR_CORE)/spec_a.o $(DIR_CORE)/coord.o \
	$(DIR_CORE)/repr.o $(DIR_CORE)/bath.o $(DIR_CORE)/setup.o \
	$(DIR_CORE)/polar_a.o $(DIR_CORE)/prop.o $(DIR_CORE)/eom.o \
	$(DIR_CORE)/cfg.o $(DIR_CORE)/mpi_cfg.o $(DIR_CORE)/field.o \
	$(DIR_CORE)/file.o $(DIR_CORE)/rotation.o \
	$(DIR_CORE)/output.o


# for the laptop
#SRC	=	para.cc field.cc prop.cc eom.cc polar.cc spec.cc setup.cc main_mpi.cc


# The default one for pinot
CC		=	icc
CFLAGS	=	-Wall -O3 -I$(DIR_INC)
LFLAGS	=	\
	/data1/home/tianmin/software/fftw3/lib/libfftw3.a \
	/data1/home/tianmin/software/lapack/lib/liblapack.a \
	/data1/home/tianmin/software/lapack/lib/librefblas.a \
	/usr/lib64/gcc/x86_64-suse-linux/4.3/libgfortran.a \
	-lgsl -lblas /data1/home/tianmin/software/cblas/lib/cblas_LINUX.a

OBJ	=	$(SRC:%.cc=%.o) 

%.o : %.cc
	$(CC) $< $(CFLAGS) -c -o $@

all : $(EXE)

$(EXE) : $(OBJ)
	$(CC) $(OBJ) $(OBJ_CORE) $(LFLAGS) -o $@ 

clean:
	rm -f $(OBJ)
	rm -f *~

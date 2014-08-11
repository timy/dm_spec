DIR_BASE	=	$(shell pwd)
DIR_INC		=	$(DIR_BASE)/inc
DIR_SRC		=	$(DIR_BASE)/src
DIR_PKG		=	$(DIR_BASE)/pkg
DIR_CFG		=	$(DIR_BASE)/cfg
DIR_FIG		=	$(DIR_BASE)/fig

# MPI for pinot
CC		=	/data1/home/tianmin/software/mpich/bin/mpicxx
CFLAGS	=	-Wall -O3 -I$(DIR_INC) -DUSE_MPI
LFLAGS	=	\
	/data1/home/tianmin/software/mpich/lib/libmpich.a  \
	/data1/home/tianmin/software/fftw3/lib/libfftw3.a \
	/data1/home/tianmin/software/lapack/lib/liblapack.a \
	/data1/home/tianmin/software/lapack/lib/librefblas.a \
	/usr/lib64/gcc/x86_64-suse-linux/4.3/libgfortran.a \
	/data1/home/tianmin/software/gsl/lib/libgsl.a \
	-lblas /data1/home/tianmin/software/cblas/lib/cblas_LINUX.a \
	/data1/home/tianmin/software/libconfig/lib/libconfig.a

# # MPI for lemaitre2
# CC	=	mpicxx
# CFLAGS	=	-Wall -O3 -I$(DIR_INC) -DUSE_MPI
# LFLAGS	=	-L/usr/lib64/atlas -lcblas -lgsl -llapack -lconfig

# # MPI for NIC4
# CC	=	mpicxx
# CFLAGS	=	-Wall -O3 -I$(DIR_INC) -DUSE_MPI
# LFLAGS	=	-L/usr/lib64/atlas -l:libcblas.so.3 -l:liblapack.so.3 \
# 		-lconfig -lgsl

# For my laptop
# CC		=	g++
# CFLAGS	=	-Wall -g -I$(DIR_INC)
# LFLAGS	=	-lfftw3 -llapack -lm -lgsl -lconfig

RM		=	rm -f
EXE		=	$(DIR_BASE)/test

export CC
export CFLAGS
export LFLAGS
export EXE

all:
	cd $(DIR_SRC) && make

clean:
	cd $(DIR_SRC) && make clean
	$(RM) $(DIR_INC)/*~
	$(RM) $(DIR_CFG)/*~
	$(RM) $(DIR_PKG)/*~ $(DIR_PKG)/*.pyc
	$(RM) $(DIR_BASE)/*~ $(DIR_BASE)/*.pyc

run:
	mkdir -p res
	mkdir -p raw
	nohup mpirun -n 20 ./test &

clean_exe:
	$(RM) $(EXE)

clean_dat:
	$(RM) res/*.dat
	$(RM) raw/*.dat
	$(RM) nohup.out

clean_fig:
	$(RM) $(DIR_FIG)/*.png $(DIR_FIG)/*.svg

cleanall: clean_exe clean_dat clean_fig clean
	cd ana && make cleanall

info:
	echo $(DIR_BASE)
	echo $(DIR_SRC)
	echo $(DIR_INC)

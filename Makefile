DIR_BASE	=	$(shell pwd)
EXE		=	$(DIR_BASE)/test
include Makefile.in

all: build_src #build_ana

build_src:
	cd $(DIR_SRC) && make

build_ana: build_src
	for subdir in $(DIR_ANA_SUB) ; do \
		cd $(DIR_ANA)/$$subdir && make ; \
	done
clean:
	cd $(DIR_SRC) && make clean
	$(RM) $(DIR_INC)/*~
	$(RM) $(DIR_CFG)/*~
	$(RM) $(DIR_PKG)/*~ $(DIR_PKG)/*.pyc
	$(RM) $(DIR_BASE)/*~ $(DIR_BASE)/*.pyc

run:
	mkdir -p res
	nohup mpirun -n 20 ./test &

clean_exe:
	$(RM) $(EXE)

clean_dat:
	$(RM) res/*.dat
	$(RM) nohup.out

clean_fig:
	$(RM) $(DIR_FIG)/*.png $(DIR_FIG)/*.svg

cleanall: clean_exe clean_dat clean_fig clean
	for subdir in $(DIR_ANA_SUB) ; do \
		cd $(DIR_ANA)/$$subdir && make cleanall ; \
	done

dist:
	$(RM) -r dist
	rsync -az \
		--exclude='*.o' --exclude='*.pyc' --exclude='test' \
		--exclude='*.dat' --exclude='*.log' --exclude='*.out' \
		--exclude='*~' --exclude='res/' --exclude='fig/' \
		$(DIR_INC) $(DIR_SRC) $(DIR_PKG) $(DIR_CFG) $(DIR_ANA) \
		Makefile Makefile.in *.py LICENSE README.md \
		dist/

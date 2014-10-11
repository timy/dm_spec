# Description
Calculation of nonlinear spectroscopy from non-perturbative propagation of
density matrix defined in spectral representation.

# Applications
For example, 2D third-order spectra can be obtained for photon-echo experiments.

# Reference
1. T. Mančal, A. V. Pisliakov, and G. R. Fleming,
The Journal of Chemical Physics 124, 234504 (2006).

2. B. Brüggemann, P. Kjellberg, and T. Pullerits,
Chemical Physics Letters 444, 192 (2007).

# Quick Startup

Before starting up, you need to set `Makefile.in` to make sure it is compilable.
The options to check are `CC`, `CFLAGS` and `LFLAGS`.
The program requires following dependencies,
* BLAS and LAPACK - Matrix inverse is used for Seidner's method
* GSL - Too lazy to write my own ODE solver
* MPICH2 - neither am I that patient to wait for results...
* libconfig - used to read [parameters.cfg](cfg/parameters.cfg) with nice format.
  But probably I will try JSON soon.

Several templates have already been there so just choose the suitable one and
comment out all the others. Then you can check it by follow the next explanations
step by step.

## 1D Nonlinear spectroscopy for single molecule (Seidner's method):
This is the option you can play on a destop machine.

* In file [src/Makefile](src/Makefile), set
  `SRC = seidner_main_single.cc`
* In file [parameters.cfg](cfg/parameters.cfg), set
  `esmb.n_esmb = 1`,
  `mvar.ny = 1`,
  `pols.ppar_calc_method = "seidner"`
* Be sure you create the folder `./res` to store results
* Run `./test`
* After the main program exits, create folder `./fig` to store image files. Then run
  `python plot_ppar.py` to see nonlinear signals in different directions. The index in
  file name specified in the script [plot_ppar.py](plot_ppar.py) suggest the direction
  of the signal,
  e.g., 37 indicates the wave vector direction of [-1, 1, 1], whose correspondence
  can be found in file [parameters.cfg](cfg/parameters.cfg). `coo2` in the file name
  is for z-direction of the emitting field.

## 2D Nonlinear spectroscopy for single molecule (Seidner's method):
This option should be run on nodes with MPI avaiable. Something like 20 nodes are good.
You can configure the required number of nodes by just modifying
`nohup mpirun -n NUMBER ./test &` in file [Makefile](./Makefile)

* In file [src/Makefile](src/Makefile), set
  `SRC = main_mpi.cc`
* In file [parameters.cfg](cfg/parameters.cfg), set
  `esmb.n_esmb = 1`,
  `mvar.ny = 20`,
  `pols.ppar_calc_method = "seidner"`
* Be sure you create the folder `./res` to store results before running.
* Run `make run`
* After the main program exists, go to [ana/seidner](ana/seidner), `make`, then run `./test`.
  Be sure `res` is also present in this folder.
* Check result by running `python plot_ppar_2d.py`. The naming convention is simlar like
  that in 1D case. Enjoy!

# Code Specification

## Number of equations of motion

For an `n`-level system (`n_lvl = n`), there are `n` terms for population
($rho_{ii}$, which has only real part),
and `n*(n-1)/2` terms contributing to a half coherence part
($rho_{ij}, i \neq j$, which has real and imaginary parts),
the other half are just the complex conjugates of the former.
Therefore, if the equations are all for real variables,
the number of equation of motions is given by
`n_eom = n + 2 * (n*(n-1)/2) = n * n`,
as is defined in [para.cc](src/para.cc#L105).

## Array of polarization:

The array for generic polarization is prepared or cleaned in [src/polar.cc](src/polar.cc).
The polarization for Seidner's method has an extra rank in order to distinguish results
from different spatial phases:

- Generic: `pol[nx][n_dpl][n_dim]`

- Seidner: `pol[n_phase][nx][n_dpl][n_dim]`

The variable `nx` may include all varying variables. In the simplest case,
`nx = nt`, which is just a 1D grid along the detection time.
For 2D case, the coherence time $\tau$ is also varied for typical 2D spectroscopy,
`nx = nt * n_tau`.

## Directions of field vectors

The k-unit-vector in lab frame if you define the euler angle (phi, theta, psi):

$$ kuvL_x = + sin(\phi) sin(\theta) $$

$$ kuvL_y = - cos(\phi) sin(\theta) $$

$$ kuvL_z =             cos(\theta) $$

The E-unit-vector in lab frame if you define angle `chi` relative to the k-unit-vector:

$$ EuvL_x = + sin(\phi) cos(\theta) cos(\psi+\chi) + cos(\phi) sin(\psi+\chi) $$

$$ EuvL_y = - cos(\phi) cos(\theta) cos(\psi+\chi) + sin(\phi) sin(\psi+\chi) $$

$$ EuvL_z =                                      - sin(\theta) cos(\psi+\chi) $$

So in order to control field polarization,
If you want EuvL_x = 0:
$$ \chi = -\psi + atan( - cos(\theta) * tan(\phi) ) $$

If you want EuvL_y = 0:
$$ \chi = -\psi + atan( + cos(\theta) / tan(\phi) ) $$

If you want EuvL_z = 0:
$$ \chi = -\psi + \pi / 2 $$

## Config Data Format (CDF)

The config file may contain a chunk of data that is not easy to be expressed with
key-value pairs as shown in [parameters.cfg](cfg/parameters.cfg).
So we introduced another config file also with the suffix name `.cfg`.
It allows defining data directly in the file.
It is useful if you have input parameters from output directly generated by other programs.
In order to read config files with different formats, one needs to define their
formats and transfer data to `para` structure by invoking `sscanf` function.
The example/template can be found in file [repr_generic.cc](src/repr_generic.cc).
Two steps are needed:

- Call [`cdf_read( file_name, n_lines, sscanf_structure, ps )`](src/repr_generic.cc#L26)
- Define the callback funciton [`sscanf_structure`](src/repr_generic.cc#L32)

The callback functions [`sscanf_st_energy`](src/repr_generic.cc#L32)
and [`sscanf_st_dipole`](src/repr_generic.cc#L41) show how energy and
dipole are defined. In the routine, it is also a good place to check if config data
has any problem.

The current format in each line for energy and dipole data are as following,

- [energy.cfg](cfg/energy.cfg) (2 columns):

| 1            | 2            |
|--------------| -------------|
| level_index  | energy_value |

- [dipole.cfg](cfg/dipole.cfg) (5 columns):

| 1             | 2             | 3        | 4        | 5        |
|---------------|---------------|----------|----------|----------|
| level_index_a | level_index_b | dipole_x | dipole_y | dipole_z |

- Each line in configuration file should contain characters less than 1024
  as defined in [cdf.cc](src/cdf.cc#L21).
- One can include blank or comment lines (`//` like in C++) in config files
  which are neglected.
- The number of data lines (not including blank or comment lines) in file
  [energy.cfg](cfg/energy.cfg)
  should agree with `n_lvl` defined in [parameters.cfg](cfg/parameters.cfg#L3).
  Similarly, the number of data lines in file
  [dipole.cfg](cfg/dipole.cfg)
  should agree with `n_dpl` defined in [parameters.cfg](cfg/parameters.cfg#L4).
  If not, function [cdf_read](src/cdf.cc#L75-78) will complain.


# TODO List

- [ ] let main_mpi.cc work for repr.type = GENERIC
- [ ] add dacay rate for new ODE
- [ ] index with JSON format
- [ ] parameters with JSON format

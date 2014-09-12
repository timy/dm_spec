# Description
Calculation of nonlinear spectroscopy from non-perturbative propagation of density matrix defined in spectral representation.

# Applications
For example, 2D third-order spectra can be obtained for photon-echo experiments.

# Reference
1. T. Mančal, A. V. Pisliakov, and G. R. Fleming, The Journal of Chemical Physics 124, 234504 (2006).
2. B. Brüggemann, P. Kjellberg, and T. Pullerits, Chemical Physics Letters 444, 192 (2007).

# Quick Startup

Nonlinear spectroscopy for single molecule:
* In file `src/Makefile`, set
  `SRC = seidner_main_single.cc`
* In file `parameters.cfg`, set
  `esmb.n_esmb = 1`,
  `mvar.ny = 1`,
  `pols.ppar_calc_method = "seidner"`
* Be sure you create the folder `./res` to store results
* Run `./test`
* After the main programme exits, create folder `./fig` to store image files. Then run
  `python plot_ppar.py` to see nonlinear signals in different directions. The index in
  file name specified in the script `plot_ppar.py` suggest the direction of the signal,
  e.g., 37 indicates directions in [-1, 1, 1], whose correspondence can be found in file
  `parameters.cfg`. `coo2` is for z-direction of the emitting field.

# Code Specification

Array of polarization:
Generic: pol[nx][n_dpl][n_dim]
Seidner: pol[n_phase][nx][n_dpl][n_dim]

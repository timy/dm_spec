# Description
Calculation of nonlinear spectroscopy from non-perturbative propagation of density matrix defined in spectral representation.

# Applications
For example, 2D third-order spectra can be obtained for photon-echo experiments.

# Reference
1. T. Mančal, A. V. Pisliakov, and G. R. Fleming, The Journal of Chemical Physics 124, 234504 (2006).
2. B. Brüggemann, P. Kjellberg, and T. Pullerits, Chemical Physics Letters 444, 192 (2007).

# Code Specification

Generic: pol[nx][n_dpl][n_dim]

Seidner: pol[n_phase][nx][n_dpl][n_dim]

TODO: don't forget to add function to calculate i_file = i_dpl * n_dim + i_dim

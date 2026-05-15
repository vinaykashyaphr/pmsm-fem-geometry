# pmsm-fem-geometry

> Parametric 2D FEM pre-processor for outrunner PMSM cross-section geometry and mesh generation in C++23 using GMSH OCC kernel.

---

## Overview

`pmsm-fem-geometry` constructs the parametric 2D cross-section geometry and triangular mesh of an outrunner Permanent Magnet Synchronous Motor (PMSM) — the pre-processing stage of a finite element magnetostatic solver.

The target governing equation is the 2D magnetostatic Poisson equation:

```
∇²Az = −μ Jz
```

where `Az` is the magnetic vector potential, `μ` is material permeability, and `Jz` is winding current density. Solving this over the motor cross-section yields magnetic flux density distribution for torque and back-EMF computation.

---

## Features

- Parametric stator slot geometry from derived angular parameters
- OCC boolean cut operations for slotted annulus generation
- Rotational slot replication across all poles
- Airgap annular region with fine uniform mesh
- Rotor permanent magnet arc parameterization
- Region-aware `MathEval` mesh size fields — graded in stator, uniform in airgap
- Physical group naming for FEM boundary condition assignment
- CMake build with vendored Eigen and GMSH dependencies

---

## Requirements

| Dependency | Version | Purpose |
|---|---|---|
| [GMSH](https://gmsh.info) | 4.x | Geometry and mesh generation via OCC kernel |
| [Eigen](https://eigen.tuxfamily.org) | 3.4+ | Sparse linear algebra (header-only) |
| CMake | 3.20+ | Build system |
| GCC / Clang | 13+ / 16+ | C++23 compiler |

---

## Installation

```bash
git clone https://github.com/vinaykashyaphr/pmsm-fem-geometry
cd pmsm-fem-geometry
```

Download the [GMSH SDK](https://gmsh.info/#Download) and place it in `deps/gmsh/`.

Download [Eigen](https://eigen.tuxfamily.org) headers and place them in `deps/eigen/`.

```
deps/
├── gmsh/
│   ├── include/
│   └── lib/
└── eigen/
    └── Eigen/
```

---

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## Run

```bash
./build/simple_inrunner
```

Opens the GMSH GUI displaying the generated 2D motor cross-section mesh.

---

## Status

Geometry and mesh generation — complete.
FEM sparse stiffness matrix assembler using Eigen — in progress.

---


## Reference

This project is based on the motor design methodology and geometry from:

> Wu, Y.-C. and Lin, B.-W. (2012). **Computer-Aided Design of a Brushless DC Motor with Exterior-Rotor Configuration**. *Computer-Aided Design & Applications*, 9(4), 457–469.
> DOI: [10.3722/cadaps.2012.457-469](https://doi.org/10.3722/cadaps.2012.457-469)

The equivalent magnetic circuit model, 20-pole/18-slot exterior-rotor configuration, and pole shoe geometry are derived from this paper. The FEA results published therein serve as the verification target for the solver under development.

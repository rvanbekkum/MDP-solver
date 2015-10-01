# MDP-solver
Tool for solving basic MDP problems using the MADP toolbox and creating inputs for PRISM for model checking.
This is based on the `MMDP_VI` example solver that comes with the MADP-toolbox.
The MDP-solver solves MDP models and using the found solution creates a `.nm` file which can be used in PRISM for model checking.

## Prerequisites
To build MDP-solver you will need to install the following tools (e.g., with a package-manager like `apt-get`) on your system:
* `cmake`
* `g++`

## Building MDP-solver
Change directory to the `build` folder and run `cmake ..`.
If building the Makefile succeeds, subsequently run `make`.
The binary will then be outputted into the `bin` directory.

## Using MDP-solver

Solve the `truck` example (MDP model defined in `truck.pomdp`) with discount-factor 0.9 and horizon infinite:

```./mdp-solver ../problems/truck.pomdp  --inf --discount=0.9```

Check out the follow page to see how problems can be defined:

http://www.pomdp.org/code/pomdp-file-spec.html


After `mdp-solver` has finished, the PRISM `.nm` input files are written to a `results` folder in the same directory as the `.pomdp` problem model file. The resulting `.nm` file can then be loaded into PRISM.

Run `./mdp-solver --help` to see parameters that can be set.

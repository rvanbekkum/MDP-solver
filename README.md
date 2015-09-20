# MDP-solver
Tool for solving basic MDP problems using the MADP toolbox
This is based on the `MMDP_VI` example solver that comes with the MADP-toolbox.

# Prerequisites
To build MDP-solver you will need to install the following tools (e.g., with a package-manager like `apt-get`) on your system:
* `cmake`
* `g++`

# Building MDP-solver
Change directory to the `build` folder and run `cmake ..`.
If building the Makefile succeeds, subsequently run `make`.
The binary will then be outputted into the `bin` directory.

# Using MDP-solver

Example run:

```./mdp-solver ../problems/example.pomdp  --inf --discount=0.9```

Check out the follow page to see how problems can be defined:

http://www.pomdp.org/code/pomdp-file-spec.html

(Todo: Update this description)

# PIGEON: Private Inference of Neural Networks

This branch extends HPMPC with private inference capabilities. The framework is structured as follows.

* [FlexNN](https://github.com/chart21/flexNN/tree/hpmpc): A templated neural network inference engine that performs the forward pass of a CNN generically.
* `Programs/functions` contains MPC-generic implementations of functions such as ReLU.
* `Protocols` Implements the MPC protocols and primitives that are required by `Programs/functions`.
* [Pygeon](https://github.com/chart21/Pygeon): Python scripts for exporting models and datsets from PyTorch to the inference engine. 

The following protocols are currently fully supported by PIGEON.

3-PC: OECL (Ours, Protocol 5), TTP (Protocol 6)

4-PC: OEC-MAL (Ours, Protocol 12), TTP (Protocol 7)

## Getting Started

You can use the provided Dockerfile or set up the project manually.
The only dependencies are OpenSSL and Eigen. Install on your target system, for instance via ```apt install libssl-dev libeigen3-dev```.

First, initialize the submodules.
> git submodule update --init --recursive

To export a model or dataset from PyTorch use [Pygeon](https://github.com/chart21/pygeon) and save the resulting bin files to `SimpleNN/dataset` or `SimpleNN/model_zoo`. Then define the model architecture in `SimpleNN/architectures/`. Finally, specify your datasetfile and modelfile in `Programs/functions/NN.hpp` and if it does not exist, add a FUNCTION_IDENTIFIER for your function.
```
    cfg.save_dir = "./SimpleNN/model_zoo"; // Your model should be in this folder
    cfg.data_dir = "./SimpleNN/dataset"; // Your dataset should be in this folder
    cfg.image_file = "cifar10-test-images.bin"; // Your test images, exported by Pygeon
    cfg.label_file = "cifar10-test-labels.bin"; // Your test labels, exported by Pygeon
    cfg.pretrained = "AlexNet.bin"; // Your model parameters, exported by Pygeon
```

Existing networks are defined in `SimpleNN/architectures`. `Programs/functions/NN.hpp` includes a FUNCTION_IDENTIFIER for different model architectures and datasets (for instance 70 for RestNet18 on CIFAR-10). 
You can select a protocol and function in the file `config.h`. The config contains numerous settings. Here are just some examples: 
* Should the weights be public or private?
* Which party should share the dataset, Which party should share the model parameters?
* How many bits should be used for the fractional part, how many bits for the total bitlength?
* Which truncation approach should be used? Should ReLUs by default be evaluated with reduced Bitwidth?
* Should the inference be optimized for latency, bandwidth, Online Phase, or total execution time? Should a Preprocessing phase be used?


The following commands are a quick way to compile the current configuration for a 3-PC protocol and run all executables locally. This compiles all player executables using g++ with -Ofast and runs all executables on localhost on the same machine.
> ./scripts/config.sh -p all3

> ./scripts/run_loally.sh -n 3

For a 4-PC protocol, you can run.

> ./scripts/config.sh -p all4

> ./scripts/run_loally.sh -n 4

## Configuration and Compilation

Most configuration is contained in the file `config.h`. Take a careful look at its documentation and different options.

The Split-Roles scripts transform a protocol into a homogeneous protocol by running multiple executables with different player assignments in parallel.

The following script compiles six executables of a 3-PC protocol for player 2 (all player combinations) to run a homogeneous 3-PC protocol on three nodes using Split-Roles.
> ./scripts/split-roles-3-compile.sh -p 2

The following script compiles 18 executables of a 3-PC protocol for player 3 to run a homogeneous 3-PC protocol on four nodes using Split-Roles.
> ./scripts/split-roles-3to4-compile.sh -p 3

The following script compiles 24 executables of a 4-PC protocol for player 0 to run a homogeneous 4-PC protocol on four nodes using Split-Roles.
> ./scripts/split-roles-4-compile.sh -p 0


### Execution

In a distributed setup, you need to specify the IP addresses for each party and run one executable on each node.

Execute P0 executable.
> ./run-P0.o IP_P1 IP_P2

Execute P1 executable.
> ./run-P1.o IP_P0 IP_P2

Execute P2 executable.
> ./run-P2.o IP_P0 IP_P1


Run Split-Roles (3) executables for Player 0.
> ./scripts/split-roles-3-execute.sh -p 0 -a IP_P0 -b IP_P1 -c IP_P2 -d IP_P3

To run all players locally on one machine, omit the IP addresses or set them to 127.0.0.1, and use -p all
> ./scripts/split-roles-3-execute.sh -p all


# MP-SPDZ

It is possible to run computation with bytecode compiled by [MP-SPDZ](https://github.com/data61/MP-SPDZ). As most of [MP-SPDZ](https://github.com/data61/MP-SPDZ/releases?page=1) 0.3.8 is supported.

## 1. Setup

1. Create two directories in `MP-SPDZ/`: `Schedules` for the schedule file and `Bytecodes` for the respective bytecode file
```sh
mkdir -p "./MP-SPDZ/Schedules" "./MP-SPDZ/Bytecodes"
```

2. Compile `.mpc` files in `MP-SPDZ/Functions`

Assuming MP-SPDZ is installed in `$MPSPDZ` copy the desired `<file>.mpc` into `"$MPSPDZ"/Programs/Source` and compile them using their compiler with the bit length specified in `config.h`.

```sh
cp "./MP-SPDZ/Functions/<file.mpc>" "$MPSPDZ"/Programs/Source/
cd "$MPSDZ" && ./compile.py -R "<BITLENGTH>" "<file>"
```

3. Move the bytecode/schedule file into the respective directory

```sh
mv "$MPSDZ/Programs/Schedules/<file>.sch" "./MP-SPDZ/Schedules/"
mv "$MPSDZ/Programs/Bytecode/*" "./MP-SPDZ/Bytecodes/"
```

## 2. To run computation for 3 Players

Make sure to use the correct `FUNCTION_IDENTIFIER` and `BITLENGTH`:
```sh
./scripts/config.sh -p all3 -f "<FUNTION_IDENTIFIER>" -a "<BITLENGTH>"
./scripts/run_locally -n 3
```

## Run the example functions

Currently there are multiple example functions in `MP-SPDZ/Functions`

Mapping from `FUNCTION_IDENTIFIER` $\to$ `.mpc` file:

`FUNCTION_IDENTIFIER` | `.mpc`
----------------------|-------
`500` | `tutorial.mpc`
`501` | `custom.mpc` (can be used for your own functions)
`505` | `int_test.mpc/int_test_32.mpc` (depending on `BITLENGTH` equal to  `64` or `32`)
`506-525` | functions used for benchmarks (mapping can be found in `MP-SPDZ/bench_scripts/measurement.sh`)

## Install MP-SPDZ compiler

You need to install [MP-SPDZ](https://github.com/data61/MP-SPDZ/releases?page=1) 0.3.8 to compile your `<filename>.mpc`
```sh
wget https://github.com/data61/MP-SPDZ/releases/download/v0.3.8/mp-spdz-0.3.8.tar.xz
tar xvf mp-spdz-0.3.8.tar.xz
```

## Run your own functions

As mentioned in `1. setup` copy the bytecode file and schedule file into the correct Directory (`./MP-SPDZ/Schedules/`, `./MP-SPDZ/Bytecodes/` respectively)
make sure that for both MP-SPDZ and this project you are using the same bit length for compilation.

Using function `501`/`custom.mpc`

Rename the schedule file `custom.sch` and compile with `FUNCTION_IDENTIFIER = 501`
```sh
mv "./MP-SPDZ/Schedules/<file>.sch" "./MP-SPDZ/Schedules/custom.sch"
./scripts/config.sh -p all3 -f 501 -a "<BITLENGTH>"
```

## Input

Input will be read from the files in `MP-SPDZ/Input/`

- public input will be read from `PUB_INPUT`
- private input will be read from `INPUT-P<player_number>-0-<vec>`
    - `<player_number>`: is the number associate with a specific player.
    - `<vec>`: is always `0` 
        - except for SIMD circuits:
            - it is between [`0` - `DATTYPE/BITLENGTH`]
            - all have to be provided otherwise there are not enough numbers to
            store in a SIMD register

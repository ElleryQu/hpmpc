# Instructions

Run all commands from the root directory of the repository.

## Setup

Run the following commands on each node.

```sh
export ITERATIONS=1 # replace with number of repeated executions you want to run
export IP0=10.10.94.2 # replace with your IPs
export IP1=10.10.94.3
export IP2=10.10.94.3
export IP3=10.10.94.3
export PID=0 # replace with node id
export DATTYPE=256 # replace with highest DATTYPE supported by your hardware
```

## Network Shaping

Make sure to apply the latencies from figure 29 for each run.

## Execution

Run the following commands on each node.

### Execute 3PC Experiments

```sh
python3 measurements/run_config.py measurements/configs/artifacts/hpmpc/figure11/figure11_runtime_baseline.conf -i $ITERATIONS -a $IP0 -b $IP1 -c $IP2 -p $PID --override PROTOCOL=2 DATTYPE=$DATTYPE
python3 measurements/run_config.py measurements/configs/artifacts/hpmpc/figure11/figure11_runtime_Trio_Quad.conf -i $ITERATIONS -a $IP0 -b $IP1 -c $IP2 -p $PID --override PROTOCOL=5 DATTYPE=$DATTYPE
```

### Execute 4PC Experiments
This experiment requires a lot of processes and system RAM. If necessary, reduce the number of processes, e.g. by adding PROCESS_NUM=1 after --override.

```sh
python3 measurements/run_config.py measurements/configs/artifacts/hpmpc/figure11/runtime_baseline.conf -i $ITERATIONS -a $IP0 -b $IP1 -c $IP2 -d $IP3 -p $PID --override PROTOCOL=9 DATTYPE=$DATTYPE
python3 measurements/run_config.py measurements/configs/artifacts/hpmpc/figure11/runtime_Trio_Quad.conf -i $ITERATIONS -a $IP0 -b $IP1 -c $IP2 -d $IP3 -p $PID --override PROTOCOL=12 DATTYPE=$DATTYPE
```

## Parse Results

Run the following commands on each node.

```sh
python3 measurements/parse_logs.py measurements/logs/
```

## Interpret Results

Open the csv files after parsing results to obtain runtime.

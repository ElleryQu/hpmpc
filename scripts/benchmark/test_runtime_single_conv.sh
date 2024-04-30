#!/bin/bash

##---Adjust these---
protocols=(5 12) # 5: 3PC, 12: 4PC
use_nvcc=(0 1 2) # 0: CPU-only, 1: GPU for matmul, 2: GPU for Convolution
pre=(0 1) # 0: no pre-processing, 1: pre-processing
##---End of adjust---

functions=(400) # do not change
num_inputs=(32 64 128 256 512)

functions2=(402) # do not change
num_inputs2=(1 2 4 8 16 32 64)

helpFunction()
{
   echo "Script to test the runtime of single inferences for different configurations"
   echo -e "\t-p Party number or all for running locally"
   echo -e "\t-a IP address of player 0 (if ip matches player_id can be empty)"
   echo -e "\t-b IP address of player 1 (if ip matches player_id can be empty)"
   echo -e "\t-c IP address of player 2 (if ip matches player_id can be empty)"
   echo -e "\t-x Compiler (g++/clang++/..)"
   echo -e "\t"-u "Comile with nvcc and cutlass GEMM (0/1)"


   exit 1 # Exit script after printing help
}

while getopts "p:a:b:c:x:u:" opt
do
   case "$opt" in
      p ) O_PARTY="$OPTARG" ;;
      a ) IP0="$OPTARG" ;;
      b ) IP1="$OPTARG" ;;
      c ) IP2="$OPTARG" ;;
      x ) COMPILER="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done


cp scripts/benchmark/base_config.h config.h 

for pr in "${protocols[@]}"
do
    sed -i -e "s/\(define PROTOCOL \).*/\1$pr/" config.h
    for f in "${functions[@]}"
    do
        sed -i -e "s/\(define FUNCTION \).*/\1$f/" config.h
        for use_nv in "${use_nvcc[@]}"
        do
            for n in "${num_inputs[@]}"
            do
            sed -i -e "s/\(define USE_CUDA_GEMM \).*/\1$use_nv/" config.h
            for prep in "${pre[@]}"
            do
            sed -i -e "s/\(define PRE \).*/\1$pre/" config.h
            if [[ "$O_PARTY" == *"all"* ]]
            then 
                if [ "$pr" -gt "6" ]
                then
                    ./scripts/config.sh -f $f -p all4 -u $use_nv -n $n
                else
                    ./scripts/config.sh -f $f -p all3 -u $use_nv -n $n
                fi
            else
                ./scripts/config.sh -f $f -p $O_PARTY -u $use_nv -n $n
            fi
            
            if [ "$use_nv" != "0" ]
            then
                ./scripts/cuda_compile.sh
            fi

            echo "Running protocol $pr, function $f, num_inputs $n, use_nvcc $use_nv, pre $prep"
            
                #if pr > 6
            if [ "$pr" -gt "6" ]
            then
                if [ "$O_PARTY" == "0" ]
                then
                    ./run-P0.o $IP1 $IP2 $IP3
                elif [ "$O_PARTY" == "1" ]
                then
                    ./run-P1.o $IP0 $IP2 $IP3
                elif [ "$O_PARTY" == "2" ]
                then
                    ./run-P2.o $IP0 $IP1 $IP3
                elif [ "$O_PARTY" == "3" ]
                then
                    ./run-P3.o $IP0 $IP1 $IP2
                elif [ "$O_PARTY" == *"all"* ]
                then
                    ./run-P0.o & 
                    ./run-P1.o &
                    ./run-P2.o &
                    ./run-P3.o &
                    wait
                else
                    echo "Invalid party number"
                fi
            else 
                if [ "$O_PARTY" == "0" ]
                then
                    ./run-P0.o $IP1 $IP2
                elif [ "$O_PARTY" == "1" ]
                then
                    ./run-P1.o $IP0 $IP2
                elif [ "$O_PARTY" == "2" ]
                then
                    ./run-P2.o $IP0 $IP1
                elif [ "$O_PARTY" == "3" ]
                then
                    ./run-P3.o $IP0 $IP1
                elif [[ "$O_PARTY" == *"all"* ]]
                then
                    ./run-P0.o &
                    ./run-P1.o &
                    ./run-P2.o &
                    wait
                else
                    echo "Invalid party number"
                fi
            fi
        done
        done
    done
done
done

for pr in "${protocols[@]}"
do
    sed -i -e "s/\(define PROTOCOL \).*/\1$pr/" config.h
    for f in "${functions2[@]}"
    do
        sed -i -e "s/\(define FUNCTION \).*/\1$f/" config.h
        for n in "${num_inputs2[@]}"
        do
            for use_nv in "${use_nvcc[@]}"
            do
            sed -i -e "s/\(define USE_CUDA_GEMM \).*/\1$use_nv/" config.h
            for prep in "${pre[@]}"
            do
                sed -i -e "s/\(define PRE \).*/\1$prep/" config.h
            if [[ "$O_PARTY" == *"all"* ]]
            then 
                if [ "$pr" -gt "6" ]
                then
                    ./scripts/config.sh -f $f -p all4 -u $use_nv -n $n
                else
                    ./scripts/config.sh -f $f -p all3 -u $use_nv -n $n
                fi
            else
                ./scripts/config.sh -f $f -p $O_PARTY -u $use_nv -n $n
            fi
            
            if [ "$use_nv" != "0" ]
            then
                ./scripts/cuda_compile.sh
            fi

            echo "Running protocol $pr, function $f, num_inputs $n, use_nvcc $use_nv, pre $prep"
            
                #if pr > 6
            if [ "$pr" -gt "6" ]
            then
                if [ "$O_PARTY" == "0" ]
                then
                    ./run-P0.o $IP1 $IP2 $IP3
                elif [ "$O_PARTY" == "1" ]
                then
                    ./run-P1.o $IP0 $IP2 $IP3
                elif [ "$O_PARTY" == "2" ]
                then
                    ./run-P2.o $IP0 $IP1 $IP3
                elif [ "$O_PARTY" == "3" ]
                then
                    ./run-P3.o $IP0 $IP1 $IP2
                elif [ "$O_PARTY" == *"all"* ]
                then
                    ./run-P0.o & 
                    ./run-P1.o &
                    ./run-P2.o &
                    ./run-P3.o &
                    wait
                else
                    echo "Invalid party number"
                fi
            else 
                if [ "$O_PARTY" == "0" ]
                then
                    ./run-P0.o $IP1 $IP2
                elif [ "$O_PARTY" == "1" ]
                then
                    ./run-P1.o $IP0 $IP2
                elif [ "$O_PARTY" == "2" ]
                then
                    ./run-P2.o $IP0 $IP1
                elif [ "$O_PARTY" == "3" ]
                then
                    ./run-P3.o $IP0 $IP1
                elif [[ "$O_PARTY" == *"all"* ]]
                then
                    ./run-P0.o &
                    ./run-P1.o &
                    ./run-P2.o &
                    wait
                else
                    echo "Invalid party number"
                fi
            fi
        done
        done
    done
done

cp scripts/benchmark/base_config.h config.h 


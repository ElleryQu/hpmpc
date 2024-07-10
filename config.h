#pragma once

// === Basic Settings ===

#ifndef PROTOCOL
#define PROTOCOL 5
#endif

// Use a preprocessing phase? Only supported by Protocols 3,5,8,11,12
#ifndef PRE
#define PRE 0
#endif

// Party ID (starting from 0)
#ifndef PARTY
#define PARTY 2
#endif

// Bitlength of integers 
#ifndef BITLENGTH
#define BITLENGTH 32
#endif

// Fractional bits to use for fixed point arithmetic
#ifndef FRACTIONAL
#define FRACTIONAL 5
#endif

// 70+ Neural network architectures (LeNet, AlexNet, VGG, ResNet, etc.) on different dataset sizes (MNIST, CIFAR-10, Imagenet). Refer to programs/functions/NN.hpp
#ifndef FUNCTION_IDENTIFIER
#define FUNCTION_IDENTIFIER 81
#endif

// Number of inputs (mostly used by Benchmarking functions or Neural Networks)
#ifndef NUM_INPUTS
#define NUM_INPUTS 20
#endif


// === Concurrency Settings ===

// Register size to use for SIMD parallelization (Bitslicing/vectorization). Supported: 1,8,16,32,64,128(SSE),256(AVX-2),512(AVX-512)
// Info: MULT64 is supported by DATTYPE 64 and 512. MULT32 is supported for DATTYPE 32 and all DATATYPEs >= 128
#ifndef DATTYPE
#define DATTYPE 32
#endif

// Number of parallel processes to use
#ifndef PROCESS_NUM
#define PROCESS_NUM 1
#endif

// === Hardware Acceleration Settings ===

// 0 = xorshift, 1 = AES_BS, 2 = VAES/AES-NI. 0 is not secure.
#ifndef RANDOM_ALGORITHM
#define RANDOM_ALGORITHM 2
#endif

#ifndef USE_SSL_AES // USE SSl's AES implementation instead
#define USE_SSL_AES 0
#endif

#ifndef ARM
#define ARM 0 // 1 if ARM processor, 0 otherwise. Can speed up Sha hashing.
#endif


// USE CUDA for matrix multiplication?
#ifndef USE_CUDA_GEMM
#define USE_CUDA_GEMM 0
#endif


// === Tweaks ===

// How many gates should be buffered until sending them to the receiving party? 0 means the data of an entire communication round is buffered
#ifndef SEND_BUFFER
#define SEND_BUFFER 10000
#endif

// How many receiving messages should be buffered until the main thread is signaled that data is ready? 0 means that all data of a communication round needs to be ready before the main thread is signaled.
#ifndef RECV_BUFFER
#define RECV_BUFFER 10000
#endif

// How many messages should be buffered until a combined hash is performed? 0 means all hashes are calculated at the very end of the protocol.
#ifndef VERIFY_BUFFER
#define VERIFY_BUFFER 16
#endif


// === Network Settings ===

// Use SSL encrypted communication?
#ifndef USE_SSL
#define USE_SSL 0
#endif


// === Neural Network Settings ===

#ifndef MODELOWNER
#define MODELOWNER P_0 // Who holds the model parameters? (-1: Dummy model parameters, P_0/P_1/P_2/P_3: Read locally from P_0/P_1/P_2/P_3 followed by secret sharing). Important: Use "P_0" not "0"!
#endif

#ifndef DATAOWNER
#define DATAOWNER P_1 // Who holds the data? (-1: Dummy dataset, P_0/P_1/P_2/P_3: Read locally from P_0/P_1/P_2/P_3 followed by secret sharing). Important: Use "P_0" not "0"!
#endif

#ifndef TRUNC_THEN_MULT
#define TRUNC_THEN_MULT 0 // 0: Truncate after multiplication, 1: Truncate before multiplication
#endif

#ifndef TRUNC_APPROACH
#define TRUNC_APPROACH 0 // 0: Probabilistic truncation, 1: Reduced Slack Truncation, 2: Exact Truncation
#endif

#ifndef TRUNC_DELAYED
#define TRUNC_DELAYED 0 // Delay CONV truncation until next ReLU 
#endif

#ifndef COMPUTE_ARGMAX
#define COMPUTE_ARGMAX 0 // 0: skip final argmax during inference, 1: Compute final argmax during inference
#endif

#ifndef PUBLIC_WEIGHTS
#define PUBLIC_WEIGHTS 0 // 0: weights are secretly shared, 1: weights are public
#endif


#ifndef COMPRESS
#define COMPRESS 0
#endif


// Reduced Bitlength that might be used for RELU, etc
#if COMPRESS == 0
#ifndef REDUCED_BITLENGTH_k
#define REDUCED_BITLENGTH_k BITLENGTH
#endif

#ifndef REDUCED_BITLENGTH_m
#define REDUCED_BITLENGTH_m 0
#endif

#else
#ifndef REDUCED_BITLENGTH_k
#define REDUCED_BITLENGTH_k 20
#endif

#ifndef REDUCED_BITLENGTH_m
#define REDUCED_BITLENGTH_m 12
#endif

#endif

#ifndef IS_TRAINING
#define IS_TRAINING 0 // Training or inference phase? Training is not supported yet.
#endif



// === Debugging Settings ===

// Print additional info?
#ifndef PRINT
#define PRINT 0
#endif

#ifndef PRINT_TIMINGS
#define PRINT_TIMINGS 1
#endif

#ifndef PRINT_IMPORTANT
#define PRINT_IMPORTANT 1
#endif


// === Other Settings ===


#ifndef SRNG_SEED
#define SRNG_SEED 0 // Seed for the random number generator.
#endif

// Starting port for required port range of the sockets, must be multiple of 1000 for some applications
#ifndef BASE_PORT
#define BASE_PORT 10000
#endif

#ifndef SPLIT_ROLES_OFFSET
#define SPLIT_ROLES_OFFSET 0
#endif

int base_port = BASE_PORT; // temporary solution

// Timeout in seconds when connecting to a socket
#ifndef CONNECTION_TIMEOUT
#define CONNECTION_TIMEOUT 500
#endif

// Timeout in milliseconds before attempting to connect again to a socket
#ifndef CONNECTION_RETRY
#define CONNECTION_RETRY 5
#endif


// === Legacy Settings ===

// Allow sharing of inputs in offline phase
#ifndef SHARE_PREP
#define SHARE_PREP 1
#endif


// Use optimized secret sharing? Often utilizes SRNG instead of secret sharing with communication
#ifndef OPT_SHARE
#define OPT_SHARE 1
#endif

// Use the initialization phase or import initialization data from a file?
#ifndef NO_INI
#define NO_INI 0
#endif

// Use the initialization phase or import initialization data from a file?
#ifndef INIT
#define INIT 1
#endif

// Use the online phase?
#ifndef LIVE
#define LIVE 1
#endif

// Use random inputs or inputs from a file? 
#ifndef INPUT
#define INPUT 'r'
#endif


// === Internal Settings, do not change ===


/* #define MULTI_INPUT 1 // activate multi input Multiplication gates? */
/* #if BANDWIDTH_OPTIMIZED == 0 || ONLINE_OPTIMIZED == 1 // if BANDWIDTH_OPTIMIZED and not ONLINE_OPTIMIZED we don't need MULTI_INPUT_AND gates */
/* #define MULTI_INPUT 1 // activate multi input Multiplication gates? */
/* #else */
#if FUNCTION_IDENTIFIER > 5
#ifndef MULTI_INPUT
#define MULTI_INPUT 1
#endif
#else
#ifndef MULTI_INPUT
#define MULTI_INPUT 0
#endif
#endif
/* #endif */

#if FUNCTION_IDENTIFIER < 65
#if FUNCTION_IDENTIFIER == 46 || FUNCTION_IDENTIFIER == 49 || FUNCTION_IDENTIFIER == 52 || FUNCTION_IDENTIFIER == 59 || FUNCTION_IDENTIFIER == 62 // RCA
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 1
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 0
#endif
#elif FUNCTION_IDENTIFIER == 47 || FUNCTION_IDENTIFIER == 50 || FUNCTION_IDENTIFIER == 53 || FUNCTION_IDENTIFIER == 60 || FUNCTION_IDENTIFIER == 63 // PPA
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 0
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 0
#endif
#elif FUNCTION_IDENTIFIER == 48 || FUNCTION_IDENTIFIER == 51 || FUNCTION_IDENTIFIER == 54 || FUNCTION_IDENTIFIER == 61 || FUNCTION_IDENTIFIER == 64 // PPA 4-Way
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 0
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 1
#endif
#endif
#elif FUNCTION_IDENTIFIER < 400
#if FUNCTION_IDENTIFIER < 100  // RCA
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 1 // 1 if bandwidth optimized (e.g. Ripple Carry Adder), 0 if Latency optimized (e.g. Multi-input AND gates, Parallel Prefix Adder)
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 0 // 1 if online optimized (e.g. MULTI_INPUT AND gates), 0 if optimized for total communication (e.g. no MULTI_INPUT AND gates)
#endif
#elif FUNCTION_IDENTIFIER < 200 // PPA
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 0
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 0
#endif
#elif FUNCTION_IDENTIFIER < 300
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 0 // PPA 4-Way
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 1
#endif
#endif
#else
#if FUNCTION_IDENTIFIER == 404 || FUNCTION_IDENTIFIER == 407 || FUNCTION_IDENTIFIER == 412 // RCA
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 1
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 0
#endif
#elif FUNCTION_IDENTIFIER == 405 || FUNCTION_IDENTIFIER == 408 || FUNCTION_IDENTIFIER == 413 // PPA
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 0
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 0
#endif
#elif FUNCTION_IDENTIFIER == 406 || FUNCTION_IDENTIFIER == 409 || FUNCTION_IDENTIFIER == 414 // PPA 4-Way
#ifndef BANDWIDTH_OPTIMIZED
#define BANDWIDTH_OPTIMIZED 0
#endif

#ifndef ONLINE_OPTIMIZED
#define ONLINE_OPTIMIZED 1
#endif
#endif
#endif

#if PROTOCOL == 4
#ifndef num_players
#define num_players 2
#endif
#elif PROTOCOL < 7
#ifndef num_players
#define num_players 3
#endif
#else
#ifndef num_players
#define num_players 4
#endif
#endif

#if PROTOCOL > 7
#ifndef MAL
#define MAL 1
#endif
#endif

#ifndef SIMULATE_MPC_FUNCTIONS
#define SIMULATE_MPC_FUNCTIONS 0
#endif

#if BITLENGTH == 64
#ifndef INT_TYPE
#define INT_TYPE int64_t
#endif

#ifndef UINT_TYPE
#define UINT_TYPE uint64_t
#endif

#ifndef LOG2_BITLENGTH
#define LOG2_BITLENGTH 6
#endif

#ifndef LOG4_BITLENGTH
#define LOG4_BITLENGTH 3
#endif
#elif BITLENGTH == 32
#ifndef INT_TYPE
#define INT_TYPE int32_t
#endif

#ifndef UINT_TYPE
#define UINT_TYPE uint32_t
#endif

#ifndef LOG2_BITLENGTH
#define LOG2_BITLENGTH 5
#endif

#ifndef LOG4_BITLENGTH
#define LOG4_BITLENGTH 3
#endif
#elif BITLENGTH == 16
#ifndef INT_TYPE
#define INT_TYPE int16_t
#endif

#ifndef UINT_TYPE
#define UINT_TYPE uint16_t
#endif

#ifndef LOG2_BITLENGTH
#define LOG2_BITLENGTH 4
#endif

#ifndef LOG4_BITLENGTH
#define LOG4_BITLENGTH 2
#endif
#elif BITLENGTH == 8
#ifndef INT_TYPE
#define INT_TYPE int8_t
#endif

#ifndef UINT_TYPE
#define UINT_TYPE uint8_t
#endif

#ifndef LOG2_BITLENGTH
#define LOG2_BITLENGTH 3
#endif

#ifndef LOG4_BITLENGTH
#define LOG4_BITLENGTH 2
#endif
#endif


#ifndef JIT_VEC
#define JIT_VEC 1 // 0: vectorize and share inputs from the beginning, 1: vectorize and share inputs just in time, load a batch of images, then vectorize
#endif

#ifndef BASETYPE
#define BASETYPE 0 // 0: Additive_Share, 1: sint
#endif

#if JIT_VEC == 0
#ifndef BASE_DIV
#define BASE_DIV 1
#endif
#else
#if BASETYPE == 0
#ifndef BASE_DIV
#define BASE_DIV DATTYPE/BITLENGTH
#endif
#else
#ifndef BASE_DIV
#define BASE_DIV DATTYPE
#endif
#endif
#endif


#ifndef PHASE_LIVE
#define PHASE_LIVE 1
#endif

#ifndef PHASE_INIT
#define PHASE_INIT 0
#endif

#ifndef PHASE_PRE
#define PHASE_PRE 2
#endif


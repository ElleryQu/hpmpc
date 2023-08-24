#pragma once
#include "../../protocols/Protocols.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <bitset>
#include "../../protocols/XOR_Share.hpp"
#include "../../protocols/Additive_Share.hpp"
#include "../../protocols/Matrix_Share.hpp"
#include "../../datatypes/k_bitset.hpp"
#include "../../datatypes/k_sint.hpp"
/* #include "boolean_adder.hpp" */
#include "boolean_adder_updated.hpp"

#include "boolean_adder_msb.hpp"
#include "ppa_msb.hpp"
#include "ppa.hpp"
#include "ppa_msb_unsafe.hpp"

#include <cmath>
/* #include <eigen/Eigen/Core> */

/* #include "boolean_adder.hpp" */
/* #include "ppa.hpp" */
#if FUNCTION_IDENTIFIER == 16 || FUNCTION_IDENTIFIER == 17
#define FUNCTION RELU_bench
#elif FUNCTION_IDENTIFIER == 18
#define FUNCTION fixed_test
#elif FUNCTION_IDENTIFIER == 13
#define FUNCTION dot_prod_bench
#elif FUNCTION_IDENTIFIER == 14
#define FUNCTION dot_prod_bench
/* #define FUNCTION matmul_bench */
#elif FUNCTION_IDENTIFIER == 15
#define FUNCTION conv2D
#endif
#define RESULTTYPE DATATYPE

#if FRACTIONAL > 0

template <typename float_type, typename uint_type, size_t fractional>
float_type fixedToFloat(uint_type val) {
    static_assert(std::is_integral<uint_type>::value, "uint_type must be an integer type");
    static_assert(fractional <= (sizeof(uint_type) * 8 - 1), "fractional bits are too large for the uint_type");

    using sint_type = typename std::make_signed<uint_type>::type;
    float_type scaleFactor = static_cast<float_type>(1ULL << fractional);
    return static_cast<float_type>(static_cast<sint_type>(val)) / scaleFactor;
}

template <typename float_type, typename uint_type, size_t fractional>
uint_type floatToFixed(float_type val) {
    static_assert(std::is_integral<uint_type>::value, "uint_type must be an integer type");
    static_assert(fractional <= (sizeof(uint_type) * 8 - 1), "fractional bits are too large for the uint_type");

    bool isNegative = (val < 0);
    if (isNegative) val = -val; // Make it positive for easier handling

    // Split into integer and fractional parts
    uint_type intPart = static_cast<uint_type>(val);
    float_type fracPart = val - intPart;

    // Convert fractional part
    fracPart *= static_cast<float_type>(1ULL << fractional);
    uint_type fracInt = static_cast<uint_type>(fracPart + 0.5); // Adding 0.5 for rounding

    // Combine
    uint_type result = (intPart << fractional) | fracInt;

    // Apply two's complement if negative
    if (isNegative) {
        result = ~result + 1;
    }

    return result;
}

#endif
    template<typename Share>
void fixed_test(DATATYPE* res)
{
    using M = Matrix_Share<DATATYPE, Share>;
    using sint = sint_t<M>;
    auto a = new sint[NUM_INPUTS][NUM_INPUTS];
    auto b = new sint[NUM_INPUTS][NUM_INPUTS];
    auto c = new sint[NUM_INPUTS][NUM_INPUTS];
    std::memset(c, 0, sizeof(M) * NUM_INPUTS * NUM_INPUTS);
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
            a[i][j]. template prepare_receive_from<P0>();
            b[i][j]. template prepare_receive_from<P1>();
        }
}
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
            a[i][j]. template complete_receive_from<P0>();
            b[i][j]. template complete_receive_from<P1>();
        }
    }


    Share::communicate(); // dummy round
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
            for(int k = 0; k < NUM_INPUTS; k++)
            {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
                /* c[i][j].prepare_dot_add(a[i][k], b[k][j]); */
            }
                c[i][j].mask_and_send_dot();
        }
    }
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
            c[i][j].complete_mult();
        }
}
    Share::communicate();
delete[] a;
delete[] b;

    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
            c[i][j].prepare_reveal_to_all();
        }
    }
    auto result_arr = new UINT_TYPE[NUM_INPUTS*2][DATTYPE];
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
            c[i][j].complete_reveal_to_all(result_arr[2*i+j]);
            /* if(current_phase == 1) */
            /* { */
            /* #if FRACTIONAL > 0 */
            /* std::cout << fixedToFloat<float, UINT_TYPE, FRACTIONAL>(result_arr[i][j][0]) << std::endl; */
            /* #else */
            /* std::cout << result_arr[2+i+j][0] << std::endl; */
            /* #endif */
            }
        }
            if(current_phase == 1)
    {
        std::cout << "P" << PARTY << ": Result: ";
        for (int i = 0; i < 4; i++)
        {
    for(int j = 0; j < DATTYPE; j++)
    {
#if FRACTIONAL > 0
        std::cout << fixedToFloat<float, UINT_TYPE, FRACTIONAL>(result_arr[i][j]) << " ";
#else
        std::cout << result_arr[i][j] << " ";
#endif
    std::cout << std::endl;
    }
    std::cout << std::endl;
}

    }



delete[] c;
delete[] result_arr;


}



    template<typename Share>
void dot_prod_bench(DATATYPE* res)
{
    using M = Matrix_Share<DATATYPE, Share>;
    auto a = new M[NUM_INPUTS][NUM_INPUTS];
    auto b = new M[NUM_INPUTS][NUM_INPUTS];
    auto c = new M[NUM_INPUTS][NUM_INPUTS];
    Share::communicate(); // dummy round
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
#if FUNCTION_IDENTIFIER == 14
            for(int k = 0; k < NUM_INPUTS; k++)
            {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
                /* c[i][j].prepare_dot_add(a[i][k], b[k][j]); */
            }
#endif
                c[i][j].mask_and_send_dot();
        }
    }
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_INPUTS; j++)
        {
            c[i][j].complete_mult();
        }
    
}
delete[] a;
delete[] b;
delete[] c;

}

    /* template<typename Share> */
/* void matmul_bench(DATATYPE* res) */
/* { */
    /* using M = Matrix_Share<DATATYPE, Share>; */
    /* Eigen::Matrix<M, NUM_INPUTS, NUM_INPUTS> a, b, c; */
    /* c = a * b; */
    
    /* for(int i = 0; i < NUM_INPUTS; i++) */
    /* { */
    /*     for(int j = 0; j < NUM_INPUTS; j++) */ 
    /*     { */
    /*         c(i, j).mask_and_send_dot(); */
    /*     } */
    /* } */

    /* Share::communicate(); */
    
    /* for(int i = 0; i < NUM_INPUTS; i++) */
    /* { */
    /*     for(int j = 0; j < NUM_INPUTS; j++) */ 
    /*     { */
    /*         c(i, j).complete_mult(); */
    /*     } */
    /* } */

/* } */



    /* template<typename Share> */
/* void matmul(DATATYPE* res) */
/* { */
    /* using S = XOR_Share<DATATYPE, Share>; */
    /* using A = Additive_Share<DATATYPE, Share>; */
    /* using M = Matrix_Share<DATATYPE, Share>; */
    /* using Bitset = sbitset_t<S>; */
    /* using sint = sint_t<A>; */
    /* using mint = sint_t<M>; */

    /* Eigen::Matrix<mint, 2, 2> mat1, mat2, result; */

    /* //Receive shares */

    /* /1* mat1 << mint().template prepare_receive_from<P0>(), mint().template prepare_receive_from<P0>(), *1/ */
    /* /1*         mint().template prepare_receive_from<P0>(), mint().template prepare_receive_from<P0>(); *1/ */
    
    /* /1* mat2 << mint().template prepare_receive_from<P1>(), mint().template prepare_receive_from<P1>(), *1/ */
    /* /1*         mint().template prepare_receive_from<P1>(), mint().template prepare_receive_from<P1>(); *1/ */
    
    /* for(int i = 0; i < 2; i++) */
    /* { */
    /*     for(int j = 0; j < 2; j++) */ 
    /*     { */
    /*         mat1(i, j). template prepare_receive_from<P0>(); */
    /*         mat2(i, j). template prepare_receive_from<P1>(); */
    /*     } */
    /* } */


    /* Share::communicate(); */

    /* //complete_receive */

    /* for(int i = 0; i < 2; i++) */
    /* { */
    /*     for(int j = 0; j < 2; j++) */ 
    /*     { */
    /*         mat1(i, j). template complete_receive_from<P0>(); */
    /*         mat2(i, j). template complete_receive_from<P1>(); */
    /*     } */
    /* } */

    /* // 5. Multiply matrices. */
    /* result = mat1 * mat2; */

    /* // send dot product */

    /* for(int i = 0; i < 2; i++) */
    /* { */
    /*     for(int j = 0; j < 2; j++) */ 
    /*     { */
    /*         result(i, j).mask_and_send_dot(); */
    /*     } */
    /* } */

    /* Share::communicate(); */

    /* /1* //complete_receive and reveal *1/ */
    
    /* for(int i = 0; i < 2; i++) */
    /* { */
    /*     for(int j = 0; j < 2; j++) */ 
    /*     { */
    /*         result(i, j).complete_mult(); */
    /*         result(i,j).prepare_reveal_to_all(); */
    /*         /1* mat2(i,j).prepare_reveal_to_all(); *1/ */
    /*     } */
    /* } */

    /* Share::communicate(); */

    /* UINT_TYPE result_arr[4][DATTYPE]; */
    /* for(int i = 0; i < 2; i++) */
    /* { */
    /*     for(int j = 0; j < 2; j++) */ 
    /*     { */
    /*         result(i, j).complete_reveal_to_all(result_arr[2*i+j]); */
    /*         /1* mat2(i,j).complete_reveal_to_all(result_arr[2*i+j]); *1/ */
    /*     } */
    /* } */


    
    /* if(current_phase == 1) */
    /* { */
    /*     std::cout << "P" << PARTY << ": Result: "; */
    /*     for (int i = 0; i < 4; i++) */
    /*     { */
    /* for(int j = 0; j < DATTYPE; j++) */
    /* { */
    /*     std::cout << result_arr[i][j] << " "; */
    /* std::cout << std::endl; */
    /* } */
    /* std::cout << std::endl; */
/* } */
    

/* } */
/* } */
/* template<typename Share> */
/* void conv2D(DATATYPE* res) */
/* { */
/*     using M = Matrix_Share<DATATYPE, Share>; */
/*     using Matrix = Eigen::Matrix<sint_t<M>, Eigen::Dynamic, Eigen::Dynamic>; */

/*     const int depth = 64; */
/*     const int kernel_size = 3; */

/*     // Input (with padding) */
/*     std::vector<Matrix> input(depth, Matrix(34, 34)); */

/*     // Kernel */
/*     std::vector<Matrix> kernel(depth, Matrix(3, 3)); */

/*     // Output */
/*     std::vector<Matrix> output(depth, Matrix(32, 32)); */

/*     for (int z = 0; z < depth; ++z) { */
/*         for (int i = 0; i <= 32 - kernel_size; ++i) { */
/*             for (int j = 0; j <= 32 - kernel_size; ++j) { */
/*                 for (int d = 0; d < depth; ++d) { */
/*                     // Extract the patch from the input using block operation */
/*                     Matrix patch = input[d].block(i, j, kernel_size, kernel_size); */
/*                     output[z](i, j) += (patch.array() * kernel[d].array()).sum(); */
/*                 } */
/*             } */
/*         } */
/*     } */

/*      for (int z = 0; z < depth; ++z) { */
/*         for (int i = 0; i < 32; ++i) { */
/*             for (int j = 0; j < 32; ++j) { */
/*                 output[z](i, j).mask_and_send_dot(); */
/*             } */
/*         } */
/*     } */

/*      Share::communicate(); */

/*           for (int z = 0; z < depth; ++z) { */
/*         for (int i = 0; i < 32; ++i) { */
/*             for (int j = 0; j < 32; ++j) { */
/*                 output[z](i, j).complete_mult(); */
/*             } */
/*         } */
/*     } */

/* } */

    template<typename Share>
void RELU_bench(DATATYPE* res)
{
    using S = XOR_Share<DATATYPE, Share>;
    using A = Additive_Share<DATATYPE, Share>;
    using Bitset = sbitset_t<S>;
    using sint = sint_t<A>;
    
    sint* val = new sint[NUM_INPUTS];
    Bitset *s1 = new Bitset[NUM_INPUTS];
    Bitset *s2 = new Bitset[NUM_INPUTS];
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        s1[i] = sbitset_t<S>::prepare_A2B_S1( (S*) val[i].get_share_pointer());
        s2[i] = sbitset_t<S>::prepare_A2B_S2( (S*) val[i].get_share_pointer());
    }
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        s1[i].complete_A2B_S1();
        s2[i].complete_A2B_S2();
    }
    /* Bitset* y = new Bitset[NUM_INPUTS]; */
    S *y = new S[NUM_INPUTS];
    /* BooleanAdder<S> *adder = new BooleanAdder<S>[NUM_INPUTS]; */
    #if FUNCTION_IDENTIFIER == 16
    std::vector<PPA_MSB_Unsafe<S>> adders;
    #else
    std::vector<BooleanAdder_MSB<S>> adders;
    #endif
    adders.reserve(NUM_INPUTS);
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        /* adder[i].set_values(s1[i], s2[i], y[i]); */
        adders.emplace_back(s1[i], s2[i], y[i]);
    }
    while(!adders[0].is_done())
    {
        for(int i = 0; i < NUM_INPUTS; i++)
        {
            adders[i].step();
        }
        Share::communicate();
    }
    delete[] s1;
    delete[] s2;
    adders.clear();
    adders.shrink_to_fit();
    
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        y[i] = ~ y[i];
    }
    sint* t1 = new sint[NUM_INPUTS];
    sint* t2 = new sint[NUM_INPUTS];
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        y[i].prepare_bit_injection_S1(t1[i].get_share_pointer());
        y[i].prepare_bit_injection_S2(t2[i].get_share_pointer());
    }
    delete[] y;
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        t1[i].complete_bit_injection_S1();
        t2[i].complete_bit_injection_S2();
    }
    sint* result = new sint[NUM_INPUTS];
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        result[i].prepare_XOR(t1[i],t2[i]);
    }
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        result[i].complete_XOR(t1[i],t2[i]);
    }
    delete[] t1;
    delete[] t2;

    for(int i = 0; i < NUM_INPUTS; i++)
    {
        result[i] = result[i] * val[i];
    }
    delete[] val;
    Share::communicate();
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        result[i].complete_mult();
    }



}


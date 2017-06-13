/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC9_VM_HPP)
#define BOOST_SPIRIT_X3_CALC9_VM_HPP

#include <vector>
#include <cstdint>

///////////////////////////////////////////////////////////////////////////
//  The Virtual Machine
///////////////////////////////////////////////////////////////////////////
constexpr uint32_t op_unary_flag = 1u << 31u;
constexpr uint32_t op_binary_flag = 1u << 30u;

enum byte_code : uint32_t
{
    op_pos = 1u | op_unary_flag,         //  positate the top stack entry
    op_neg = 2u | op_unary_flag,         //  negate the top stack entry
    op_lognot = 3u | op_unary_flag,      //  logical negate the top stack entry
    op_bitnot = 4u | op_unary_flag,      //  bitwise negate the top stack entry

    op_add = 5u | op_binary_flag,         //  add top two stack entries
    op_sub = 6u | op_binary_flag,         //  subtract top two stack entries
    op_mul = 7u | op_binary_flag,         //  multiply top two stack entries
    op_div = 8u | op_binary_flag,         //  divide top two stack entries
    op_mod = 9u | op_binary_flag,         //  modulo of the top two stack entries

    op_eq = 10u | op_binary_flag,          //  compare the top two stack entries for ==
    op_neq = 11u | op_binary_flag,         //  compare the top two stack entries for !=
    op_lt = 12u | op_binary_flag,          //  compare the top two stack entries for <
    op_lte = 13u | op_binary_flag,         //  compare the top two stack entries for <=
    op_gt = 14u | op_binary_flag,          //  compare the top two stack entries for >
    op_gte = 15u | op_binary_flag,         //  compare the top two stack entries for >=

    op_bitwise_or = 16u | op_binary_flag,   // bitwise or operation on the top two stack entries
    op_bitwise_xor = 17u | op_binary_flag,  // bitwise xor operation on the top two stack entries
    op_bitwise_and = 18u | op_binary_flag,  // bitwise and operation on the top two stack entries

    op_shift_left = 19u | op_binary_flag,  // bitwise shift left
    op_shift_right = 20u | op_binary_flag, // bitwise shift right

    op_logical_and = 21u | op_binary_flag, //  logical and top two stack entries
    op_logical_or = 22u | op_binary_flag,  //  logical or top two stack entries

    op_load = 23u,        //  load a variable
    op_store = 24u,       //  store a variable

    op_int = 25u,         //  push constant integer into the stack

    op_jump_if = 26u,     //  jump to a relative position in the code if top stack
                    //  evaluates to false
    op_jump = 27u,        //  jump to a relative position in the code

    op_stk_adj = 28u,     // adjust the stack (for args and locals)
    op_call = 29u,        // function call
    op_return = 30u      // return from function
};

class vmachine
{
public:

    vmachine(unsigned stackSize = 4096)
      : stack(stackSize)
    {
    }

    int execute(
        std::vector<int> const& code            // the program code
      , std::vector<int>::const_iterator pc     // program counter
      , std::vector<int>::iterator frame_ptr    // start of arguments and locals
    );

    int execute(std::vector<int> const& code)
    {
        return execute(code, code.begin(), stack.begin());
    };

    std::vector<int> const& get_stack() const { return stack; };

    static int evaluateUnary(byte_code opcode, int x);
    static int evaluateBinary(byte_code opcode, int a, int b);

private:

    std::vector<int> stack;
};


#endif


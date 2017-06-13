/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "vm.hpp"
#include <boost/assert.hpp>
#include <functional>
#include <iostream>

int vmachine::execute(std::vector<int> const &code,
                      std::vector<int>::const_iterator pc,
                      const std::vector<int>::iterator frame_ptr
) {
    // counter for locals
    int n_locals = 0;
    std::vector<int>::iterator stack_ptr = frame_ptr;
    std::function<int()> popInt = [&stack_ptr, frame_ptr, &n_locals](){
        int top = stack_ptr[-1];
        stack_ptr--;
        BOOST_ASSERT(stack_ptr >= frame_ptr + n_locals);
        return top;
    };

    std::function<void(int)> pushInt = [&stack_ptr](int value){
        stack_ptr++;
        stack_ptr[-1] = value;
    };

    std::function<void()> printStackFrame = [frame_ptr, &stack_ptr](){
        auto frame_ptr_copy = frame_ptr;
        int i = 0;
        std::cout << "--- stack begin ---" << std::endl;
        while (frame_ptr_copy != stack_ptr)
        {
            std::cout << "stack[" << i++ << "] = " << *frame_ptr_copy << std::endl;
            frame_ptr_copy++;
        }
        std::cout << "--- stack end ---" << std::endl;
    };

    while (pc != code.end()) {
        BOOST_ASSERT(pc < code.end() && pc >= code.begin());
        int a, b;
        byte_code op_code = byte_code(*pc++);
        if (op_code & op_unary_flag)
        {
            pushInt(evaluateUnary(op_code, popInt()));
        }
        else if (op_code & op_binary_flag)
        {
            int b = popInt();
            int a = popInt();
            pushInt(evaluateBinary(op_code, a, b));
        } else
        {
            switch (op_code) {
                case op_load:
                    *stack_ptr++ = frame_ptr[*pc++];
                    break;

                case op_store:
                    --stack_ptr;
                    frame_ptr[*pc++] = stack_ptr[0];
                    break;

                case op_int:
                    *stack_ptr++ = *pc++;
                    break;

                case op_jump:
                    pc += *pc;
                    break;

                case op_jump_if:
                    if (!stack_ptr[-1])
                        pc += *pc;
                    else
                        ++pc;
                    --stack_ptr;
                    break;

                case op_stk_adj:
                    n_locals = *pc++;
                    stack_ptr = stack.begin() + n_locals;
                    break;

                case op_call: {
                    int nargs = *pc++;
                    int jump = *pc++;

                    // a function call is a recursive call to execute
                    int r = execute(
                            code, code.begin() + jump, stack_ptr - nargs
                    );

                    // cleanup after return from function
                    stack_ptr[-nargs] = r;      //  get return value
                    stack_ptr -= (nargs - 1);   //  the stack will now contain
                    //  the return value
                }
                    break;

                case op_return:
                    return stack_ptr[-1];

                default:
                    BOOST_ASSERT_MSG(false, "unknown op code");
            }
        }
    }
    return -1;
}

int vmachine::evaluateUnary(byte_code opcode, int x)
{
    switch (opcode)
    {
        case op_pos: return +x;
        case op_neg: return -x;
        case op_lognot: return !x;
        case op_bitnot: return ~x;
        default:
            BOOST_ASSERT_MSG(false, "unknown op code");
            return -1;
    }
}

int vmachine::evaluateBinary(byte_code opcode, int a, int b)
{
    switch (opcode)
    {
        case op_add: return a + b;
        case op_sub: return a - b;
        case op_mul: return a * b;
        case op_div: return a / b;
        case op_mod: return a % b;
        case op_eq: return a == b;
        case op_neq: return a != b;
        case op_lt: return a < b;
        case op_lte: return a <= b;
        case op_gt: return a > b;
        case op_gte: return a >= b;
        case op_bitwise_or: return a | b;
        case op_bitwise_xor: return a ^ b;
        case op_bitwise_and: return a & b;
        case op_shift_left: return a << b;
        case op_shift_right: return a >> b;
        case op_logical_and: return a && b;
        case op_logical_or: return a || b;
        default:
            BOOST_ASSERT_MSG(false, "unknown op code");
            return -1;
    }
}

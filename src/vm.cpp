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

        switch (*pc++) {
            case op_pos:
                pushInt(+popInt());
                break;

            case op_neg:
                pushInt(-popInt());
                break;

            case op_lognot:
                pushInt(!popInt());
                break;

            case op_bitnot:
                pushInt(~popInt());
                break;

            case op_add:
                --stack_ptr;
                stack_ptr[-1] += stack_ptr[0];
                break;

            case op_sub:
                --stack_ptr;
                stack_ptr[-1] -= stack_ptr[0];
                break;

            case op_mul:
                --stack_ptr;
                stack_ptr[-1] *= stack_ptr[0];
                break;

            case op_div:
                --stack_ptr;
                stack_ptr[-1] /= stack_ptr[0];
                break;

            case op_mod:
                --stack_ptr;
                stack_ptr[-1] %= stack_ptr[0];
                break;

            case op_shift_left:
                --stack_ptr;
                stack_ptr[-1] <<= stack_ptr[0];
                break;

            case op_shift_right:
                --stack_ptr;
                stack_ptr[-1] >>= stack_ptr[0];
                break;

            case op_eq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] == stack_ptr[0]);
                break;

            case op_neq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] != stack_ptr[0]);
                break;

            case op_lt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] < stack_ptr[0]);
                break;

            case op_lte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] <= stack_ptr[0]);
                break;

            case op_gt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] > stack_ptr[0]);
                break;

            case op_gte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] >= stack_ptr[0]);
                break;

            case op_bitwise_and:
                --stack_ptr;
                stack_ptr[-1] = stack_ptr[-1] & stack_ptr[0];
                break;

            case op_bitwise_xor:
                --stack_ptr;
                stack_ptr[-1] = stack_ptr[-1] ^ stack_ptr[0];
                break;

            case op_bitwise_or:
                --stack_ptr;
                stack_ptr[-1] = stack_ptr[-1] | stack_ptr[0];
                break;

            case op_logical_and:
                --stack_ptr;
                stack_ptr[-1] = stack_ptr[-1] && stack_ptr[0];
                break;

            case op_logical_or:
                --stack_ptr;
                stack_ptr[-1] = stack_ptr[-1] || stack_ptr[0];
                break;

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
        }
    }
    return -1;
}

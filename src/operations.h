#ifndef OPERATIONS_H
#define OPERATIONS_H

//------------------TASK 2------------------//
void op_bipush();

void op_iadd();

void op_isub();

void op_iand();

void op_ior();

void op_swap();

void op_dup();

void op_in();

void op_out() ;

//------------------TASK 3------------------//

void op_goto();

void op_ifeq();

void op_iflt();

void op_if_icmpeq();

//------------------TASK 4------------------//

void op_ldc_w() ;

void op_iload();

void op_istore();

void op_iinc();

void op_wide();

//------------------TASK 5------------------//


void op_invokevirtual() ;

void op_ireturn();

//------------------BONUS NETWORK------------------//


void op_netbind();

void op_netconnect();

void op_netin();

void op_netout();

void op_netclose();

#endif

#include "spimcore.h"


/* ALU */
/* 10 Points */
/*isabella*/
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    //uing a switch case for simplicity
    switch (ALUControl){
        case 0: //000 Z=A+B
            *ALUresult = A+B;
            break;
        case 1: //001 Z=A-B
            *ALUresult = A-B;
            break;
        case 2: //010 A<B,Z=1 else Z=0 (signed comparison)
             *ALUresult = ((int)A < (int)B) ? 1: 0;
             break;
        case 3: //011 same as case 2, but unsigned comparison
             *ALUresult = (A < B) ? 1: 0;
             break;
        case 4: //100 Z= A & B
            *ALUresult = A & B;
            break;
        case 5: //101 Z= A || B
            *ALUresult= A | B;//bitwise or operator
            break;
        case 6: //110 Z= shift B left by 16
            *ALUresult = B << 16;
            break;
        case 7: //111 Z=NOT A
            *ALUresult= ~A;
            break;
        default: //base case just in case
            *ALUresult =0;
            break;
    }

    //if the result is 0, than *Zero = 1, else *Zero = 0
    if (*ALUresult == 0){
        *Zero = 1;//assery
    }else{
        *Zero = 0;
    }
        

}




/* instruction fetch */
/* 10 Points */
/*isabella*/
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //first, need to check if pc is within correct bounds
    if(PC % 4 != 0){
        return 1; //halt
    }

    //getting the instruction from the memory
    *instruction = Mem[PC/4];

    return 0;//success

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}


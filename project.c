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
/* Harold Saenz */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // The 32 bit must be broken into parts respectively to the instruction like this
    
    // op = 6 bits       , instruction [31-26]
    // r1 = 5 bits       , instruction [25-21]
    // r2 = 5 bits       , instruction [20-16]
    // r3 = 5 bits       , instruction [15-11]
    // funct = 6 bits    , instruction [5-0]
    // offset = 16 bits  , instruction [15-0]
    // jsec = 26 bits    , instruction [25-0]

    // 32 - 6 = 26
    *op = instruction >> 26;

    // Now the next bits r1, r2, and r3 should take 5 bits

    // 32 - 6 - 5 = 21
    // Must remove the 6 bits since that was for the op, so shift then mask and keep the last 5 bits
    *r1 = (instruction >> 21) & 0b11111;

    // 32 - 6 - 5 - 5 = 16
    // Here as well, must shift then keep the last 5 bits 
    *r2 = (instruction >> 16) & 0b11111;

    // 32 - 6 - 5 - 5 - 5 =  11
    // Here as well, must shift then keep the last 5 bits
    *r3 = (instruction >> 11) & 0b11111;

    // For funct its the last 6 bits instruction [5-0]
    *funct = instruction & 0b111111;

    // Offset is the last 16 bits instruction [15-0]
    *offset = instruction & 0b1111111111111111;

    // jsec is the last 26 bits instruction [25-0]
    *jsec = instruction & 0x3FFFFFF;     // In hex 
    
}



/* instruction decode */
/* Harold Saenz */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    // Decode the instruction using the opcode

    /*  

            Control Signals

                RegDst 
                    Deasserted: the register destination number for 
                    the write register comes from the rt field (bits 20:16)

                    Asserted: the register destination number for 
                    the write register comes from the rd field (bits 15:11)

                Jump

                Branch

                MemRead
                    Asserted: Data memory contents designated by the address input
                    are put on the read data output

                MemtoReg
                    Deasserted: The value fed to the register write data input
                    comes from the ALU

                    Asserted: The value fed to the register write data input
                    comes from the data memory

                ALUOp
                    Select the ALU operation

                MemWrite 
                    Asserted: Data memory contents designated by the address input
                    are replaced by the value on the write data input

                ALUSrc
                    Deasserted: The second ALU operand comes from 
                    the second register file output

                    Asserted: The second ALU operand is the 
                    sign-extended lower 16 bits of the instruction

                RegWrite 
                    Asserted: The register destination specified by the write register 
                    input is written with the value from the write data input
            
    */

    switch (op)
    {

    // For R-type which are add, sub, or, and, slt, sltu

        // 0000
        case 0:
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;    // its an r type instruction binary value is 111
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;

            break;
        
    // For the I-type instructions addi lw sw lui beq slti sltiu

        // 1000
        case 8: // addi
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;    // uses addition
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;

            break;

        // 0010 0011
        case 35: // lw
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;  // needs to read from memory then load it into register
            controls->MemtoReg = 1;
            controls->ALUOp = 0;    // it adds to compute address
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;

            break;

        // 0010 1011
        case 43:  // sw
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp  = 0;   // it adds to compute address
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;

            break;

        // 1111
        case 15: // lui
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;  // memory does not need to be read
            controls->MemtoReg = 0;
            controls->ALUOp = 6;    // left shift by 16
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;

            break;
        
        // 0100
        case 4: // beq
            controls->RegDst = 2;
            controls->Jump = 0; // PC would change this
            controls->Branch = 1; 
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 1; // uses subtraction
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;

            break;
        
        // 1010    
        case 10: // slti 
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;    // less than operation 
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;

            break;

        // 1011
        case 11: //sltiu
            controls->RegDst =0;
            controls->Jump = 0;
            controls->Branch =0;
            controls->MemRead =0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;    // less than unsigned operation
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
 
            break;

        // 0010
        case 2: // jump
            controls->RegDst = 2;   // dont care (doesnt come from rt field or rd field)
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead =0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0; // dont care
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;

            break;

        default:
            return 1; // 1 is for a halt condition
    }

    return 0; // It's good
    
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
//test
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
//test1
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
/* Kevin Rodriguez */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // Do nothing if register write is disabled.
    if (RegWrite != 1)
        return;
    
    // If RegDst is 1, the destination is r3
    // otherwise (RegDst == 0) the destination is r2
    unsigned dest_reg = (RegDst == 1) ? r3 : r2;
    
    // In MIPS, register $zero (register 0) must always remain 0.
    if (dest_reg == 0)
        return;
    
    // If MemtoReg is 1, use memdata
    // otherwise, use ALUresult.
    unsigned write_data = (MemtoReg == 1) ? memdata : ALUresult;
    
    Reg[dest_reg] = write_data;
}

/* PC update */
/* 10 Points */
/* Kevin Rodriguez */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // Default PC update
    unsigned pc_next = *PC + 4;
    
    // Jump takes precedence over branch.
    if (Jump == 1)
    {
        // Compute the jump target address using the upper 4 bits of (PC+4)
        // and the lower 28 bits from jsec (shifted left by 2 for word alignment).
        pc_next = (pc_next & 0xF0000000) | (jsec << 2);
    }
    // Otherwise, if a branch is signaled and the ALU result was zero, update PC with branch offset.
    else if (Branch == 1 && Zero == 1)
    {
        // The branch offset is sign-extended and then shifted left 2 bits to get the byte offset.
        pc_next = pc_next + (extended_value << 2);
    }
    
    // Update PC with the computed address.
    *PC = pc_next;
}


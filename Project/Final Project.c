#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

/*Take in 2 32 bit inputs & 1 4 bit function selection input, carry out function on inputs
and then return the result in ALU result & Zero*/
	//Selecting & executing the function corresponding to the input control signal
    switch(ALUControl){
        //ADD
        case 0:
            //Addition of A & B
            *ALUresult = A + B;

            //Checking for zero
            if(ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
                }

            break;
        //SUBTRACT
        case 1:
            //Subtraction of A - B
            *ALUresult = A - B;

            //Checking for zero
            if(ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
                }

            break;
        
        // Set on Less than, A<B
        case 2:
            if ((int)A < (int)B) {
                *ALUresult = 1;
            } else {
                *ALUresult = 0;
            }

            // Checking for zero
            if(*ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
            }
            break;

        //Set on less than unsigned, A<B (A and B unsigned)
        case 3:
            if(A < B){
                *ALUresult = 1;
            }
            else{
                *ALUresult = 0;
            } 
            
            //Checking for zero
            if(ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
                }

            break;
        
        //AND
        case 4:
            //Bitwise AND
            *ALUresult = A & B;

            //Checking for zero
            if(ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
                }

            break;
        //OR
        case 5:
            //Bitwise OR
            *ALUresult = A | B;

            //Checking for zero
            if(ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
                }

            break;
        //Shift B 16 bites
        case 6:
            //ALU result is B shifted left 16 bits
            *ALUresult = B << 16;

            //Checking for zero
            if(ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
                }

            break;
        //Not A
        case 7:
            //not A
            *ALUresult = ~A;

            //Checking for zero
            if(ALUresult == 0){
                *Zero = 0;
            }
            else{
                *Zero = 1;
                }

            break;
    }


}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	
/*Take in index of instruction location in memory & base address of memory (mem[0]), 
then move the instruction pointer to the corresponding address in memory*/

    //checking if memory is byte-algined
    if( (PC % 4) != 0){
        return 1;
    }

    //Having *instruction point to the right instruction in memory
    else{
        *instruction = Mem[PC >> 2]; //Mem[PC >> 2] here is just shifting the instruction right by 2 bits (dividing by 4)
        return 0;
    }

	
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //Set op = instruction [31:26]
    *op = (instruction & 0xFC000000) >> 26;
        
    //Set r1 = instruction [25:21]
    *r1 = (instruction & 0x03E00000) >> 21;

    //Set r2 = instruction [20:16]
    *r2 = (instruction & 0x001F0000) >> 16;

    //Set r3 = instruction [15:11]
    *r3 = (instruction & 0x0000F800) >> 11;

    //Set funct = instruction [5:0]
    *funct = (instruction & 0x0000003F);

    //Set offset = instruction [15:0]
    *offset = (instruction & 0x0000FFFF);

    //Set jsec = instruction [25:0]
    *jsec = (instruction & 0x03FFFFFF);

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    if (controls == NULL){
        return 0;
    }
    // set all vaules to 0 (that way i only need to set true values)
    controls->RegDst = 0x00;
    controls->ALUOp = 0x00;
    controls->ALUSrc = 0x00;
    controls->MemtoReg = 0x00;
    controls->RegWrite = 0x00;
    controls->MemRead = 0x00;
    controls->MemWrite = 0x00;
    controls->Branch = 0x00;
    controls->Jump = 0x00;
    
    // For ALUOp: 
       // 0x00 == add/default
       // 0x01 == subtract
       // 0x07 == Look at func field (is an R-type instruction)

    switch (op) {
        case 0x00:  // R-Format
            controls->RegDst = 0x01;
            controls->ALUOp = 0x07;  // R-type ALU operation
            controls->RegWrite = 0x01;
            break;
        case 0x23:  // lw
            controls->ALUSrc = 0x01;
            controls->MemRead = 0x01;
            controls->MemtoReg = 0x01;
            controls->RegWrite = 0x01;
            break;
        case 0x2B:  // sw
            controls->ALUSrc = 0x01;
            controls->MemWrite = 0x01;
            break;
        case 0x04:  // beq
            controls->Branch = 0x01;
            controls->ALUOp = 0x01;
            break;
        case 0x05:  // bne
            controls->Branch = 0x01;
            controls->ALUOp = 0x01;
            break;
        case 0x02:  // j
            controls->Jump = 0x01;
            break;
        case 0x08:  // addi
            controls->ALUSrc = 0x01;
            controls->RegWrite = 0x01;
            controls->ALUOp = 0x00;
            break;
	case 0x0A:  // slti
            controls->ALUSrc = 0x01;
            controls->RegWrite = 0x01;
            controls->ALUOp = 0x05;  // Custom ALU op for SLT
            break;
    case 0xf:  // lui
            controls->ALUSrc = 0x01;
            controls->RegWrite = 0x01;
            controls->ALUOp = 0x06;  // Custom ALU op for SLT
            break;
    case 0x0b:  // sltiu
            controls->ALUSrc = 0x01;
            controls->RegWrite = 0x01;
            controls->ALUOp = 0x03;  // Custom ALU op for SLT
            break;

    case 0x0C:  // andi
            controls->ALUSrc = 0x01;
            controls->RegWrite = 0x01;
            controls->ALUOp = 0x03;  // Custom ALU op for AND
            break;

    case 0x0D:  // ori
            controls->ALUSrc = 0x01;
            controls->RegWrite = 0x01;
            controls->ALUOp = 0x04;  // Custom ALU op for OR
            break;
    
       
        default:
            return 1;
            break;
        }
    // successful setting of control varibles:
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2) 
{
    // check if regesters or reg data is null
    if (Reg == NULL || data1 == NULL || data2 == NULL) {
        return; 
    }
    // copy registers 1 and 2
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //Make a compare int, comparing the binary between it and offset, and shifting right by 15
    unsigned compare = (offset & 0b00000000000000001000000000000000) >> 15;

    //Check to see if the sign-extend is negative
    if(compare == 0b00000000000000000000000000000001){
        //fill left with 1s if constant is negative
        *extended_value = offset + 0b11111111111111110000000000000000;
    }
    //Check to see if the sign-extend is positive
    else if(compare == 0b00000000000000000000000000000000){
        //fill left with 0s if constant is positive
        *extended_value = (offset & 0b00000000000000001111111111111111);
    }
}

/* ALU operations */
/* 10 Points */
// My take on the ALUop, i just swaped around some of the functions and used the funct feild when dealing with r-type, have a feeling
// will have to fine tune it though since i dont know the funct values for the diffrent r-type alu ops
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	//looks at ALU Op to determine which function to do not sure what format ALUOp is tho
	switch(ALUOp){
		case 0x0:
		//does addition
			if (ALUSrc == 1){
                ALU(data1, extended_value, ALUOp, ALUresult, Zero);
                return 0;
            }
			else {
               ALU(data1, data2, ALUOp, ALUresult, Zero); 
               return 0;
            }
					
		case 0x1: 
		//subtraction
			if (ALUSrc == 1){
                ALU(data1, extended_value, ALUOp, ALUresult, Zero);
                return 0;
            }
				
			else{
                ALU(data1, data2, ALUOp, ALUresult, Zero);	
                return 0;
            }
			break;
        
        case 0x3: 
		//andi
			if (ALUSrc == 1){
                ALU(data1, extended_value, 0x4, ALUresult, Zero);
                return 0;
            }
			else{
                ALU(data1, data2, 0x4, ALUresult, Zero);	
                return 0;
            }
			break;
        
        case 0x4: 
		//ori
			if (ALUSrc == 1){
                ALU(data1, extended_value, 0x5, ALUresult, Zero);
                return 0;
            }
			else{
                ALU(data1, data2, 0x5, ALUresult, Zero);	
                return 0;
            }
			break;
        
        case 0x5: 
		//slti
			if (ALUSrc == 1){
                ALU(data1, extended_value, 0x2, ALUresult, Zero);
                return 0;
            }
			else{
                ALU(data1, data2, 0x2, ALUresult, Zero);	
                return 0;
            }
			break;

        case 0x6: 
		//lui
			if (ALUSrc == 1){
                ALU(data1, extended_value, ALUOp, ALUresult, Zero);
                return 0;
            }
			else{
                ALU(data1, data2, ALUOp, ALUresult, Zero);	
                return 0;
            }
			break;
		
		case 0x7:
	            switch(funct){
	                case 0x20:
	                //does addition
                        if (ALUSrc == 1){
                            ALU(data1, extended_value, 0x0, ALUresult, Zero);
                            return 0;
                        }
                        else {
                            ALU(data1, data2, 0x0, ALUresult, Zero); 
                            return 0;
                        }
                        break;
	                case 0x22: 
	                //subtraction
                        if (ALUSrc == 1){
                            ALU(data1, extended_value,  0x1, ALUresult, Zero);
                            return 0;
                        }
                            
                        else{
                            ALU(data1, data2,  0x1, ALUresult, Zero);	
                            return 0;
                        }
                        break;
	                case 0x2a:
	                //slt
	                    if (ALUSrc == 1){
                            ALU(data1, extended_value, 0x2, ALUresult, Zero); 
                            return 0;
                        }
	                        
	                    else{
                            ALU(data1, data2, 0x2, ALUresult, Zero);
                            return 0;
                        }
	                        
	                    break;
	                case 0x2b:
	                //sltu
	                    if (ALUSrc == 1){
                             ALU(data1, extended_value, 0x3, ALUresult, Zero); 
                        }
	                       
	                    else{
                            ALU(data1, data2, 0x3, ALUresult, Zero);
                            return 0;
                        }
	                        	
	                    break;
	                case 0x24:
	                //and
	                    if (ALUSrc == 1){
                            ALU(data1, extended_value, 0x4, ALUresult, Zero);
                            return 0;
                        }
	                        
	                    else{
                            ALU(data1, data2, 0x4, ALUresult, Zero);
                            return 0;
                        }
	                        	
	                    break;
	                case 0x25:
	                //or
	                    if (ALUSrc == 1){
                            ALU(data1, extended_value, 0x5, ALUresult, Zero);
                            return 0;
                        }
	                         
	                    else {
                            ALU(data1, data2, 0x5, ALUresult, Zero);
                            return 0;	
                        }
	                        
	                    break;
	                case 0x6:
	                //sll
	                    if (ALUSrc == 1){
                            ALU(data1, extended_value, 0x6, ALUresult, Zero);
                            return 0; 
                        }
	                        
	                    else{
                            ALU(data1, data2, 0x6, ALUresult, Zero);
                            return 0;
                        }	
	                    break;
		default:
			return 1;
		}
            }
            
	return 0;
		
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	//check if aluresult is valid
	if(MemWrite == 0x1 || MemRead == 0x1){
		//address not aligned
		if ((ALUresult % 4) != 0){
			//puts up halt flag
			return 0;
        }
		if (ALUresult < 0x0000 || ALUresult > 0xFFFF){
          		  return 0;
        }
	}
	//lw into mem
	if (MemRead == 0x0 && MemWrite == 0x1) {
		Mem[ALUresult >> 2] = data2;
	}
	//sw into mem
	if (MemRead == 0x1 && MemWrite == 0x0) {
		*memdata = Mem[ALUresult >> 2];
	}
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if (RegWrite == 0) {
		return; // do nothing
	}
	
	unsigned data;
	if (MemtoReg == 0) {
		data = ALUresult; // use ALU data
	} else if (MemtoReg == 1) {
		data = memdata; // use memdata
	} 
	
	unsigned route;
	if (RegDst == 0) {
		route = r2; // rt field, I-type, lw, addi
	} else if (RegDst == 1) {
		route = r3; // rd field, R-type, add, sub
	} 
	Reg[route] = data; // write data to the selected register (route)
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	if (Branch == 0 && Jump == 0) {
		*PC = *PC + 4; // go to next instruction
		return;
	}
	
	if (Branch == 1 && Zero == 1) {
		*PC = *PC + 4 + (extended_value << 2); // update PC to to branch being used
		return;
        
	}
	
	if (Jump == 1) {
		// Jump address PC[31:28] | (jsec << 2)
		unsigned upperPC = *PC & 0xF0000000; // keep top 4 bits of (PC + 4) and set remaining bits to 0
		
		*PC = upperPC | (jsec << 2); // combine top 4 bits with jsec bits to give us the jump address

		return;
	}

	*PC = *PC + 4;
}

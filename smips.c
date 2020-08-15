//ASSIGNMENT BY SONALI SHARMA (Z5258346)
//COMP1521 20T2

#define MAX_INSTRUCTION_CODES 1000

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////
     /* FUNCTIONS FOR PRINTING DECODED INSTRUCTIONS*/
//////////////////////////////////////////////////////////////////
void print_instruction(uint32_t code, char *argv[], int32_t registers[], int j, int codes[]);

//decode mul instruction
void decode_mul_part1(uint32_t code, int32_t registers[]);
// decode add instruction
void decode_add_part1(uint32_t code, int32_t registers[]);
//decode sub instruction
void decode_sub_part1(uint32_t code, int32_t registers[]);
//decode and instruction
void decode_and_part1(uint32_t code, int32_t registers[]);
//decode or instruction
void decode_or_part1(uint32_t code, int32_t registers[]);
//decode slt instruction
void decode_slt_part1(uint32_t code, int32_t registers[]);

//decode beq instruction
void decode_beq_part1(uint32_t code, int32_t registers[], int j, int codes[], char *argv[]);
//decode bne instruction
void decode_bne_part1(uint32_t code, int32_t registers[], int j, int codes[], char *argv[]);
//decode addi instruction
void decode_addi_part1(uint32_t code, int32_t registers[]);
//decode slti instruction
void decode_slti_part1(uint32_t code, int32_t registers[]);
//decode andi instruction
void decode_andi_part1(uint32_t code, int32_t registers[]);
//decode ori instruction
void decode_ori_part1(uint32_t code, int32_t registers[]);
//decode lui instruction
void decode_lui_part1(uint32_t code, int32_t registers[]);

//////////////////////////////////////////////////////////////
         /*FUNCTIONS FOR PRINTING PART 2 AND 3*/
//////////////////////////////////////////////////////////////
void decode_instruction_output(uint32_t code, char *argv[], int32_t registers[], int j, int codes[], int *PC);

//print registers after execution
void registers_after_execution(int32_t registers[]);


int main(int argc, char *argv[]) {
    //check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Must enter one filename\n");
        return 1;
    }
    
    FILE *f = fopen(argv[1], "r");
    //check open succeeded
    if (f == NULL) {
        perror(argv[1]); //prints why the open failed
        return 1;
    }
    
    int codes[MAX_INSTRUCTION_CODES];
    int n_instructions = 0;
    while (n_instructions < MAX_INSTRUCTION_CODES && 
        fscanf(f, "%x", &codes[n_instructions]) == 1) {
        n_instructions++;
    }
    
    //check for array overflow
    if (n_instructions == MAX_INSTRUCTION_CODES) {
        fprintf(stderr, "Array overflow\n");
        return 1;
    }
    
    //array to store registers
    int32_t registers[32] = {0};
    int pointer = 0;
    int *PC = &pointer;

    //print decoded instructions
    printf("Program\n");
    for (int j = 0; j < n_instructions; j++) {
        printf(" %2d: ", j);
        print_instruction(codes[j], argv, registers, j, codes);
    }
    
    
    printf("Output\n");
    //reset registers array
    for (int i = 0; i < 32; i++) {
        registers[i] = 0;
    }
    
    //reset PC pointer for part 2 and 3
    int pointer2 = 0;
    PC = &pointer2;
    
    while (*PC < n_instructions) {
        int j = 0;
        decode_instruction_output(codes[*PC], argv, registers, j, codes, PC);
        *PC += 1;
    }
    
    //print registers after execution
    registers_after_execution(registers);
    
    return 0;
}

//decoding instructions for desired program output
void decode_instruction_output(uint32_t code, char *argv[], int32_t registers[], int j, int codes[], int *PC) {
    if (code == 12) {
        
        // if we are printing output for part 3 of the program
        //check argument in $a0
        if (registers[2] == 1) {
            printf("%d", registers[4]);
        } else if (registers[2] == 10) {
            registers_after_execution(registers);
            exit(0);
        } else if (registers[2] == 11) {
            //print lowest 8 bits of $a0
            int32_t lowest_eight_bits = registers[4] & 0xFF;
            printf("%c", lowest_eight_bits);
        } else {
            //halt this whole program if unknown system call
            printf("Unknown system call: %d\n", registers[2]);
            registers_after_execution(registers);
            exit(0);
        }

    } else {
        // check if first six bits are 0
        uint32_t result = code;
        result = result >> 26;
        uint32_t mask = result & 0x3F;
        if (mask == 0) {
            mask = code & 0x3F;
            // code is one of the top functions and we need to now look at last
            // 6 bits of top 6 instructions
            if (mask == 32) {
                // add instruction
                //add $d, $s, $t	d = s + t	
                //000000ssssstttttddddd00000100000
                uint32_t result = code;
                //decode $d register
                result = result >> 11;
                uint32_t mask_d = result & 0x1f;
    
                //decode $s register
                result = result >> 10;
                uint32_t mask_s = result & 0x1f;
    
                //decode $t register
                result = code >> 16;
                uint32_t mask_t = result & 0x1f;
    
                uint32_t addition = registers[mask_t] + registers[mask_s];
                registers[mask_d] = addition;
                //reset register $0 to 0
                registers[0] = 0;
                
            } else if (mask == 34) {
                //sub instruction
                // sub $d, $s, $t	d = s - t	
                //000000ssssstttttddddd00000100010
                uint32_t result = code;
                //decode $d register
                result = result >> 11;
                uint32_t mask_d = result & 0x1f;
    
                //decode $s register
                result = result >> 10;
                uint32_t mask_s = result & 0x1f;
    
                //decode $t register
                result = code >> 16;
                uint32_t mask_t = result & 0x1f;
    
                uint32_t subtraction = registers[mask_s] - registers[mask_t];
                registers[mask_d] = subtraction;
                //reset register $0 to 0
                registers[0] = 0;
                
            } else if (mask == 36) {
                //and instruction
                //and $d, $s, $t	d = s & t	
                //000000ssssstttttddddd00000100100
                uint32_t result = code;
                //decode $d register
                result = result >> 11;
                uint32_t mask_d = result & 0x1f; 
    
                //decode $s register
                result = result >> 10;
                uint32_t mask_s = result & 0x1f;
    
                //decode $t register
                result = code >> 16;
                uint32_t mask_t = result & 0x1f;
    
                uint32_t and = registers[mask_s] & registers[mask_t];
                registers[mask_d] = and;
                //reset register $0 to 0
                registers[0] = 0;
                
            } else if (mask == 37) {
                //or instruction
                //or $d, $s, $t	d = s | t	
                //000000ssssstttttddddd00000100101
                uint32_t result = code;
                //decode $d register
                result = result >> 11;
                uint32_t mask_d = result & 0x1f;
    
                //decode $s register
                result = result >> 10;
                uint32_t mask_s = result & 0x1f;
    
                //decode $t register
                result = code >> 16;
                uint32_t mask_t = result & 0x1f;
    
                uint32_t or = registers[mask_s] | registers[mask_t];
                registers[mask_d] = or;
                //reset register $0 to 0
                registers[0] = 0;
                
            } else if (mask == 42) {
                //slt instruction
                //slt $d, $s, $t	d = 1 if s < t else 0	
                //000000ssssstttttddddd00000101010
                uint32_t result = code;
                //decode $d register
                result = result >> 11;
                uint32_t mask_d = result & 0x1f;
    
                //decode $s register
                result = result >> 10;
                uint32_t mask_s = result & 0x1f;
    
                //decode $t register
                result = code >> 16;
                uint32_t mask_t = result & 0x1f;

                if (registers[mask_s] < registers[mask_t]) {
                    registers[mask_d] = 1;
                } else {
                    registers[mask_d] = 0;
                }
                //reset register $0 to 0
                registers[0] = 0;
                
            } else {
                //invalid instruction code
                fprintf(stdout, "%s: invalid instruction code: %08X\n", argv[1], code);
                exit(1);
    }
        } else if (mask == 0b011100) {
            //mul instruction
            // mul $d $s $t 
            // 011100ssssstttttddddd00000000010
            uint32_t result = code;
            // decode $d register
            result = result >> 11;
            uint32_t mask_d = result & 0x1f;
    
            //decode $s register
            result = result >> 10;
            uint32_t mask_s = result & 0x1f;
    
            //decode $t register
            result = code >> 16;
            uint32_t mask_t = result & 0x1f;
    
    
            uint32_t product = registers[mask_t] * registers[mask_s];
            registers[mask_d] = product;
            //reset register $0 to 0
            registers[0] = 0;
            
        } else if (mask == 0b000100) {
            //beq instruction
            //beq $s, $t, I
            //000100ssssstttttIIIIIIIIIIIIIIII
            //if (s == t) PC += I
            uint32_t result = code;
    
            //decode $s register
            result = result >> 21;
            uint32_t mask_s = result & 0x1f;
    
            //decode $t register
            result = code >> 16;
            uint32_t mask_t = result & 0x1f;
    
            //decode Immediate value
            result = code;
            uint32_t mask = result & 0xFFFF;
            int16_t imm_mask = mask; 

    
            if (registers[mask_s] == registers[mask_t]) {
                *PC += (imm_mask - 1);
            }
            //reset register $0 to 0
            registers[0] = 0;
            
        } else if (mask == 0b000101) {
            //bne instruction
            //bne $s, $t, I
            //000101ssssstttttIIIIIIIIIIIIIIII
            //if (s != t) PC += I 
            uint32_t result = code;
    
            //decode $s register
            result = result >> 21;
            uint32_t mask_s = result & 0x1f;
    
            //decode $t register
            result = code >> 16;
            uint32_t mask_t = result & 0x1f;
    
            //decode Immediate value
            result = code;
            uint32_t mask = result & 0xFFFF;
            int16_t imm_mask = mask;

            if (registers[mask_s] != registers[mask_t]) {
                *PC += (imm_mask - 1);
            }
            //reset register $0 to 0
            registers[0] = 0;
            
        } else if (mask == 0b001000) {
            //addi instruction
            //addi $t, $s, I
            //001000ssssstttttIIIIIIIIIIIIIIII
            //t = s + I
            uint32_t result = code;
    
            //decode $t register
            result = result >> 16;
            uint32_t mask_t = result & 0x1f;  
    
            //decode $s register
            result = result >> 5;
            uint32_t mask_s = result & 0x1f;

            //decode Immediate value
            result = code;
            uint32_t mask = result & 0xFFFF;
            int16_t imm_mask = mask;
    
    
            int32_t imm_mask2 = imm_mask;
            int32_t addition = registers[mask_s] + imm_mask2; 
            registers[mask_t] = addition;
            //reset register $0 to 0
            registers[0] = 0;
            
        } else if (mask == 0b001010) {
            //slti instruction
            //slti $t, $s, I
            //001010ssssstttttIIIIIIIIIIIIIIII
            //t = (s < I)
            uint32_t result = code;
    
            //decode $t register
            result = result >> 16;
            uint32_t mask_t = result & 0x1f;
    
            //decode $s register
            result = result >> 5;
            uint32_t mask_s = result & 0x1f;
    
            //decode Immediate value
            result = code;
            uint32_t mask = result & 0xFFFF;
            int16_t imm_mask = mask;
     
            int32_t imm_mask2 = imm_mask;
            if (registers[mask_s] < imm_mask2) {
                registers[mask_t] = 1;
            } else {
                registers[mask_t] = 0;
            }
            //reset register $0 to 0
            registers[0] = 0;
            
        } else if (mask == 0b001100) {
            //andi instruction
            //andi $t, $s, I
            //001100ssssstttttIIIIIIIIIIIIIIII
            //t = s & I
            uint32_t result = code;
    
            //decode $t register
            result = result >> 16;
            uint32_t mask_t = result & 0x1f;
    
            //decode $s register
            result = result >> 5;
            uint32_t mask_s = result & 0x1f;
    
            //decode Immediate value
            result = code;
            uint32_t mask = result & 0xFFFF;
            int16_t imm_mask = mask;
    
            int32_t imm_mask2 = imm_mask;
            int32_t andi = registers[mask_s] & imm_mask2;
            registers[mask_t] = andi;
            //reset register $0 to 0
            registers[0] = 0;
            
        } else if (mask == 0b001101) {
            //ori instruction
            //ori $t, $s, I
            //001101ssssstttttIIIIIIIIIIIIIIII
            //t = s | I
            uint32_t result = code;
    
            //decode $t register
            result = result >> 16;
            uint32_t mask_t = result & 0x1f;
    
            //decode $s register
            result = result >> 5;
            uint32_t mask_s = result & 0x1f;
    
            //decode Immediate value
            result = code;
            uint32_t mask = result & 0xFFFF;
            int16_t imm_mask = mask;
    
            int32_t imm_mask2 = imm_mask;
            int32_t ori = registers[mask_s] | imm_mask2;
            registers[mask_t] = ori;
            //reset register $0 to 0
            registers[0] = 0;
            
        } else if (mask == 0b001111) {
            //lui instruction
            //lui $t, I
            //00111100000tttttIIIIIIIIIIIIIIII
            //t = I << 16
            uint32_t result = code;
    
            //decode $t register
            result = result >> 16;
            uint32_t mask_t = result & 0x1f;
    
            //decode Immediate value
            result = code;
            uint32_t mask = result & 0xFFFF;
            int16_t imm_mask = mask;
    
            int32_t imm_mask2 = imm_mask;
            int32_t lui = imm_mask2 << 16;
            registers[mask_t] = lui;
            registers[0] = 0;
        } else {
            //invalid instruction code
            fprintf(stdout, "%s: invalid instruction code: %08X\n", argv[1], code);
            exit(1);
        }  
    }
}


///////////////////////////////////////////////////
/* FUNCTIONS FOR DECODING INDIVIDUAL INSTRUCTIONS*/
///////////////////////////////////////////////////

//print registers after execution
void registers_after_execution(int32_t registers[]) {
    printf("Registers After Execution\n");
    for (int j = 0; j < 32; j++) {
        if (registers[j] != 0) {
            printf("$%-2d = %d\n", j, registers[j]);
        }
    }
}


/////////////////////////////////////////////
/*FUNCTIONS FOR PRINTING DECODED INSTRUCTIONS*/
/////////////////////////////////////////////
void print_instruction(uint32_t code, char *argv[], int32_t registers[], int j, int codes[]) {
    if (code == 12) {
        printf("syscall\n");

    } else {
        // check if first six bits are 0
        uint32_t result = code;
        result = result >> 26;
        uint32_t mask = result & 0x3F;
        if (mask == 0) {
            mask = code & 0x3F;
            // code is one of the top functions and we need to now look at last
            // 6 bits of top 6 instructions
            if (mask == 32) {
                // add instruction
                decode_add_part1(code, registers);
            } else if (mask == 34) {
                //sub instruction
                decode_sub_part1(code, registers);
            } else if (mask == 36) {
                //and instruction
                decode_and_part1(code, registers);
            } else if (mask == 37) {
                //or instruction
                decode_or_part1(code, registers);
            } else if (mask == 42) {
                //slt instruction
                decode_slt_part1(code, registers);
            } else {
                //invalid instruction code
                fprintf(stdout, "%s: invalid instruction code: %08X\n", argv[1], code);
                exit(1);
    }
        } else if (mask == 0b011100) {
            //mul instruction
            printf("mul  ");
            decode_mul_part1(code, registers);
        } else if (mask == 0b000100) {
            //beq instruction
            decode_beq_part1(code, registers, j, codes, argv);
        } else if (mask == 0b000101) {
            //bne instruction
            decode_bne_part1(code, registers, j, codes, argv);
        } else if (mask == 0b001000) {
            //addi instruction
            decode_addi_part1(code, registers);
        } else if (mask == 0b001010) {
            //slti instruction
            decode_slti_part1(code, registers);
        } else if (mask == 0b001100) {
            //andi instruction
            decode_andi_part1(code, registers);
        } else if (mask == 0b001101) {
            //ori instruction
            decode_ori_part1(code, registers);
        } else if (mask == 0b001111) {
            //lui instruction
            decode_lui_part1(code, registers);
        } else {
            //invalid instruction code
            fprintf(stdout, "%s: invalid instruction code: %08X\n", argv[1], code);
            exit(1);
        }  
    }
}


///////////////////////////////////////////////////
/* FUNCTIONS FOR DECODING INDIVIDUAL INSTRUCTIONS*/
///////////////////////////////////////////////////

void decode_mul_part1(uint32_t code, int32_t registers[]) {
    // mul $d $s $t 
    // 011100ssssstttttddddd00000000010
    uint32_t result = code;
    // decode $d register
    result = result >> 11;
    uint32_t mask_d = result & 0x1f;
    printf("$%d, ", mask_d);
    
    //decode $s register
    result = result >> 10;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d \n", mask_t);
    
    
    uint32_t product = registers[mask_t] * registers[mask_s];
    registers[mask_d] = product;
    //reset register $0 to 0
    registers[0] = 0;
}

void decode_add_part1(uint32_t code, int32_t registers[]) {
    //add $d, $s, $t	d = s + t	
    //000000ssssstttttddddd00000100000
    printf("add  ");
    
    uint32_t result = code;
    //decode $d register
    result = result >> 11;
    uint32_t mask_d = result & 0x1f;
    printf("$%d, ", mask_d);
    
    //decode $s register
    result = result >> 10;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d \n", mask_t);
    
    uint32_t addition = registers[mask_t] + registers[mask_s];
    registers[mask_d] = addition;
    //reset register $0 to 0
    registers[0] = 0;
}

void decode_sub_part1(uint32_t code, int32_t registers[]) {
    // sub $d, $s, $t	d = s - t	
    //000000ssssstttttddddd00000100010
    printf("sub  ");
    
    uint32_t result = code;
    //decode $d register
    result = result >> 11;
    uint32_t mask_d = result & 0x1f;
    printf("$%d, ", mask_d);
    
    //decode $s register
    result = result >> 10;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d \n", mask_t);
    
    
    uint32_t subtraction = registers[mask_s] - registers[mask_t];
    registers[mask_d] = subtraction;
    //reset register $0 to 0
    registers[0] = 0;
}


void decode_and_part1(uint32_t code, int32_t registers[]) {
    //and $d, $s, $t	d = s & t	
    //000000ssssstttttddddd00000100100
    printf("and  ");

    uint32_t result = code;
    //decode $d register
    result = result >> 11;
    uint32_t mask_d = result & 0x1f;
    printf("$%d, ", mask_d);   
    
    //decode $s register
    result = result >> 10;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d \n", mask_t);
    
    uint32_t and = registers[mask_s] & registers[mask_t];
    registers[mask_d] = and;
    //reset register $0 to 0
    registers[0] = 0;
}


void decode_or_part1(uint32_t code, int32_t registers[]) {
    //or $d, $s, $t	d = s | t	
    //000000ssssstttttddddd00000100101
    printf("or  ");

    uint32_t result = code;
    //decode $d register
    result = result >> 11;
    uint32_t mask_d = result & 0x1f;
    printf("$%d, ", mask_d);
    
    //decode $s register
    result = result >> 10;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d \n", mask_t);
    
    uint32_t or = registers[mask_s] | registers[mask_t];
    registers[mask_d] = or;
    //reset register $0 to 0
    registers[0] = 0;
}


void decode_slt_part1(uint32_t code, int32_t registers[]) {
    //slt $d, $s, $t	d = 1 if s < t else 0	
    //000000ssssstttttddddd00000101010
    printf("slt  ");
 
    uint32_t result = code;
    //decode $d register
    result = result >> 11;
    uint32_t mask_d = result & 0x1f;
    printf("$%d, ", mask_d);
    
    //decode $s register
    result = result >> 10;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d \n", mask_t);

    if (registers[mask_s] < registers[mask_t]) {
        registers[mask_d] = 1;
    } else {
        registers[mask_d] = 0;
    }
    //reset register $0 to 0
    registers[0] = 0;
}


void decode_beq_part1(uint32_t code, int32_t registers[], int j, int codes[], char *argv[]) {
    //beq $s, $t, I
    //000100ssssstttttIIIIIIIIIIIIIIII
    //if (s == t) PC += I
    printf("beq  ");
    uint32_t result = code;
    
    //decode $s register
    result = result >> 21;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d, ", mask_t);
    
    //decode Immediate value
    result = code;
    uint32_t mask = result & 0xFFFF;
    int16_t imm_mask = mask;
    printf("%d \n", imm_mask); 

    //reset register $0 to 0
    registers[0] = 0;
}

void decode_bne_part1(uint32_t code, int32_t registers[], int j, int codes[], char *argv[]) {
    //bne $s, $t, I
    //000101ssssstttttIIIIIIIIIIIIIIII
    //if (s != t) PC += I
    printf("bne  ");  
    uint32_t result = code;
    
    //decode $s register
    result = result >> 21;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode $t register
    result = code >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d, ", mask_t);
    
    //decode Immediate value
    result = code;
    uint32_t mask = result & 0xFFFF;
    int16_t imm_mask = mask;
    printf("%d \n", imm_mask);


    //reset register $0 to 0
    registers[0] = 0;     
}


void decode_addi_part1(uint32_t code, int32_t registers[]) {
    //addi $t, $s, I
    //001000ssssstttttIIIIIIIIIIIIIIII
    //t = s + I
    printf("addi ");
    uint32_t result = code;
    
    //decode $t register
    result = result >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d, ", mask_t);   
    
    //decode $s register
    result = result >> 5;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode Immediate value
    result = code;
    uint32_t mask = result & 0xFFFF;
    int16_t imm_mask = mask;
    printf("%d \n", imm_mask);
    
    
    int32_t imm_mask2 = imm_mask;
    int32_t addition = registers[mask_s] + imm_mask2; 
    registers[mask_t] = addition;
    //reset register $0 to 0
    registers[0] = 0;
}

void decode_slti_part1(uint32_t code, int32_t registers[]) {
    //slti $t, $s, I
    //001010ssssstttttIIIIIIIIIIIIIIII
    //t = (s < I)
    printf("slti  ");
    uint32_t result = code;
    
    //decode $t register
    result = result >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d, ", mask_t);
    
    //decode $s register
    result = result >> 5;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode Immediate value
    result = code;
    uint32_t mask = result & 0xFFFF;
    int16_t imm_mask = mask;
    printf("%d \n", imm_mask);
     
    int32_t imm_mask2 = imm_mask;
    if (registers[mask_s] < imm_mask2) {
        registers[mask_t] = 1;
    } else {
        registers[mask_t] = 0;
    }
    //reset register $0 to 0
    registers[0] = 0;

}


void decode_andi_part1(uint32_t code, int32_t registers[]) {
    //andi $t, $s, I
    //001100ssssstttttIIIIIIIIIIIIIIII
    //t = s & I
    printf("andi  ");
    uint32_t result = code;
    
    //decode $t register
    result = result >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d, ", mask_t);
    
    //decode $s register
    result = result >> 5;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode Immediate value
    result = code;
    uint32_t mask = result & 0xFFFF;
    int16_t imm_mask = mask;
    printf("%d \n", imm_mask);
    
    int32_t imm_mask2 = imm_mask;
    int32_t andi = registers[mask_s] & imm_mask2;
    registers[mask_t] = andi;
    //reset register $0 to 0
    registers[0] = 0;
}


void decode_ori_part1(uint32_t code, int32_t registers[]) {
    //ori $t, $s, I
    //001101ssssstttttIIIIIIIIIIIIIIII
    //t = s | I
    printf("ori  ");
    uint32_t result = code;
    
    //decode $t register
    result = result >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d, ", mask_t);
    
    //decode $s register
    result = result >> 5;
    uint32_t mask_s = result & 0x1f;
    printf("$%d, ", mask_s);
    
    //decode Immediate value
    result = code;
    uint32_t mask = result & 0xFFFF;
    int16_t imm_mask = mask;
    printf("%d \n", imm_mask);
    
    int32_t imm_mask2 = imm_mask;
    int32_t ori = registers[mask_s] | imm_mask2;
    registers[mask_t] = ori;
    //reset register $0 to 0
    registers[0] = 0;
}


void decode_lui_part1(uint32_t code, int32_t registers[]) {
    //lui $t, I
    //00111100000tttttIIIIIIIIIIIIIIII
    //t = I << 16
    printf("lui  ");
    uint32_t result = code;
    
    //decode $t register
    result = result >> 16;
    uint32_t mask_t = result & 0x1f;
    printf("$%d, ", mask_t);
    
    //decode Immediate value
    result = code;
    uint32_t mask = result & 0xFFFF;
    int16_t imm_mask = mask;
    printf("%d \n", imm_mask);
    
    int32_t imm_mask2 = imm_mask;
    int32_t lui = imm_mask2 << 16;
    registers[mask_t] = lui;
    registers[0] = 0;
}


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MEMORY_SIZE 8192
int memory[MEMORY_SIZE];
int main(int argc,char* argv[])
{
    int A=0;
    int B=0;
    int PC=0;
    int SP=0;

    int running=1;
    int instruction;
    int opcode;
    int operand;

    FILE* objfile;
    FILE *dumpfile;
    int words_read;
    int i;

    char dump_filename[256];
    char *dot;
    if(argc<2)
    {
        printf("Usage: %s <filename.obj>\n", argv[0]);
        return 1;
    }

    objfile=fopen(argv[1],"rb");
    if(objfile==NULL)
    {
        printf("Error: Could not open object file %s\n", argv[1]);
        return 1;
    }

    words_read=fread(memory,sizeof(int),MEMORY_SIZE,objfile);
    printf("Successfully loaded %d instruction words into memory.\n", words_read);
    fclose(objfile);
    SP=MEMORY_SIZE-1;
    printf("Starting execution...\n");

    while(running)
    {
        if (PC < 0 || PC >= MEMORY_SIZE) {
            printf("\n[EMULATOR WARNING] PC out of bounds (%04X). Auto-halting to prevent crash!\n", PC);
            break; 
        }
        instruction=memory[PC];

        opcode=instruction & 0xFF;
        operand=instruction >> 8;

        PC++;

        switch(opcode)
        {
            case 0:
            B=A;
            A=operand;
            break;

            case 1:
            A=A+operand;
            break;

            case 2:
            B=A;
            A=memory[SP+ operand];
            break;

            case 3:
            memory[SP+operand]=A;
            A=B;
            break;

            case 4:
            A=memory[A+operand];
            break;

            case 5:
            memory[A+operand]=B;
            break;

            case 6:
            A=B+A;
            break;

            case 7:
            A=B-A;
            break;

            case 8:
            A=B<<A;
            break;
            
            case 9:
            A=B>>A;
            break;

            case 10:
            SP=SP+operand;
            break;

            case 11:
            SP=A;
            A=B;
            break;

            case 12:
            B=A;
            A=SP;
            break;

            case 13:
            B=A;
            A=PC;
            PC=PC+operand;
            break;

            case 14:
            PC=A;
            A=B;
            break;

            case 15:
            if (A==0)
            PC=PC+operand;
            break;

            case 16:
            if (A<0) 
            PC=PC+operand;
            break;

            case 17:
            PC=PC+operand;
            break;
            
            case 18:
            running =0;
            break;

            default:
            printf("CPU ERROR: Unknown opcode %d at memory address %d\n", opcode, PC - 1);
            running=0;
            break;
        }
    
    }

    printf("\n--- Execution Halted ---\n");
    printf("Final CPU State:\n");
    printf("A:  %08X (%d)\n", A, A);
    printf("B:  %08X (%d)\n", B, B);
    printf("PC: %08X (%d)\n", PC, PC);
    printf("SP: %08X (%d)\n", SP, SP);

    
    /* --- 5. Complete Memory Dump to File --- */
    strncpy(dump_filename, argv[1], sizeof(dump_filename) - 1);
    dump_filename[sizeof(dump_filename) - 1] = '\0';
    
    /* Find the last dot in the filename to replace the extension */
    dot = strrchr(dump_filename, '.');
    if (dot != NULL) {
        /* Change this to ".lst" if you want to overwrite the assembler's file! */
        strcpy(dot, "_dump.lst"); 
    } else {
        strcat(dump_filename, "_dump.lst");
    }

    printf("Writing complete memory dump to '%s'...\n", dump_filename);
    
    dumpfile = fopen(dump_filename, "w");
    if (dumpfile == NULL) {
        printf("Error: Could not create %s\n", dump_filename);
    } else {
        fprintf(dumpfile, "--- SIMPLEX EMULATOR MEMORY DUMP ---\n");
        fprintf(dumpfile, "Address    Hex Value  (Decimal)\n");
        fprintf(dumpfile, "-----------------------------------\n");
        
        for (i = 0; i < MEMORY_SIZE; i++) {
            fprintf(dumpfile, "[%04X]    %08X   (%d)\n", i, memory[i], memory[i]);
        }
        
        fclose(dumpfile);
        printf("Memory dump saved successfully!\n");
    }
    return 0;
}
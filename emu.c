#include<stdio.h>
#include<stdlib.h>
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
    int words_read;
    int i;

    if(argc<2)
    {
        printf("Usage: %s <filename.obj>\n", argv[0]);
        return 1;
    }

    objfile=fopen(argv[i],"rb");
    if(objfile==NULL)
    {
        printf("Error: Could not open object file %s\n", argv[1]);
        return 1;
    }

    words_read=fread(memory,sizeof(int),MEMORY_SIZE,objfile);
    return 0;
}
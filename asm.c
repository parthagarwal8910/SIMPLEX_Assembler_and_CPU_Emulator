#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct{
    const char* mnemonic;
    int opcode;
    int operand;
}Instruction;


void trim_whitespace(char *str) {
    char *end;
    char *start = str;

    while (isspace((unsigned char)*start)) {
        start++;
    }

   
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    if (*str == '\0') {
        return;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
}
Instruction InstructionTable[]={  {"data", -1, 1},  
    {"ldc", 0, 1},    
    {"adc", 1, 1},    
    {"ldl", 2, 1},   
    {"stl", 3, 1},    
    {"ldnl", 4, 1},    
    {"stnl", 5, 1},   
    {"add", 6, 0},     
    {"sub", 7, 0},   
    {"shl", 8, 0},     
    {"shr", 9, 0},     
    {"adj", 10, 1},    
    {"a2sp", 11, 0},   
    {"sp2a", 12, 0},   
    {"call", 13, 1},   
    {"return", 14, 0}, 
    {"brz", 15, 1},   
    {"brlz", 16, 1},   
    {"br", 17, 1},     
    {"HALT", 18, 0},   
    {"SET", -2, 1}     
};
#define NumberInstructions (int)(sizeof(InstructionTable)/sizeof(Instruction))
#define MAX_LABELS 500
#define MAX_LINE_LENGTH 256
typedef struct
{
   char name[50];
   int address;
   int is_used;
}Symbol;

Symbol symbol_table[MAX_LABELS];
int symbol_count = 0;

int main(int argc,char* argv[])
{
    FILE* infile;
    char line[MAX_LINE_LENGTH];
     int pc=0;
     char *comment_ptr;
    char *token;
    int j;
    int found=0;

    FILE* objfile;
    FILE* lstfile;
    char* operand_str;
    int operand_val;
    int machine_code;
    int i;
    int label_found;
    char* endptr;
 
    int k;
    int is_dup;
    char* extra;
    char obj_filename[256];
    char lst_filename[256];

    char *colon_ptr;
    char *attached_inst;

    if(argc<2)
    {
        printf("Usage:%s \n",argv[0]);
        return 1;
    }
    infile = fopen(argv[1], "r");
    if (infile == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    printf("Starting Pass 1...\n");
    while(fgets(line,sizeof(line),infile)!=NULL)
    {
       comment_ptr=strchr(line,';');
       if(comment_ptr!=NULL)
       {
        *comment_ptr='\0';
       }
       token=strtok(line," \t\r\n");
       if(token==NULL)
       {
          continue;
       }
       colon_ptr = strchr(token, ':');

      if(colon_ptr != NULL)
       {
        *colon_ptr = '\0'; /* Cut the string at the colon */
        trim_whitespace(token);
        attached_inst = colon_ptr + 1;

        if (!isalpha(token[0])) 
        {
            printf("ERROR at PC %04X: Invalid label name '%s' (must start with a letter)\n", pc, token);
        }
        else
        {
            is_dup = 0;
                for (k = 0; k < symbol_count; k++) {
                    if (strcmp(symbol_table[k].name, token) == 0) {
                        printf("ERROR at PC %04X: Duplicate label definition '%s'\n", pc, token);
                        is_dup = 1;
                        break;
        }
    }
    
        if(!is_dup && symbol_count<MAX_LABELS)
        {
            strcpy(symbol_table[symbol_count].name,token);
            symbol_table[symbol_count].address=pc;
            symbol_table[symbol_count].is_used = 0;
            symbol_count++;
        }
       }

       if (*attached_inst != '\0') {
            token = attached_inst;
        } else {
            /* Otherwise, just grab the next word separated by a space */
            token = strtok(NULL, " \t\r\n");
        }
        
        if(token == NULL) continue;
    }
    
       if(token!=NULL)
       {
        found=0;
        for(j=0;j<NumberInstructions;j++)
        {
            if(strcmp(token,InstructionTable[j].mnemonic)==0)
            {
            found=1;
            if(strcmp(token,"SET")==0)
                {
    
                    char *set_operand = strtok(NULL, " \t\r\n");
                    if (set_operand != NULL && symbol_count > 0) {
                        symbol_table[symbol_count - 1].address = strtol(set_operand, NULL, 0);
                    } else {
                        printf("ERROR at PC %04X: Missing operand for SET\n", pc);
                    }
                }
                else
                {
                    pc++; 
                }
            break;
            }
        }
        if(!found)
        printf("Instruction Not found\n");
       }
    }
    printf("Pass 1 complete. Found %d labels.\n", symbol_count);
    for(j=0;j<symbol_count;j++)
        {
           printf("Label: %-10s -> Address: %d\n", symbol_table[j].name, symbol_table[j].address);
        }
    
        strcpy(obj_filename,argv[1]);
        strcpy(lst_filename,argv[1]);

        token= strrchr(obj_filename,'.');
        if(token!=NULL)
        {
            strcpy(token,".obj");
        }
        else
        {
            strcat(obj_filename,".obj");
        }

         token= strrchr(lst_filename,'.');
        if(token!=NULL)
        {
            strcpy(token,".lst");
        }
        else
        {
            strcat(obj_filename,".lst");
        }

        objfile=fopen(obj_filename,"wb");
        lstfile=fopen(lst_filename,"w");

        if(objfile==NULL || lstfile==NULL)
        {
            printf("Error Could not create output files");
            return 1;
        }
        rewind(infile);
        pc=0;
        printf("Starting pass 2....\n");
        while(fgets(line,sizeof(line),infile)!=NULL)
        {
            comment_ptr=strchr(line,';');
            if(comment_ptr!=NULL)
            {
                *comment_ptr='\0';
            }
            token=strtok(line," \t\r\n");
            if(token==NULL)
            {
                continue;
            }
            colon_ptr = strchr(token, ':');
            if(colon_ptr != NULL)
            {
                 *colon_ptr = '\0'; /* Cut the string at the colon */
                 trim_whitespace(token);
                 attached_inst = colon_ptr + 1; /* Point to whatever comes immediately after */
                 
                 if (*attached_inst != '\0') {
                     token = attached_inst;
                 } else {
                     token = strtok(NULL," \t\r\n");
                 }
                 
                 if(token==NULL)
                 {
                     continue;
                 }
            }

            found=0;
            for(j=0;j<NumberInstructions;j++)
            {
                if(strcmp(token,InstructionTable[j].mnemonic)==0)
                {
                    found=1;
                    operand_val=0;
                    if(InstructionTable[j].operand)
                    {
                        operand_str=strtok(NULL," \t\r\n");
                        if(operand_str==NULL)
                        {
                            printf("ERROR at PC %04X: Missing operand for %s\n",pc,token);
                            pc++;
                            break;
                        }
                       operand_val=strtol(operand_str,&endptr,0);
                       if (strcmp(token, "SET") == 0) {
                        
                        fprintf(lstfile, "         %08X %s\n", operand_val, line); 
                        
                        continue; 
                       }
                       if(*endptr!='\0' && *endptr!=';')
                       {
                        if (isdigit(operand_str[0]) || (operand_str[0] == '-' && isdigit(operand_str[1]))) {
                                printf("ERROR at PC %04X: Invalid number format '%s'\n", pc, operand_str);
                                operand_val = 0;
                        }
                        else
                        {
                        label_found=0;
                        for(i=0;i<symbol_count;i++)
                        {
                            if(strcmp(symbol_table[i].name,operand_str)==0)
                            {
                                operand_val=symbol_table[i].address;
                                label_found=1;
                                symbol_table[i].is_used=1;

                                if (strcmp(InstructionTable[j].mnemonic, "br") == 0) {
                                    if (operand_val == pc) {
                                        printf("WARNING at PC %04X: Infinite loop detected (branch to itself)\n", pc);
                                    }
                                }
                                break;
                            }
                        }
                        if(!label_found)
                        {
                            printf("ERROR at PC %04X: No such label '%s'\n", pc, operand_str);
                            operand_val=0;
                        }
                        else
                        {
                            if(InstructionTable[j].opcode==13 || (InstructionTable[j].opcode>=15 && InstructionTable[j].opcode<=17))
                            {
                                operand_val=operand_val-pc-1;
                            }
                        }
                       }
                    }
                    extra = strtok(NULL, " \t\r\n");
                        if (extra != NULL && extra[0] != ';') {
                            printf("ERROR at PC %04X: Extra text '%s' at end of line\n", pc, extra);
                        }
                        
                    }
                    else 
                    {
                        /* ERROR CHECK: Unexpected operand (Instruction doesn't take one) */
                        extra = strtok(NULL, " \t\r\n");
                        if (extra != NULL && extra[0] != ';') {
                            printf("ERROR at PC %04X: Unexpected operand '%s' for %s\n", pc, extra, token);
                        }
                    }
                    if(InstructionTable[j].opcode==-2)
                    {
                        continue;
                    }

                    if(InstructionTable[j].opcode==-1)
                    {
                        machine_code=operand_val;
                    }
                    else
                    {
                        machine_code=((operand_val<<8) & 0xFFFFFF00)|(InstructionTable[j].opcode & 0xFF);
                    }
        
                    fwrite(&machine_code,sizeof(int),1,objfile);
                    if (InstructionTable[j].operand)
                    {
                    fprintf(lstfile, "%08X %08X %s %s\n", pc, machine_code, InstructionTable[j].mnemonic, operand_str);
                    } else
                     {
                     fprintf(lstfile, "%08X %08X %s\n", pc, machine_code, InstructionTable[j].mnemonic);
                      }
                    if(InstructionTable[j].opcode!=-2)
                    {
                        pc++;
                    }
                    break;
                }
            }
            if(!found)
            {
                printf("ERROR at PC %04X: Unknown instruction '%s'\n", pc, token);
            }
        }
        for (i = 0; i < symbol_count; i++) {
        if (symbol_table[i].is_used == 0) {
            printf("WARNING: label '%s' defined but not used\n", symbol_table[i].name);
        }
    }
    fclose(infile);
    fclose(objfile);
    fclose(lstfile);
    printf("Assembly complete. Generated %s and %s\n", obj_filename, lst_filename);
    return 0;
}
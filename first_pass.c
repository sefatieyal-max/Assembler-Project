#include "first_pass.h"


/**
 *@brief check if the label name defined accordingly
 * @param label is the label name we are checking
 * @return TRUE is the label name defined accordingly and FALSE if not
 */
boolean label_name_check(const char *label) {
    int i;
    /*check length*/
    if (strlen(label)>LABEL_SIZE) {
        printf("Error - label \"%s\" is too long, max length is %d | ",label, LABEL_SIZE);
        return FALSE;
    }
    /*check if the first characters is a letter*/
    if (!isalpha(label[0])) {
        printf("Error - label \"%s\"  doesn't start with a letter | ", label);
        return FALSE;
    }
    /*check if the label is only letters and numbers */
    for (i = 1; label[i] != '\0'; i++) {
        if (!isalnum(label[i])) {
            printf("Error - label \"%s\" contains invalid characters | ", label);
            return FALSE;
        }
    }
    /*check if the label is register/directive/command*/
    if (is_reserved_word(label)) {
        printf("Error - label \"%s\" is reserved word | ", label);
        return FALSE;
    }
    return TRUE;
}
/**
 *@brief check if the label defined accordingly
 *
 * @param label_head is pointer to the head of the symbol table
 * @param label is the label
 * @param macro is pointer to the macro list head
 * @return TRUE is the label defined accordingly and FALSE if not
 */
boolean label_check(symbol_table * label_head, char * label, macro * macro) {
    /*check if the label name is valid*/
    if (label_name_check(label) == FALSE) {
        return FALSE;
    }
    /*check if the label name is macro*/
    if (macro_search(label,macro) != NULL) {
        printf("Error - label \"%s\" was defined as macro | ", label);
        return FALSE;
    }
    /* checks if we have already defined this label*/
    if (symbol_table_search(label,label_head) != NULL) {
        printf("Error - label \"%s\" already defined | ", label);
        return FALSE;
    }
    return TRUE;
}
/**
 *@brief insert string into the data image, each letter in 1 row
 *
 * @param data_head is a pointer to the head of the data image
 * @param ptr is pointer to the command, starting with .string
 * @param dc is the data counter
 *
 * @return OK if the insert was successful, return ERROR if there is a mistake
 */
int insert_string(data_mgr *data_head, const char *ptr, int dc) {
    data_image *curr;
    int i;
    char line[MAX_LINE_LEN], *str,*tmp;
    strcpy(line,ptr);
    strtok(line,"\"");
    tmp = strtok(NULL,"\n\r");
    /*check if we have " in the start and in the end*/
    if ((tmp == NULL) || strrchr(tmp,'\"') == NULL) {
        printf("Error - missing \" | ");
        return ERROR;
    }
    str = strtok(tmp,"\"");
     for(i = 0 ; str[i] != '\0';i++) {
         /*check if the character are in the limits*/
         if (str[i] < 32) {
             printf("Error - characters invalid | ");
             return ERROR;
         }
        /*allocate memory*/
         curr = create_malloc(sizeof(data_image));
         /*insert data*/
         curr->data = (int)str[i];
         curr->dc = ++dc;
         curr->next = data_head->head;
         data_head->head = curr;
     }
    /*create end string \0*/
    /*allocate memory*/
    curr = create_malloc(sizeof(data_image));
    /*insert data*/
    curr->data = 0;
    curr->dc = ++dc;
    curr->next = data_head->head;
    data_head->head = curr;
    /*check if there is a characters after the string*/
    if ((strtok(NULL,"\n")) == NULL) {
        return OK;
    }
    else {
        printf("Error -  invalid characters outside the string | ");
        return ERROR;
    }

}
/**
 *@brief insert data into the data image, each number in 1 row
 *
 * @param data_head is a pointer to the head of the data image
 * @param ptr is pointer to the command, starting with .string
 * @param dc is the data counter
 *
 * @return OK if the insert was successful, return ERROR if there is a mistake
 */
int insert_data(data_mgr *data_head, const char *ptr, int dc) {
    data_image *curr;
    int num;
    char *rest;
    ptr = ptr + strlen(".data");
    /*skip spaces*/
    while (isspace(*ptr)) {
        ptr++;
    }
    if (comma_check(ptr) == ERROR) {
        return ERROR;
    }
    if (*ptr == '\0') {
        printf("Error - missing data | ");
        return ERROR;
    }
    while (*ptr != '\0'){
        /* extract the next number*/
        num = (int)strtol(ptr,&rest,10);
        /*check if we do not find any number*/
        if (ptr == rest) {
            printf("Error - invalid characters inside data (\"%s\") | ",ptr);
            return ERROR;
        }
        /*check if the number is causing an overflow*/
        if (num < MINNUM || num >  MAXNUM) {
            printf("Error - the number %d is not between (-2047)-(2048) | ",num);
            return ERROR;
        }
        /*allocate memory*/
        curr = create_malloc(sizeof(data_image));
        /*insert data*/
        curr->data = num;
        curr->dc = ++dc;
        curr->next = data_head->head;
        data_head->head = curr;

        /*increase pointers and dc*/
        while ((*rest == ',') || isspace(*rest)) {/*skip comma and spaces*/
            rest++;
        }
        ptr = rest;
    }
    return OK;
}
/**
 *@brief check and insert data to the data image
 *
 * @param data_head is a pointer to the  head of the data image
 * @param cmd is the command line we read
 * @param dc ic the data counter
 * @return OK if the insert was successful and ERROR if an error is found
 */
int cmd_data(data_mgr *data_head,const char * cmd, int dc) {
    char line[MAX_LINE_LEN],*ptr;

    /*make a copy of the cmd*/
    strcpy(line,cmd);

    /*check if .string*/
    if ((ptr = strstr(line,".string "))) {
        if (insert_string(data_head,ptr,dc) == ERROR) {
            return ERROR;
        }
        return OK;
    }
    else if ((ptr = strstr(line,".data"))) {
        if (insert_data(data_head,ptr,dc) == ERROR) {
            return ERROR;
        }
        return OK;
    }
    printf("Error - command \"%s\" does not exist | ",line);
    return ERROR;
}

/**
 * @brief the function check if the operands defined according to the command, if there single operand the function switch between the source and destination operand
 *
 * @param opcode the opcode of the command
 * @param src_wrd the source operand
 * @param dst_wrd the destination operand
 *
 * @return OK if the operands defined according to the command, error if not
 */
int addressing_check(int opcode, char** src_wrd, char** dst_wrd) {
    if (TWO_OPERAND(opcode)) {
        if ((*src_wrd != NULL) && (*dst_wrd != NULL)) {
            return OK;
        }
    }
    else if (SINGLE_OPERAND(opcode)) {
        if ((*src_wrd != NULL) && (*dst_wrd == NULL)) {
            /*switch the source and destination words*/
            *dst_wrd = *src_wrd;
            *src_wrd = NULL;
            return OK;
        }
    }
    else if (ZERO_OPERAND(opcode)) {
        if ((*src_wrd == NULL) && (*dst_wrd == NULL)) {
            return OK;
        }
    }
    printf("Error - operands doesn't fit to the command | ");
    return ERROR;
}
/**
 * @brief the function receive an operand and return the address mode ( 0-immediate | 1-direct | 2-relative | 3-register)
 *
 * @param operand is the operand we are checking
 * @return the address mode we are using for the given operand
 */
int address_mode(const char* operand) {
    if (operand == NULL) {
        return OK;
    }
    /*immediate addressing*/
    if ((operand[0] == '#') && is_number(operand + 1)) {
        return IMMEDIATE_ADDRESSING;
    }
    /*register addressing*/
    if (is_register(operand)) {
        return REGISTER_ADDRESSING;
    }
    /*relative addressing*/
    if ((operand[0] == '%') && label_name_check(operand + 1)) {
        return RELATIVE_ADDRESSING;
    }
    /*direct addressing*/
    if (label_name_check(operand)) {
        return DIRECT_ADDRESSING;
    }
    printf("Error - the operand \"%s\" doesn't defined properly | ",operand);
    return ERROR;
}
/**
 * @check if the addressing mode allowed for the given command
 *
 * @param mode is the addressing mode we are checking
 * @param allowed is the allowed addressing modes
 * @return OK - if it allowed | ERROR - if not allowed
 */
int address_mode_check(const int mode, const int allowed) {
    int check;
    check = 1<<mode; /*translate the mode into a mask*/
    if ((check&allowed) == 0) {
        printf("Error - address mode (%d) does not allowed | ",mode);
        return ERROR;
    }
    else {
    return OK;
    }
}
/**
 * @brief the function receive a command line, translate it into binary and store in the code image (command and operands)
 * she calculates the command length and update the ic counter
 *
 * @param cmd the command line
 * @param ic the ic counter
 * @param code the code image
 *
 * @return ERROR if the command doesn't define properly and OK if it is
 */
int translate_code(char const * cmd,int *ic,code_image * code) {
    char *cmd_wrd, *dst_wrd, *src_wrd,*args, cmd_cpy[MAX_LINE_LEN];
    int i,l,src_mode,dst_mode;
    static command commands[]={/*define the commands*/
        {"mov",0,0,IMMEDIATE_MASK|DIRECT_MASK|REGISTER_MASK,DIRECT_MASK|REGISTER_MASK},
        {"cmp",1,0,IMMEDIATE_MASK|DIRECT_MASK|REGISTER_MASK,IMMEDIATE_MASK|DIRECT_MASK|REGISTER_MASK},
        {"add",2,10,IMMEDIATE_MASK|DIRECT_MASK|REGISTER_MASK,DIRECT_MASK|REGISTER_MASK},
        {"sub",2,11,IMMEDIATE_MASK|DIRECT_MASK|REGISTER_MASK,DIRECT_MASK|REGISTER_MASK},
        {"lea",4,0,DIRECT_MASK,DIRECT_MASK|REGISTER_MASK},
        {"clr",5,10,NONE,DIRECT_MASK|REGISTER_MASK},
        {"not",5,11,NONE,DIRECT_MASK|REGISTER_MASK},
        {"inc",5,12,NONE,DIRECT_MASK|REGISTER_MASK},
        {"dec",5,13,NONE,DIRECT_MASK|REGISTER_MASK},
        {"jmp",9,10,NONE,DIRECT_MASK|RELATIVE_MASK},
        {"bne",9,11,NONE,DIRECT_MASK|RELATIVE_MASK},
        {"jsr",9,12,NONE,DIRECT_MASK|RELATIVE_MASK},
        {"red",12,0,NONE,DIRECT_MASK|REGISTER_MASK},
        {"prn",13,0,NONE,IMMEDIATE_MASK|DIRECT_MASK|REGISTER_MASK},
        {"rts",14,0,NONE,NONE},
        {"stop",15,0,NONE,NONE},
        {"null",ERROR,ERROR},
    };
    /*initialize variable*/
    l = 1;/*add the command word length*/
    src_mode = 0;
    dst_mode = 0;

    /*cut the command*/
    strcpy(cmd_cpy,cmd);
    cmd_wrd = strtok(cmd_cpy," \t\n,");
    if (cmd_wrd == NULL) {
        printf("Error - missing command | ");
        return ERROR;
    }
    args = strtok(NULL,"\r\n");
    /*check arg commas*/
    if ((args != NULL )&& comma_check(args)==ERROR) {
        return ERROR;
    }
    src_wrd = strtok(args," \t\n,");
    dst_wrd = strtok(NULL," \t\n,");

    /*deal with the command word*/
    for (i=0;commands[i].opcode != ERROR;i++) {
        if (strcmp(cmd_wrd,commands[i].name)==0) {
            break;
        }
    }
    if (commands[i].opcode == ERROR ) {
        printf("Error -  \"%s\" is invalid command name | ",cmd_wrd);
        return ERROR;
    }
    insert_opcode(&code[(*ic)],commands[i].opcode);
    insert_funct(&code[(*ic)],commands[i].funct);
    code[*ic].code |= A;
    code[*ic].ic = (*ic);
    if (addressing_check(commands[i].opcode,&src_wrd,&dst_wrd) == ERROR) {
        return ERROR;
    }
    if (src_wrd != NULL) { /*deal with the source word*/
        src_mode = address_mode(src_wrd);
        if (src_mode == ERROR || address_mode_check(src_mode,commands[i].src_method) == ERROR) {
            return ERROR;
        }
        /*translate the source operand*/
        switch (src_mode) {
            case IMMEDIATE_ADDRESSING:
                code[(*ic)+l].code = ((atoi(src_wrd+1)&CODE_MASK) | A);
                break;
            case REGISTER_ADDRESSING:
                code[(*ic)+l].code = ((1 << (atoi(src_wrd+1)&CODE_MASK)) | A);
                break;
            default:
                break;
        }
        l++;/*add the source word length*/
    }
    if (dst_wrd != NULL) { /*deal with the destination word*/
        dst_mode = address_mode(dst_wrd);
        if (dst_mode == ERROR || address_mode_check(dst_mode,commands[i].dst_method) == ERROR) {
            return ERROR;
        }
        /*translate the destination operand*/
        switch (dst_mode) {
            case IMMEDIATE_ADDRESSING:
                code[(*ic)+l].code = ((atoi(dst_wrd+1)&CODE_MASK) | A);
                break;
            case REGISTER_ADDRESSING:
                code[(*ic)+l].code = ((1 << (atoi(dst_wrd+1)&CODE_MASK)) | A);
                break;
            default:
                break;
        }
        l++; /*add the destination word length*/
    }
    insert_addressing_mode(&code[(*ic)],src_mode,dst_mode);
    (*ic)+= l;/*add the length to ic*/
    return OK;
}
/**
 * @brief the function update the dc in the date symbols according to the final ic
 *
 * @param symbol_head a pointer to the symbol table linked list
 * @param ic the final ic counter after the first pass
 */
void update_data_symbol(const symbol_table_mgr symbol_head, int ic) {
    symbol_table *curr;
    curr = symbol_head.head;
    while (curr != NULL) {
        if (curr->data == TRUE) {
            curr->value += ic;
        }
        curr = curr->next;
    }
}

/**
 * @brief execute the first pass of the assembler
 *
 * @param file_name the file we are decoding
 * @param macro_head pointer to the macro linked list head
 * @param symbol_head pointer to the symbol table linked list head
 * @param data_head pointer to the date table linked list head
 * @param code the code image for the file we are decoding
 *
 * @return OK if the first passage was successful and ERROR if not
 */
int run_first_pass(const char *file_name, const macro_mgr *macro_head,symbol_table_mgr *symbol_head,data_mgr *data_head,code_image *code) {
    char *file_am,cpy_line[MAX_LINE_LEN], line[MAX_LINE_LEN],*label, *cmd;
    FILE *fp_am;
    int ic,dc,line_count,error_count,total_error;

    /*initialize variables*/
    ic = FIRST_MEMORY;
    dc = 0;
    line_count = 0;
    error_count = 0;
    total_error = 0;


    /*open the file after the macro proceeding -".am"*/
    file_am = make_file_name(file_name,".am");
    if ((fp_am = fopen(file_am, "r") ) == NULL) {/*check if we succeed to open the file*/
        printf("Error opening file - %s\n", file_am);
        /*release and close file if we don't succeed to open*/
        end_use("p",file_am);
        return ERROR;
    }

    while (fgets(line,MAX_LINE_LEN,fp_am)) {

        /*copy line content and update the line counter*/
        strcpy(cpy_line,line);
        line_count++;

        /*skip comment line*/
        if (is_comment(cpy_line)) {
            continue;
        }
        /*check if data directives*/
        if (is_data_direct(cpy_line)) {
            if (is_label(cpy_line)) {
                /* separate the line into the label and the command*/
                label = strtok(line,":");
                cmd = strtok(NULL,"\n");
                if (label_check(symbol_head->head,label,macro_head->head)) {
                    insert_symbol(symbol_head,label,dc,"data");
                }
                else {/*the label is defined wrong*/
                    error_count++;
                }
            }
            else {
                cmd = strtok(line,"\n");
            }
            if (cmd_data(data_head,cmd,dc) == ERROR) {/*the data is written wrong*/
                error_count++;
            }
            else {/*enter the data only  if the dat is written properly*/
                dc = data_head->head->dc;
            }
        }
        /*check if is entry directive*/
        else if (is_entry(cpy_line)){
            continue;/*deal in the second pass*/
        }
        /*check if extern directive*/
        else if (is_extern(cpy_line)) {
            /* skip the .extern*/
            label = strtok(cpy_line + strlen(".extern "), " \t\n");
            if (label_check(symbol_head->head,label,macro_head->head)) {
                insert_symbol(symbol_head,label,0,"external");
            }
            else {
                error_count++;
            }


        }
        /*code line*/
        else {
            /*check for label*/
            if (is_label(cpy_line)) {
                /* separate the line into the label and the command*/
                label = strtok(line,":");
                cmd = strtok(NULL,"\n");
                if (label_check(symbol_head->head,label,macro_head->head)) {
                    insert_symbol(symbol_head,label,ic,"code");
                }
                else {/*the label is defined wrong*/
                    error_count++;
                }
            }
            else {
                cmd = cpy_line;
            }
            if (cmd != NULL) {/*check if we have command in the line*/
                if (translate_code(cmd,&ic,code) == ERROR) {
                    error_count++;
                }
            }
        }
        if (total_error < error_count) {
            printf("[line %d]\n",line_count);
            total_error = error_count;
        }
    }
    update_data_symbol(*symbol_head,ic);
    end_use("pf",file_am,fp_am);
    /*check if we have errors on the first pass*/
    if (total_error > 0) {
        return ERROR;
    }
    return OK;
}
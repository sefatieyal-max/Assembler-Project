#include "second_pass.h"


/**
 * @brief the function receive a command line end translate and enter the labels into the code image
 *
 * @param symbol_head pointer to the symbol table head
 * @param code is the code image
 * @param ic is pointer to the ic of the line
 * @param cmd is the cmd we are decoding
 *
 * @return OK if the decoding was successful and ERROR if not
 */
int translate_label(symbol_table_mgr * symbol_head, code_image * code, int * ic, const char * cmd) {
    char *dst_wrd, *src_wrd, cmd_cpy[MAX_LINE_LEN];
    int src_mode,dst_mode,opcode;
    symbol_table *curr;


    /*cut the command*/
    strcpy(cmd_cpy,cmd);
    strtok(cmd_cpy," \t\n,");
    src_wrd = strtok(NULL," \t\n,");
    dst_wrd = strtok(NULL," \t\n,");

    /*extract the addressing mode*/
    src_mode = extract_addressing_mode(&code[*ic],SRC_ADDRESS_POSITION);
    dst_mode = extract_addressing_mode(&code[*ic],DST_ADDRESS_POSITION);

    /*search and replace labels*/
    opcode = extract_opcode(&code[*ic]);
    (*ic)++;
    if (SINGLE_OPERAND(opcode)) {
        switch (dst_mode) {
            case DIRECT_ADDRESSING:
                if ((curr = symbol_table_search(src_wrd,symbol_head->head)) == NULL) {
                    printf("Error - label \"%s\" is not defined | ",src_wrd);
                    return ERROR;
                }
                if (curr->external) {
                    code[(*ic)].code = E;
                    /*add the to the symbol table with the current ic for print ext*/
                    insert_symbol(symbol_head,curr->name,(*ic),"external");
                }
                else {
                    code[(*ic)].code = (curr->value | R);
                }
                break;
            case RELATIVE_ADDRESSING:
                if ((curr = symbol_table_search(src_wrd+1,symbol_head->head)) == NULL) {
                    printf("Error - label \"%s\" is not defined | ",src_wrd+1);
                    return ERROR;
                }
                if (curr->external) {
                    code[(*ic)].code = E;
                    /*add the to the symbol table with the current ic for print ext*/
                    insert_symbol(symbol_head,curr->name,(*ic),"external");
                }
                else {
                    code[(*ic)].code = ((curr->value -(*ic)) | R);
                }
                break;
            default:;
        }
        (*ic)++;
    }
    else if (TWO_OPERAND(opcode)) {
        switch (src_mode) {
            case DIRECT_ADDRESSING:
                if ((curr = symbol_table_search(src_wrd,symbol_head->head)) == NULL) {
                    printf("Error - label \"%s\" is not defined | ",src_wrd);
                    return ERROR;
                }
                if (curr->external) {
                    code[(*ic)].code = E;
                    /*add the to the symbol table with the current ic for print ext*/
                    insert_symbol(symbol_head,curr->name,(*ic),"external");
                }
                else {
                    code[(*ic)].code = (curr->value | R);
                }
                break;
            case RELATIVE_ADDRESSING:
                if ((curr = symbol_table_search(src_wrd+1,symbol_head->head)) == NULL) {
                    printf("Error - label \"%s\" is not defined | ",src_wrd+1);
                    return ERROR;
                }
                if (curr->external) {
                    code[(*ic)].code = E;
                    /*add the to the symbol table with the current ic for print ext*/
                    insert_symbol(symbol_head,curr->name,(*ic),"external");
                }
                else {
                    code[(*ic)].code = ((curr->value -(*ic)) | R);
                }
                break;
            default:;
        }
        (*ic)++;
        switch (dst_mode) {
            case DIRECT_ADDRESSING:
                if ((curr = symbol_table_search(dst_wrd,symbol_head->head)) == NULL) {
                    printf("Error - label \"%s\" is not defined | ",dst_wrd);
                    return ERROR;
                }
                if (curr->external) {
                    code[(*ic)].code = E;
                    /*add the to the symbol table with the current ic for print ext*/
                    insert_symbol(symbol_head,curr->name,(*ic),"external");
                }
                else {
                    code[(*ic)].code = (curr->value | R);
                }
                break;
            case RELATIVE_ADDRESSING:
                if ((curr = symbol_table_search(dst_wrd+1,symbol_head->head)) == NULL) {
                    printf("Error - label \"%s\" is not defined | ",dst_wrd+1);
                    return ERROR;
                }
                if (curr->external) {
                    code[(*ic)].code = E;
                    /*add the to the symbol table with the current ic for print ext*/
                    insert_symbol(symbol_head,curr->name,(*ic),"external");
                }
                else {
                    code[(*ic)].code = ((curr->value -(*ic)) | R);
                }
                break;
            default:;
        }
        (*ic)++;
    }
    return OK;
}
/**
 * @brief execute the second pass of the assembler it the pass was successful we star make the output file
 *
 * @param file_name the file we are decoding
 * @param symbol_head pointer to the symbol table linked list head
 * @param data_head pointer to the date table linked list head
 * @param code the code image for the file we are decoding
 *
 * @return OK if the first passage was successful and ERROR if not
 */
int run_second_pass(char * file_name, symbol_table_mgr * symbol_head, const data_mgr * data_head, code_image * code) {
    char *file_am,cpy_line[MAX_LINE_LEN], line[MAX_LINE_LEN],*cmd;
    FILE *fp_am;
    int line_count,error_count,total_error,ic;
    symbol_table *curr;

    /*initialize variables*/
    line_count = 0;
    error_count = 0;
    total_error = 0;
    ic = FIRST_MEMORY;


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
        /*check for label and skip the label*/
        if (is_label(cpy_line)) {
            /* separate the line into the label and the command*/
            strtok(line,":");
            cmd = strtok(NULL,"\n");
        }
        else {
            cmd = cpy_line;
        }
        /*skip if data/string/extern*/
        if (is_data_direct(cmd) || is_extern(cmd) || is_data_direct(cmd)) {
            continue;
        }
        /*deal with entry*/
        else if (is_entry(cmd)) {
            /*skip the entry command*/
            cmd = strtok(cmd + strlen(".entry "), "\n");
            if ((curr = symbol_table_search(cmd,symbol_head->head)) == NULL) {
                printf("Error - entry \"%s\" is not defined | ",cmd);
                error_count++;
            }
            else {
            curr->entry = TRUE;
            }
        }
        else if (translate_label( symbol_head,code,&ic,cmd) == ERROR) {
            error_count++;
        }
        /*print the line if was an error*/
        if (total_error < error_count) {
            printf("[line %d]\n",line_count);
            total_error = error_count;
        }
    }
    end_use("pf",file_am,fp_am);
    /*check if we have errors on the first pass*/
    if (total_error > 0) {
        return ERROR;
    }
    /*make the output files*/
    if (make_output_files(file_name,symbol_head,data_head,code,ic,data_head->head->dc) == ERROR) {
        return ERROR;
    }
    return OK;
}

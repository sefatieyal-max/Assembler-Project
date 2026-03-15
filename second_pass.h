#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H

#include "globals.h"
#include "utilities.h"
#include "structs.h"
#include "output_files.h"



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
int translate_label(symbol_table_mgr * symbol_head, code_image * code, int * ic, const char * cmd);
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
int run_second_pass(char * file_name, symbol_table_mgr * symbol_head, const data_mgr * data_head, code_image * code);

#endif //ASSEMBLER_SECOND_PASS_H
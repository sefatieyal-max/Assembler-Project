#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

#include "globals.h"
#include "structs.h"

/**
 *@brief check if the label name defined accordingly
 * @param label is the label name we are checking
 * @return TRUE is the label name defined accordingly and FALSE if not
 */
boolean label_name_check(const char *label);
/**
 *@brief check if the label defined accordingly
 *
 * @param label_head is pointer to the head of the symbol table
 * @param label is the label
 * @param macro is pointer to the macro list head
 * @return TRUE is the label defined accordingly and FALSE if not
 */
boolean label_check(symbol_table * label_head, char * label, macro * macro);
/**
 *@brief insert string into the data image, each letter in 1 row
 *
 * @param data_head is a pointer to the head of the data image
 * @param ptr is pointer to the command, starting with .string
 * @param dc is the data counter
 *
 * @return OK if the insert was successful, return ERROR if there is a mistake
 */
int insert_string(data_mgr *data_head, const char *ptr, int dc);
/**
 *@brief insert data into the data image, each number in 1 row
 *
 * @param data_head is a pointer to the head of the data image
 * @param ptr is pointer to the command, starting with .string
 * @param dc is the data counter
 *
 * @return OK if the insert was successful, return ERROR if there is a mistake
 */
int insert_data(data_mgr *data_head, const char *ptr, int dc);
/**
 *@brief check and insert data to the data image
 *
 * @param data_head is a pointer to the  head of the data image
 * @param cmd is the command line we read
 * @param dc ic the data counter
 * @return OK if the insert was successful and ERROR if an error is found
 */
int cmd_data(data_mgr *data_head,const char * cmd, int dc);

/**
 * @brief the function check if the operands defined according to the command, if there single operand the function switch between the source and destination operand
 *
 * @param opcode the opcode of the command
 * @param src_wrd the source operand
 * @param dst_wrd the destination operand
 *
 * @return OK if the operands defined according to the command, error if not
 */
int addressing_check(int opcode, char** src_wrd, char** dst_wrd);
/**
 * @brief the function receive an operand and return the address mode ( 0-immediate | 1-direct | 2-relative | 3-register)
 *
 * @param operand is the operand we are checking
 * @return the address mode we are using for the given operand
 */
int address_mode(const char* operand);
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
int translate_code(char const * cmd,int *ic,code_image * code);
/**
 * @brief the function update the dc in the date symbols according to the final ic
 *
 * @param symbol_head a pointer to the symbol table linked list
 * @param ic the final ic counter after the first pass
 */
void update_data_symbol(symbol_table_mgr symbol_head, int ic);

/**
 * @brief execute the first pass of the assembler, it the first pass was successful the function call for the second pass
 *
 * @param file_name the file we are decoding
 * @param macro_head pointer to the macro linked list head
 * @param symbol_head pointer to the symbol table linked list head
 * @param data_head pointer to the date table linked list head
 * @param code the code image for the file we are decoding
 *
 * @return OK if the first passage was successful and ERROR if not
 */
int run_first_pass(const char *file_name, const macro_mgr *macro_head,symbol_table_mgr *symbol_head,data_mgr *data_head,code_image *code);
#endif //ASSEMBLER_FIRST_PASS_H
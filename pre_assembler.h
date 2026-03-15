#ifndef ASSEMBLER_MACRO_H
#define ASSEMBLER_MACRO_H

#include "structs.h"
#include "globals.h"

/**
 *@brief return if we have "mcro" in the given line
 *
 *@param line is the line we are checking
 *@return true if there "mcro" and FALSE if it doesn't
 */
boolean macro_start(const char *line);
/**
 *@brief return if we have "mcro" in the given line
 *
 *@param line is the line we are checking
 *@return true if there "mcro" and FALSE if it doesn't
 */
boolean macro_end(const char *line);
/**
 *@brief this function check if the macro end line write properly
 *
 *@param line is the line we are checking
 *@return TRUE if the line written properly and FALSE if its doesn't
 */
boolean macro_end_check(const char * line);
/**
 *@brief this function check if the macro name is legal
 *
 *@param macro_name is the name we are checking
 *@return TRUE if the name written properly and FALSE if its doesn't
 */
boolean macro_name_check(char * macro_name);
/**
 *@brief the function add macro the macro linked list
 *
 *she takes the macro check if he writes accordingly and then copy all the contact into the linked list
 *
 *@param mgr is the pointer to the last macro we have (head)
 *@param fp is the pointer to the lines we read from the file
 *@param line is the first line of the macro
 *@return pointer to the head in the macro linked list
 */
int add_macro(macro_mgr *mgr, FILE * fp, char * line);
/**
 *@brief create a new macro - without content
 *
 *@param mgr is pointer to the head of the linked list
 *@param line is the line with the name of the macro
 *@return pointer to the created macro (the new head)
 */
int create_macro(macro_mgr * mgr, char * line);
/**
 *@brief the main function of the pre_assembler - she takes a file and remove and replace all the macros
 *
 *@param file_name is the file name we are macr decoding
 *@param mgr is the head for the macro linked list
 *@return OK if the macro decoding was successful and ERROR if not
 */
int run_pre_assembler(char *file_name,macro_mgr *mgr);

#endif //ASSEMBLER_MACRO_H
#ifndef ASSEMBLER_OUTPUT_FILES_H
#define ASSEMBLER_OUTPUT_FILES_H

#include "structs.h"


/**
 * @brief create the object file and print the code and data
 *
 * @param file_name the name of the file
 * @param data_head pointer to the table data head
 * @param code is the code image
 * @param fic the final number of ic
 * @param fdc the final number if dc
 *
 * @return OK if the print was successful and ERROR if not
 */
int make_ob_file(const char * file_name, const data_image * data_head, const code_image * code, int fic, int fdc);
/**
 * @brief the function crate the entry file and print the entry use and location in it
 *
 * @param file_name is the name of the file
 * @param head is pointer to the head of the symbol table
 * @return OK if creating the file was successful end ERROR if not
 */
int make_ent_file(const char * file_name, symbol_table * head);
/**
 * @brief the function crate the external file and print the external use and location in it
 *
 * @param file_name is the name of the file
 * @param head is pointer to the head of the symbol table
 * @return OK if creating the file was successful end ERROR if not
 */
int make_ext_file(const char * file_name, symbol_table * head);
/**
 * @brief create end print the output files: object,external and entry
 *
 * @param file_name the name of the file
 * @param symbol_head is pointer to the head of the symbol table
 * @param data_head pointer to the table data head
 * @param code is the code image
 * @param fic the final number of ic
 * @param fdc the final number if dc
 *
 * @return OK if creating the files was successful end ERROR if not
 */
int make_output_files(const char * file_name, const symbol_table_mgr * symbol_head, const data_mgr * data_head, const code_image * code, int fic, int fdc);
#endif //ASSEMBLER_OUTPUT_FILES_H
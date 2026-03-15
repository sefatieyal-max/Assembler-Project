#ifndef ASSEMBLER_UTILITIES_H
#define ASSEMBLER_UTILITIES_H

#include "globals.h"

/**
 *@brief this function allocate memory and checking if the allocation was successful
 *
 *@param size is the size of memory we want to allocate
 *@return the pointer to the memory
 */
void* create_malloc(size_t size);
/**
 *@brief this function allocate memory and checking if the allocation was successful
 *
 *@param size is the size of memory we want to allocate
 *@param memory is the memory we reallocate
 *@return the pointer to the memory
 */
void* do_realloc(char *memory,size_t size);
/**
 *@brief this function clean spaces in 1 line
 *
 *@param src is the pointer to source line
 *@param dst is the pointer to the "clean" line
 */
void clean_line_spaces(char* dst, const char* src);
/**
 *@brief the clean al the extra spaces from the files
 *
 *@param filename is the name of the file we are cleaning
    *
 *@return a pointer to the clean file
 */
char *clean_file_spaces(char filename[]);
/**
 *@brief the function create a file with a requested ending
 *
 *@param file_name is the name of the new file
 *@param finish is the ending of the new lin
 *@return a pointer to the new file
 *
 *@note the function delete in the file name all the content after '.'
 */
char* make_file_name(const char *file_name, const char *finish);
/**
 *@brief copy a file content into a new file
 *
 *@param src_file is the source file pointer
 *@param dst_file is the destination file pointer
 *@return OK if the copying was successful or ERROR if not
 */
int copy_file(char *src_file, char *dst_file);
/**
 *@brief check if the given word is a command
 *
 * @param word is the word we are checking
 * @return TRUE if the word is command FALSE if it is not
 */
boolean is_command(const char *word);
/**
 *@brief check if the given word is a directive
 *
 * @param word is the word we are checking
 * @return TRUE if the word is a directive FALSE if it is not
 */
boolean is_directive(const char *word);
/**
 *@brief check if the given word is a register
 *
 * @param word is the word we are checking
 * @return TRUE if the word is a register FALSE if it is not
 */
boolean is_register(const char *word);
/**
 *@brief check if the given word is a command or directives or register
 *
 * @param word is the word we are checking
 * @return TRUE if the word is command/directives/register FALSE if it is not
 */
boolean is_reserved_word(const char *word);
/**
 *@brief this function add text into given source
 *
 * @param src is the text we are coping into
 * @param text is the text we are adding
 * @param len is the length of the source text
 * @return pointer to the txt
 */
char *copy_text(char *src, const char *text,int len);
/**
 *@brief this function receive a list of files and pointer and close the file / release the pointer
 *
 * @param format a string represent the type of the variable p-pointer | f-file
 * @param ...   a list of variable
 */
void end_use(const char *format, ...);
/**
 * @brief the function checking if the line is a comment (start with ';')
 * @param line is the line we are checking
 * @return true if the line is a comment, false if not
 */
boolean is_comment(const char line[MAX_LINE_LEN]);
/**
 *@brief function check if the comma in the command are valid, if not she print an error message accordingly.
 *
 *@param str is the command we are checking
 *@return OK if the commas are valid, ERROR if the comma invalid.
 */
int comma_check(const char *str);


/* checking lines function*/

/**
 *@brief this function search if there label on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there label, return FALSE if isn't
 */
boolean is_label(const char *line);
/**
 *@brief this function search if their data directive on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there is data directive, return FALSE if isn't
 */
boolean is_data_direct(const char * line);
/**
 *@brief this function search if their entry directive on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there is entry directive, return FALSE if isn't
 */
boolean is_entry(const char *line);
/**
 *@brief this function search if their extern directive on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there is extern directive, return FALSE if isn't
 */
boolean is_extern(const char *line);
/**
 * @brief the function check if the line is only a number
 *
 * @param line is the line we are checking
 * @return TRUE if the line is only a number and FALSE if not
 */
boolean is_number(const char *line);
#endif //ASSEMBLER_UTILITIES_H
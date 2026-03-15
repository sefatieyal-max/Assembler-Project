#ifndef ASSEMBLER_STRUCTS_H
#define ASSEMBLER_STRUCTS_H

#include "globals.h"
#include "utilities.h"

/*linked list for the macros*/
typedef struct macro {
    char *name; /*macro name*/
    char *content; /*macro contact - all the line in 1 string*/
    struct macro *next; /*pointer do the next macro*/
}macro;
/* pointer to the linked list head*/
typedef struct {
    macro *head;
}macro_mgr;

/**
 * @brief the function release all the macro linked list
 * @param head  is the head of the linked list
 */
void free_macro(macro * head);
/**
 *@brief search if we have macro fit to the name
 *
 *@param name the macro name we are searching
 *@param curr pointer to the head of the list
 *@return pointer to the macro if we find and NULL if we don't
 */
macro *macro_search(const char * name, macro *curr);



/*linked list represent the symbols in the program*/
typedef struct symbol_table{
    char name[LABEL_SIZE]; /*symbol name*/
    int value; /*the value of the symbol*/
    boolean data;/* data directive in the line*/
    boolean external;/* external directive in the line*/
    boolean entry;/* entry directive in the line*/
    boolean code; /*code in the line*/
    struct symbol_table *next;/*pointer to the next symbol*/
}symbol_table;
/* pointer to the linked list head*/
typedef struct {
    symbol_table *head;
}symbol_table_mgr;
/**
 * @brief add label/symbol to the symbol table (linked list)
 *
 * @param label_head is the head of the linked list
 * @param name is the label
 * @param dc is the DC counter
 * @param type is the type of the symbol type (code/entry/external/date)
 * @return pointer to the new head of the table
 */
int insert_symbol(symbol_table_mgr *label_head,  char *name, int dc, const char *type);
/**
 * @brief  search for label in the symbol table
 * @param label  is the label name we are searching
 * @param curr  is a pointer to the tabel head
 * @return pointer to the label in the symbol table if we find and NULL if not
 */
symbol_table * symbol_table_search(const char * label, symbol_table * curr);
/**
 * @brief the function release all the symbol table linked list
 * @param head  is the head of the linked list
 */
void free_symbol(symbol_table * head);

/*linked list were we save all the string adn variable in the program*/
typedef struct data_image {
    int data;
    int dc;
    struct data_image *next;
}data_image;
/* pointer to the linked list head*/
typedef struct {
    data_image *head;
}data_mgr;

/**
 * @brief the function release all the data image linked list
 * @param head  is the head of the linked list
 */
void free_data(data_image *head);


/*a struct for the command*/
typedef struct command {
    char name[CMD_SIZE];
    int opcode;
    int funct;
}command;
/*the struct contain the code image from the code in binary*/
typedef struct code_image {
    unsigned short code;
    int ic;
}code_image;
/**
 * @brief the function enter the opcode int the code image in binary (bits 8-11)
 *
 * @param code_image pointer to the code in the code image, the value in the array is the [ic]
 * @param opcode the opcode we translate
 */
void insert_opcode(code_image * code_image, int opcode);
/**
 * @brief the function enter the funct into the code image in binary (bits 4-7)
 *
 * @param code_image pointer to the code in the code image, the value in the array is the [ic]
 * @param funct the funct we translate
 */
void insert_funct(code_image * code_image, int funct);
/**
 * @brief the function enter the addressing modes into the code image in binary (src bit 2-3 | dst bits 0-1)
 *
 * @param code_image pointer to the code in the code image, the value in the array is the [ic]
 * @param src source addressing mode
 * @param dst destination addressing mode
 */
void insert_addressing_mode(code_image * code_image, int src, int dst);
/**
 * @brief extract the addressing mode from the code
 *
 * @param code_image the code we are extracting from
 * @param position the position we are extracting
 *
 * @return the addressing mode according to the position
 */
int extract_addressing_mode(const code_image *code_image, int position);
/**
 * @brief extract the opcode from the code
 *
 * @param code_image the code we are extracting from
 *
 * @return the opcode
 */
int extract_opcode(const code_image *code_image);
/**
 * @brief extract the code
 *
 * @param code_image the code image we are extracting from
 *
 * @return the code
 */
int extract_code(const code_image *code_image);
/**
 * @brief extract A,R,E status from the code
 *
 * @param code_image the code we are extracting from
 *
 * @return A|R|E according to the code
 */
char extract_are(const code_image *code_image);

#endif //ASSEMBLER_STRUCTS_H
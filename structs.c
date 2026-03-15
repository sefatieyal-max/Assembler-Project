#include "structs.h"



/**
 * @brief the function release all the macro linked list
 * @param head  is the head of the linked list
 */
void free_macro(macro * head) {
    macro *curr,*temp;
    curr = head;
    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }
}
/**
 *@brief search if we have macro fit to the name
 *
 *@param name the macro name we are searching
 *@param curr pointer to the head of the list
 *@return pointer to the macro if we find and NULL if we don't
 */
macro *macro_search(const char * name, macro *curr) {
    /*make a copy to remove new line chars*/
    char temp[MAX_LINE_LEN];
    strcpy(temp,name);
    temp[strcspn(temp,"\n")] = '\0';
    while (curr != NULL) {
        if (strcmp(curr->name,temp) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/**
 * @brief add label/symbol to the symbol table (linked list)
 *
 * @param label_head is the head of the linked list
 * @param name is the label
 * @param dc is the DC counter
 * @param type is the type of the symbol type (code/entry/external/date)
 * @return pointer to the new head of the table
 */
int insert_symbol(symbol_table_mgr *label_head,  char *name, const int dc, const char *type) {
    symbol_table *curr;
    int i;
    static const char *types[]= {"code","data","external","entry",NULL};

    /*cut the end of line*/
    strtok(name,"\n");
    /*allocate memory*/
    curr = create_malloc(sizeof(symbol_table));
    /*add a unit in the symbol table*/
    strcpy(curr->name,name);
    curr->value = dc;
    curr->next = label_head->head;
    label_head->head = curr;

    /*find which type the symbol and put the correct flag*/
    for (i=0; (types[i] != NULL); i++){
        if (strcmp(types[i],type)==0) {
            break;
        }
    }
    switch (i) {
        case 0: /*code type*/
            curr->data = FALSE;
            curr->code = TRUE;
            curr->entry = FALSE;
            curr->external = FALSE;
            break;
        case 1: /*data type*/
            curr->data = TRUE;
            curr->code = FALSE;
            curr->entry = FALSE;
            curr->external = FALSE;
            break;
        case 2: /*external type*/
            curr->data = FALSE;
            curr->code = FALSE;
            curr->entry = FALSE;
            curr->external = TRUE;
            break;
        case 3:/*entry type*/
            curr->entry = TRUE;
            break;
        default:
            curr->data = FALSE;
            curr->code = FALSE;
            curr->entry = FALSE;
            curr->external = FALSE;
            break;
    }
    return OK;
}
/**
 * @brief  search for label in the symbol table
 * @param label  is the label name we are searching
 * @param curr  is a pointer to the tabel head
 * @return pointer to the label in the symbol table if we find and NULL if not
 */
symbol_table * symbol_table_search(const char * label, symbol_table * curr) {
    while (curr != NULL) {
        if (strcmp(label,curr->name) == 0 ) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
/**
 * @brief the function release all the symbol table linked list
 * @param head  is the head of the linked list
 */
void free_symbol(symbol_table * head) {
    symbol_table *curr,*temp;
    curr = head;
    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }
}
/**
 * @brief the function release all the data image linked list
 * @param head  is the head of the linked list
 */
void free_data(data_image *head) {
    data_image *curr,*temp;
    curr = head;
    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }
}


/**
 * @brief the function enter the opcode int the code image in binary (bits 8-11)
 *
 * @param code_image pointer to the code in the code image, the value in the array is the [ic]
 * @param opcode the opcode we translate
 */
void insert_opcode(code_image * code_image, int opcode) {
    (code_image->code) |= (opcode << OPCODE_POSITION);
}
/**
 * @brief the function enter the funct into the code image in binary (bits 4-7)
 *
 * @param code_image pointer to the code in the code image, the value in the array is the [ic]
 * @param funct the funct we translate
 */
void insert_funct(code_image * code_image, int funct) {
    (code_image->code) |= (funct << FUNCT_POSITION);
}
/**
 * @brief the function enter the addressing modes into the code image in binary (src bit 2-3 | dst bits 0-1)
 *
 * @param code_image pointer to the code in the code image, the value in the array is the [ic]
 * @param src source addressing mode
 * @param dst destination addressing mode
 */
void insert_addressing_mode(code_image * code_image, int src, int dst) {
    (code_image->code) |= (src << SRC_ADDRESS_POSITION);
    (code_image->code) |= (dst << DST_ADDRESS_POSITION);
}
/**
 * @brief extract the addressing mode from the code
 *
 * @param code_image the code we are extracting from
 * @param position the position we are extracting
 *
 * @return the addressing mode according to the position
 */
int extract_addressing_mode(const code_image *code_image, const int position) {
    return ((code_image->code)>>position) & ADDRESS_MASK;
}
/**
 * @brief extract the opcode from the code
 *
 * @param code_image the code we are extracting from
 *
 * @return the opcode
 */
int extract_opcode(const code_image *code_image) {
    return ((code_image->code)>>OPCODE_POSITION) & OPCODE_MASK;
}
/**
 * @brief extract the code
 *
 * @param code_image the code image we are extracting from
 *
 * @return the code
 */
int extract_code(const code_image *code_image) {
    return ((code_image->code)& CODE_MASK);
}
/**
 * @brief extract A,R,E status from the code
 *
 * @param code_image the code we are extracting from
 *
 * @return A|R|E according to the code
 */
char extract_are(const code_image *code_image) {
    int are;
    are = (code_image->code & ARE_MASK);
    switch (are) {
        case(A):
            return 'A';
        case(R):
            return 'R';
        case(E):
            return 'E';
        default:
            return '?';
    }
}

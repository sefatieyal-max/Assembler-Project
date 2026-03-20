#include "globals.h"
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"

/**
 *@brief this is the main function the processes input files and make the output files
 *
 *@param argc the number of command line arguments
 *@param argv array of string containing the command line arguments
 *@return 0 if the program run successfully
 */
int main(int argc, char *argv[]) {
    macro_mgr macro_head;
    symbol_table_mgr symbol_head;
    data_mgr data_head;
    code_image code[MEMORY_SIZE];
    while (--argc > 0) {
        /*reset the structs*/
        macro_head.head = NULL;
        symbol_head.head = NULL;
        data_head.head = NULL;
        memset(code,0,sizeof(code));
        printf("\n\nstart file -\"%s\"...\n",argv[argc]);
        /*run and check if the macro processor seceded*/
        if (run_pre_assembler(argv[argc],&macro_head) == OK) {
            /*run and check the first pass on the file*/
            if (run_first_pass(argv[argc],&macro_head,&symbol_head,&data_head,code) == OK) {
                /*run the second pass on the file*/
                run_second_pass(argv[argc],&symbol_head,&data_head,code);
            }
        }
        /*release memory*/
        free_data(data_head.head);
        free_macro(macro_head.head);
        free_symbol(symbol_head.head);
    }
    return OK;
}




















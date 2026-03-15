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
    static code_image code[MEMORY_SIZE];
    while (--argc > 0) {
        /*reset the structs*/
        macro_head.head = NULL;
        symbol_head.head = NULL;
        data_head.head = NULL;
        /*run and check if the macro processor seceded*/
        if (run_pre_assembler(argv[argc],&macro_head) == ERROR) {
            continue;
        }
        /*run and check the first pass on the file*/
        if (run_first_pass(argv[argc],&macro_head,&symbol_head,&data_head,code) == ERROR) {
            return ERROR;
        }
        /*run and check the second pass on the file*/
        if (run_second_pass(argv[argc],&symbol_head,&data_head,code) == ERROR) {
            return ERROR;
        }
    }
    return OK;
}




















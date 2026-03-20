#include "output_files.h"

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
int make_ob_file(const char * file_name, const data_image * data_head, const code_image * code, const int fic, const int fdc) {
    int ic,i,word,data[MEMORY_SIZE] ;
    char *file, type;
    FILE *fp;
    /*create and open the ".ob" file for writing*/
    file = make_file_name(file_name,".ob");
    if ((fp = fopen(file, "w") ) == NULL) {/*check if we succeed to open the file*/
        printf("Error opening file - %s\n", file);
        /*release and close file if we don't succeed to open*/
        end_use("p",file);
        return ERROR;
    }
    /*print the size of the code and data*/
    fprintf(fp, "\t%d %d\n", (fic-FIRST_MEMORY),fdc);
    /*print the code*/
    for (ic = FIRST_MEMORY; ic < fic ; ic++) {
        word = extract_code(&code[ic]);
        type = extract_are(&code[ic]);
        fprintf(fp, "%04d %03X %c\n",ic,word,type);
    }
    /*extract the data*/
    for (i = (fdc-1); i >=0;i-- ) {
        data[i] = (data_head->data & CODE_MASK);
        data_head = data_head->next;
    }
    /*print data*/
    for (i = 0; i < fdc; i++) {
        type = 'A';
        fprintf(fp, "%04d %03X %c\n",ic++,data[i],type);
    }
    end_use("pf",file,fp);
    return OK;
}
/**
 * @brief the function crate the entry file and print the entry use and location in it
 *
 * @param file_name is the name of the file
 * @param head is pointer to the head of the symbol table
 * @return OK if creating the file was successful end ERROR if not
 */
int make_ent_file(const char * file_name, symbol_table * head) {
    boolean entry_flag;
    char *file;
    symbol_table *curr;
    FILE *fp;
    /*initialize variable*/
    entry_flag = FALSE;
    curr = head;
    /*check if entry symbol is exist*/
    while (curr != NULL) {
        if (curr->entry == TRUE) {
            entry_flag = TRUE;
            break;
        }
        curr = curr->next;
    }
    if (entry_flag) {
        /*create and open the ".ent" file for writing*/
        file = make_file_name(file_name,".ent");
        if ((fp = fopen(file, "w") ) == NULL) {/*check if we succeed to open the file*/
            printf("Error opening file - %s\n", file);
            /*release and close file if we don't succeed to open*/
            end_use("p",file);
            return ERROR;
        }
        /* print the entry symbol*/
        while (curr != NULL) {
            if (curr->entry == TRUE) {
                fprintf(fp,"%s %04d\n",curr->name,curr->value);
            }
            curr = curr->next;
        }
        end_use("pf",file,fp);
    }
    return OK;
}
/**
 * @brief the function crate the external file and print the external use and location in it
 *
 * @param file_name is the name of the file
 * @param head is pointer to the head of the symbol table
 * @return OK if creating the file was successful end ERROR if not
 */
int make_ext_file(const char * file_name, symbol_table * head) {
    boolean extern_flag;
    char *file;
    symbol_table *curr;
    FILE *fp;
    /*initialize variable*/
    extern_flag = FALSE;
    curr = head;
    /*check if entry symbol is exist*/
    while (curr != NULL) {
        if ((curr->external == TRUE)&&(curr->value != 0)) {/*the usage for external save with the ic position of use*/
            extern_flag = TRUE;
            break;
        }
        curr = curr->next;
    }
    if (extern_flag) {
        /*create and open the ".ext" file for writing*/
        file = make_file_name(file_name,".ext");
        if ((fp = fopen(file, "w") ) == NULL) {/*check if we succeed to open the file*/
            printf("Error opening file - %s\n", file);
            /*release and close file if we don't succeed to open*/
            end_use("p",file);
            return ERROR;
        }
        /* print the entry symbol*/
        while (curr != NULL) {
        if ((curr->external == TRUE)&&(curr->value != 0)) {/*the usage for external save with the ic position of use*/
                fprintf(fp,"%s %04d\n",curr->name,curr->value);
            }
            curr = curr->next;
        }
        end_use("pf",file,fp);
    }
    return OK;
}
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
int make_output_files(const char * file_name, const symbol_table_mgr * symbol_head, const data_mgr * data_head, const code_image * code, const int fic, const int fdc){
    /*check if the file is empty*/
    if ((fic == FIRST_MEMORY)&&(fdc == 0)) {
        printf("no code written in \"%s\" -the file is empty or only comment",file_name);
        return OK;
    }
    if (make_ob_file(file_name,data_head->head,code,fic,fdc) == ERROR) {
        return ERROR;
    }
    if (make_ent_file(file_name,symbol_head->head) == ERROR) {
        return ERROR;
    }
    if (make_ext_file(file_name,symbol_head->head) == ERROR) {
        return ERROR;
    }
    return OK;
}

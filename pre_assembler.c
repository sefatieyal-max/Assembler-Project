#include "pre_assembler.h"


/**
 *@brief return if we have "mcro" in the given line
 *
 *@param line is the line we are checking
 *@return true if there "mcro" and FALSE if it doesn't
 */
boolean macro_start(const char *line) {
    return (strstr(line, "mcro ") != NULL);
}
/**
 *@brief return if we have "mcro" in the given line
 *
 *@param line is the line we are checking
 *@return true if there "mcro" and FALSE if it doesn't
 */
boolean macro_end(const char *line) {
    return (strstr(line, "mcroend") != NULL);
}
/**
 *@brief this function check if the macro end line write properly
 *
 *@param line is the line we are checking
 *@return TRUE if the line written properly and FALSE if its doesn't
 */
boolean macro_end_check(const char * line) {
    boolean flag;

    /*check if the line start with mcroend*/
    if(strncmp(line, "mcroend",strlen("mcroend"))== 0) {
        line = line + strlen("mcroend");
        /*check for extra characters*/
        while (*line != '\0') {
            if (!isspace(*line)) {
                return FALSE;
            }
            line++;
        }
        return TRUE;
    }

    return FALSE;
}
/**
 *@brief this function check if the macro name is legal
 *
 *@param macro_name is the name we are checking
 *@return TRUE if the name written properly and FALSE if its doesn't
 */
boolean macro_name_check(char * macro_name) {
    macro_name = strtok(macro_name," \n\t\r");
    if (is_reserved_word(macro_name)) {
        printf("Error - macro doesn't declared properly (reserved word)\n");
        return FALSE;
    }
    if (strtok(NULL,"\n") != NULL) {
        printf("Error - macro doesn't declared properly (extra text)\n");
        return FALSE;
    }
    return TRUE;
}
/**
 *@brief the function add macro the macro linked list
 *
 *she takes the macro check if he writes accordingly and then copy all the contact into the linked list
 *
 *@param mgr is the pointer to the last macro we have (head)
 *@param fp is the pointer to the lines we read from the file
 *@param line is the first line of the macro
 *@return OK if the nacro dding was seccsecfull and ERROR if not
 */
int add_macro(macro_mgr *mgr, FILE * fp, char * line) {
    int content_len,line_len;
    char *temp;
    macro *curr;
    if (create_macro(mgr,line) == ERROR) {/*if the macro crate is error clean th macro*/
        while (fgets(line,MAX_LINE_LEN,fp)) {
            if (macro_end(line) && macro_end_check(line)) {
                break;
            }
        }
        return ERROR;
    }

    /*open the macro head*/
    curr = mgr->head;
    /*run until the end of the macro*/
    while (fgets(line,MAX_LINE_LEN,fp)) {
        /*check if the macro end*/
        if (macro_end(line) && macro_end_check(line)) {
            return OK;
        }
        /*calculate and relocate memory for the line in the macro content*/
        line_len = (int)strlen(line);
        content_len = (curr->content == NULL) ? 0 : (int)strlen(curr->content); /*find the length of the conant*/
        temp = do_realloc(curr->content,content_len+line_len+1);
        curr->content = copy_text(temp,line,content_len);
    }
    printf("Error - the macro does not end properly\n");
    return ERROR;
}
/**
 *@brief create a new macro - without content
 *
 *@param mgr is pointer to the head of the linked list
 *@param line is the line with the name of the macro
 *@return pointer to the created macro (the new head)
 */
int create_macro(macro_mgr * mgr, char * line) {
    char *macro_name;
    macro *curr;
    line = line + strlen("mcro"); /*removing "mcro" from the line*/
    if(!macro_name_check(line)){
        return ERROR;
    }
    macro_name = strtok(line," \n\t\r");
    /*create the new macro*/
    curr = create_malloc(sizeof(macro));
    curr->name = create_malloc(strlen(macro_name));
    curr->name = copy_text(curr->name, macro_name, 0);
    curr->content = NULL;
    curr->next = mgr->head;
    /*update the head in the macro_mgr*/
    mgr->head = curr;
    return OK;
}
/**
 *@brief the main function of the pre_assembler - she takes a file and remove and replace all the macros
 *
 *@param file_name is the file name we are macro decoding
 *@param mgr is the head for the macro linked list
 *@return OK if the macro decoding was successful and ERROR if not
 */
int run_pre_assembler(char *file_name,macro_mgr *mgr) {
    macro *curr;
    char line[MAX_LINE_LEN];
    char *file_as, *file_tmp,*file_am;
    FILE *fp,*fp_am;
    int error_count;

    error_count = 0;
    /*create and open .as files and work on file.tmp after clean extra spaces*/
    file_as = make_file_name(file_name, ".as");
    if ((file_tmp = clean_file_spaces(file_as)) == NULL) {
        return ERROR;
    }
    if ((fp = fopen(file_tmp, "r") ) == NULL) {/*check if we succeed to open the file*/
        printf("Error opening file - %s\n", file_tmp);
        /*release and close file if we don't succeed to open*/
        end_use("pp",file_as,file_tmp);
        return ERROR;
    }
    /*create an .am file for the pre_assembler*/
    file_am = make_file_name(file_tmp, ".am");
    if ((fp_am = fopen(file_am, "w") ) == NULL) {/*check if we succeed to open the file*/
        printf("Error opening file - %s\n", file_am);
        /*release and close file if we don't succeed to open*/
        end_use("pppf",file_am,file_tmp,file_as,fp);
        return ERROR;
    }
    while (fgets(line,MAX_LINE_LEN,fp)){
        /*skip comment line*/
        if (is_comment(line)) {
            continue;
        }
        /*we need to add macro*/
        if (macro_start(line)) {
            if (add_macro(mgr,fp,line) == ERROR) {
                error_count++;
            }
        }
        /*we need to replace a macro*/
        else if ((curr = macro_search(line,mgr->head))!=NULL) {
            fputs(curr->content,fp_am);
        }
        /*regular code*/
        else {
            fputs(line,fp_am);
        }
    }
    end_use("pppff",file_am,file_tmp,file_as,fp,fp_am);
    if (error_count>0) {
        return ERROR;
    }
    return OK;
}

#include "utilities.h"

/**
 *@brief this function allocate memory and checking if the allocation was successful
 *
 *@param size is the size of memory we want to allocate
 *@return the pointer to the memory
 */
void* create_malloc(size_t size) {
    void* ptr = malloc(size);
    /*check if the memory allocation failed*/
    if (ptr == NULL) {
        printf("Error: memory allocation failed\n");
    }
    return ptr;
}
/**
 *@brief this function allocate memory and checking if the allocation was successful
 *
 *@param size is the size of memory we want to allocate
 *@param memory is the memory we reallocate
 *@return the pointer to the memory
 */
void* do_realloc(char *memory,size_t size) {
    void* ptr = realloc(memory,size);
    /*check if the memory allocation failed*/
    if (ptr == NULL) {
        printf("Error: memory allocation failed\n");
    }
    return ptr;
}
/**
 *@brief this function clean spaces in 1 line
 *
 *@param src is the pointer to source line
 *@param dst is the pointer to the "clean" line
 */
void clean_line_spaces(char* dst, const char* src) {
    int i,j;
    i=0,j=0;

    /*skip all the leading whitespaces*/
    while (src[i] && isspace(src[i])) {
        i++;
    }

    /*run on the rest of the line*/
    while (src[i]) {
        /*if the char isn't space - copy*/
        if (!isspace(src[i])) {
            dst[j++] = src[i++];
        }
        /*delete extra spaces*/
        else {
            dst[j++] = ' ';/*put a space*/
            /*clean extra spaces*/
            while (src[i] && isspace(src[i])) {
                i++;
            }
        }
    }
    /*clean extra space in the end*/
    if (j>0 && (dst[j-1] == ' ' )){
        j--;
    }
    /*close line*/
    dst[j] = '\0';
}
/**
 *@brief the clean al the extra spaces from the file
 *
 *@param filename is the name of the file we are cleaning
    *
 *@return a pointer to the clean file
 */
char *clean_file_spaces(char filename[]) {
    FILE *fp_src, *fp_dst;
    char *tmp_file;
    char line[MAX_LINE_LEN];
    char clean_line[MAX_LINE_LEN];

    /*create temp file*/
    tmp_file = make_file_name(filename, ".tmp");

    /*open source file*/
    if ((fp_src = fopen(filename, "r") ) == NULL) {
        /*check if we succeed to open the file*/
        printf("Error opening file - %s\n", filename);
        end_use("p",tmp_file);
        return NULL;
    }
    /*open temp file*/
    if ((fp_dst = fopen(tmp_file, "w") ) == NULL) {
        /*check if we succeed to open the file*/
        printf("Error opening file - %s\n", tmp_file);
        end_use("pf",tmp_file,fp_src);
        return NULL;
    }
    /*clean the file*/
    while (fgets(line, MAX_LINE_LEN, fp_src)) {
        clean_line_spaces(clean_line, line);
        if (strlen(clean_line) > 0) {
            fprintf(fp_dst, "%s\n", clean_line);
        }
    }
    /*close files*/
    end_use("ff",fp_dst,fp_src);
    return tmp_file;
}
/**
 *@brief the function create a file with a requested ending
 *
 *@param file_name is the name of the new file
 *@param finish is the ending of the new lin
 *@return a pointer to the new file
 *
 *@note the function delete in the file name all the content after '.'
 */
char* make_file_name(const char *file_name, const char *finish) {
    char *new_fname, *c;
    new_fname = create_malloc(MAX_LINE_LEN * sizeof(char));
    strcpy(new_fname, file_name);
    /*deleting all the conant after '.'*/
    if ((c = strrchr(new_fname, '.')) != NULL) {
        *c = '\0';
    }
    /*add the ending of the new file*/
    strcat(new_fname, finish);
    return new_fname;
}
/**
 *@brief copy a file content into a new file
 *
 *@param src_file is the source file pointer
 *@param dst_file is the destination file pointer
 *@return OK if the copying was successful or ERROR if not
 */
int copy_file(char *src_file, char *dst_file) {
    char str[MAX_LINE_LEN];
    FILE *fp, *fp_dst;
    fp = fopen(src_file, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", src_file);
        return ERROR;
    }
    fp_dst = fopen(dst_file, "w");
    if (fp_dst == NULL) {
        printf("Error: could not open file %s\n", dst_file);
        end_use("f",fp);
        return ERROR;
    }
    while (fgets(str, MAX_LINE_LEN, fp) != NULL) {
        fprintf(fp_dst, "%s", str);
    }
    end_use("ff",fp_dst,fp);
    return OK;
}
/**
 *@brief check if the given word is a command
 *
 * @param word is the word we are checking
 * @return TRUE if the word is command FALSE if it is not
 */
boolean is_command(const char *word) {
    int i,len;
    /*array of commands*/
    char *cmd[] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"};
    len = sizeof(cmd)/sizeof(char*)-1;
    /*check if the word is a command*/
    for (i = 0; i < len; i++) {
        if (strcmp(cmd[i], word) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}
/**
 *@brief check if the given word is a directive
 *
 * @param word is the word we are checking
 * @return TRUE if the word is a directive FALSE if it is not
 */
boolean is_directive(const char *word) {
    int i,len;
    /*array of directives*/
    char *dirs[] = {".data",".string",".entry",".extern"};
    len = sizeof(dirs)/sizeof(char*)-1;
    /*check if the word is a directives*/
    for (i = 0; i < len; i++) {
        if (strcmp(dirs[i], word) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}
/**
 *@brief check if the given word is a register
 *
 * @param word is the word we are checking
 * @return TRUE if the word is a register FALSE if it is not
 */
boolean is_register(const char *word) {
    int i,len;
    /*array of registers*/
    char *regs[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
    len = sizeof(regs)/sizeof(char*)-1;
    /*check if the word is a registers*/
    for (i = 0; i < len; i++) {
        if (strcmp(regs[i], word) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}
/**
 *@brief check if the given word is a command or directives or register
 *
 * @param word is the word we are checking
 * @return TRUE if the word is command/directives/register FALSE if it is not
 */
boolean is_reserved_word(const char *word) {
    return (is_command(word) || is_directive(word) || is_register(word));
}
/**
 *@brief this function add text into given source
 *
 * @param src is the text we are coping into
 * @param text is the text we are adding
 * @param len is the length of the source text
 * @return pointer to the txt
 */
char *copy_text(char *src, const char *text,int len) {
    if (len == 0) {/*if the text is the first text*/
        strcpy(src, text);
    }
    else {/*if there is a text*/
        strcat(src, text);
    }
    return src;
}
/**
 *@brief this function receive a list of files and pointer and close the file / release the pointer
 *
 * @param format a string represent the type of the variable p-pointer | f-file
 * @param ...   a list of variable
 */
void end_use(const char *format, ...) {
    const char *ptr = format;
    char *mem_ptr;
    FILE *fp;
    va_list args;
    va_start(args, format);

    while (*ptr != '\0') {
        if (*ptr == 'p') {/*p - pointer (release memory)*/
            mem_ptr = va_arg(args, char*);
            free(mem_ptr);
        }
        else if (*ptr == 'f') {/* f - file (close file)*/
            fp = va_arg(args, FILE*);
            if (fp != NULL) {
                fclose(fp);
            }
        }
        ptr++;
    }
    va_end(args);
}
/**
 * @brief the function checking if the line is a comment (start with ';')
 * @param line is the line we are checking
 * @return true if the line is a comment, false if not
 */
boolean is_comment(const char line[MAX_LINE_LEN]) {
    return (line[0] == ';') ;
}
/**
 *@brief function check if the comma in the command are valid, if not she print an error message accordingly.
 *
 *@param str is the command we are checking
 *@return OK if the commas are valid, ERROR if the comma invalid.
 */
int comma_check(const char *str) {
    int comma_flag = 0;
    int space_flag = 0;
    int number_flag = 0;
    int i;
    for (i=0; str[i]!='\0'; i++) {
        if (isspace(str[i])) {
            space_flag = 1;
        }
        else if (str[i]==',' && comma_flag == 1 ) { /*check for multiple commas*/
            printf("Multiple consecutive commas ");
            return ERROR;
        }
        else if (str[i]==',' && number_flag==0) {/*check fo illegal comma*/
            printf("Illegal comma ");
            return ERROR;
        }
        else if (str[i]==',' && comma_flag==0) {/*suitable comma*/
            comma_flag = 1;
            space_flag = 0;
            number_flag = 0;
        }
        else if (space_flag == 1 && number_flag == 1) {/*check for missing comma*/
            printf("Missing comma ");
            return ERROR;
        }
        else {/*not a comma or space*/
            number_flag = 1;
            comma_flag = 0;
            space_flag = 0;
        }
    }
    if (comma_flag == 1) {/*check for comma at the end of the command*/
        printf("Illegal comma ");
        return ERROR;
    }
    return OK;
}

/* checking lines function*/

/**
 *@brief this function search if there label on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there label, return FALSE if isn't
 */
boolean is_label(const char *line) {
    char first_word[MAX_LINE_LEN];
    int len;
    /*check if there is a first word*/
    if (sscanf(line,"%s",first_word) == 1) {
        /*find the end of the first word*/
        len = (int)strlen(first_word);
        if (first_word[len-1] == ':') {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 *@brief this function search if their data directive on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there is data directive, return FALSE if isn't
 */
boolean is_data_direct(const char * line) {
    return strstr(line,".string") || strstr(line,".data");
}
/**
 *@brief this function search if their entry directive on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there is entry directive, return FALSE if isn't
 */
boolean is_entry(const char *line) {
    return (strstr(line,".entry ") != NULL);
}
/**
 *@brief this function search if their extern directive on the line
 *
 * @param line is the line we are searching
 * @return TRUE if there is extern directive, return FALSE if isn't
 */
boolean is_extern(const char *line) {
    return (strstr(line,".extern ") != NULL);
}
/**
 * @brief the function check if the line is only a number
 *
 * @param line is the line we are checking
 * @return TRUE if the line is only a number and FALSE if not
 */
boolean is_number(const char *line) {
    char *end;
    strtol(line,&end,10);
    if (end == (char*)line) {/*check if we don't have any numbers*/
        return FALSE;
    }
    if (*end != '\0') {/*check if we have characters after the number*/
        return FALSE;
    }
    return TRUE;
}
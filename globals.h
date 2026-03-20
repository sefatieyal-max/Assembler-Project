#ifndef ASSEMBLER_GLOBALS_H
#define ASSEMBLER_GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define OK 0
#define ERROR (-2)
#define NONE (-3)
#define MAXNUM 2047
#define MINNUM (-2048)
#define MAX_LINE_LEN 82 /*80 chars + \n*/
#define FIRST_MEMORY 100
#define LABEL_SIZE 31
#define MEMORY_SIZE 4096
#define CMD_SIZE 5 /*4 for the commands length and 1  for \0*/
#define CODE_MASK 0xFFF
#define OPCODE_POSITION 8
#define OPCODE_MASK 0xF
#define FUNCT_POSITION 4
#define SRC_ADDRESS_POSITION 2
#define DST_ADDRESS_POSITION 0
#define ADDRESS_MASK 0x3
#define IMMEDIATE_ADDRESSING 0
#define IMMEDIATE_MASK 1 /* 1<<0 */
#define DIRECT_ADDRESSING 1
#define DIRECT_MASK 2/* 1<<1 */
#define RELATIVE_ADDRESSING 2
#define RELATIVE_MASK 4 /* 1<<2 */
#define REGISTER_ADDRESSING 3
#define REGISTER_MASK 8 /* 1<<0 */
#define TWO_OPERAND(op)   ((op) == 0 || (op) == 1 || (op) == 2 || (op) == 4)
#define SINGLE_OPERAND(op) ((op) == 5 || (op) == 9 || (op) == 12 || (op) == 13)
#define ZERO_OPERAND(op)  ((op) == 14 || (op) == 15)
#define JMP_CMD_OPCODE 9
#define A (1 << 12)
#define R (2 << 12)
#define E (3 << 12)
#define ARE_MASK ((0x3) << 12)
typedef enum {FALSE, TRUE} boolean;


#endif
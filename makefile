#comiplation macros
CC = gcc #compailer declaretion
CFLAGS = -ansi -Wall -pedantic -g #flags
EXEC = assembler #final run file
DIR = source#the source folder for the file
GLOBAL_OBJ = $(DIR)/globals.h #dependecies for every file
EXE_OBJS = assembler.o first_pass.o output_files.o pre_assembler.o second_pass.o structs.o utilities.o #list for all the files

#defult command
all: $(EXEC)

#link inad crate the files
$(EXEC) : $(EXE_OBJS)
	$(CC) $(CFLAGS) $(EXE_OBJS) -o $(EXEC) -lm
	
#compiling .c to .o
%.o: $(DIR)/%.c $(GLOBAL_OBJ)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXEC)


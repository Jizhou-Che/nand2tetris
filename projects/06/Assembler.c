#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Holds a symbol table.
// Actually a singly linked list.
struct symbol_table{
	char * symbol;
	int address;
	struct symbol_table * next;
};

// Appends string str with a single character chr.
// Returns 0 on memory allocation failure, 1 on success.
int string_append(char ** str, char chr){
	int len;
	if(*str == NULL){
		len = 0;
	}else{
		len = (int)strlen(*str);
	}
	if(len == 0){
		*str = malloc(sizeof(char) * 16);
		if(*str == NULL){
			return 0;
		}else{
			(*str)[len] = chr;
			(*str)[len + 1] = '\0';
			return 1;
		}
	}else{
		if((len + 1) % 16 == 0){
			char * temp = malloc(sizeof(char) * (len + 17));
			if(temp == NULL){
				return 0;
			}else{
				for(int i = 0; i < len; i++){
					temp[i] = (*str)[i];
				}
				free(*str);
				*str = temp;
				(*str)[len] = chr;
				(*str)[len + 1] = '\0';
				return 1;
			}
		}else{
			(*str)[len] = chr;
			(*str)[len + 1] = '\0';
			return 1;
		}
	}
}

// Adds a new entry to the symbol table.
// Returns 0 on memory allocation failure, 1 on success.
// Actually performing a list_push.
int symbol_table_add_entry(struct symbol_table ** headSymbol, char * s, int addr){
	struct symbol_table * new_node = malloc(sizeof(struct symbol_table));
	if(new_node == NULL){
		return 0;
	}else{
		new_node->symbol = s;
		new_node->address = addr;
		new_node->next = NULL;
		if(*headSymbol == NULL){
			*headSymbol = new_node;
		}else{
			new_node->next = *headSymbol;
			*headSymbol = new_node;
		}
		return 1;
	}
}

// Clears the symbol table.
void symbol_table_clear(struct symbol_table ** headSymbol){
	while(*headSymbol != NULL){
		struct symbol_table * tempSymbol = *headSymbol;
		*headSymbol = (*headSymbol)->next;
		free(tempSymbol->symbol);
		free(tempSymbol);
	}
	return;
}

// Initializes the symbol table with pre-defined symbols.
// Returns a pointer to the initialized symbol table.
// Returns NULL on memory allocation failure.
struct symbol_table * symbol_table_initialize(){
	struct symbol_table * symbolTable = NULL;
	if(!symbol_table_add_entry(&symbolTable, "SP", 0)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "LCL", 1)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "ARG", 2)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "THIS", 3)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "THAT", 4)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R0", 0)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R1", 1)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R2", 2)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R3", 3)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R4", 4)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R5", 5)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R6", 6)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R7", 7)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R8", 8)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R9", 9)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R10", 10)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R11", 11)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R12", 12)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R13", 13)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R14", 14)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "R15", 15)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "SCREEN", 16384)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_add_entry(&symbolTable, "KBD", 24576)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	return symbolTable;
}

// First pass of the assembly process.
// Expands the symbol table with user-defined labels.
// Returns 0 on memory allocation failure, 1 on success.
int assembly_first_pass(FILE * inputFile, struct symbol_table ** headSymbol){
	int lineNumber = -1;
	char thisChar;
	while(fscanf(inputFile, "%c", &thisChar) == 1){
		if(thisChar == '\n' || thisChar == '\r' || thisChar == ' ' || thisChar == '\t'){
			// Empty line or space or tab.
			continue;
		}else if(thisChar == '/'){
			// Comment line.
			while(fscanf(inputFile, "%c", &thisChar) == 1){
				if(thisChar == '\n'){
					break;
				}
			}
		}else if(thisChar == '('){
			// Beginning of label.
			char * thisLabel = NULL;
			while(fscanf(inputFile, "%c", &thisChar) == 1){
				if(thisChar == ')'){
					// End of label.
					symbol_table_add_entry(headSymbol, thisLabel, lineNumber + 1);
					break;
				}else{
					// Label character.
					if(!string_append(&thisLabel, thisChar)){
						free(thisLabel);
						return 0;
					}
				}
			}
			// Ignore everything after label.
			while(fscanf(inputFile, "%c", &thisChar) == 1){
				if(thisChar == '\n'){
					break;
				}
			}
		}else{
			// Beginning of instruction.
			lineNumber++;
			while(fscanf(inputFile, "%c", &thisChar) == 1){
				if(thisChar == '\n'){
					break;
				}
			}
		}
	}
	return 1;
}

int main(int argc, char ** argv){
	// Get input file.
	if(argc != 2){
		printf("Invalid command line arguments. Usage: Assembler FILE[.asm]\n");
		return 0;
	}
	FILE * inputFile = fopen(argv[1], "r");
	if(inputFile == NULL){
		printf("Cannot open file %s.\n", argv[1]);
		return 0;
	}

	// Initialize symbol table.
	struct symbol_table * symbolTable = symbol_table_initialize();
	if(symbolTable == NULL){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}

	// First pass of the assembly process.
	if(!assembly_first_pass(inputFile, &symbolTable)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}

	printf("%s %d\n", symbolTable->symbol, symbolTable->address);
	return 0;
}

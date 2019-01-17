// This is a very basic implementation of the Hack assembler.
// It assumes the input program is error-free.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Holds a symbol table.
// Actually a singly linked list.
struct symbol_table{
	char * symbol;
	int address;
	struct symbol_table * next;
};

// Appends string str with a single character chr.
// String str must be dynamically allocated.
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

// Concatenates string str2 to the end of string str1.
// String str1 must be dynamically allocated.
// Returns 0 on memory allocation failure, 1 on success.
int string_concatenate(char ** str1, char * str2){
	int size = (int)(((strlen(*str1) + strlen(str2)) / 16 + 1) * 16);
	char * dst = malloc(size * sizeof(char));
	if(dst == NULL){
		return 0;
	}else{
		int flag = 0;
		int subFlag = 0;
		while((*str1)[subFlag] != '\0'){
			dst[flag] = (*str1)[subFlag];
			flag++;
			subFlag++;
		}
		subFlag = 0;
		while(str2[subFlag] != '\0'){
			dst[flag] = str2[subFlag];
			flag++;
			subFlag++;
		}
		dst[++flag] = '\0';
		free(*str1);
		*str1 = dst;
		return 1;
	}
}

// Converts a decimal value to an unsigned binary string of n bits.
// Returns a pointer to the binary string.
// Does not check for errors.
char * dec_to_bin(int decValue, int n){
	char * binString = NULL;
	int bin;
	for(int i = n - 1; i >= 0; i--){
		bin = decValue >> i;
		if(bin & 1){
			string_append(&binString, '1');
		}else{
			string_append(&binString, '0');
		}
	}
	return binString;
}

// Checks whether a given string is a symbol in the symbol table.
// Returns the respective address of the symbol if it is already in the symbol table.
// Returns -1 if the symbol is not in the symbol table.
int symbol_table_check(struct symbol_table * headSymbol, char * s){
	struct symbol_table * thisSymbol = headSymbol;
	while(thisSymbol != NULL){
		if(strcmp(thisSymbol->symbol, s) == 0){
			return thisSymbol->address;
		}
		thisSymbol = thisSymbol->next;
	}
	return -1;
}

// Adds a new entry to the symbol table.
// Returns 0 on memory allocation failure, 1 on success.
// Actually performing a list_push.
int symbol_table_push(struct symbol_table ** headSymbol, char * s, int addr){
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
	if(!symbol_table_push(&symbolTable, "SP", 0)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "LCL", 1)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "ARG", 2)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "THIS", 3)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "THAT", 4)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R0", 0)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R1", 1)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R2", 2)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R3", 3)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R4", 4)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R5", 5)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R6", 6)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R7", 7)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R8", 8)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R9", 9)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R10", 10)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R11", 11)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R12", 12)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R13", 13)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R14", 14)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "R15", 15)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "SCREEN", 16384)){
		printf("Assembly failed while allocating memory.\n");
		symbol_table_clear(&symbolTable);
		return NULL;
	}
	if(!symbol_table_push(&symbolTable, "KBD", 24576)){
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
		if(thisChar == '\r' || thisChar == '\n' || thisChar == ' ' || thisChar == '\t'){
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
			// Label line.
			char * thisLabel = NULL;
			while(fscanf(inputFile, "%c", &thisChar) == 1){
				if(thisChar == ')'){
					symbol_table_push(headSymbol, thisLabel, lineNumber + 1);
					break;
				}else{
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
			// Instruction line.
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

// Second pass of the assembly process.
// Returns 0 on memory allocation failure, 1 on success.
int assembly_second_pass(FILE * inputFile, FILE * outputFile, struct symbol_table ** headSymbol){
	int nextVariableAddress = 16;
	char thisChar;
	while(fscanf(inputFile, "%c", &thisChar) == 1){
		if(thisChar == '\r' || thisChar == '\n' || thisChar == ' ' || thisChar == '\t'){
			// Empty line or space or tab.
			continue;
		}else if(thisChar == '/' || thisChar == '('){
			// Comment line or label line.
			while(fscanf(inputFile, "%c", &thisChar) == 1){
				if(thisChar == '\n'){
					break;
				}
			}
		}else{
			// Instruction line.
			char * thisInstruction = NULL;
			string_append(&thisInstruction, '\0');
			if(thisChar == '@'){
				// A instruction.
				if(!string_append(&thisInstruction, '0')){
					return 0;
				}
				fscanf(inputFile, "%c", &thisChar);
				if(isdigit(thisChar)){
					// Constant.
					int thisConstant = thisChar - 48;
					while(fscanf(inputFile, "%c", &thisChar) == 1){
						if(isdigit(thisChar)){
							thisConstant = thisConstant * 10 + thisChar - 48;
						}else{
							// End of constant.
							if(!string_concatenate(&thisInstruction, dec_to_bin(thisConstant, 15))){
								free(thisInstruction);
								return 0;
							}
							fprintf(outputFile, "%s\n", thisInstruction);
							free(thisInstruction);
							// Ignore everything after constant.
							if(thisChar != '\n'){
								while(fscanf(inputFile, "%c", &thisChar) == 1){
									if(thisChar == '\n'){
										break;
									}
								}
							}
							break;
						}
					}
				}else{
					// Symbol.
					char * thisSymbol = NULL;
					if(!string_append(&thisSymbol, thisChar)){
						return 0;
					}
					while(fscanf(inputFile, "%c", &thisChar) == 1){
						if(!isalnum(thisChar) && thisChar != '_' && thisChar != '.' && thisChar != '$' && thisChar != ':'){
							// End of symbol.
							int thisAddress = symbol_table_check(*headSymbol, thisSymbol);
							if(thisAddress == -1){
								// New symbol.
								if(!symbol_table_push(headSymbol, thisSymbol, nextVariableAddress)){
									free(thisSymbol);
									return 0;
								}
								if(!string_concatenate(&thisInstruction, dec_to_bin(nextVariableAddress, 15))){
									free(thisInstruction);
									return 0;
								}
								fprintf(outputFile, "%s\n", thisInstruction);
								free(thisInstruction);
								nextVariableAddress++;
							}else{
								// Recorded symbol.
								free(thisSymbol);
								if(!string_concatenate(&thisInstruction, dec_to_bin(thisAddress, 15))){
									free(thisInstruction);
									return 0;
								}
								fprintf(outputFile, "%s\n", thisInstruction);
								free(thisInstruction);
							}
							// Ignore everything after symbol.
							if(thisChar != '\n'){
								while(fscanf(inputFile, "%c", &thisChar) == 1){
									if(thisChar == '\n'){
										break;
									}
								}
							}
							break;
						}else{
							if(!string_append(&thisSymbol, thisChar)){
								free(thisSymbol);
								return 0;
							}
						}
					}
				}
			}else{
				// C instruction.
				string_concatenate(&thisInstruction, "111");
				char * thisLine = NULL;
				if(!string_append(&thisLine, thisChar)){
					return 0;
				}
				while(fscanf(inputFile, "%c", &thisChar) == 1){
					if(thisChar == '\r' || thisChar == '\n'){
						// End of line.
						break;
					}else if(thisChar == ' ' || thisChar == '\t'){
						// Space or tab.
						continue;
					}else if(thisChar == '/'){
						// Beginning of comment.
						while(fscanf(inputFile, "%c", &thisChar) == 1){
							if(thisChar == '\n'){
								break;
							}
						}
						break;
					}else{
						// Instruction code.
						if(!string_append(&thisLine, thisChar)){
							free(thisLine);
							return 0;
						}
					}
				}
				// Parse this line.
				int isDest = 0;
				int isJump = 0;
				if(strchr(thisLine, '=') != NULL){
					isDest = 1;
				}
				if(strchr(thisLine, ';') != NULL){
					isJump = 1;
				}
				char * tempLine = malloc((int)(strlen(thisLine) + 1) * sizeof(char));
				if(tempLine == NULL){
					return 0;
				}
				strcpy(tempLine, thisLine);
				// Parse comp.
				char * thisComp = NULL;
				if(isDest){
					thisComp = strtok(tempLine, "=");
					thisComp = strtok(NULL, ";");
				}else{
					thisComp = strtok(tempLine, ";");
				}
				if(strcmp(thisComp, "0") == 0){
					if(!string_concatenate(&thisInstruction, "0101010")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "1") == 0){
					if(!string_concatenate(&thisInstruction, "0111111")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "-1") == 0){
					if(!string_concatenate(&thisInstruction, "0111010")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D") == 0){
					if(!string_concatenate(&thisInstruction, "0001100")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "A") == 0){
					if(!string_concatenate(&thisInstruction, "0110000")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "M") == 0){
					if(!string_concatenate(&thisInstruction, "1110000")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "!D") == 0){
					if(!string_concatenate(&thisInstruction, "0001101")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "!A") == 0){
					if(!string_concatenate(&thisInstruction, "0110001")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "!M") == 0){
					if(!string_concatenate(&thisInstruction, "1110001")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "-D") == 0){
					if(!string_concatenate(&thisInstruction, "0001111")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "-A") == 0){
					if(!string_concatenate(&thisInstruction, "0110011")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "-M") == 0){
					if(!string_concatenate(&thisInstruction, "1110011")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D+1") == 0){
					if(!string_concatenate(&thisInstruction, "0011111")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "A+1") == 0){
					if(!string_concatenate(&thisInstruction, "0110111")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "M+1") == 0){
					if(!string_concatenate(&thisInstruction, "1110111")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D-1") == 0){
					if(!string_concatenate(&thisInstruction, "0001110")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "A-1") == 0){
					if(!string_concatenate(&thisInstruction, "0110010")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "M-1") == 0){
					if(!string_concatenate(&thisInstruction, "1110010")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D+A") == 0){
					if(!string_concatenate(&thisInstruction, "0000010")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D+M") == 0){
					if(!string_concatenate(&thisInstruction, "1000010")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D-A") == 0){
					if(!string_concatenate(&thisInstruction, "0010011")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D-M") == 0){
					if(!string_concatenate(&thisInstruction, "1010011")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "A-D") == 0){
					if(!string_concatenate(&thisInstruction, "0000111")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "M-D") == 0){
					if(!string_concatenate(&thisInstruction, "1000111")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D&A") == 0){
					if(!string_concatenate(&thisInstruction, "0000000")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D&M") == 0){
					if(!string_concatenate(&thisInstruction, "1000000")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D|A") == 0){
					if(!string_concatenate(&thisInstruction, "0010101")){
						free(thisInstruction);
						return 0;
					}
				}
				if(strcmp(thisComp, "D|M") == 0){
					if(!string_concatenate(&thisInstruction, "1010101")){
						free(thisInstruction);
						return 0;
					}
				}
				// Parse dest.
				char d1 = '0', d2 = '0', d3 = '0';
				if(isDest){
					strcpy(tempLine, thisLine);
					if(strchr(strtok(tempLine, "="), 'A') != NULL){
						d1 = '1';
					}
					if(strchr(strtok(tempLine, "="), 'D') != NULL){
						d2 = '1';
					}
					if(strchr(strtok(tempLine, "="), 'M') != NULL){
						d3 = '1';
					}
					free(tempLine);
				}
				if(!string_append(&thisInstruction, d1)){
					free(thisInstruction);
					return 0;
				}
				if(!string_append(&thisInstruction, d2)){
					free(thisInstruction);
					return 0;
				}
				if(!string_append(&thisInstruction, d3)){
					free(thisInstruction);
					return 0;
				}
				// Parse jump.
				char j1 = '0', j2 = '0', j3 = '0';
				if(isJump){
					strcpy(tempLine, thisLine);
					char * thisJump = strtok(tempLine, ";");
					thisJump = strtok(NULL, ";");
					if(strcmp(thisJump, "JGT") == 0){
						j3 = '1';
					}
					if(strcmp(thisJump, "JEQ") == 0){
						j2 = '1';
					}
					if(strcmp(thisJump, "JGE") == 0){
						j2 = '1';
						j3 = '1';
					}
					if(strcmp(thisJump, "JLT") == 0){
						j1 = '1';
					}
					if(strcmp(thisJump, "JNE") == 0){
						j1 = '1';
						j3 = '1';
					}
					if(strcmp(thisJump, "JLE") == 0){
						j1 = '1';
						j2 = '1';
					}
					if(strcmp(thisJump, "JMP") == 0){
						j1 = '1';
						j2 = '1';
						j3 = '1';
					}
					free(tempLine);
				}
				if(!string_append(&thisInstruction, j1)){
					free(thisInstruction);
					return 0;
				}
				if(!string_append(&thisInstruction, j2)){
					free(thisInstruction);
					return 0;
				}
				if(!string_append(&thisInstruction, j3)){
					free(thisInstruction);
					return 0;
				}
				fprintf(outputFile, "%s\n", thisInstruction);
				free(thisInstruction);
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

	// Second pass of the assembly process.
	fseek(inputFile, 0, SEEK_SET);
	int flag = 0;
	char * outputFileName = NULL;
	while(argv[1][flag] != '.'){
		string_append(&outputFileName, argv[1][flag]);
		flag++;
	}
	string_concatenate(&outputFileName, ".hack");
	FILE * outputFile = fopen(outputFileName, "w");
	if(outputFile == NULL){
		printf("Cannot create file %s.\n", outputFileName);
		return 0;
	}
	if(!assembly_second_pass(inputFile, outputFile, &symbolTable)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}

	printf("Assembly successful.\n");
	return 0;
}

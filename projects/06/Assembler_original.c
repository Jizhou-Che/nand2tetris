#include <stdio.h>
#include <stdlib.h>

struct symbol_table{
	char * symbol;
	int address;
	struct symbol_table * next;
};

struct instruction{
	char * this_instruction;
	struct instruction * next;
};

int string_append(char ** s, char c, int * len){
	if(*len == 0){
		*s = malloc(sizeof(char));
		if(*s == NULL){
			return 0;
		}else{
			(*s)[*len] = c;
			(*len)++;
			return 1;
		}
	}else{
		char * temp = malloc(sizeof(char) * (*len + 1));
		if(temp == NULL){
			free(*s);
			*s = NULL;
			*len = 0;
			return 0;
		}else{
			for(int i = 0; i < *len; i++){
				temp[i] = (*s)[i];
			}
			free(*s);
			*s = temp;
			(*s)[*len] = c;
			(*len)++;
			return 1;
		}
	}
};

int table_append(struct symbol_table ** table_head_ref, char * s, int addr){
	struct symbol_table * new_node = malloc(sizeof(struct symbol_table));
	if(new_node == NULL){
		return 0;
	}else{
		new_node->symbol = s;
		new_node->address = addr;
		new_node->next = NULL;
		if(*table_head_ref == NULL){
			*table_head_ref = new_node;
		}else{
			struct symbol_table * current_node = *table_head_ref;
			while(current_node->next != NULL){
				current_node = current_node->next;
			}
			current_node->next = new_node;
		}
		return 1;
	}
};

int instruction_append(struct instruction ** instrcution_head_ref, char * s){
	struct instruction * new_node = malloc(sizeof(struct instruction));
	if(new_node == NULL){
		return 0;
	}else{
		int flag = 0;
		while(s[flag] != '\0'){
			string_append(&(new_node->this_instruction), s[flag], &flag);
		}
		string_append(&(new_node->this_instruction), '\0', &flag);
		new_node->next = NULL;
		if(*instrcution_head_ref == NULL){
			*instrcution_head_ref = new_node;
		}else{
			struct instruction * current_node = *instrcution_head_ref;
			while(current_node->next != NULL){
				current_node = current_node->next;
			}
			current_node->next = new_node;
		}
		return 1;
	}
};

int parse_line_first_pass(char ** s, int * n, struct symbol_table ** t){
	char * s_parsed = NULL;
	int s_parsed_length = 0;
	if((*s)[0] == '@'){
		(*n)++;
		int isNumber = 1;
		int flag = 1;
		while((*s)[flag] != '\0'){
			if((*s)[flag] < '0' || (*s)[flag] > '9'){
				isNumber = 0;
				break;
			}
			flag++;
		}
		if(isNumber){
			int number = 0;
			flag = 1;
			while((*s)[flag] != '\0'){
				number *= 10;
				number += ((*s)[flag] - 48);
				flag++;
			}
			string_append(&s_parsed, '0', &s_parsed_length);
			int bin;
			for(int i = 14; i >= 0; i--){
				bin = number >> i;
				if(bin & 1){
					string_append(&s_parsed, '1', &s_parsed_length);
				}else{
					string_append(&s_parsed, '0', &s_parsed_length);
				}
			}
			string_append(&s_parsed, '\0', &s_parsed_length);
			free(*s);
			*s = s_parsed;
		}
	}else if((*s)[0] == '('){
		char * label = NULL;
		int label_length = 0;
		int flag = 1;
		while((*s)[flag] != ')'){
			string_append(&label, (*s)[flag], &label_length);
			flag++;
		}
		string_append(&label, '\0', &label_length);
		if(!table_append(t, label, (*n) + 1)){
			return 0;
		}
		string_append(&s_parsed, '\0', &s_parsed_length);
		free(*s);
		*s = s_parsed;
	}else{
		(*n)++;
		int isDest = 0;
		int isJump = 0;
		int flag = 0;
		while((*s)[flag] != '\0'){
			if((*s)[flag] == '='){
				isDest = 1;
			}
			if((*s)[flag] == ';'){
				isJump = 1;
			}
			flag++;
		}
		string_append(&s_parsed, '1', &s_parsed_length);
		string_append(&s_parsed, '1', &s_parsed_length);
		string_append(&s_parsed, '1', &s_parsed_length);
		char d1 = '0';
		char d2 = '0';
		char d3 = '0';
		char j1 = '0';
		char j2 = '0';
		char j3 = '0';
		if(isDest){
			flag = 0;
			while((*s)[flag] != '='){
				if((*s)[flag] == 'A'){
					d1 = '1';
				}
				if((*s)[flag] == 'D'){
					d2 = '1';
				}
				if((*s)[flag] == 'M'){
					d3 = '1';
				}
				flag++;
			}
		}
		if(isJump){
			flag = 0;
			while((*s)[flag] != ';'){
				flag++;
			}
			flag += 2;
			if((*s)[flag] == 'G' && (*s)[flag + 1] == 'T'){
				j3 = '1';
			}else if((*s)[flag] == 'E' && (*s)[flag + 1] == 'Q'){
				j2 = '1';
			}else if((*s)[flag] == 'G' && (*s)[flag + 1] == 'E'){
				j2 = '1';
				j3 = '1';
			}else if((*s)[flag] == 'L' && (*s)[flag + 1] == 'T'){
				j1 = '1';
			}else if((*s)[flag] == 'N' && (*s)[flag + 1] == 'E'){
				j1 = '1';
				j3 = '1';
			}else if((*s)[flag] == 'L' && (*s)[flag + 1] == 'E'){
				j1 = '1';
				j2 = '1';
			}else if((*s)[flag] == 'M' && (*s)[flag + 1] == 'P'){
				j1 = '1';
				j2 = '1';
				j3 = '1';
			}
		}
		char * comp = NULL;
		int comp_length = 0;
		flag = 0;
		if(isDest && isJump){
			while((*s)[flag] != '='){
				flag++;
			}
			flag++;
			while((*s)[flag] != ';'){
				string_append(&comp, (*s)[flag], &comp_length);
				flag++;
			}
			string_append(&comp, '\0', &comp_length);
		}else if(isDest){
			while((*s)[flag] != '='){
				flag++;
			}
			flag++;
			while((*s)[flag] != '\0'){
				string_append(&comp, (*s)[flag], &comp_length);
				flag++;
			}
			string_append(&comp, '\0', &comp_length);
		}else if(isJump){
			while((*s)[flag] != ';'){
				string_append(&comp, (*s)[flag], &comp_length);
				flag++;
			}
			string_append(&comp, '\0', &comp_length);
		}else{
			comp = *s;
		}
		flag = 0;
		if(comp[0] == '0' && comp[1] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if(comp[0] == '1' && comp[1] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == 'D' && comp[1] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if(comp[0] == 'A' && comp[1] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if(comp[0] == 'M' && comp[1] == '\0'){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if(comp[0] == '-' && comp[1] == '1' && comp[2] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if(comp[0] == '!' && comp[1] == 'D' && comp[2] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == '!' && comp[1] == 'A' && comp[2] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == '!' && comp[1] == 'M' && comp[2] == '\0'){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == '-' && comp[1] == 'D' && comp[2] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == '-' && comp[1] == 'A' && comp[2] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == '-' && comp[1] == 'M' && comp[2] == '\0'){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == 'D' && comp[1] == '-' && comp[2] == 'A' && comp[3] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == 'D' && comp[1] == '-' && comp[2] == 'M' && comp[3] == '\0'){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == 'A' && comp[1] == '-' && comp[2] == 'D' && comp[3] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == 'M' && comp[1] == '-' && comp[2] == 'D' && comp[3] == '\0'){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if(comp[0] == 'D' && comp[1] == '-' && comp[2] == '1' && comp[3] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if(comp[0] == 'A' && comp[1] == '-' && comp[2] == '1' && comp[3] == '\0'){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if(comp[0] == 'M' && comp[1] == '-' && comp[2] == '1' && comp[3] == '\0'){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if((comp[0] == 'D' && comp[1] == '+' && comp[2] == '1' && comp[3] == '\0') || (comp[0] == '1' && comp[1] == '+' && comp[2] == 'D' && comp[3] == '\0')){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if((comp[0] == 'A' && comp[1] == '+' && comp[2] == '1' && comp[3] == '\0') || (comp[0] == '1' && comp[1] == '+' && comp[2] == 'A' && comp[3] == '\0')){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if((comp[0] == 'M' && comp[1] == '+' && comp[2] == '1' && comp[3] == '\0') || (comp[0] == '1' && comp[1] == '+' && comp[2] == 'M' && comp[3] == '\0')){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if((comp[0] == 'D' && comp[1] == '+' && comp[2] == 'A' && comp[3] == '\0') || (comp[0] == 'A' && comp[1] == '+' && comp[2] == 'D' && comp[3] == '\0')){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if((comp[0] == 'D' && comp[1] == '+' && comp[2] == 'M' && comp[3] == '\0') || (comp[0] == 'M' && comp[1] == '+' && comp[2] == 'D' && comp[3] == '\0')){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if((comp[0] == 'D' && comp[1] == '&' && comp[2] == 'A' && comp[3] == '\0') || (comp[0] == 'A' && comp[1] == '&' && comp[2] == 'D' && comp[3] == '\0')){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if((comp[0] == 'D' && comp[1] == '&' && comp[2] == 'M' && comp[3] == '\0') || (comp[0] == 'M' && comp[1] == '&' && comp[2] == 'D' && comp[3] == '\0')){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
		}else if((comp[0] == 'D' && comp[1] == '|' && comp[2] == 'A' && comp[3] == '\0') || (comp[0] == 'A' && comp[1] == '|' && comp[2] == 'D' && comp[3] == '\0')){
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}else if((comp[0] == 'D' && comp[1] == '|' && comp[2] == 'M' && comp[3] == '\0') || (comp[0] == 'M' && comp[1] == '|' && comp[2] == 'D' && comp[3] == '\0')){
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
			string_append(&s_parsed, '0', &s_parsed_length);
			string_append(&s_parsed, '1', &s_parsed_length);
		}
		string_append(&s_parsed, d1, &s_parsed_length);
		string_append(&s_parsed, d2, &s_parsed_length);
		string_append(&s_parsed, d3, &s_parsed_length);
		string_append(&s_parsed, j1, &s_parsed_length);
		string_append(&s_parsed, j2, &s_parsed_length);
		string_append(&s_parsed, j3, &s_parsed_length);
		string_append(&s_parsed, '\0', &s_parsed_length);
		free(*s);
		*s = s_parsed;
	}
	return 1;
};

int parse_line_second_pass(char ** s, struct symbol_table ** head_ref, int * addr){
	char * s_parsed = NULL;
	int s_parsed_length = 0;
	char * label = NULL;
	int label_length = 0;
	int flag = 1;
	while((*s)[flag] != '\0'){
		string_append(&label, (*s)[flag], &label_length);
		flag++;
	}
	string_append(&label, '\0', &label_length);
	int isInTable;
	struct symbol_table * current_node = *head_ref;
	while(current_node != NULL){
		isInTable = 1;
		flag = 0;
		while(label[flag] != '\0' && (current_node->symbol)[flag] != '\0'){
			isInTable *= (label[flag] == (current_node->symbol)[flag]);
			flag++;
		}
		isInTable *= (label[flag] == (current_node->symbol)[flag]);
		if(isInTable){
			break;
		}
		current_node = current_node->next;
	}
	if(isInTable){
		string_append(&s_parsed, '0', &s_parsed_length);
		int bin;
		for(int i = 14; i >= 0; i--){
			bin = current_node->address >> i;
			if(bin & 1){
				string_append(&s_parsed, '1', &s_parsed_length);
			}else{
				string_append(&s_parsed, '0', &s_parsed_length);
			}
		}
		string_append(&s_parsed, '\0', &s_parsed_length);
		free(*s);
		*s = s_parsed;
	}else{
		table_append(head_ref, label, *addr);
		string_append(&s_parsed, '0', &s_parsed_length);
		int bin;
		for(int i = 14; i >= 0; i--){
			bin = *addr >> i;
			if(bin & 1){
				string_append(&s_parsed, '1', &s_parsed_length);
			}else{
				string_append(&s_parsed, '0', &s_parsed_length);
			}
		}
		string_append(&s_parsed, '\0', &s_parsed_length);
		free(*s);
		*s = s_parsed;
		(*addr)++;
	}
	return 1;
};

int main(int argc, char * argv[]){
	if(argv[1] == '\0'){
		printf("Please supply an input file.\n");
		return 0;
	}
	FILE * input_file = NULL;
	input_file = fopen(argv[1], "r");
	if(input_file == NULL){
		printf("Cannot open file %s.\n", argv[1]);
		return 0;
	}

	struct symbol_table * table = NULL;
	if(!table_append(&table, "SP", 0)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "LCL", 1)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "ARG", 2)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "this_instruction", 3)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "THAT", 4)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R0", 0)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R1", 1)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R2", 2)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R3", 3)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R4", 4)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R5", 5)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R6", 6)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R7", 7)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R8", 8)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R9", 9)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R10", 10)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R11", 11)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R12", 12)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R13", 13)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R14", 14)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "R15", 15)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "SCREEN", 16384)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}
	if(!table_append(&table, "KBD", 24576)){
		printf("Assembly failed while allocating memory.\n");
		return 0;
	}

	struct instruction * lines = NULL;
	int line_number = -1;
	char * this_line = NULL;
	int line_length = 0;
	char this_char = getc(input_file);
	while(this_char != EOF){
		if(this_char == '\n' || this_char == 13){
			if(!string_append(&this_line, '\0', &line_length)){
				printf("Assembly failed while allocating memory.\n");
				return 0;
			}
			if(this_line[0] != '\0'){
				if(parse_line_first_pass(&this_line, &line_number, &table)){
					if(this_line[0] != '\0'){
						instruction_append(&lines, this_line);
					}
				}else{
					printf("Assembly failed while allocating memory.\n");
					return 0;
				}
			}
			free(this_line);
			this_line = NULL;
			line_length = 0;
			this_char = getc(input_file);
		}else if(this_char == '/'){
			if(!string_append(&this_line, '\0', &line_length)){
				printf("Assembly failed while allocating memory.\n");
				return 0;
			}
			if(this_line[0] != '\0'){
				if(parse_line_first_pass(&this_line, &line_number, &table)){
					if(this_line[0] != '\0'){
						instruction_append(&lines, this_line);
					}
				}else{
					printf("Assembly failed while allocating memory.\n");
					return 0;
				}
			}
			while(this_char != '\n' && this_char != 13){
				this_char = getc(input_file);
			}
			free(this_line);
			this_line = NULL;
			line_length = 0;
			this_char = getc(input_file);
		}else if(this_char == ' ' || this_char == 9){
			this_char = getc(input_file);
		}else{
			if(!string_append(&this_line, this_char, &line_length)){
				printf("Assembly failed while allocating memory.\n");
				return 0;
			}
			this_char = getc(input_file);
		}
	}

	char * input_file_name = argv[1];
	char * output_file_name = NULL;
	int output_file_name_length = 0;
	int flag = 0;
	while(input_file_name[flag] != '.'){
		string_append(&output_file_name, input_file_name[flag], &output_file_name_length);
		flag++;
	}
	string_append(&output_file_name, '.', &output_file_name_length);
	string_append(&output_file_name, 'h', &output_file_name_length);
	string_append(&output_file_name, 'a', &output_file_name_length);
	string_append(&output_file_name, 'c', &output_file_name_length);
	string_append(&output_file_name, 'k', &output_file_name_length);
	string_append(&output_file_name, '\0', &output_file_name_length);
	FILE * output_file = NULL;
	output_file = fopen(output_file_name, "w");
	int variable_address = 16;
	struct instruction * current_node = lines;
	while(current_node != NULL){
		if((current_node->this_instruction)[0] == '@'){
			parse_line_second_pass(&(current_node->this_instruction), &table, &variable_address);
		}
		fprintf(output_file, "%s\n", current_node->this_instruction);
		current_node = current_node->next;
	}
	printf("Assembly successful.\n");

	fclose(input_file);
	fclose(output_file);
	return 0;
};
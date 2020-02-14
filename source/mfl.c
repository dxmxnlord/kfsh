#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/mfl.h"

/*
	make new file

	arguments: 
		1. path to the directory:
			1.1 relative from the current directory (./)
			1.2 absolute path
		2. name of the new file
*/

void make_file(char* file_path){
	FILE* file_stream = fopen(file_path,"w+");
	if(file_stream == NULL){
		fprintf(stderr,"kfsh: Could not create file\n");
		return;
	}
	fclose(file_stream);
}

void mfl(char* parsed[],int count){
	if(count == 1){
		fprintf(stderr, "kfsh: Invalid arguments\n");
		return;
	}

	int b_flag = 0;
	char* parsed_string = (char*)malloc(1024 * sizeof(char));
	char* parsed_string_two = (char*)malloc(1024 * sizeof(char));
	int parsed_string_length = 0;
	int parsed_string_length_two = 0;
	int rep=1;
	if(parsed[1][0] == '"'){
		for(; rep < count; rep++){
			int last_char = strlen(parsed[rep]);
			if(parsed[rep][0] == '"' && parsed[rep][last_char-1] != '"'){
				if(b_flag == 0){
					b_flag = 1;
					strcpy(parsed_string,parsed[rep]);
					parsed_string_length = strlen(parsed_string);
					parsed_string[parsed_string_length++] = ' ';
				}
			}
			else if(parsed[rep][0] != '"' && parsed[rep][last_char-1] == '"'){
				if(b_flag == 1){
					b_flag = 0;
					strcat(parsed_string,parsed[rep]);
					break;
				}
			}
			else if(parsed[rep][0] == '"' && parsed[rep][last_char-1] == '"'){
				strcpy(parsed_string,parsed[rep]);
				break;
			}
			else if(b_flag == 1){
				strcat(parsed_string,parsed[rep]);
				parsed_string_length = strlen(parsed_string);
				parsed_string[parsed_string_length++] = ' ';
			}
		}
		parsed_string++;
		parsed_string[strlen(parsed_string)-1] = '\0';
		rep++;
	}
	else{
		strcpy(parsed_string,parsed[1]);
		rep++;
	}

	b_flag = 0;
	if(parsed[rep][0] == '"'){
		for(; rep < count; rep++){
			int last_char = strlen(parsed[rep]);
			if(parsed[rep][0] == '"' && parsed[rep][last_char-1] != '"'){
				if(b_flag == 0){
					b_flag = 1;
					strcpy(parsed_string_two,parsed[rep]);
					parsed_string_length_two = strlen(parsed_string_two);
					parsed_string_two[parsed_string_length_two++] = ' ';
				}
			}
			else if(parsed[rep][0] != '"' && parsed[rep][last_char-1] == '"'){
				if(b_flag == 1){
					b_flag = 0;
					strcat(parsed_string_two,parsed[rep]);
					break;
				}
			}
			else if(parsed[rep][0] == '"' && parsed[rep][last_char-1] == '"'){
				strcpy(parsed_string_two,parsed[rep]);
				break;
			}
			else if(b_flag == 1){
				strcat(parsed_string_two,parsed[rep]);
				parsed_string_length_two = strlen(parsed_string_two);
				parsed_string_two[parsed_string_length_two++] = ' ';
			}
		}
		parsed_string_two++;
		parsed_string_two[strlen(parsed_string_two)-1] = '\0';
	}
	else{
		strcpy(parsed_string_two,parsed[rep]);
	}

	char* dir_path = parsed_string;
	char* file_name = parsed_string_two;
	if(dir_path[0] == '/'){
		strcat(dir_path,file_name);
		make_file(dir_path);
	}
	else{
		char* curr_dir_path = getcwd(NULL,0);
		curr_dir_path = (char*)realloc(curr_dir_path,sizeof(curr_dir_path)+1024);
		strcat(curr_dir_path,"/");
		strcat(curr_dir_path,dir_path);
		strcat(curr_dir_path,"/");
		strcat(curr_dir_path,file_name);
		make_file(curr_dir_path);
	}
}
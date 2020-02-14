#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/pfl.h"

/*
	print file output

	arguments: 
		1. path to the file:
			1.1 relative from the current directory (./)
			1.2 absolute path
*/

void print_file(char* file_path){
	FILE* file_stream = fopen(file_path,"r");
	if(file_stream == NULL){
		fprintf(stderr,"kfsh: Could not open file");
		return;
	}
	while(1){
		int rchr = fgetc(file_stream);
		if( rchr == EOF )
			break;
		else
			fprintf(stdout,"%c",rchr);
	}
	fprintf(stdout, "\n");
	fclose(file_stream);
}

void pfl(char* parsed[],int count){
	if(count == 1){
		fprintf(stderr, "kfsh: No file to read from\n");
		return;
	}

	int b_flag = 0;
	char* parsed_string = (char*)malloc(1024 * sizeof(char));
	int parsed_string_length = 0;
	if(parsed[1][0] == '"'){
		for(int rep = 1; rep < count; rep++){
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
	}
	else{
		strcpy(parsed_string,parsed[1]);
	}

	char* dir_path = parsed_string;
	if(dir_path[0] == '/'){
		print_file(dir_path);
	}
	else{
		char* curr_dir_path = getcwd(NULL,0);
		curr_dir_path = (char*)realloc(curr_dir_path,sizeof(curr_dir_path)+1024);
		strcat(curr_dir_path,"/");
		strcat(curr_dir_path,dir_path);
		print_file(curr_dir_path);
		free(curr_dir_path);
	}
	// free(parsed_string);
}
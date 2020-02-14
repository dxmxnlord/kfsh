#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/rmf.h"

/*
	remove folder

	arguments:
			1. path to the directory:
				1.1 relative from the current directory (./)
				1.2 absolute path
*/

void rmf(char* parsed[],int count){
	if(count == 1){
		fprintf(stderr, "kfsh: No folder to delete\n");
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

	int ret_val;
	char* dir_path = parsed_string;
	if(dir_path[0] == '/'){
		ret_val = rmdir(dir_path);
	}
	else{
		char* curr_dir_path = getcwd(NULL,0);
		strcat(curr_dir_path,"/");
		strcat(curr_dir_path,dir_path);
		ret_val = rmdir(curr_dir_path);
	}
	if(!ret_val){
		fprintf(stdout, "Deleted directory\n");
	}
	else{
		switch(errno){
			case EACCES: {
				fprintf(stderr, "kfsh: Dont have appropriate permissions\n");
				break;
			}
			case ENOTDIR: {
				fprintf(stderr, "kfsh: Not a directory\n");
				break;
			}
			case ENOTEMPTY: {
				fprintf(stderr, "kfsh: Directory not empty\n");
				break;
			}
			case ENAMETOOLONG: {
				fprintf(stderr, "kfsh: Name is too long\n");
				break;
			}
			default:{
				fprintf(stderr, "kfsh: Could not delete directory\n");
				break;
			}
		}
	}
}
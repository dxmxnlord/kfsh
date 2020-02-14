#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/rfl.h"

/*
	rename file

	arguments: 
		1. path to the old directory:
			1.1 relative from the current directory (./)
			1.2 absolute path
		2. path to the new directory:
			1.1 relative from the current directory (./)
			1.2 absolute path
*/

void rfl(char* parsed[],int count){
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

	char* old_path = parsed_string;
	char* new_path = parsed_string_two;
	char* curr_dir_path_old = getcwd(NULL,0);
	char* curr_dir_path_new = getcwd(NULL,0);
	if(old_path[0] != '/'){
		strcat(curr_dir_path_old,"/");
		curr_dir_path_old = (char*)realloc(curr_dir_path_old,sizeof(curr_dir_path_old)+1024);
		strcat(curr_dir_path_old,old_path);
		old_path = curr_dir_path_old;
	}
	if(new_path[0] != '/'){
		curr_dir_path_new = (char*)realloc(curr_dir_path_new,sizeof(curr_dir_path_new)+1024);
		strcat(curr_dir_path_new,"/");
		strcat(curr_dir_path_new,new_path);
		new_path = curr_dir_path_new;
	}
	int ret_val = rename(old_path,new_path);
	if(!ret_val){
		fprintf(stdout, "Renamed file\n");
	}
	else{
		switch(errno){
			case EACCES: {
				fprintf(stderr, "kfsh: Dont have appropriate permissions\n");
				break;
			}
			case EBUSY: {
				fprintf(stderr, "kfsh: File is being used by a process\n");
				break;
			}
			case EINVAL: {
				fprintf(stderr, "kfsh: Cant make a directory a subdirectory\n");
				break;
			}
			case EISDIR: {
				fprintf(stderr, "kfsh: Cannot make a file into a directory\n");
				break;
			}
			case EEXIST: {
				fprintf(stderr, "kfsh: Cannot rename into a non empty existing directory\n");
				break;
			}
			default:{
				fprintf(stderr, "kfsh: Could not rename\n");
				break;
			}
		}
	}
}
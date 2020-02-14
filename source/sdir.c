#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/sdir.h"

/*
	search directory for file

	arguments: 
		1. path to the directory:
			1.1 relative from the current directory (./)
			1.2 absolute path
		2. name of the file
*/

void search_dir(char* dir_path, char* file_name){
	DIR* dir_stream = opendir(dir_path);
	if(dir_stream == NULL){
		switch(errno){
			case EACCES: {
				fprintf(stderr, "kfsh: Dont have appropriate permissions\n");
				break;
			}
			case ENOENT: {
				fprintf(stderr, "kfsh: Not a directory\n");
				break;
			}
			case ENOMEM: {
				fprintf(stderr, "kfsh: No memory to complete operation\n");
				break;
			}
			case ENOTDIR: {
				fprintf(stderr, "kfsh: Name is not a directory\n");
				break;
			}
			default:{
				fprintf(stderr, "kfsh: Could not open folder\n");
				break;
			}
		}
		return;
	}

	errno = 0;
	while(1){
		struct dirent* dir_entry = readdir(dir_stream);
		if(dir_entry == NULL && !errno){
			fprintf(stdout, "kfsh: File not found\n");
			break;
		}
		else if(dir_entry == NULL && errno){
			fprintf(stderr, "kfsh: Error while reading from directory\n");
			break;
		}
		else{
			if(DT_REG == dir_entry->d_type){
				if(!strcmp(dir_entry->d_name,file_name)){
					fprintf(stdout, "kfsh: File found\n");
					break;
				}
			}
		}
	}

	closedir(dir_stream);
}

void sdir(char* parsed[],int count){
	if(count == 1){
		fprintf(stderr, "kfsh: No folder to search\n");
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
		search_dir(dir_path,file_name);
	}
	else{
		char* curr_dir_path = getcwd(NULL,0);
		strcat(curr_dir_path,"/");
		strcat(curr_dir_path,dir_path);
		search_dir(curr_dir_path,file_name);
	}
}
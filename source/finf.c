#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/finf.h"

/*
	get file info

	arguments: 
		1. path to the file:
			1.1 relative from the current directory
			1.2 absolute path
*/
void print_info(char* file_path){
	struct stat* stat_buffer = (struct stat*)malloc(sizeof(struct stat));
	int ret_val = stat(file_path,stat_buffer);
	if(!ret_val){
		fprintf(stdout, "Device ID: %ld\n",stat_buffer->st_dev);
		fprintf(stdout, "Inode No: %ld\n",stat_buffer->st_ino);
		fprintf(stdout, "Owner User ID: %d\n",stat_buffer->st_uid);
		fprintf(stdout, "Owner Group ID: %d\n",stat_buffer->st_gid);
		fprintf(stdout, "Total Size: %ld\n",stat_buffer->st_size);
	}
	else{
		switch(errno){
			case EACCES: {
				fprintf(stderr, "kfsh: Dont have appropriate permissions\n");
				break;
			}
			case ENOTDIR: {
				fprintf(stderr, "kfsh: Part of path is not a directory\n");
				break;
			}
			case EFAULT: {
				fprintf(stderr, "kfsh: Bad address\n");
				break;
			}
			case ENAMETOOLONG: {
				fprintf(stderr, "kfsh: Name is too long\n");
				break;
			}
			default:{
				fprintf(stderr, "kfsh: Could not get info\n");
				break;
			}
		}
	}
}

void finf(char* parsed[],int count){
	if(count == 1){
		fprintf(stderr, "kfsh: No file to get information from\n");
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

	char* file_path = parsed_string;
	if(file_path[0] == '/'){
		print_info(file_path);
	}
	else{
		char* curr_dir_path = getcwd(NULL,0);
		strcat(curr_dir_path,"/");
		curr_dir_path = (char*)realloc(curr_dir_path,sizeof(curr_dir_path)+1024);
		strcat(curr_dir_path,file_path);
		print_info(curr_dir_path);
	}
}
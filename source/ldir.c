#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/ldir.h"

/*
	list directories and files

	arguments: 
		1. path to the directory:
			1.1 relative from the current directory (./)
			1.2 absolute path

	options:
		none	print long info
		-s 		print short info

	format:
		ldir <arguments> <options>
*/

void print_dir(char* dir_name){
	DIR* dir_stream = opendir(dir_name);
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
			break;
		}
		else if(dir_entry == NULL && errno){
			fprintf(stderr, "kfsh: Error while reading from directory\n");
			break;
		}
		else{
			fprintf(stdout, "File Name: %s\n", dir_entry->d_name );
			fprintf(stdout, "File Size: %hu\n", dir_entry->d_reclen );
			if(DT_DIR == dir_entry->d_type)
				fprintf(stdout, "File Type: directory\n");
			else if(DT_REG == dir_entry->d_type)
				fprintf(stdout, "File Type: regular file\n");
			else if(DT_BLK == dir_entry->d_type)
				fprintf(stdout, "File Type: block device\n");
			else if(DT_CHR == dir_entry->d_type)
				fprintf(stdout, "File Type: character device\n");
			else if(DT_LNK == dir_entry->d_type)
				fprintf(stdout, "File Type: symbolic link\n");
			else if(DT_UNKNOWN == dir_entry->d_type)
				fprintf(stdout, "File Type: unknown\n");
			fprintf(stdout, "\n");
		}
	}

	closedir(dir_stream);
}

void print_dir_short(char* dir_name){
	DIR* dir_stream = opendir(dir_name);
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
			break;
		}
		else if(dir_entry == NULL && errno){
			fprintf(stderr, "kfsh: Error while reading from directory\n");
			break;
		}
		else{
			if(DT_DIR == dir_entry->d_type){
				fprintf(stdout, "%s/\n", dir_entry->d_name );
			}
			else if(DT_REG == dir_entry->d_type){
				fprintf(stdout, "%s\n", dir_entry->d_name );
			}
			else if(DT_UNKNOWN == dir_entry->d_type)
				fprintf(stdout, "%s(unk)\n",dir_entry->d_name);
		}
	}

	closedir(dir_stream);
}

void ldir(char* parsed[],int count){
	if(count == 1){
		fprintf(stderr, "kfsh: No folder to list\n");
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

	optind = 0;
	int opt;
	int flag = 0;
	while(1){
		opt = getopt(count,parsed,":s");
		if(opt == -1){
			if(!flag){
				char* dir_path = parsed_string;
				if(dir_path[0] == '/'){
					print_dir(dir_path);
				}
				else{
					char* curr_dir_path = getcwd(NULL,0);
					strcat(curr_dir_path,"/");
					strcat(curr_dir_path,dir_path);
					print_dir(curr_dir_path);
				}
				break;
			}
			else
				break;
		}
		switch(opt){
			case 's':{
				char* dir_path = parsed_string;
				if(dir_path[0] == '/'){
					print_dir_short(dir_path);
				}
				else{
					char* curr_dir_path = getcwd(NULL,0);
					strcat(curr_dir_path,"/");
					strcat(curr_dir_path,dir_path);
					print_dir_short(curr_dir_path);
				}
				flag = 1;
				break;
			}
			case '?':{
				fprintf(stderr, "kfsh: Could not recognise that argument\n");
				flag = 1;
				break;
			}
		}
	}
}
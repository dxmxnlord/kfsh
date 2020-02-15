#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "../headers/mkf.h"

/*
	make new folder
	
	arguments: 
		1. path to the directory:
			1.1 relative from the current directory (./)
			1.2 absolute path
	options:
		-u 	permissions(rwx)	reqd
		-g	permissions(rwx)	reqd
		-o	permissions(rwx)	reqd
*/

void mkf(char* parsed[],int count){

	optind = 0;
	int opt;
	int flag = 0;
	int u_flag = 0;
	int g_flag = 0;
	int o_flag = 0;
	int mask = 0;
	while(1){
		opt = getopt(count,parsed,":u:g:o:");
		if(opt == -1){
			if(!flag){
				fprintf(stderr, "kfsh: Requires arguments\n");
				break;
			}
			else
				break;
		}
		else{
			switch(opt){
				case 'u':{
					u_flag = 1;
					if(!strcmp(optarg,"r"))
						mask = mask | S_IRUSR;
					else if(!strcmp(optarg,"w"))
						mask = mask | S_IWUSR;
					else if(!strcmp(optarg,"x"))
						mask = mask | S_IXUSR;
					else if(!strcmp(optarg,"rw") || !strcmp(optarg,"wr"))
						mask = mask | S_IRUSR | S_IWUSR;
					else if(!strcmp(optarg,"rx") || !strcmp(optarg,"xr"))
						mask = mask | S_IRUSR | S_IXUSR;
					else if(!strcmp(optarg,"wx") || !strcmp(optarg,"xw"))
						mask = mask | S_IXUSR | S_IWUSR;
					else if(!strcmp(optarg,"rwx") || !strcmp(optarg,"rxw") || !strcmp(optarg,"wrx") || !strcmp(optarg,"xwr") || !strcmp(optarg,"xrw") || !strcmp(optarg,"wxr"))
						mask = mask | S_IRWXU;
					else{
						fprintf(stderr, "kfsh: Invalid permission combination\n");
						u_flag = 0;
					}
					break;
				}
				case 'g':{
					g_flag = 1;
					if(!strcmp(optarg,"r"))
						mask = mask | S_IRGRP;
					else if(!strcmp(optarg,"w"))
						mask = mask | S_IWGRP;
					else if(!strcmp(optarg,"x"))
						mask = mask | S_IXGRP;
					else if(!strcmp(optarg,"rw") || !strcmp(optarg,"wr"))
						mask = mask | S_IRGRP | S_IWGRP;
					else if(!strcmp(optarg,"rx") || !strcmp(optarg,"xr"))
						mask = mask | S_IRGRP | S_IXGRP;
					else if(!strcmp(optarg,"wx") || !strcmp(optarg,"xw"))
						mask = mask | S_IXGRP | S_IWGRP;
					else if(!strcmp(optarg,"rwx") || !strcmp(optarg,"rxw") || !strcmp(optarg,"wrx") || !strcmp(optarg,"xwr") || !strcmp(optarg,"xrw") || !strcmp(optarg,"wxr"))
						mask = mask | S_IRWXG;
					else{
						fprintf(stderr, "kfsh: Invalid permission combination\n");
						g_flag = 0;
					}
					break;
				}
				case 'o':{
					o_flag = 1;
					if(!strcmp(optarg,"r"))
						mask = mask | S_IROTH;
					else if(!strcmp(optarg,"w"))
						mask = mask | S_IWOTH;
					else if(!strcmp(optarg,"x"))
						mask = mask | S_IXOTH;
					else if(!strcmp(optarg,"rw") || !strcmp(optarg,"wr"))
						mask = mask | S_IROTH | S_IWOTH;
					else if(!strcmp(optarg,"rx") || !strcmp(optarg,"xr"))
						mask = mask | S_IROTH | S_IXOTH;
					else if(!strcmp(optarg,"wx") || !strcmp(optarg,"xw"))
						mask = mask | S_IXOTH | S_IWOTH;
					else if(!strcmp(optarg,"rwx") || !strcmp(optarg,"rxw") || !strcmp(optarg,"wrx") || !strcmp(optarg,"xwr") || !strcmp(optarg,"xrw") || !strcmp(optarg,"wxr"))
						mask = mask | S_IRWXO;
					else{
						fprintf(stderr, "kfsh: Invalid permission combination\n");
						o_flag = 0;
					}
					break;
				}
				case ':':{
					fprintf(stderr, "kfsh: Missing argument value\n");
					break;
				}
				case '?':{
					fprintf(stderr, "kfsh: Could not recognise that argument\n");
					break;
				}
			}
			if(!flag)
				flag = 1;
		}
	}

	if(!(u_flag && g_flag && o_flag)){
		fprintf(stderr, "kfsh: Give permissions for every user type\n");
		return;
	}
	else{
		if(optind == count){
			fprintf(stderr, "kfsh: No folder to create\n");
			return;
		}
		if(parsed[optind][0] == '"'){
			int b_flag = 0;
			char* parsed_string = (char*)malloc(1024 * sizeof(char));
			int parsed_string_length = 0;
			for(int rep = optind; rep < count; rep++){
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

			char* dir_path = parsed_string;
			int ret_val;
			if(dir_path[0] == '/'){
				ret_val = mkdir(dir_path,mask);
			}
			else{
				char* curr_dir_path = getcwd(NULL,0);
				strcat(curr_dir_path,"/");
				strcat(curr_dir_path,dir_path);
				ret_val = mkdir(curr_dir_path,mask);
			}
			if(!ret_val){
				fprintf(stdout, "Created directory\n");
			}
			else{
				switch(errno){
					case EACCES: {
						fprintf(stderr, "kfsh: Dont have appropriate permissions\n");
						break;
					}
					case EDQUOT: {
						fprintf(stderr, "kfsh: Dont have space left\n");
						break;
					}
					case EEXIST: {
						fprintf(stderr, "kfsh: A directory with the same name alredy exists\n");
						break;
					}
					case ENAMETOOLONG: {
						fprintf(stderr, "kfsh: Name is too long\n");
						break;
					}
					default:{
						fprintf(stderr, "kfsh: Could not create directory\n");
						break;
					}
				}
			}
		}
		else{
			char* dir_path = parsed[optind];
			int ret_val;
			if(dir_path[0] == '/'){
				ret_val = mkdir(dir_path,mask);
			}
			else{
				char* curr_dir_path = getcwd(NULL,0);
				strcat(curr_dir_path,"/");
				strcat(curr_dir_path,dir_path);
				ret_val = mkdir(curr_dir_path,mask);
			}
			if(!ret_val){
				fprintf(stdout, "Created directory\n");
			}
			else{
				switch(errno){
					case EACCES: {
						fprintf(stderr, "kfsh: Dont have appropriate permissions\n");
						break;
					}
					case EDQUOT: {
						fprintf(stderr, "kfsh: Dont have space left\n");
						break;
					}
					case EEXIST: {
						fprintf(stderr, "kfsh: A directory with the same name alredy exists\n");
						break;
					}
					case ENAMETOOLONG: {
						fprintf(stderr, "kfsh: Name is too long\n");
						break;
					}
					default:{
						fprintf(stderr, "kfsh: Could not create directory\n");
						break;
					}
				}
			}
		}
	}
}

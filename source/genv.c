#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "../headers/genv.h"

char* get_env_var(char* env_var){
	char* env_val = getenv(env_var);
	return env_val;
}

/*
	get environment variable

	arguments: none

	options:
		--uname 	print USER
		--home 		print HOME
		--editor 	print EDITOR
		--shell 	print SHELL
		--lang 		print LANG
		--path 		print PATH

*/
void genv(char* parsed[],int count){
	//reset the opt index for multiple calls
	optind = 0;
	int opt;
	int flag = 0;
	struct option* longopts = (struct option*)malloc(7*sizeof(struct option));

	// option --uname
	longopts[0].name = "uname";
	longopts[0].has_arg = 0;
	longopts[0].flag = NULL;
	longopts[0].val = 1;

	// option --home
	longopts[1].name = "home";
	longopts[1].has_arg = 0;
	longopts[1].flag = NULL;
	longopts[1].val = 2;

	// option --editor
	longopts[2].name = "editor";
	longopts[2].has_arg = 0;
	longopts[2].flag = NULL;
	longopts[2].val = 3;

	// option --shell
	longopts[3].name = "shell";
	longopts[3].has_arg = 0;
	longopts[3].flag = NULL;
	longopts[3].val = 4;

	// option --lang
	longopts[4].name = "lang";
	longopts[4].has_arg = 0;
	longopts[4].flag = NULL;
	longopts[4].val = 5;

	// option --path
	longopts[5].name = "path";
	longopts[5].has_arg = 0;
	longopts[5].flag = NULL;
	longopts[5].val = 6;

	// longopts terminator
	longopts[6].name = 0;
	longopts[6].has_arg = 0;
	longopts[6].flag = 0;
	longopts[6].val = 0;

	while(1){
		opt = getopt_long(count,parsed,":",longopts,NULL);
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
				case 1:{
					char* env_val = get_env_var("USER");
					if(env_val == NULL){
						fprintf(stderr, "kfsh: Cannot get username\n");
					}
					else{
						printf("Username: %s\n",env_val);
					}
					break;
				}
				case 2:{
					char* env_val = get_env_var("HOME");
					if(env_val == NULL){
						fprintf(stderr, "kfsh: Cannot get Home directory\n");
					}
					else{
						printf("Home directory: %s\n",env_val);
					}
					break;
				}
				case 3:{
					char* env_val = get_env_var("EDITOR");
					if(env_val == NULL){
						fprintf(stderr, "kfsh: Cannot get Editor\n");
					}
					else{
						printf("Editor: %s\n",env_val);
					}
					break;
				}
				case 4:{
					char* env_val = get_env_var("SHELL");
					if(env_val == NULL){
						fprintf(stderr, "kfsh: Cannot get Shell\n");
					}
					else{
						printf("Shell: %s\n",env_val);
					}
					break;
				}
				case 5:{
					char* env_val = get_env_var("LANG");
					if(env_val == NULL){
						fprintf(stderr, "kfsh: Cannot get Language\n");
					}
					else{
						printf("Language: %s\n",env_val);
					}
					break;
				}
				case 6:{
					char* env_val = get_env_var("PATH");
					if(env_val == NULL){
						fprintf(stderr, "kfsh: Cannot get Path\n");
					}
					else{
						printf("Path: %s\n",env_val);
					}
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
	free(longopts);
}
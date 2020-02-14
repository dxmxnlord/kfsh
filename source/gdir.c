#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/gdir.h"

/*
	get current directory

	arguments: none

	options:
		none	full directory
		-s  	short version
*/
void gdir(char* parsed[],int count){
	//reset the opt index variable
	optind = 0;
	int opt;
	int flag = 0;
	while(1){
		opt = getopt(count,parsed,":s");
		if(opt == -1){
			if(!flag){
				char* return_value = getcwd(NULL,0);
				if(return_value == NULL){
					fprintf(stderr, "kfsh: Could not get current directory\n");
					exit(EXIT_FAILURE);
				}
				else{
					printf("Working directory is %s\n",return_value);
				}
				break;
			}
			else
				break;
		}
		switch(opt){
			case 's':{
				char* return_value = getcwd(NULL,0);
				if(return_value == NULL){
					fprintf(stderr, "kfsh: Could not get current directory\n");
					exit(EXIT_FAILURE);
				}
				else{
					char* username = getenv("USER");
					int ret_size = strlen(return_value);
					memmove(&return_value[0],&return_value[5],ret_size-4);
					memmove(&return_value[0],&return_value[strlen(username)+1],ret_size-4-strlen(username));
					printf("Working directory is ~%s\n",return_value);
				flag = 1;
				break;
				}
			}
			case '?':{
				fprintf(stderr, "kfsh: Could not recognise that argument\n");
				flag = 1;
				break;
			}
		}
	}
}

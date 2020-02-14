#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "../headers/prt.h"

/*
	print string	

	arguments: 
		1. string to print

	options:
		-n  	direct print version
		-r  	with regex and escaped version
		-s  	stripped version

	format:
		prt -[nrs] <string>
*/

void prt(char* parsed[],int count){
	char* reparsed[10];
	int reparsed_count = 0;
	int b_flag = 0;
	char* parsed_string = (char*)malloc(1024 * sizeof(char));
	int parsed_string_length = 0;
	for(int rep = 0; rep < count; rep++){
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
				reparsed[reparsed_count] = parsed_string;
				reparsed_count++;
			}
		}
		else{
			reparsed[reparsed_count] = parsed[rep];
			reparsed_count++;
		}
	}

	optind = 0;
	int opt;
	int o_flag = 0;
	while(1){
		opt = getopt(reparsed_count,reparsed,":n:r:s:");
		if(opt == -1){
			if(!o_flag){
				fprintf(stderr, "kfsh: Needs arguments\n");
				exit(EXIT_FAILURE);
			}
			else
				break;
		}
		switch(opt){
			case 'n':{
				char* mod_optarg = (char*)malloc(1024*sizeof(char));
				int mod_optarg_length = 0;
				for(int iter = 0; iter < strlen(optarg); iter++){
					if(optarg[iter] == '\n'){
						mod_optarg[mod_optarg_length++] = '\\';
						mod_optarg[mod_optarg_length++] = 'n';
					}
					else if(optarg[iter] == '\b'){
						mod_optarg[mod_optarg_length++] = '\\';
						mod_optarg[mod_optarg_length++] = 'b';
					}
					else if(optarg[iter] == '\t'){
						mod_optarg[mod_optarg_length++] = '\\';
						mod_optarg[mod_optarg_length++] = 't';
					}
					else if(optarg[iter] == '\a'){
						mod_optarg[mod_optarg_length++] = '\\';
						mod_optarg[mod_optarg_length++] = 'a';
					}
					else if(optarg[iter] == '\f'){
						mod_optarg[mod_optarg_length++] = '\\';
						mod_optarg[mod_optarg_length++] = 'f';
					}
					else if(optarg[iter] == '\r'){
						mod_optarg[mod_optarg_length++] = '\\';
						mod_optarg[mod_optarg_length++] = 'r';
					}
					// else if(optarg[iter] == '\''){
					// 	mod_optarg[mod_optarg_length++] = '\\';
					// 	mod_optarg[mod_optarg_length++] = '\'';
					// }
					// else if(optarg[iter] == '"'){
					// 	mod_optarg[mod_optarg_length++] = '\\';
					// 	mod_optarg[mod_optarg_length++] = '"';
					// }
					else{
						mod_optarg[mod_optarg_length++] = optarg[iter];
					}
				}
				fprintf(stdout,"%s\n",mod_optarg);
				o_flag = 1;
				free(mod_optarg);
				break;
			}
			case 'r':{
				char* mod_optarg = (char*)malloc(1024*sizeof(char));
				int mod_optarg_length = 0;
				for(int iter = 0; iter < strlen(optarg); iter++){
					if(optarg[iter] == '\\' && iter != strlen(optarg)-1 && optarg[iter+1] == 'n'){
						mod_optarg[mod_optarg_length++] = '\n';
						iter++;
					}
					else if(optarg[iter] == '\\' && iter != strlen(optarg)-1 && optarg[iter+1] == 'b'){
						mod_optarg[mod_optarg_length++] = '\b';
						iter++;
					}
					else if(optarg[iter] == '\\' && iter != strlen(optarg)-1 && optarg[iter+1] == 't'){
						mod_optarg[mod_optarg_length++] = '\t';
						iter++;
					}
					else if(optarg[iter] == '\\' && iter != strlen(optarg)-1 && optarg[iter+1] == 'a'){
						mod_optarg[mod_optarg_length++] = '\a';
						iter++;
					}
					else if(optarg[iter] == '\\' && iter != strlen(optarg)-1 && optarg[iter+1] == 'f'){
						mod_optarg[mod_optarg_length++] = '\f';
						iter++;
					}
					else if(optarg[iter] == '\\' && iter != strlen(optarg)-1 && optarg[iter+1] == 'r'){
						mod_optarg[mod_optarg_length++] = '\r';
						iter++;
					}
					else{
						mod_optarg[mod_optarg_length++] = optarg[iter];
					}
				}
				fprintf(stdout,"%s\n",mod_optarg);
				o_flag = 1;
				break;
			}
			case 's':{
				char* mod_optarg = (char*)malloc(2*sizeof(optarg));
				strcpy(mod_optarg,optarg);
				mod_optarg++;
				mod_optarg[strlen(mod_optarg)-1] = '\0';
				fprintf(stdout,"%s\n",mod_optarg);
				o_flag = 1;
				break;
			}
			case ':':{
				fprintf(stderr, "kfsh: Needs value\n");
				break;
			}
			case '?':{
				fprintf(stderr, "kfsh: Could not recognise that argument\n");
				o_flag = 1;
				break;
			}
		}
	}
}
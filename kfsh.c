#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kfsh.h"
#include "headers/gdir.h"
#include "headers/genv.h"
#include "headers/prt.h"
#include "headers/mkf.h"
#include "headers/rmf.h"
#include "headers/ldir.h"
#include "headers/sdir.h"
#include "headers/cdir.h"
#include "headers/pfl.h"
#include "headers/mfl.h"
#include "headers/rfl.h"
#include "headers/finf.h"

#define MIN_BUFFER_SIZE 1024

/*

char** commands;
int command_count;

void create_command_history(){
	commands = (char**)malloc(50 * sizeof(char*));
	command_count = 0;
}

void add_command_to_history(char* command){
	commands[command_count] = (char*)malloc(512*sizeof(char));
	strcpy(commands[command_count++],command);
}

*/

/*
	read the user input into a buffer and dynamically 
	increase the buffer size
*/
char* read_line(void)
{
  // Define a buffer of initial size 1024 bytes
  int buffer_size = MIN_BUFFER_SIZE;
  int curr_position = 0;
  char* buffer = malloc(sizeof(char) * buffer_size);
  int inpc;

  if (!buffer) {
	fprintf(stderr, "kfsh: Allocation Error\n");
	exit(EXIT_FAILURE);
  }

  printf("> ");
  while (1) {
	inpc = getchar();
	if (inpc == EOF || inpc == '\n') {
	  buffer[curr_position] = '\0';
	  return buffer;
	} else {
	  buffer[curr_position] = inpc;
	}
	curr_position++;
	if (curr_position >= buffer_size) {
	  buffer_size += MIN_BUFFER_SIZE;
	  buffer = realloc(buffer, buffer_size);
	  if (!buffer) {
		fprintf(stderr, "kfsh: Allocation Error\n");
		exit(EXIT_FAILURE);
	  }
	}
  }
}
/*
	parses the input buffer given to it into space separated token
*/
void parse_input_buffer(char* input, char** parsed_input, int* parsed_count){
	if(!input){
		fprintf(stderr, "kfsh: No Input To Parse\n");
		exit(EXIT_FAILURE);
	}

	// add_command_to_history(input);

	int count = 0;
	char* sep_return;
	while(1){
		sep_return = strsep(&input," ");
		if(sep_return == NULL)
			break;
		else{
			parsed_input[count] = sep_return;
			count++;
		}
	}
	if(parsed_count){
		*parsed_count = count;
	}
}

/*
	execute appropriate command from parsed tokens
*/
int find_parsed_command(char** parsed, int* parsed_count){
	if(parsed == NULL || parsed[0] == NULL){
		fprintf(stderr, "kfsh: No Input To Parse\n");
		exit(EXIT_FAILURE);
	}
	// get current directory
	if(!strcmp(parsed[0],"gdir")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		gdir(parsed_args,*parsed_count);
	}
	// get environment variables
	else if(!strcmp(parsed[0],"genv")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		genv(parsed_args,*parsed_count);
	}
	// print to stdout
	else if(!strcmp(parsed[0],"prt")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		prt(parsed_args,*parsed_count);
	}
	//create directory
	else if(!strcmp(parsed[0],"mkf")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		mkf(parsed_args,*parsed_count);
	}
	//delete directory
	else if(!strcmp(parsed[0],"rmf")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		rmf(parsed_args,*parsed_count);
	}
	//list directory
	else if(!strcmp(parsed[0],"ldir")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		ldir(parsed_args,*parsed_count);
	}
	//search file
	else if(!strcmp(parsed[0],"sdir")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		sdir(parsed_args,*parsed_count);
	}
	//change directory
	else if(!strcmp(parsed[0],"cdir")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		cdir(parsed_args,*parsed_count);
	}
	//print file
	else if(!strcmp(parsed[0],"pfl")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		pfl(parsed_args,*parsed_count);
	}
	//create file
	else if(!strcmp(parsed[0],"mfl")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		mfl(parsed_args,*parsed_count);
	}
	//rename file
	else if(!strcmp(parsed[0],"rfl")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		rfl(parsed_args,*parsed_count);
	}
	//get file info
	else if(!strcmp(parsed[0],"finf")){
		char* parsed_args[*parsed_count];
		for(int i=0;i<*parsed_count;i++)
			parsed_args[i] = parsed[i];
		finf(parsed_args,*parsed_count);
	}
	else if(!strcmp(parsed[0],"exit")){
		return -1;
	}
	else{
		fprintf(stderr, "kfsh: Couldn't recognize that command\n");
	}
	return 0;
}

void kfsh_run(){
	while(1){
		// create_command_history();
		char* read_input = read_line();
		int count = 0;
		char** parse = (char**)malloc(10*sizeof(char*));
		parse_input_buffer(read_input,parse,&count);
		int i = find_parsed_command(parse,&count);
		if(i == -1)
			break;
	}
}
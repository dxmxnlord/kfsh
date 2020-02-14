#ifndef INCL_KFSH
	#define INCL_KFSH
	char* read_line(void);
	void parse_input_buffer(char* input,char** parsed_input,int* parsed_count);
	int find_parsed_command(char** parsed, int* parsed_count);
	// void add_command_to_history(char* command);
	void kfsh_run();
#endif
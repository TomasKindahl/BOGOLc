#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* CLASS: program
 * PURPOSE: contains the program
 */
typedef struct _program_line_S {
	char *line;
} program_line;

program_line *program[1000];

/* METHOD: program_init
 * ARGS: --
 * RETURNS: --
 */

void program_init(void) {
	int i;
	for(i = 0; i < 1000; i++) {
		program[i] = 0;
	}
}

/* METHOD: program_set_line
 * ARGS: int lnum - line number
 *       char *line - textual line content
         char **tlist - token list
         int tlist_len - size of token list
 * RETURNS: 1 if successful, 0 otherwise
 */
 
int program_set_line(int lnum, char *line, char **tlist, int tlist_len) {
	program_line *new_line = (program_line *)malloc(sizeof(program_line));
	new_line->line = strdup(line);
	program[lnum] = new_line;
	return 1;
}

/* METHOD: program_list
 * ARGS: --
 * RETURNS: --
 */

void program_list(void) {
	int i;
	for(i = 0; i < 1000; i++) {
		if(program[i] != 0) {
			printf("%d %s\n", i, program[i]->line);
		}
	}
}

/* METHOD: program_save
 * ARGS: --
 * RETURNS: --
 */

void program_save(void) {
	int i;
    FILE *out = fopen("program.bog", "wt");
	for(i = 0; i < 1000; i++) {
		if(program[i] != 0) {
			fprintf(out, "%d %s\n", i, program[i]->line);
		}
	}
	fclose(out);
}

char **scan_words(char *line, char **words, int *num) {
	*num = 0;
	while(*line != '\0') {
		while(isspace(*line)) line++;
		// Space before end of string => just exit!
		if(*line == '\0') ; 
		// Single non-alphanum chars constitute one operator:
		else if(!isalnum(*line)) {
			char *new_word = (char *)malloc(sizeof(char)*2);
			new_word[0] = *line;
			new_word[1] = '\0';
			words[*num] = new_word;
			(*num)++;
			line++;
		}
		// Else an alphanum keyword or identifer:
		else {
			char buf[128];
			int n = 0;
			char *new_word;
			while(isalnum(*line)) {
				buf[n] = *line;
				n++;
				line++;
			}
			buf[n] = '\0';
			new_word = strdup(buf);
			words[*num] = new_word;
			(*num)++;
		}
	}
	return words;
}

int is_number(char *word) {
	int i;
	for(i = 0; word[i] != '\0'; i++) {
		if(!isdigit(word[i])) return 0;
	}
	return 1;
}

int main(int argc, char **argv) {
	printf("Welcome to BOGOL, the ultimate programming language!\n");
	char command[128];
	char **cword = (char**)malloc(sizeof(char *)*128);
	int num_words = 0;
	program_init();
	do {
		printf("> ");
		fgets(command, 100, stdin);
		command[strlen(command)-1] = '\0';
		cword = scan_words(command, cword, &num_words);
		if(0 == strcmp(cword[0], "quit")) {
		    program_save();
			printf("Good bye!\n");
		}
		else if (0 == strcmp(cword[0], "list")) {
			program_list();
		}
		else if (is_number(cword[0])) {
			int scan, num;
			sscanf(command, " %i%n", &scan, &num);
			if(command[num] == ' ') num++;
			printf("Store: '%s' at %i\n", &command[num], scan);
			program_set_line(scan, &command[num], cword, num_words);
		}
		else {
			int i;
		    printf("Unknown command: %s\n", command);
		    for(i = 0; i < num_words; i++) {
		    	printf("%i: '%s'\n", i, cword[i]);
		    }
		}
	} while(0 != strcmp(cword[0], "quit"));
    return 0;
}
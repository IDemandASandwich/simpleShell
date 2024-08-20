#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_split_line(char *line);

int main(int argc, char **argv){

    lsh_loop();

    return EXIT_SUCCESS;
}

void lsh_loop(void){
    char *line;
    char **args;
    int status;

    do{
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while(status);
}

#define LSH_RL_BUFSIZE 1024
char *lsh_read_line(void){
    char *line = NULL;
    ssize_t bufsize = 0;

    if(getline(&line, &bufsize, stdin) == -1){
        if(feof(stdin)){
            exit(EXIT_SUCCESS);
        }
        else{
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }
    
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line){
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    do{
        token = strtok(line, LSH_TOK_DELIM);
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }while(token != NULL);

    tokens[position] == NULL;
    return tokens;
}
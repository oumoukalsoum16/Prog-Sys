#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void execute(char *argv[]);

int main() {

    char cmd[255];
    char *commande;
    char *argvExec[255];
    char *saveptr1;
    char *saveptr2;
    int i;

    while (1) {
        printf("Minishell > ");
        fflush(stdout);

        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;

        cmd[strcspn(cmd, "\n")] = 0;

        if (strcmp(cmd, "quit") == 0)
            break;

      
        commande = strtok_r(cmd, ";", &saveptr1);

        while (commande != NULL) {

            i = 0;

           
            argvExec[i] = strtok_r(commande, " \t", &saveptr2);
            while (argvExec[i] != NULL) {
                i++;
                argvExec[i] = strtok_r(NULL, " \t", &saveptr2);
            }

            if (argvExec[0] != NULL)
                execute(argvExec);

            commande = strtok_r(NULL, ";", &saveptr1);
        }
    }

    return 0;
}

void execute(char *argv[]) {

    pid_t pidfils = fork();

    if (pidfils < 0) {
        perror("fork");
    }
    else if (pidfils == 0) {
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }
    else {
        wait(NULL);
    }
}

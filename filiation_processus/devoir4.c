#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <niveau_max>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int K = atoi(argv[1]);
    if (K < 0) {
        fprintf(stderr, "Le niveau doit être >= 0\n");
        exit(EXIT_FAILURE);
    }

    printf("Processus racine %d (niveau 0)\n", getpid());

    /* Calcul du nombre total de noeuds : 2^(K+1) - 1 */
    int taille_max = 1;
    for (int i = 0; i <= K; i++) {
        taille_max *= 2;
    }
    taille_max -= 1;

    pid_t *pids_niveau = malloc(sizeof(pid_t) * taille_max);
    if (!pids_niveau) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    pids_niveau[index++] = getpid();

    for (int niveau = 0; niveau < K; niveau++) {

        int nb_actuel = 1;
        for (int j = 0; j < niveau; j++)
            nb_actuel *= 2;

        int debut = index - nb_actuel;

        for (int i = 0; i < nb_actuel; i++) {

            pid_t pid_g = fork();
            if (pid_g == 0) {
                printf("Processus %d créé niveau %d (fils gauche de %d)\n",
                        getpid(), niveau + 1, pids_niveau[debut + i]);
                exit(0);
            }

            pid_t pid_d = fork();
            if (pid_d == 0) {
                printf("Processus %d créé niveau %d (fils droite de %d)\n",
                        getpid(), niveau + 1, pids_niveau[debut + i]);
                exit(0);
            }

            pids_niveau[index++] = pid_g;
            pids_niveau[index++] = pid_d;

            waitpid(pid_g, NULL, 0);
            waitpid(pid_d, NULL, 0);
        }
    }

    free(pids_niveau);
    return 0;
}

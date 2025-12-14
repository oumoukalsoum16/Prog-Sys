#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void usage() {
    printf("Usage :\n");
    printf("./matrix -c -d entier -f fichier [-b|-t]\n");
    printf("./matrix -a -d entier -f fichier [-b|-t]\n");
    exit(1);
}

int main(int argc, char * argv[]){
    int create = 0, affiche = 0;
    int dim = -1;
    int mode_binaire = 1;       // par défaut binaire
    char nom_fichier[255] = "";

    // lecture des options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            create = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            affiche = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            if (i + 1 < argc)
                dim = atoi(argv[++i]);
            else
                usage();
        } else if (strcmp(argv[i], "-b") == 0) {
            mode_binaire = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            mode_binaire = 0;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc)
                strcpy(nom_fichier, argv[++i]);
            else
                usage();
        } else {
            usage();
        }
    }

    // Vérification des options obligatoires
    if (dim <= 0 || nom_fichier[0] == '\0') {
        usage();
    }

    // Creation de la matrice
    int **mat = malloc(dim * sizeof(int*));
    for (int i = 0; i < dim; i++) {
        mat[i] = malloc(dim * sizeof(int));
    }

    // génération de valeurs aléatoires si demandé
    if (create) {
        srand(time(NULL));
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                mat[i][j] = rand() % 100; // valeurs 0-99
            }
        }
    }

    // sauvegarde
    if (mode_binaire) {
        FILE *f = fopen(nom_fichier, "wb");
        if (!f) {
            perror("Erreur ouverture fichier");
            exit(1);
        }
        fwrite(&dim, sizeof(int), 1, f);
        for (int i = 0; i < dim; i++)
            fwrite(mat[i], sizeof(int), dim, f);
        fclose(f);
    } else {
        FILE *f = fopen(nom_fichier, "w");
        if (!f) {
            perror("Erreur ouverture fichier");
            exit(1);
        }
        fprintf(f, "%d\n", dim);
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                fprintf(f, "%d ", mat[i][j]);
            }
            fprintf(f, "\n");
        }
        fclose(f);
    }

    printf("Matrice creee et sauvegardee dans %s\n", nom_fichier);

    // Affichage de la matrice si demandé
    if (affiche) {
        if (mode_binaire) {
            FILE *f = fopen(nom_fichier, "rb");
            if (!f) { 
                perror("Erreur ouverture fichier"); 
                exit(1); 
            }
            int d_file;
            fread(&d_file, sizeof(int), 1, f);
            if (d_file != dim) {
                printf("Erreur: dimension fournie (-d) differente du fichier.\n");
                exit(1);
            }
            for (int i = 0; i < dim; i++)
                fread(mat[i], sizeof(int), dim, f);
            fclose(f);
        } else {
            FILE *f = fopen(nom_fichier, "r");
            if (!f) { 
                perror("Erreur ouverture fichier"); 
                exit(1); 
            }
            int d_file;
            fscanf(f, "%d", &d_file);
            if (d_file != dim) {
                printf("Erreur: dimension fournie (-d) differente du fichier.\n");
                exit(1);
            }
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    fscanf(f, "%d", &mat[i][j]);
                }
            }
            fclose(f);
        }

        // affichage dans la console
        printf("Contenu du fichier %s :\n", nom_fichier);
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                printf("%3d ", mat[i][j]);
            }
            printf("\n");
        }
    }

    // libération de mémoire
    for (int i = 0; i < dim; i++) 
        free(mat[i]);
    free(mat);

    return 0;
}

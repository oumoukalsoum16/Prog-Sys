#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_NOM 256

// Structure pour contenir les informations de chaque fichier (nom et taille)

typedef struct {
    char nom[MAX_NOM];  
    int taille;         
} InfoFichier;

// Fonction pour créer l'archive

void creer_archive(int argc, char *argv[]) {
    FILE *archive = fopen(argv[1], "wb");  
    if (!archive) {
        perror("Erreur d'ouverture de l'archive");
        return;
    }


    int n_fichiers = argc - 2;  
    fwrite(&n_fichiers, sizeof(int), 1, archive);  

    for (int i = 2; i < argc; i++) {
        FILE *fichier = fopen(argv[i], "rb");  
        if (!fichier) {
            perror("Erreur d'ouverture du fichier");
            fclose(archive);
            return;
        }

        // Récupération des informations du fichier 

        InfoFichier info;
        strncpy(info.nom, argv[i], MAX_NOM);  
        fseek(fichier, 0, SEEK_END);  
        info.taille = ftell(fichier); 
        fseek(fichier, 0, SEEK_SET);  

        

        fwrite(&info, sizeof(InfoFichier), 1, archive);

        // Lecture et écriture du contenu du fichier dans l'archive

        char *buffer = (char *)malloc(info.taille);  
        fread(buffer, 1, info.taille, fichier);  
        fwrite(buffer, 1, info.taille, archive);  
        free(buffer);  
        fclose(fichier);  
    }

    fclose(archive);  
    printf("Archive créée avec succès.\n");
}

// Fonction pour extraire l'archive

void extraire_archive(char *nom_archive) {
    FILE *archive = fopen(nom_archive, "rb");  
    if (!archive) {
        perror("Erreur d'ouverture de l'archive");
        return;
    }

    int n_fichiers;
    fread(&n_fichiers, sizeof(int), 1, archive); 

    // Pour chaque fichier, lit les données et extrait son contenu

    for (int i = 0; i < n_fichiers; i++) {
        InfoFichier info;
        fread(&info, sizeof(InfoFichier), 1, archive);  

        FILE *fichier = fopen(info.nom, "wb");  
        if (!fichier) {
            perror("Erreur de création du fichier");
            fclose(archive);
            return;
        }

        // Lecture du contenu du fichier depuis l'archive et écriture dans le fichier restauré

        char *buffer = (char *)malloc(info.taille);  
        fread(buffer, 1, info.taille, archive);  
        fwrite(buffer, 1, info.taille, fichier); 
        free(buffer); 
        fclose(fichier);  
    }

    fclose(archive);  // Ferme l'archive
    printf("Archive extraite avec succès.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {

        fprintf(stderr, "Usage: %s archive.l3info fichier1 fichier2 ... \n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0) {
        
        if (argc != 3) {
            fprintf(stderr, "Usage: %s -e archive.l3info\n", argv[0]);
            return 1;
        }
        extraire_archive(argv[2]);
    } else {

        creer_archive(argc, argv);
    }

    return 0;
}

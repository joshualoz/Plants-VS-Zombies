#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define LONGUEUR_MAX 20

typedef struct tourelle {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int prix;
    int degats;             //permet d'indiquer combien de dégâts la tourelle inflige
    int use;                //utilisé par le sniper, qui tire un tour sur deux
    struct tourelle *next;
}Tourelle;

typedef struct etudiant {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int tour;
    int degats;             /*permet d'indiquer combien de dégâts l'etudiant inflige*/
    int score;              /*permet d'indiquer quel est le score gagné lorque l'on tue cet ennemi*/
    int use;                /*permet d'indiquer si un etudiant a déjà été avancé ou non*/
    struct etudiant *next;
    struct etudiant* next_line;
    struct etudiant* prev_line;
}Etudiant;

typedef struct utilisateur {                            /*Permet la sauvegarde de ces informations dans le fichier sauvegarde.txt*/
    char *pseudo;
    int score;
    int nb_etoiles;
} Utilisateur;

typedef struct {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
    Utilisateur* U;
}Jeu;


int budget_initial=0;          //permet de stocker la valeur de la cagotte initiale

int repere_charge=0;            //permet de repérer si le jeu est une partie chargée ou non. Si c'est le cas, on appelle tour_gen-charge et pas tour_gen

int repere_meilleurs_scores=0;  //permet de repérer si le score en cours fait partie des 10 meilleurs ou non. Si c'est le cas on va libérer la mémoire occupée par l'utilisateur dans la fonction meilleurs_scores donc on ne doit pas le faire dans libere_memoire_jeu.



/*
███████  ██████ ███████ ███    ██  █████  ██████  ██  ██████
██      ██      ██      ████   ██ ██   ██ ██   ██ ██ ██    ██
███████ ██      █████   ██ ██  ██ ███████ ██████  ██ ██    ██
     ██ ██      ██      ██  ██ ██ ██   ██ ██   ██ ██ ██    ██
███████  ██████ ███████ ██   ████ ██   ██ ██   ██ ██  ██████
*/

void activerCodesANSI() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hConsole, &mode)) {
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

void affichage_intro(void) {            //Affiche l'introduction
    //enableAnsiEscapeCodes();
    printf("\033[2J");     // Efface tout le terminal
    printf("\033[H");      // Replace le curseur en haut à gauche
    printf("\033[31m");
    printf( " _______  _______  _______  _______  _______  _              _______  _______  _______\n"
                  "(  ___  )(  ____ )(  ___  )(  ____ \\(  ___  )( \\   |\\     /|(  ____ )(  ____ \\(  ____ \\ \n"
                  "| (   ) || (    )|| (   ) || (    \\/| (   ) || (   ( \\   / )| (    )|| (    \\/| (    \\/ \n"
                  "| (___) || (____)|| |   | || |      | (___) || |    \\ (_) / | (____)|| (_____ | (__       \n"
                  "|  ___  ||  _____)| |   | || |      |  ___  || |     \\   /  |  _____)(_____  )|  __)      \n"
                  "| (   ) || (      | |   | || |      | (   ) || |      ) (   | (            ) || (         \n"
                  "| )   ( || )      | (___) || (____/\\| )   ( || (____/\\| |   | )      /\\____) || (____/\\   \n"
                  "|/     \\||/       (_______)(_______/|/     \\|(_______/\\_/   |/       \\_______)(_______/   \n");
    printf("\033[0m");
    sleep(2);
    printf("\nVeuillez appuyez sur Entrer pour poursuivre.\n");
    getchar();
    printf("\033[2J");
    printf("\033[H");
    printf("\033[31m");
    printf( " _______  _______  _______  _______  _______  _              _______  _______  _______\n"
                  "(  ___  )(  ____ )(  ___  )(  ____ \\(  ___  )( \\   |\\     /|(  ____ )(  ____ \\(  ____ \\ \n"
                  "| (   ) || (    )|| (   ) || (    \\/| (   ) || (   ( \\   / )| (    )|| (    \\/| (    \\/ \n"
                  "| (___) || (____)|| |   | || |      | (___) || |    \\ (_) / | (____)|| (_____ | (__       \n"
                  "|  ___  ||  _____)| |   | || |      |  ___  || |     \\   /  |  _____)(_____  )|  __)      \n"
                  "| (   ) || (      | |   | || |      | (   ) || |      ) (   | (            ) || (         \n"
                  "| )   ( || )      | (___) || (____/\\| )   ( || (____/\\| |   | )      /\\____) || (____/\\   \n"
                  "|/     \\||/       (_______)(_______/|/     \\|(_______/\\_/   |/       \\_______)(_______/   \n");
    printf("\033[0m");
    printf("\n");
    printf("\033[1;34mCree par Joshua Lozano et Clovis Rollet.\033[0m\n");
    getchar();             // la suite du code est activé si on rentre un caractère dans le terminal tel que "Entrer"
    printf("\033[2J");
    printf("\033[H");
    int n;
    do {
        printf("Voulez-vous lire l'introduction ? Tapez 1 pour oui et 0 pour non.\n");
        if (scanf("%d",&n)!=1) {
            printf("\nErreur de saisie.\n");
            exit(1);
        }
    } while (n!=1 && n!=0);
    if (n==0) {
        printf("\033[2J");
        printf("\033[H");
        return;
    }
    printf("\033[2J");
    printf("\033[H");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\033[1;35m                 ~~~~~~~~~~~~~~~\n");         // uniquement le dessin du vaisseau spatial a été généré par l'IA
    printf("               ~~~~~~~~~~~~~~~~~~~~\n");
    printf("             ~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("            ~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("            |                         |\n");
    printf("            |      _____     _____    |\n");
    printf("           /|\\    |     |   |     |   /|\\\n");
    printf("          /_|_\\   |_____|   |_____|  /_|_\\\n");
    printf("         |     |           o         |     |\n");
    printf("        /|     |                     |     |\\\n");
    printf("       /_|_____|_____________________|_____|_\\\n");
    printf("      |                                         |\n");
    printf("      |              LoopInfinie                |\n");
    printf("      |                                         |\n");
    printf("      |_________________________________________|\n");
    printf("        \\_____________________________________/ \n");
    printf("           \\_______________________________/ \n");
    printf("               /       /       \\       \\\n");
    printf("              /       /         \\       \\\n");
    printf("             '~~~~~~~'           '~~~~~~~'\033[0m\n\n\n");
    printf("\033[1;34mBienvenue en 2125 voyageur. Je sais qui tu es et c'est moi qui t'ai fait venir ici.\n\nAvant de te dire pourquoi laisse moi te presenter la situation.\n\nLe monde a bien change depuis 100 ans.\n\nTu te trouves ici dans un des derniers endroits sur terre ou l'Homme a survecu.\n\n");
    printf("Assoife par leur desir de richesse et de pouvoir,\nles grandes entreprises de ton temps ont tente de corrompre les hommes en propageant le virus SegFault-X.\n");
    getchar();
    printf("Ce dernier avait pour but de nous abrutir afin de nous pousser a une consommation toujours plus extreme.\n\nIl creait un manque insupportable, que les malades ne pouvaient combler qu'en succombant a un shopping effrene.\n\n");
    printf("Seulement, la nature est imprevisible et le virus a mute. Des symptomes semblables a la rage sont apparus.\n\nCeux qui avaient reussi a echapper au SegFault-X au debut,\nont commence a se faire attaque par les malades, les contaminant au passage.\n\n");
    printf("Heureusement, des communautes comme la notre ont reussi a echapper a cette grande pandemie zombie.\n\nNous nous faisons appeler la \"C-family\".\n\nNous sommes de grands codeurs au service des deux Dieux: Ken Thompson et Dennis Ritchie.\n\n");
    printf("Reclus sur notre vaisseau spatial le \"LoopInfinie\", nous vivons dans la peur constante de ...\033[0m\n\n");
    getchar();
    printf("\n");
    printf("(Des sirenes retentissent) \033[1;31mALERTE !\n\nA tous les membres de l'equipage, des zombies ont reussi a entrer sur le pont principal !\033[\n\n\n\n");
    printf("\033[1;34mMalheureusement, je n'ai pas le temps de finir. Si tu es ici c'est pour nous venir en aide.\n\nDesole, mais tu n'as pas le choix. Si les zombies atteignent notre moteur, nous allons tous mourir, toi y compris !\n\n");
    printf("Mais nous connaissons tes capacites et avons confiance en toi. Nous savons que tu es un dauphinois\nqui est venu a bout des 9 plus grandes epreuves de ton temps: les legendaires TP du professeur Emmanuel Lazard.\n\n");
    getchar();
    printf("Depechons-nous desormais.\n\nRepousse les zombies en l'honneur de Ken Thompson et Dennis Ritchie et nous te revelerons leur plus grand secret.\n");
    printf("Echoue et tu moureras.\n\n");
    printf("Bienvenue dans: l'APOCALYPSE !\033[0m\n\n");
    getchar();
    printf("\033[2J");
    printf("\033[H");
}



/*
███████  █████  ██    ██ ██    ██ ███████  ██████   █████  ██████  ██████  ███████     ███████ ████████      ██████ ██   ██  █████  ██████   ██████  ███████
██      ██   ██ ██    ██ ██    ██ ██      ██       ██   ██ ██   ██ ██   ██ ██          ██         ██        ██      ██   ██ ██   ██ ██   ██ ██       ██
███████ ███████ ██    ██ ██    ██ █████   ██   ███ ███████ ██████  ██   ██ █████       █████      ██        ██      ███████ ███████ ██████  ██   ███ █████
     ██ ██   ██ ██    ██  ██  ██  ██      ██    ██ ██   ██ ██   ██ ██   ██ ██          ██         ██        ██      ██   ██ ██   ██ ██   ██ ██    ██ ██
███████ ██   ██  ██████    ████   ███████  ██████  ██   ██ ██   ██ ██████  ███████     ███████    ██         ██████ ██   ██ ██   ██ ██   ██  ██████  ███████
*/

void sauvegarde(Jeu *j) {               /*stocke toutes les informations d'une partie dans un fichier texte de 6 lignes. 1e ligne: cagnotte initiale; 2e: tour et cagnotte; 3e: pseudo et score; 4e: liste chaînée des etudiants et leurs caractéristiques; 5e: ligne "---\n"; 6e: liste chaînée des tourelles et leurs caractéristiques*/
    printf("\033[2J");
    printf("\033[H");
    
    if (j==NULL) {printf("Aucune partie n'a ete trouvee.");return;}
    FILE *f = fopen("sauvegarde.txt", "w");
    if (f == NULL) {printf("Erreur de sauvegarde.");return;}


    fprintf(f,"%d\n",budget_initial);                   //stocke la valeur du budget initial
    fprintf(f,"%d %d\n",j->tour,j->cagnotte);           //stocke le numéro du tour et la cagnotte restante
    fprintf(f,"%s %d\n",j->U->pseudo,j->U->score);      //stocke le pseudo de l'utilisateur et son score


    Etudiant *etudiant = j->etudiants;
    while (etudiant!=NULL) {                    //stocke les caractéristiques de chaque élément de la liste chaînée des étudiants
        fprintf(f,"%c %d %d %d %d %d %d %d %d ",etudiant->type, etudiant->pointsDeVie,etudiant->ligne,etudiant->position,etudiant->vitesse,etudiant->tour,etudiant->degats,etudiant->score,etudiant->use);
        etudiant=etudiant->next;
    }
    fprintf(f,"\n");


    fprintf(f,"---\n");


    Tourelle *tourelle = j->tourelles;
    while (tourelle!=NULL) {                    //stocke les caractéristiques de chaque élément de la liste chaînée des tourelles
        fprintf(f,"%d %d %d %d %d %d ",tourelle->type,tourelle->pointsDeVie,tourelle->ligne,tourelle->position,tourelle->prix,tourelle->degats);
        tourelle=tourelle->next;
    }


    free(j->U->pseudo);            /*La mémoire est également libérée*/
    free(j->U);
    etudiant = j->etudiants;
    while (etudiant!=NULL) {
        Etudiant *etudiant2=etudiant;
        etudiant=etudiant->next;
        free(etudiant2);
    }
    tourelle = j->tourelles;
    while (tourelle!=NULL) {
        Tourelle* tourelle2 = tourelle;
        tourelle=tourelle->next;
        free(tourelle2);
    }
    free(j);
    fclose(f);


    printf("La partie a bien ete sauvegardee.\n\n");
    printf("\033[1;34mAlors comme ca tu as besoin d'une pause voyageur. Depeche toi, nous allons essayez de retenir les zombies pendant ton absence.\n\n\033[0m\n");
}


Jeu *charger(void) {        /*charge la partie précedemment sauvegardée à partir du fichier texte créé*/
    Jeu *j = malloc(sizeof(Jeu));
    if (j==NULL) {printf("Erreur d'allocation.");return NULL;}
    FILE *f = fopen("sauvegarde.txt", "r");
    if (f == NULL) {printf("Erreur de chargement de la dernière sauvegarde/elle n'existe pas.");return NULL;}


    fscanf(f,"%d\n",&budget_initial);                           //récupère la cagnotte initiale
    fscanf(f,"%d %d\n",&j->tour,&j->cagnotte);                  //récupère le numéro du tour et la cagnotte restante


    j->U=malloc(sizeof(Utilisateur));
    if (j->U==NULL) {printf("Erreur d'allocation.");return NULL;}
    j->U->pseudo = malloc(21 * sizeof(char));
    if (j->U->pseudo == NULL) {
        printf("Erreur d'allocation.\n");
        return NULL;
    }
    fscanf(f, "%20s %d\n", j->U->pseudo, &j->U->score);        //récupère le pseudo de l'utilisateur et son score


    j->etudiants=malloc(sizeof(Etudiant));
    if (j->etudiants==NULL) {printf("Erreur d'allocation.");return NULL;}
    Etudiant *etudiant=j->etudiants;
    Etudiant *etudiant2=NULL;
    char tempChar;
    int m=fscanf(f,"%c %d %d %d %d %d %d %d %d ",&tempChar, &etudiant->pointsDeVie,&etudiant->ligne,&etudiant->position,&etudiant->vitesse,&etudiant->tour,&etudiant->degats,&etudiant->score,&etudiant->use);
    etudiant->type=tempChar;
    while (m==9) {                                                     //récupère les caractéristiques de chaque élément de la liste chaînée des étudiants
        etudiant2=etudiant;
        etudiant->next_line=etudiant->prev_line=NULL;
        etudiant=malloc(sizeof(Etudiant));
        if (etudiant==NULL) {printf("Erreur d'allocation.");return NULL;}
        etudiant2->next=etudiant;
        char tempChar;
        m=fscanf(f,"%c %d %d %d %d %d %d %d %d ",&tempChar, &etudiant->pointsDeVie,&etudiant->ligne,&etudiant->position,&etudiant->vitesse,&etudiant->tour,&etudiant->degats,&etudiant->score,&etudiant->use);
        etudiant->type=tempChar;
    }
    free(etudiant);                    //on a alloué un espace mémoire en trop donc on le libère
    if (etudiant2!=NULL)
        etudiant2->next=NULL;
    else
        j->etudiants=NULL;


    Etudiant *etudiant3 = j->etudiants;                             /*DIFFICULTE: chainer les etudiants avec next_line et prev_line*/
    while (etudiant3!=NULL) {                                       //on chaîne les étudiants avec next_line et prev_line
        int min=100;
        Etudiant *etudiant4=j->etudiants;
        while (etudiant4!=NULL) {
            if (etudiant4->ligne==etudiant3->ligne && etudiant4!=etudiant3) {
                if (etudiant4->tour>etudiant3->tour && etudiant4->tour<etudiant3->tour+min) {
                    min=etudiant4->tour-etudiant3->tour;
                    etudiant3->prev_line=etudiant4;
                    etudiant4->next_line=etudiant3;
                }
            }
            etudiant4=etudiant4->next;
        }
        etudiant3=etudiant3->next;
    }


    j->tourelles=malloc(sizeof(Tourelle));                      /*DIFFICULTE: comprendre la fonction fscanf*/
    if (j->tourelles==NULL) {printf("Erreur d'allocation.");return NULL;}
    Tourelle *tourelle=j->tourelles;
    Tourelle *tourelle2=NULL;
    fscanf(f,"---\n");
    int n=fscanf(f,"%d %d %d %d %d %d ",&tourelle->type,&tourelle->pointsDeVie,&tourelle->ligne,&tourelle->position,&tourelle->prix,&tourelle->degats);
    while (n==6) {                                      //récupère les caractéristiques de chaque élément de la liste chaînée des tourelles
        tourelle2=tourelle;
        tourelle=malloc(sizeof(Tourelle));
        if (tourelle==NULL) {printf("Erreur d'allocation.");return NULL;}
        tourelle2->next=tourelle;
        n=fscanf(f,"%d %d %d %d %d %d ",&tourelle->type,&tourelle->pointsDeVie,&tourelle->ligne,&tourelle->position,&tourelle->prix,&tourelle->degats);
    }
    free(tourelle);
    if (tourelle2!=NULL)
        tourelle2->next=NULL;
    else
        j->tourelles=NULL;
    fclose(f);


    printf("\033[1;34mTe revoila a point nomme ! Au combat voyageur !\033[0m\n\n");
    sleep(2);
    return j;
    }


/*
████████ ██    ██ ██████  ███████ ███████     ███████  ██████  ███    ███ ██████  ██ ███████ ███████
   ██     ██  ██  ██   ██ ██      ██             ███  ██    ██ ████  ████ ██   ██ ██ ██      ██
   ██      ████   ██████  █████   ███████       ███   ██    ██ ██ ████ ██ ██████  ██ █████   ███████
   ██       ██    ██      ██           ██      ███    ██    ██ ██  ██  ██ ██   ██ ██ ██           ██
   ██       ██    ██      ███████ ███████     ███████  ██████  ██      ██ ██████  ██ ███████ ███████
*/

void type_orateur(Etudiant *E) { //zombie qui ameliore de 1 point les degats du zombie devant lui
    if ((char) E->type=='O') {
        E->pointsDeVie=3;
        E->vitesse = 1;
        E->degats=1;
        E->score=10;
    }
}

void type_docteur(Etudiant *E) {    //zombie qui soigne le zombie devant lui en lui ajoutant 1 point de vie
    if ((char) E->type=='D') {
        E->pointsDeVie=2;
        E->vitesse = 1;
        E->degats=1;
        E->score=8;
    }
}

void type_geant(Etudiant *E) {      //zombie lent mais avec beaucoup de points de vie
    if ((char) E->type=='G') {
        E->pointsDeVie=10;
        E->vitesse = 1;
        E->degats=3;
        E->score=15;
    }
}

void type_athlete(Etudiant *S) {        //zombie rapide capable de sauter au dessus des autres zombies
    if (((char) S->type)=='A') {
        S->pointsDeVie = 3;
        S->vitesse=2;
        S->degats=2;
        S->score=10;
    }
}



void type_Z(Etudiant* etudiant) {                //affecte automatiquement les bons caractéristiques à un ennemi de type Z
    if (((char) etudiant->type)=='Z') {
        etudiant->pointsDeVie =5;
        etudiant->vitesse=1;
        etudiant->degats=1;
        etudiant->score=5;
    }
}

void type_etudiant(Etudiant* etudiant) {        //affecte automatiquement les bons caractéristiques à un ennemi en fonction de son type
    type_Z(etudiant);
    type_athlete(etudiant);
    type_geant(etudiant);
    type_orateur(etudiant);
    type_docteur(etudiant);
}


/*
███████ ██    ██ ███████ ████████ ███████ ███    ███ ███████     ██████  ███████     ███████  ██████  ██████  ██████  ███████
██       ██  ██  ██         ██    ██      ████  ████ ██          ██   ██ ██          ██      ██      ██    ██ ██   ██ ██
███████   ████   ███████    ██    █████   ██ ████ ██ █████       ██   ██ █████       ███████ ██      ██    ██ ██████  █████
     ██    ██         ██    ██    ██      ██  ██  ██ ██          ██   ██ ██               ██ ██      ██    ██ ██   ██ ██
███████    ██    ███████    ██    ███████ ██      ██ ███████     ██████  ███████     ███████  ██████  ██████  ██   ██ ███████
*/

//Difficulté : gérer lorsque l'utilisateur a 0 étoile
void meilleurs_scores(Utilisateur *U) {       /*Créé un fichier texte meilleur_scores.txt en lisant celui précédemment créé (s'il existe) pour afficher le score de la partie en cours s'il fait partie du top 10*/
    FILE* f = fopen("meilleurs_scores.txt", "r");


    Utilisateur** TAB=malloc(11*sizeof(Utilisateur*));                  //on utilise un tableau de pointeur sur le type Utlisateur
    if (TAB==NULL) {
        printf("Erreur d'allocation.");
        return;
    }
    for (int i = 0; i < 11; i++) {
        TAB[i] = malloc(sizeof(Utilisateur));
        if (TAB[i] == NULL) {
            printf("Erreur d'allocation.");
            return;
        }
        TAB[i]->pseudo = malloc(21 * sizeof(char));  // Allocation du pseudo
        if (!TAB[i]->pseudo) {
            printf("Erreur d'allocation mémoire pour le pseudo.\n");
            fclose(f);
            return;
        }
    }


    char str[4];
    int i=0;
    while (fscanf(f, "%s", TAB[i]->pseudo) != EOF) {

        fscanf(f, "%d", &TAB[i]->score );                           //chaque élément du tableau récupère les caractéristiques inscrits dans les lignes de meilleurs_scores.txt
        fscanf(f, " %s", str);
        int k;
        for (k=0;str[k]=='*';k++) {}
        TAB[i]->nb_etoiles=k;
        i++;
    }                                                                    //en sortie i vaut le nombre d'utilisateurs qui étaient déjà présents sur le fichier


    int k=0;
    while (k<i && U->score<=TAB[k]->score) {                //on regarde si le nouveau score fait partie des 10 meilleurs
        k++;
    }


    if (k==10) {
        sleep(2);
        printf("\033[2J");
        printf("\033[H");
        printf("Votre score n'est pas parmi les 10 meilleurs scores. Peut-etre une prochaine fois ...\n\n");
        repere_meilleurs_scores=1;
        fclose(f);
        for (int i = 0; i < 11; i++) {
            free(TAB[i]->pseudo);
            free(TAB[i]);
        }
        free(TAB);
        return;                                             //si ce n'est pas le cas on ne touche pas au fichier
    }


    if (k==i){
        free(TAB[k]->pseudo);
        free(TAB[k]);
        TAB[k]=U;
    }                                    //si c'est le cas on insère le pseudo, le score et le nombre d'étoiles de l'utilisateur à la bonne ligne en décalant tous les autres lignes si nécessaire

    else {
        Utilisateur *retient1=TAB[k];
        TAB[k]=U;
        while (k<i) {
            Utilisateur *retient2=TAB[k+1];
            TAB[k+1]=retient1;
            retient1=retient2;
            k++;
        }
        free(retient1->pseudo);
        free(retient1);
    }
    fclose(f);


    f=fopen("meilleurs_scores.txt", "w");

    if (i<10) {                                                         //2 cas possibles: soit il y a moins de 10 scores déjà inscrits

        for (int k=0;k<i;k++) {
            fprintf(f,"%s %d ",TAB[k]->pseudo,TAB[k]->score);

            if (TAB[k]->nb_etoiles==0) {
                fprintf(f,"_\n");
            }

            else {
                for (int l=0;l<TAB[k]->nb_etoiles;l++) {
                    fprintf(f,"*");
                }
                fprintf(f,"\n");
            }
        }

        fprintf(f,"%s %d ",TAB[k]->pseudo,TAB[k]->score);
        if (TAB[k]->nb_etoiles==0) {
            fprintf(f,"_");
        }

        else {
            for (int l=0;l<TAB[k]->nb_etoiles;l++) {
                fprintf(f,"*");
            }
        }
    }


    else {                                                              //soit il y en a déjà 10. Dans ce dernier cas, la dernière ligne du fichier meilleurs_score.txt va être supprimée

        for (int k=0;k<i-1;k++) {
            fprintf(f,"%s %d ",TAB[k]->pseudo,TAB[k]->score);
            if (TAB[k]->nb_etoiles==0) {
                fprintf(f,"_\n");
            }

            else {
                for (int l=0;l<TAB[k]->nb_etoiles;l++) {
                    fprintf(f,"*");
                }
                fprintf(f,"\n");
            }
        }

        fprintf(f,"%s %d ",TAB[k]->pseudo,TAB[k]->score);

        if (TAB[k]->nb_etoiles==0) {
            fprintf(f,"_");
        }

        else {
            for (int l=0;l<TAB[k]->nb_etoiles;l++) {
                fprintf(f,"*");
            }
        }
    }

    for (int i = 0; i < 11; i++) {             //on libère enfin la mémoire utilisée par le tableau
        free(TAB[i]->pseudo);
        free(TAB[i]);
    }
    free(TAB);
    fclose(f);
    sleep(2);
    printf("\033[2J");
    printf("\033[H");
    printf("Votre score fait partie des 10 meilleurs !\n\n");
}

int win(Etudiant *E) {
    return E==NULL;
}
int loose(Etudiant *E) {
    while (E) {
        if (E->position<=1) {
            return 1;
        }
        E=E->next;
    }
    return 0;
}


void calcul_nb_etoiles_et_score_final(Jeu* j) {        //permet le calcul et l'affichage du nombre d'étoiles gagnées et du score de l'utilisateur en fonction de sa victoire/défaite, du montant dépensé pour construire les tourelles et du temps écoulé pour tuer tous les ennemis
    if (win(j->etudiants)) {

        j->U->nb_etoiles=1;
        int retient=j->U->score;
        j->U->score/=4;                                 //le score est divisé par 4 si plus de 95% du budget initial a été depensé et l'utilisateur obtient 1 étoile

        if (j->cagnotte>=budget_initial/20){
            j->U->nb_etoiles=2;
            j->U->score=retient/2;                      //le score est divisé par 2 si plus 90% du budget initial (mais moins de 95%) a été depensé et l'utilisateur obtient 2 étoiles
        }

        if (j->cagnotte>=budget_initial/10){             //le score reste le même si moins de 90% du budget initial a été dépensé et l'utilisateur obtient 3 étoiles
            j->U->nb_etoiles=3;
            j->U->score=retient;
        }

        if (j->tour<20) {                               //le score est multiplié par 4 si l'utilisateur tue tous les zombies en moins de 19 tours
            j->U->score*=4;
        }

        else if (j->tour<25) {                          //le score est multiplié par 2 si l'utilisateur tue tous les zombies en moins de 21 tours
            j->U->score*=2;
        }
    }


    else {
        j->U->score/=10;                                //si l'utilisateur a perdu, il obtient 0 étoile et son score est divisé par 10
        j->U->nb_etoiles=0;
    }

    sleep(2);
    printf("\033[2J");
    printf("\033[H");
    printf("Vous avez obtenu un score de %d et %d etoiles.\n\n",j->U->score,j->U->nb_etoiles);
}

void affichage_score(void) {
    FILE* f=fopen("meilleurs_scores.txt","r");
    if (f==NULL) {
        fclose(f);
        return;
    }
    int n;
    do {
        printf("\nSouhaitez-vous voir les meilleurs scores ? Tapez 1 pour oui et tapez 0 pour non.");
        if (scanf("%d",&n)!=1) {
            printf("\nErreur de saisie.\n");
            exit(1);
        }
    }while (n!=0 && n!=1);

    if (n) {
        char* pseudo=malloc(21*sizeof(char));
        int score;
        char* etoile=malloc(4*sizeof(char));
        while (fscanf(f, "%s %d %s\n",pseudo,&score,etoile)!=EOF) {
            printf("%s %d %s\n",pseudo,score,etoile);
        }
        free(pseudo);
        free(etoile);
        fclose(f);
        sleep(2);
    }
    else {
        fclose(f);
        return;
    }
}

/*
███████  ██████  ███    ██  ██████ ████████ ██  ██████  ███    ██     ██████  ██ ██    ██ ███████ ██████  ███████ ███████ ███████
██      ██    ██ ████   ██ ██         ██    ██ ██    ██ ████   ██     ██   ██ ██ ██    ██ ██      ██   ██ ██      ██      ██
█████   ██    ██ ██ ██  ██ ██         ██    ██ ██    ██ ██ ██  ██     ██   ██ ██ ██    ██ █████   ██████  ███████ █████   ███████
██      ██    ██ ██  ██ ██ ██         ██    ██ ██    ██ ██  ██ ██     ██   ██ ██  ██  ██  ██      ██   ██      ██ ██           ██
██       ██████  ██   ████  ██████    ██    ██  ██████  ██   ████     ██████  ██   ████   ███████ ██   ██ ███████ ███████ ███████
*/

void affichage_zombies(void) {            //permet d'expliquer les caractéristiques de chaque zombie
    int i;
    printf("\n\033[1;34mGrace a nos multiples expeditions dans les zones infectees, nous avons pu recolter de nombreuses informations sur les zombies.\n");
    do {
        printf("\033[1;34mVeux-tu que nous te les enseignons ?\033[0m Tape 1 pour oui et 0 pour non.\n");
        if (scanf("%d",&i)!=1) {
            printf("\nErreur de saisie.\n");
            exit(1);
        }
    } while (i!=0 && i!=1);
    if (i==1) {
        printf("\n");
        printf("\033[1;33m");
        printf("Six differents types de zombies existent : le zombie de base (Z), le docteur (D), le geant (G), l'orateur (O) et l'athlete (A).\n");
        printf("Zombie de base  |  Docteur  |   Geant   |  Orateur  |  Athlete  |\n");
        printf("vitesse:1       | vitesse:1 | vitesse:1 | vitesse:1 | vitesse:2 |\n");
        printf("degats:1        | degats:1  | degats:3  | degats:1  | degats:2  |\n");
        printf("vie:5           |   vie:2   |   vie:10  |   vie:3   |   vie:3   |\n");
        printf("type:'Z'        |  type:'D' |  type:'G' | type:'O'  |  type:'A' |\n\n");
        printf("Description des pouvoirs des zombies:\n");
        printf("    - Docteur: zombie qui augmente les points de vie du zombie devant lui.\n");
        printf("    - Geant: zombie lent mais ultra-resistant et infligeant de tres gros degats.\n");
        printf("    - Orateur: zombie qui ameliore les degats du zombie devant lui.\n");
        printf("    - Athlete: zombie rapide infligeant de gros degats.\033[0m\n\n");
    }
}

void affichage_vagues(Etudiant*** T) {                  /*permet l'affichage initiale des vagues à venir à partir du double tableau créé dans la fonction creation_jeu*/
    printf("Vagues a venir:\n");
    for (int i=0;i<7;i++) {
        printf("%d| ",i+1);
        for (int k=14;k>=0;k--) {
            if (T[i][k]!=NULL) {
                printf("\033[1;31m %c \033[0m",T[i][k]->type);
            }
            else {
                printf(" . ");
            }
        }
        printf("\n");
    }
}

void affichage_outro(Jeu *j){
    sleep(2);
    printf("\033[2J");
    printf("\033[H");
    if (win(j->etudiants)) {
        printf("\033[1;34mBravo voyageur, tu nous a tous sauve.\n\n");
        printf("Comme promis, je vais te reveler le grand precepte secret de notre communaute:\n\n\033[0m\033[1;36mEn C, on manipule la memoire comme des maitres forgerons,tandis qu'en Python, on attend que l'interpreteur nous mache le travail comme des apprentis paresseux.\n\n\033[0m");
        printf("\033[1;34mAdieu voyageur et merci encore. Tu a echappe a l'\n\n\033[0m");
        printf("\033[31m");
    printf( " _______  _______  _______  _______  _______  _              _______  _______  _______\n"
                  "(  ___  )(  ____ )(  ___  )(  ____ \\(  ___  )( \\   |\\     /|(  ____ )(  ____ \\(  ____ \\ \n"
                  "| (   ) || (    )|| (   ) || (    \\/| (   ) || (   ( \\   / )| (    )|| (    \\/| (    \\/ \n"
                  "| (___) || (____)|| |   | || |      | (___) || |    \\ (_) / | (____)|| (_____ | (__       \n"
                  "|  ___  ||  _____)| |   | || |      |  ___  || |     \\   /  |  _____)(_____  )|  __)      \n"
                  "| (   ) || (      | |   | || |      | (   ) || |      ) (   | (            ) || (         \n"
                  "| )   ( || )      | (___) || (____/\\| )   ( || (____/\\| |   | )      /\\____) || (____/\\   \n"
                  "|/     \\||/       (_______)(_______/|/     \\|(_______/\\_/   |/       \\_______)(_______/   \n");
    printf("\033[0m");
    printf("\n");
    }
    else{
        printf("\033[1;34mTu as echoue voyageur. Tu es condamne a devenir un zombie et a errer pour l'eternite dans l'\n\n\033[0m");
        printf("\033[31m");
    printf( " _______  _______  _______  _______  _______  _              _______  _______  _______\n"
                  "(  ___  )(  ____ )(  ___  )(  ____ \\(  ___  )( \\   |\\     /|(  ____ )(  ____ \\(  ____ \\ \n"
                  "| (   ) || (    )|| (   ) || (    \\/| (   ) || (   ( \\   / )| (    )|| (    \\/| (    \\/ \n"
                  "| (___) || (____)|| |   | || |      | (___) || |    \\ (_) / | (____)|| (_____ | (__       \n"
                  "|  ___  ||  _____)| |   | || |      |  ___  || |     \\   /  |  _____)(_____  )|  __)      \n"
                  "| (   ) || (      | |   | || |      | (   ) || |      ) (   | (            ) || (         \n"
                  "| )   ( || )      | (___) || (____/\\| )   ( || (____/\\| |   | )      /\\____) || (____/\\   \n"
                  "|/     \\||/       (_______)(_______/|/     \\|(_______/\\_/   |/       \\_______)(_______/   \n");
    printf("\033[0m");
    printf("\n");
    }
}

int est_pseudo_valide(const char *pseudo) {        //permet de vérifier si le pseudo entré ne possède pas d'espaces, fait moins de 20 caractères et ne commence pas par un chiffre.
    int longueur = strlen(pseudo);

    if (longueur > LONGUEUR_MAX) {        //on vérifie si la longueur du pseudo est inférieur à 20 caractères
        printf("Erreur : le pseudo ne doit pas depasser 20 caracteres.\n");
        return 0;
    }

    if (isdigit(pseudo[0])) {                //on vérifie si le premier caractère est un chiffre
        printf("Erreur : le pseudo ne doit pas commencer par un chiffre.\n");
        return 0;
    }

    for (int i = 0; i < longueur; i++) {        //on vérifie s'il y a un espace dans le pseudo
        if (isspace(pseudo[i])) {
            printf("Erreur : le pseudo ne doit pas contenir d'espace.\n");
            return 0;
        }
    }

    return 1;
}


void vider_buffer() {        //permet de vider le buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
██████  ███████  ██████ ██    ██ ██████  ███████ ██████   █████  ████████ ██  ██████  ███    ██     ██████   █████  ██████  ████████ ██ ███████ ███████
██   ██ ██      ██      ██    ██ ██   ██ ██      ██   ██ ██   ██    ██    ██ ██    ██ ████   ██     ██   ██ ██   ██ ██   ██    ██    ██ ██      ██
██████  █████   ██      ██    ██ ██████  █████   ██████  ███████    ██    ██ ██    ██ ██ ██  ██     ██████  ███████ ██████     ██    ██ █████   ███████
██   ██ ██      ██      ██    ██ ██      ██      ██   ██ ██   ██    ██    ██ ██    ██ ██  ██ ██     ██      ██   ██ ██   ██    ██    ██ ██           ██
██   ██ ███████  ██████  ██████  ██      ███████ ██   ██ ██   ██    ██    ██  ██████  ██   ████     ██      ██   ██ ██   ██    ██    ██ ███████ ███████
*/

Jeu *creation_jeu(void) {                          /*créé une partie, affiche la cagnotte disponible et toutes les vagues, et retourne le jeu complètement initialisé (la liste chaînée des tourelles est initialisée à NULL)*/
    int n;
    FILE *f=NULL;
    do {                    //donne le choix à l'utilisateur de jouer aux parties prédéfinies ou de jouer à une partie sauvegardée
        printf("Vous avez le choix entre 6 parties de niveaux de difficulte croissant (allant de 1 a 6)\nou de charger une partie sauvegardee.\nEntrez un nombre de 1 a 6 pour les parties predefinies ou taper 7 pour charger votre ancienne partie.\n");
        if (scanf("%d",&n)!=1) {
            printf("\nErreur de saisie.\n");
            exit(1);
        }
        getchar();
    }  while (n>7 || n<1);
    if (n<=6) {
        if (n==1) {
            FILE* g = fopen("partie.txt", "r");
            if (g == NULL) {printf("La partie n'a pas ete trouve.");return NULL;}
            f=g;
        }
        if (n==2) {
            FILE* g = fopen("partie2.txt", "r");
            if (g == NULL) {printf("La partie n'a pas ete trouve.");return NULL;}
            f=g;
        }
        if (n==3) {
            FILE* g = fopen("partie3.txt", "r");
            if (g == NULL) {printf("La partie n'a pas ete trouve.");return NULL;}
            f=g;
        }
        if (n==4) {
            FILE* g = fopen("partie4.txt", "r");
            if (g == NULL) {printf("La partie n'a pas ete trouve.");return NULL;}
            f=g;
        }
        if (n==5) {
            FILE* g = fopen("partie5.txt", "r");
            if (g == NULL) {printf("La partie n'a pas ete trouve.");return NULL;}
            f=g;
        }
        if (n==6) {
            FILE* g = fopen("partie6.txt", "r");
            if (g == NULL) {printf("La partie n'a pas ete trouve.");return NULL;}
            f=g;
        }
    }


    if (n==7) {                     //si on charge une ancienne partie, la fonction charger() nous renvoie dircetment le bon pointeur sur la structure Jeu
        Jeu *j=charger();
        repere_charge=1;
        return j;
    }


    repere_charge=0;
    Jeu *j = malloc(sizeof(Jeu));               //sinon on doit initialiser ce pointeur à partir d'un fichier texte
    if (j == NULL) {printf("Erreur d'allocation");return NULL;}


    int budget;
    if (fscanf(f, "%d", &budget)!=1) {              //on récupère la cagnotte inscrit sur la première ligne
        printf("La partie lue n'est pas valide.");
        exit(1);
    }
    if (budget<50) {
        printf("La partie lue n'est pas valide.");
        exit(1);
    }
    budget_initial=j->cagnotte = budget;
    j->tour=0;


    j->etudiants=malloc(sizeof(Etudiant));
    if (j->etudiants==NULL){printf("Erreur d'allocation"); return NULL;}
    int x=fscanf(f,"%d",&j->etudiants->tour);
    int retient_tour=j->etudiants->tour;
    if (j->etudiants->tour<=0 || j->etudiants->tour>14) {
        printf("La partie lue n'est pas valide.");
        exit(1);
    }
    if (x==1){                                          /*initialise le premier etudiant de la liste chaînée*/

        if (fscanf(f,"%d",&j->etudiants->ligne)!=1) {
            printf("La partie lue n'est pas valide.");
            exit(1);
        }
        if (j->etudiants->ligne<=0 || j->etudiants->ligne>7) {
            printf("La partie lue n'est pas valide.");
            exit(1);
        }
        char tempChar;  // Variable temporaire pour stocker le caractère
        if (fscanf(f," %c\n",&tempChar)!=1) {
            printf("La partie lue n'est pas valide.");
            exit(1);
        }
        if (tempChar!='Z' && tempChar!='A' && tempChar!='G' && tempChar!='O' && tempChar!='D') {
            printf("La partie lue n'est pas valide.");
            exit(1);
        }
        j->etudiants->type=tempChar;
        j->etudiants->next=NULL;
        j->etudiants->next_line=NULL;
        j->etudiants->prev_line=NULL;
        j->etudiants->use=0;
        type_etudiant(j->etudiants);
        j->etudiants->position=1;
        Etudiant *etudiant1=j->etudiants;


        Etudiant*** T=malloc(7*sizeof(Etudiant**));   //on utilise un tableau à 2 dimensions de pointeurs sur le type Etudiant
        for (int i = 0; i < 7; i++) {
            T[i] = malloc(15 * sizeof(Etudiant*));
            for (int k = 0; k < 15; k++) {
                T[i][k] = NULL;
            }
        }


        int var;
        T[(j->etudiants->ligne)-1][(j->etudiants->position)-1]=j->etudiants;
        while (fscanf(f,"%d",&var)==1) {
            if (var<=0 || var>14 || var<retient_tour) {
                if (fscanf(f," %c\n",&tempChar)!=1) {
                    printf("La partie lue n'est pas valide.");
                    exit(1);
                }
                if (tempChar!='Z' && tempChar!='A' && tempChar!='G' && tempChar!='O' && tempChar!='D') {
                    printf("La partie lue n'est pas valide.");
                    exit(1);
                }
            }
            Etudiant *etudiant2=malloc(sizeof(Etudiant));                       /*creation de la liste chaînée des étudiants*/
            if (etudiant2==NULL){printf("Erreur d'allocation");return NULL;}
            etudiant2->tour=var;
            etudiant1->next=etudiant2;
            etudiant2->next=NULL;
            if (fscanf(f,"%d",&etudiant2->ligne)!=1) {
                printf("La partie lue n'est pas valide.");
                exit(1);
            }
            if (etudiant2->ligne<=0 || etudiant2->ligne>7) {
                printf("La partie lue n'est pas valide.");
                exit(1);
            }
            char tempChar;  //variable temporaire pour stocker le caractère
            if (fscanf(f," %c\n",&tempChar)!=1) {
                printf("La partie lue n'est pas valide.");
                exit(1);
            }
            if (tempChar!='Z' && tempChar!='A' && tempChar!='G' && tempChar!='O' && tempChar!='D') {
                printf("La partie lue n'est pas valide.");
                exit(1);
            }
            etudiant2->type = tempChar;  //permet de convertir le caractère en entier
            type_etudiant(etudiant2);
            etudiant2->use=0;
            etudiant2->position=1;


            if (etudiant2->tour!=retient_tour) {        //on regarde si le nouvel élément apparaît au même tour que les éléments précédent
                retient_tour=etudiant2->tour;           //si ce n'est pas le cas, on décale tous les anciens éléments du bon nombre de colonnes vers la gauche dans le tableau T et retient_tour vaut le tour du nouvel élément
                for (int i=0;i<7;i++) {                 //si c'est le cas, on ne fait rien
                    for (int k=14;k>=0;k--) {
                        if (T[i][k]!=NULL) {
                            T[i][k]->position=T[i][k]->vitesse+(etudiant2->tour-T[i][k]->tour);
                            T[i][T[i][k]->position-1]=T[i][k];
                            T[i][k]=NULL;
                        }
                    }
                }
            }


            T[(etudiant2->ligne) -1][(etudiant2->position)-1]=etudiant2;    //on fait apparaître le nouvel élément dans le tableau
            etudiant1=etudiant2;
        }


        for (int i=0;i<7;i++) {                 //on chaîne les éléments de la liste chaînée des étudiants par ligne grâce à next_line et prev_line, et à l'aide du tableau
            for (int k=0;k<15;k++) {
                if (T[i][k]!=NULL) {
                    int l=k-1;
                    while (l>=0 && T[i][l]==NULL) {
                        l--;
                    }
                    if (l>=0) {
                        T[i][k]->prev_line=T[i][l];
                    }
                    else {
                        T[i][k]->prev_line=NULL;
                    }
                    l=k+1;
                    while (l<15 && T[i][l]==NULL) {
                        l++;
                    }
                    if (l<15) {
                        T[i][k]->next_line=T[i][l];
                    }
                    else {
                        (T[i][k])->next_line=NULL;
                    }
                }
            }
        }

        j->tourelles=NULL;


        j->U = malloc(sizeof(Utilisateur));   //on initialise les informations de l'utilisateur
        j->U->score=0;
        char pseudo[LONGUEUR_MAX + 2]; //+2 pour gérer '\n' et '\0'
        do {
            printf("Entrez votre pseudo (avec au maximum 20 caracteres, sans espace et ne commencant pas par un chiffre) : ");
            fgets(pseudo, sizeof(pseudo), stdin);
            if (pseudo[strlen(pseudo) - 1] != '\n') {    //on vérifie si l'entrée a dépassé la taille du buffer et vide le reste
                vider_buffer();
                continue;
            }
            pseudo[strcspn(pseudo, "\n")] = '\0';    //on supprime le retour à la ligne ajouté par fgets
        } while (!est_pseudo_valide(pseudo));
        j->U->pseudo = malloc((strlen(pseudo) + 1) * sizeof(char)); // +1 pour le '\0'
        if (j->U->pseudo == NULL) {
            printf("Erreur d'allocation.\n");
            return NULL;
        }
        strcpy(j->U->pseudo, pseudo);


        printf("\033[2J");
        printf("\033[H");

        printf("Ton pseudo est : %s\n",j->U->pseudo);
        printf("\n");
        printf("Ton argent disponible : %d \n",j->cagnotte);             //on affiche l'argent disponible et les vagues à venir, pour l'utilisateur
        printf("\n");
        affichage_zombies();
        printf("\n");
        affichage_vagues(T);
        printf("\n");
        for (int i=0;i<7;i++) {                                     //on libère la mémoire occupée par le tableau
            free(T[i]);
        }
        free(T);
        fclose(f);
        return j;
    }
    printf("La partie lue n'est pas valide.\n");
    exit(1);
}



/*

████████ ██    ██ ██████  ███████ ███████     ████████  ██████  ██    ██ ██████  ███████ ██      ██      ███████ ███████ 
   ██     ██  ██  ██   ██ ██      ██             ██    ██    ██ ██    ██ ██   ██ ██      ██      ██      ██      ██      
   ██      ████   ██████  █████   ███████        ██    ██    ██ ██    ██ ██████  █████   ██      ██      █████   ███████ 
   ██       ██    ██      ██           ██        ██    ██    ██ ██    ██ ██   ██ ██      ██      ██      ██           ██ 
   ██       ██    ██      ███████ ███████        ██     ██████   ██████  ██   ██ ███████ ███████ ███████ ███████ ███████ 

*/

void tank(Tourelle *T) {//degats sur le zombie et ceux des deux ligne à coté
    T->type=6;
    T->prix=300;
    T->pointsDeVie=5;
    T->degats=4;
    T->position=0;
    T->ligne=0;
    T->use=0;
}

void ralentir(Tourelle *T) {//empeche le zombie d'avancer pendant un tour
    T->type=5;
    T->prix=150;
    T->pointsDeVie=4;
    T->degats=1;
    T->position=0;
    T->ligne=0;
    T->use=0;
}

void sniper(Tourelle *T) {
    T->type=4;
    T->prix=250;
    T->pointsDeVie=4;
    T->degats=3;
    T->position=0;
    T->ligne=0;
    T->use=0;
}

void mine(Tourelle *T){
    T->type=3;
    T->prix=75;
    T->pointsDeVie=1;
    T->degats=5;
    T->position=0;
    T->ligne=0;
    T->next=NULL;
}

void mur(Tourelle *T){
    T->type=2;
    T->prix=50;
    T->pointsDeVie=3;
    T->degats=0;
    T->position=0;
    T->ligne=0;
    T->next=NULL;
}


void tourelle_base(Tourelle *T){
    if (T->type == 1) {
        T->prix=100;
        T->pointsDeVie=3;
        T->degats=1;
        T->position=0;
        T->ligne=0;
        T->next=NULL;
    }
    else {
        printf("Erreur de type de tourelle");
    }
}
/*

███████ ██    ██ ██████  ██████  ███████ ███████ ███████ ██  ██████  ███    ██ 
██      ██    ██ ██   ██ ██   ██ ██      ██      ██      ██ ██    ██ ████   ██ 
███████ ██    ██ ██████  ██████  █████   ███████ ███████ ██ ██    ██ ██ ██  ██ 
     ██ ██    ██ ██      ██   ██ ██           ██      ██ ██ ██    ██ ██  ██ ██ 
███████  ██████  ██      ██   ██ ███████ ███████ ███████ ██  ██████  ██   ████ 

*/

int est_mort(int pointsDeVie){
    if (pointsDeVie<=0){
        return 1;
    }
    return 0;
}

Tourelle *suppr_tourelles(Tourelle *T){
    if (!T){
        return T;
    }
    Tourelle* current = T; //supprime les tourelles mortes
    Tourelle* prev = NULL;
    while (current){
        if (est_mort(current->pointsDeVie)){
            Tourelle *suppr=current;
            if (prev){
                prev->next=current->next; // si c'est la première tourelle qui est morte
            }
            else {
                T=current->next;
            }
            current=current->next;// on chaine avec la tourelle 
            free(suppr);
            }
        else {
            prev = current;
            current = current->next;
        }
        }

    return T;
}

Etudiant *suppr_etudiants(Etudiant *E) {
    if (!E) return NULL;

    Etudiant *current = E;
    Etudiant *prev = NULL;

    while (current) {
        if (current->pointsDeVie <= 0) {
            // Mettre à jour le chaînage de next_line et prev_line
            if (current->prev_line) {
                current->prev_line->next_line = current->next_line;
            }
            if (current->next_line) {
                current->next_line->prev_line = current->prev_line;
            }

            // Supprimer le nœud dans la liste principale (next)
            if (prev) {
                prev->next = current->next;
            } else {
                E = current->next; // Si c'est le premier élément, la tête change
            }

            // Libérer la mémoire
            Etudiant *to_free = current;
            current = current->next;
            free(to_free);
        } else {
            prev = current;
            current = current->next;
        }
    }

    return E;
}

/*

██████  ██       █████   ██████ ███████ ███    ███ ███████ ███    ██ ████████     ████████  ██████  ██    ██ ██████  ███████ ██      ██      ███████ ███████ 
██   ██ ██      ██   ██ ██      ██      ████  ████ ██      ████   ██    ██           ██    ██    ██ ██    ██ ██   ██ ██      ██      ██      ██      ██      
██████  ██      ███████ ██      █████   ██ ████ ██ █████   ██ ██  ██    ██           ██    ██    ██ ██    ██ ██████  █████   ██      ██      █████   ███████ 
██      ██      ██   ██ ██      ██      ██  ██  ██ ██      ██  ██ ██    ██           ██    ██    ██ ██    ██ ██   ██ ██      ██      ██      ██           ██ 
██      ███████ ██   ██  ██████ ███████ ██      ██ ███████ ██   ████    ██           ██     ██████   ██████  ██   ██ ███████ ███████ ███████ ███████ ███████ 

*/


Tourelle *placer_tourelle(Jeu *J, int **tab){
    if (J->cagnotte>=50){     //prix de la tourelle  la moins cher proposée
        Tourelle *T=malloc(sizeof(Tourelle));
        if (!T){
          printf("Erreur d'allocation\n");
          return NULL;
        }
        printf("Entrez le type de votre tourelle : ");
        if (scanf("%d", &T->type)!=1) { // si on entre une chaine de caractere ou un caractere
            printf("\nErreur de saisie.\n");
            exit(1);
        }
        if (T->type == 1){
            tourelle_base(T);
            J->cagnotte-=T->prix;
        }
        else if (T->type == 2){
            mur(T);
            J->cagnotte-=T->prix;
        }
        else if (T->type == 3){
            mine(T);
            J->cagnotte-=T->prix;
        }
        else if (T->type == 4) {
            sniper(T);
            J->cagnotte-=T->prix;
        }
        else if (T->type == 5) {
            ralentir(T);
            J->cagnotte-=T->prix;
        }
        else if (T->type==6) {
            tank(T);
            J->cagnotte-=T->prix;
        }
        else{
            printf("Erreur dans le type de tourelle entre veuillez en entrer un nouveau :");
            if (scanf("%d", &T->type)!=1) {
                printf("\nErreur de saisie.\n");
                exit(1);
            }
            while (T->type<1 || T->type>6){ // tant que l'entier donné n'est pas entre 1 et 6 (type des tourelles)
                printf("Erreur dans le type de tourelle entre veuillez en entrer un nouveau :");
                if (scanf("%d", &T->type)!=1) {
                    printf("\nErreur de saisie.\n");
                    exit(1);
                }
            }
            if (T->type == 1){
                tourelle_base(T);
                J->cagnotte-=T->prix;
            }
            else if (T->type == 2){
                mur(T);
                J->cagnotte-=T->prix;
            }
            else if (T->type == 3){
                mine(T);
                J->cagnotte-=T->prix;
            }
            else if (T->type == 4) {
                sniper(T);
                J->cagnotte-=T->prix;
            }
            else if (T->type == 5) {
                ralentir(T);
                J->cagnotte-=T->prix;
            }
            else if (T->type==6) {
                tank(T);
                J->cagnotte-=T->prix;
            }
        }

        int ligne=0;
        printf("A quelle ligne souhaitez vous placer votre tourelle ?");
        if (scanf("%d", &ligne)!=1) {
            printf("\nErreur de saisie.\n");
            exit(1);
        }
        while (1>ligne || 7<ligne){
            printf("Erreur\n");
            printf("A quelle ligne souhaitez vous placer votre tourelle (1 a 7)?");
            if (scanf("%d", &ligne)!=1) {
                printf("\nErreur de saisie.\n");
                exit(1);
            }
        }
        T->ligne=ligne;
        int position;
        printf("A quelle position sur la ligne %d souhaitez vous placez votre tourelle ? (Les positions se comptent de gauche a droite)", T->ligne);
        if (scanf("%d", &position)!=1){
            printf("\nErreur de saisie.\n");
            exit(1);
        }
        while (1>position || 14<position){ // on ne peut pas mettre de tourelles en position 15 car sinon les zombies ne s'afficheront pas 
            printf("Erreur\n");
            printf("A quelle position sur la ligne %d souhaitez vous placez votre tourelle (1 a 14)?", T->ligne);
            if (scanf("%d", &position)!=1) {
                printf("\nErreur de saisie.\n");
                exit(1);
            }
        }
        T->position=position;
        if (tab[ligne-1][position-1]==0) {
            if (T->type==1) {
                tab[ligne-1][position-1]='T';
            }
            else if (T->type==2) {
                tab[ligne-1][position-1]= 'W';
            }
            else if (T->type==3) {
                tab[ligne-1][position-1]='M';
            }
            else if (T->type==4) {
                tab[ligne-1][position-1]='S';
            }
            else if (T->type==5) {
                tab[ligne-1][position-1]='R';
            }
            else {
                tab[ligne-1][position-1]='C';
            }
        }
        else {
            printf("vous avez deja place une tourelle a cet emplacement choissisez en un autre.\n");
            while (tab[ligne-1][position-1]!=0) { // si une tourelle est déjà placée à cette emplacement 
                printf("A quelle ligne souhaitez vous placer votre tourelle (1 a 7)?\n");
                if (scanf("%d", &ligne)!=1) {
                    printf("\nErreur de saisie.\n");
                    exit(1);
                }
                while (1>ligne || 7<ligne) {
                    printf("Erreur\n");
                    printf("A quelle ligne souhaitez vous placer votre tourelle (1 a 7)?");
                    if (scanf("%d", &ligne)!=1) {
                        printf("\nErreur de saisie.\n");
                        exit(1);
                    }
                }
                printf("A quelle position sur la ligne %d souhaitez vous placez votre tourelle ?\n", T->ligne);
                if (scanf("%d", &position)!=1) {
                    printf("\nErreur de saisie.\n");
                    exit(1);
                }
                while (1>position || 15<position){ 
                    printf("Erreur\n");
                    printf("A quelle position sur la ligne %d souhaitez vous placez votre tourelle (1 a 15)?", T->ligne);
                    if (scanf("%d", &position)!=1) {
                        printf("\nErreur de saisie.\n");
                        exit(1);
                    }
                }
            }
            T->ligne=ligne;
            T->position=position;
            if (T->type==1) {
                tab[ligne-1][position-1]='T';
            }
            else if (T->type==2) {
                tab[ligne-1][position-1]= 'W';
            }
            else if (T->type==3) {
                tab[ligne-1][position-1]='M';
            }
            else if (T->type==4) {
                tab[ligne-1][position-1]='S';
            }
            else if (T->type==5) {
                tab[ligne-1][position-1]='R';
            }
            else {
                tab[ligne-1][position-1]='C';
            }
        }
        printf("Votre tourelle est placee a la ligne %d et a la position %d\n", T->ligne, T->position);
        //T->next=NULL;
        return T;
    }
    return NULL;
}
/*

 █████  ███████ ███████ ██  ██████ ██   ██  █████   ██████  ███████ 
██   ██ ██      ██      ██ ██      ██   ██ ██   ██ ██       ██      
███████ █████   █████   ██ ██      ███████ ███████ ██   ███ █████   
██   ██ ██      ██      ██ ██      ██   ██ ██   ██ ██    ██ ██      
██   ██ ██      ██      ██  ██████ ██   ██ ██   ██  ██████  ███████ 

*/
void affichage_placements_tourelles(int **tab) {
    for (int i=1; i<8; i++) {
        printf("\n");
        printf("%d|  ", i);
        for (int j=0;j<15; j++) {
            if (tab[i-1][j]!=0) {
                printf("\033[1;31m%3c\033[0m", (char) tab[i-1][j]);
            }
            else {
                int z='.';
                printf("%3c",(char) z);
            }
        }
    }
    printf("\n");
}

int tour_affichage(Jeu *J, int **tab) {
    if (!J || !tab) {
        printf("erreur\n");
        return 0;
    }

    Tourelle *t = J->tourelles;
    Etudiant *e = J->etudiants;
    //on remplit le tableau avec les zombies 
    while (e && e->tour <= J->tour) {
        if (e->ligne >= 0 && e->ligne <= 7 && e->position - 1 >= 0 && e->position - 1 < 15) {
            if (tab[e->ligne-1][e->position - 1] == '.') {
                tab[e->ligne-1][e->position - 1] = (char)e->type;
            }
        }
        e = e->next;
    }

    e = J->etudiants;
    printf("Tour %d                                 ", J->tour);
    if (J->U) {
        printf("score:%d\n", J->U->score);
    } else {
        printf("score: (inconnu)\n");
    }
    for (int i = 1; i < 8; i++) {
        printf("\n%d|  ", i);
        for (int j = 0; j < 15; j++) {
        //on affiche 
            if (tab[i - 1][j] != '.') {
                if (tab[i - 1][j] == 'Z' || tab[i - 1][j] == 'A' || tab[i - 1][j] == 'G' || tab[i - 1][j] == 'D'|| tab[i - 1][j] == 'K'|| tab[i - 1][j] == 'O' ) {
                    while (e && (e->ligne != i || e->position != j + 1)) { // on cherche le zombie correspondant 
                        e = e->next;
                    }
                    if (e) {
                        printf("\033[1;31m %2d%c \033[0m", e->pointsDeVie, (char)e->type); //affichage en rouge 
                    }
                    e = J->etudiants;
                }
                else {
                    if ((tab[i - 1][j] == 'T' || tab[i - 1][j] == 'S' || tab[i - 1][j] == 'M' || tab[i - 1][j] == 'W'|| tab[i - 1][j] == 'C'|| tab[i - 1][j] == 'R' )) {
                        while (t && (t->ligne != i || t->position != j + 1)) { // on cherche la tourelle correspondante
                            t = t->next;
                        }
                        if (t) {
                            if (t->pointsDeVie>=10) {
                                printf("\033[1;32m %2d%c \033[0m", t->pointsDeVie, (char) tab[i-1][j]); // affichage en vert
                            }
                            else {
                                printf("\033[1;32m  %d%c \033[0m", t->pointsDeVie, (char) tab[i-1][j]); 
                            }
                        }
                        t = J->tourelles;
                    }

                }
            }
            else {
                printf("  %c  ", (char)tab[i-1][j]); // affichage des points
            }
        }
    }
    return 1;
}

void affichage_compte_a_rebours() {
    printf("\033[1;33m");
    printf("Etes vous pret ?\n");
    sleep(1);
    printf("\033[2J");
    printf("\033[H");
    printf("  ||||||  \n");
    printf("      ||  \n");
    printf("  ||||||  \n");
    printf("      ||  \n");
    printf("  ||||||  \n");
    sleep(1);
    printf("\033[2J");
    printf("\033[H");
    printf("  ||||||  \n");
    printf(" ||    || \n");
    printf("     ||   \n");
    printf("   ||     \n");
    printf(" |||||||| \n");
    sleep(1);
    printf("\033[2J");
    printf("\033[H");
    printf("    ||    \n");
    printf("   |||    \n");
    printf("    ||    \n");
    printf("    ||    \n");
    printf("  ||||||  \n");
    sleep(1);
    printf("\033[2J");
    printf("\033[H");
    printf("Let's go !!!\n");
    sleep(1);
    printf("\033[2J");
    printf("\033[H");
    printf("\033[0m");
}

/*

 █████  ██    ██  █████  ███    ██  ██████ ███████     ███████  ██████  ███    ███ ██████  ██ ███████ 
██   ██ ██    ██ ██   ██ ████   ██ ██      ██             ███  ██    ██ ████  ████ ██   ██ ██ ██      
███████ ██    ██ ███████ ██ ██  ██ ██      █████         ███   ██    ██ ██ ████ ██ ██████  ██ █████   
██   ██  ██  ██  ██   ██ ██  ██ ██ ██      ██           ███    ██    ██ ██  ██  ██ ██   ██ ██ ██      
██   ██   ████   ██   ██ ██   ████  ██████ ███████     ███████  ██████  ██      ██ ██████  ██ ███████ 

*/

int pos_max_T(Tourelle *T, int ligne ) {
// on cherche la tourelle la plus avancée sur la ligne
    Tourelle *t = T;
    int max = 1;
    while (t->next) {
        if (t->ligne == ligne && t->position>max) {
            max=t->position;
        }
        t=t->next;
    }
    if (t->ligne == ligne && t->position>max) {
        max=t->position;
    }
    return max;
}

int ralentir_sur_la_ligne(Tourelle *T, int ligne, int tour) {
    //1 pour le ralentir déjà utilisé et 0 sinon 
    Tourelle *t=T;
    while (t) {
        if (t->ligne == ligne && t->type==5) {
            if (tour%2!=t->use%2) {
                return 0;
            }
            if ( tour%2!=t->use) {
                return 0;
            }
        }
        t=t->next;
    }
    return 1;
}

Etudiant *reinit_use(Etudiant *E) {
// on reinitialise les use a 0
    Etudiant *head = E;
    while (E) {
        E->use = 0;
        E = E->next;
    }
    return head;
}

Etudiant *init_pos_z(Etudiant *E) {
// initialise la position des zombies à 15 avant les tours 
    Etudiant *head = E;
    while (E) {
        E->position = 15;
        E = E->next;
    }
    return head;
}

int tourelle_pres(Tourelle *T, int pos, int ligne) {
// si une tourelle n'est pas présente on renvoie 1 sinon renvoie 0
    Tourelle *t=T;
    while (t) {
        if (t->position == pos && t->ligne == ligne) {
            return 0;
        }
        t=t->next;
    }
    return 1;
}

Etudiant * avance_zombie(Etudiant *E, Tourelle *T, int tour, int **tab) { //quand la tour et le zombie sont au même endroit on avance pas
    if (!E) {
        printf("la liste chainee est vide");
        return NULL;
    }
    Tourelle *t=T;
    Etudiant *e=E;
    while (E) {
        if (E->tour<=tour && E->use==0) {
            if ((char) E->type !='A') {
                if (E->next_line){
                    if (E->position - E->next_line->position > E->vitesse && tourelle_pres(t, E->position-1, E->ligne-1)==1){//probleme si un zombie meurt et que le suivant va vite faire un truc avec les tourelles
                        E->position-=E->vitesse;
                    }

                }
                else {
                    if (tourelle_pres(t, E->position-1, E->ligne)==1) {
                        E->position-=E->vitesse;
                    }
                }
            }
            else { //pour l'athlete
            // on vérifie toutes les conditions pour l'avancée de l'athlète si il est bloqué par une tourelle un zombie...
                if (E->next_line) {
                    if ((E->position - E->vitesse) > E->next_line->position) {// si le next_line ne bloque pas l'avancée
                        //printf("%d", (E->position - E->vitesse) > E->next_line->position);
                        if (tourelle_pres(T, E->position-2, E->ligne)==1) { 
                            E->position-=2;
                        }
                        else if (tourelle_pres(T, E->position-1, E->ligne)==1) {
                            E->position-=1;
                        }
                        T=t;
                    }
                    else if ((E->position - E->next_line->position) == 2) {
                        if (tourelle_pres(T, E->position-1, E->ligne)==1) {
                            E->position-=1;
                        }
                        T=t;
                    }
                    else if ((E->position - E->next_line->position) == 1) {
                        //printf("%d", (E->position - E->vitesse) > E->next_line->position);
                        if (E->next_line->next_line) {
                            if ((E->next_line->next_line->position - E->position) > 2) {
                                if (tourelle_pres(T, E->position-2, E->ligne)==1) {
                                    E->position-=2;
                                }
                                T=t;
                            }
                        }
                        else {
                            if (tourelle_pres(T, E->position-2, E->ligne)==1) {
                                E->position-=2;
                            }
                            T=t;
                        }
                    }
                }
                else {
                    if (tourelle_pres(T, E->position-2,E->ligne)==1) {
                        E->position-=2;
                    }
                    else if (tourelle_pres(T, E->position-1,E->ligne)==1) {
                        E->position-=1;
                    }
                    T=t;
                }
            }
        }
        E->use=1;
        E=E->next;
    }
    return e;
}

/*

 █████  ██    ██ ████████ ██████  ███████ ███████ 
██   ██ ██    ██    ██    ██   ██ ██      ██      
███████ ██    ██    ██    ██████  █████   ███████ 
██   ██ ██    ██    ██    ██   ██ ██           ██ 
██   ██  ██████     ██    ██   ██ ███████ ███████ 

*/

void libere_tab(int **tab) {
// on libère le tableau utilisé dans tour_gen
    for (int i=0; i<7; i++) {
        free(tab[i]);
    }
    free(tab);
}

void manuel_utilisation(void) {
    printf("\033[1;34mMysterieux voyageur, avant de commencer ton periple, tu dois savoir une derniere chose. Apres 100 ans de lutte acharnee,\n");
    printf("notre communaute a developpe de nouvelles technologies et de nouvelles armes afin de se proteger des zombies atteints du virus Segfault-X.\n");
    printf("Les plus grands sages de notre communaute ont donc mis au point un manuel d'utilisation de chacune des armes pour ta venue afin que tu puisses eliminer les zombies dans les meilleurs conditions.\n");
    printf("pour le consulter tape 1 et si tu as assez confiance en toi et en tes capacites pour ne pas le consulter tape 0. \033[0m\n\n\n");
}


void deg_orateur(Etudiant *E) {
// on ajoute les degats au zombie devant l'orateur 
    Etudiant *e=E;
    while (e) {
        if ((char) e->type=='O') {
            if (e->next_line) {
                e->next_line->degats+=3;
            }
        }
        e=e->next;
    }
}

void soin_doc(Etudiant *E) {
    // on ajoute 3 de vie au zombie devant le docteur
    Etudiant *e=E;
    while (e) {
        if ((char) e->type=='D') {
            if (e->next_line) {
                e->next_line->pointsDeVie+=3;
            }
        }
        e=e->next;
    }
}

void libere_memoire_jeu(Jeu *j) {           /*permet de libérer toute la mémoire occupée par une partie*/

    if (repere_meilleurs_scores) {
        free(j->U->pseudo);
        free(j->U);
    }

    Etudiant *etudiant = j->etudiants;
    while (etudiant!=NULL) {
        Etudiant *etudiant2=etudiant;
        etudiant=etudiant->next;
        free(etudiant2);
    }

    Tourelle *tourelle = j->tourelles;
    while (tourelle!=NULL) {
        Tourelle* tourelle2 = tourelle;
        tourelle=tourelle->next;
        free(tourelle2);
    }

    free(j);
}


/*
████████  ██████  ██    ██ ██████
   ██    ██    ██ ██    ██ ██   ██
   ██    ██    ██ ██    ██ ██████
   ██    ██    ██ ██    ██ ██   ██
   ██     ██████   ██████  ██   ██
*/


int tour_gen(Jeu *J) {
    if (!J) {
        printf("Erreur le jeu est nul");
        return 0;
    }
    manuel_utilisation();
    int z;
    if (scanf("%d", &z)!=1) {
        printf("\nErreur de saisie.\n");
        exit(1);
    }
    while (z!=0 && z!=1) {
        printf("Tapez 1 pour consulter le manuel, 0 si vous ne voulez pas le consulter :");
        if (scanf("%d", &z)!=1) {
            printf("\nErreur de saisie.\n");
            exit(1);
        }
    }
    if (z==1) {
        printf("\033[1;33m");
        printf("Six differents types de tourelles sont proposees : la tourelle de base (T), le mur (W), la mine (M), le sniper (S), le ralentisseur (R) et le char d'assaut/tank (C).\n");
        printf("Tourelle de base    |    Mur    |    Mine   |   Sniper  | Ralentisseur |    Tank   |\n");
        printf("prix:100            |  prix:50  |  prix:75  |  prix:250 |   prix:150   |  prix:300 |\n");
        printf("degats:1            | degats:0  |  degats:5 |  degats:3 |   degats:1   |  degats:4 |\n");
        printf("vie:3               |   vie:3   |   vie:1   |   vie:4   |    vie:4     |   vie:5   |\n");
        printf("type:1              |   type:2  |   type:3  |   type:4  |    type:5    |   type:6  |\n\n");
        printf("Description des pouvoirs des tourelles:\n");
        printf("    -Mur: tourelle capable de stopper l'avancee des zombies.\n");
        printf("    -Mine: tourelle qui explose et inflige un grand nombre de degats au zombie a son contact.\n");
        printf("    -Sniper: tourelle extrememnt sophistique qui tire des frappes ultra-precises.\n");
        printf("    -Ralentisseur: tourelle capable de ralentir les zombies et de leur infliger de legers degats.\n");
        printf("    -Tank: notre meilleur tourelle, qui inflige de lourds degats de zone un tour sur deux.\033[0m\n\n");

    }
    J->tour=1;
    int **tab=malloc(7*sizeof(int*)); // on créé le tableau pour l'affichage
    for (int i=0; i<7; i++) {
        tab[i]=malloc(15*sizeof(int));
        for (int j=0; j<15; j++) {
            tab[i][j]=0;
        }
    }
    tab[0][0]=0;
    tab[0][1]=0;
    int tmp1;
    while (J->cagnotte >= 50){  //50 prix de la tourelle le moins cher
        Tourelle *nouvelle_tourelle=placer_tourelle(J, tab); // on créé le maillon de la nouvelle tourelle 
        tmp1=J->cagnotte;
        printf("\n");
        printf("Votre cagnotte est maintenant de %d.\n", J->cagnotte);
	sleep(2);
        if (!nouvelle_tourelle) {
            return 0;
        }
        printf("\033[2J");
    	printf("\033[H");
        printf("Voici le placement de vos tourelles :\n");
        affichage_placements_tourelles(tab);
        printf("\n\n");
        if (!J->tourelles) { // c'est le premier élément de la liste chaînée
            J->tourelles=nouvelle_tourelle;
            J->tourelles->next=NULL;
        }
        else {
            Tourelle *tmp= J->tourelles;
            while (tmp->next) {  // on parcourt les tourelles afin de pouvoir ajouter la tourelle en fin de liste
                tmp=tmp->next;
            }
            tmp->next = nouvelle_tourelle; // ajout en fin de liste de le nouvelle tourelle
            nouvelle_tourelle->next=NULL;
        }

        int rep;
        if (J->cagnotte >= 50){printf("Voulez-vous placer une nouvelle tourelle ? Tapez 1 (oui) ou tapez 0 (non) :\n");}
        else {break;}
        if (scanf("%d", &rep )!=1) {
            printf("\nErreur de saisie.\n");
            exit(1);
        }
        while (rep!=1 && rep!=0) {
            printf("Erreur! Veuillez taper 1 (oui) ou taper 0 (non) :\n ");
            if (scanf("%d", &rep)!=1) {
                printf("\nErreur de saisie.\n");
                exit(1);
            }
        }
        if (rep==0) {
            tmp1=J->cagnotte;
            J->cagnotte=0;
	    break;
        }
       
        
        printf("\033[1;33m");
        printf("\nTourelle de base    |    Mur    |    Mine   |   Sniper  | Ralentisseur |    Tank   |\n");
        printf("prix:100            |  prix:50  |  prix:75  |  prix:250 |   prix:150   |  prix:300 |\n");
        printf("type:1              |   type:2  |   type:3  |   type:4  |    type:5    |   type:6  |\033[0m\n\n");
        printf("\n");
	printf("Votre cagnotte est de: %d.\n\n",J->cagnotte);
    }

    J->cagnotte=tmp1;
    sleep(3);
    printf("\033[2J");      // Efface tout le terminal
    printf("\033[H");       // Replace le curseur en haut à gauche
    affichage_compte_a_rebours();
    init_pos_z(J->etudiants);

    printf("-----------------------------------------------\n");
    for (int i=0; i<7; i++) { // on reinitialise le tableau pour pouvoir le remplir avec les informations du nouveau tour
        for (int j=0; j<15; j++) {
            if (tab[i][j]==0) {
                tab[i][j]='.';
            }
        }
    }
    deg_orateur(J->etudiants);
    soin_doc(J->etudiants);
    tour_affichage(J, tab);
    printf("\n");
    reinit_use(J->etudiants);

    while (!win(J->etudiants) && !loose(J->etudiants)) { // si win ou loose on stop la boucle 
        int n;
        do {
            printf("\n\nVoulez-vous sauvegardez votre partie ? Tapez 1 pour oui et 0 pour non.\n");
            if (scanf("%d",&n)!=1) {
                printf("\nErreur de saisie.\n");
                exit(1);
            }
        } while (n!=1 && n!=0);
        if (n==1) {
            libere_tab(tab);
            sauvegarde(J);
            return 10;
        }
        Tourelle *T= J->tourelles;
        while(T){ //boucle qui s'assure de la validité de tourelle (quelle ne soit pas NULL)
            Etudiant *current = J->etudiants;
            while (current && current->tour<=J->tour){ //boucle qui s'assure que les zombies sont ceux du tour et ceux des tours précédents ainsi que de la validité du pointeur
                if (T->ligne == current->ligne && T->position== current->position-1) { // si la tourelle et le zombie sont face a face

                    if (T->type!=6) {
                        current->pointsDeVie-=T->degats; // inflige les degats a la tourelle 
                        T->pointsDeVie-=current->degats; // inflige les degats au zombie
                    }
                    else if (T->use%2==0){ // car le tank tire une fois sur deux 
                        current->pointsDeVie-=T->degats;
                        if (current->prev_line && current->prev_line->tour<=J->tour) {
                            current->prev_line->pointsDeVie-=T->degats/2; //pas de problème pour la suppression des zombies suppr_etudiants supprime tous les etudiants qui sont morts
                        }
                    }
                    else {
                        T->pointsDeVie-=current->degats; // si le tank ne tire pas
                    }
                    current->use=1;

                    if (est_mort(current->pointsDeVie)) {
                        J->U->score+=current->score;
                        if (current->prev_line){// dans le cas ou le tank tue l'étudiant prev_line
                        	if (est_mort(current->prev_line->pointsDeVie)){
                        		J->U->score+=current->prev_line->score;
                        	}
                        }
                        J->etudiants = suppr_etudiants(J->etudiants);
                    }
                    if (T->pointsDeVie<=0) {
                        J->tourelles = suppr_tourelles(J->tourelles);
                    }
                    if (T->next) { //s'il y a une autre tourelle on continue sinon on s'arrete la (explication dans le rapport)
                        T=T->next;
                    }
                    else {
                        break;
                    }
                    current=J->etudiants; // on recommence la liste chainée des étudiants au début
                }
                else if (T->ligne == current->ligne && T->position!= current->position){
                    if (T->type!=6 && T->type!=5 && T->type!=3) {// si cest different du tank et du ralentisseur car ils tirent une fois sur 2 ou si c'est une mine car elle s'active uniquement au contact d'un zombie
                        current->pointsDeVie-=T->degats;
                    }
                    else if (T->type==6 && T->use%2==0) { 
                        current->pointsDeVie-=T->degats;
                        T->use+=1;
                        if (current->prev_line && current->prev_line->tour<=J->tour) {
                            current->prev_line->pointsDeVie-=T->degats/2; //pas de problème pour la suppression des zombies suppr_etudiants supprime tous les etudiants qui sont morts
                        }
                    }
                    else if (T->type == 6 && T->use%2==1) {
                        T->use+=1;
                    }
                    if (T->type!=5 && ralentir_sur_la_ligne(T, T->ligne, J->tour)) {
                        if (current->next_line){ // pour avancer le zombie on vérifie qu'il n'y pas de ralentisseur actif 
                            if (current->position - current->vitesse > current->next_line->position && current->use==0) {
                                if ((char) current->type!='A') {
                                    current->position-=current->vitesse;
                                    current->use=1;
                                }
                            }
                        }
                        else {
                            if (pos_max_T(T, T->ligne)<current->position-current->vitesse && current->use==0) { // verification pour avancer le zombie on regarde si la tour la plus proche des zombies est assez loin pour pouvoir avancer le zombie 
                                if ((char) current->type!='A') {
                                    current->position-=current->vitesse;
                                    current->use=1;
                                }
                            }
                        }
                    }
                    else if (T->type==5){ // ralentisseur et conditions
                        if (T->use%2==0){
                            current->pointsDeVie-=T->degats;
                            if ((char) current->type!='A') {
                                current->use=1;
                            }
                        }
                        else {
                            if (pos_max_T(T, T->ligne)<current->position-current->vitesse && current->use==0) {
                                if ((char) current->type!='A') {
                                    current->position-=current->vitesse;
                                    current->use=1;
                                }
                            }
                        }

                        T->use+=1;
                    }
                    if (est_mort(current->pointsDeVie)) {
                        J->U->score+=current->score;
                        J->etudiants = suppr_etudiants(J->etudiants);

                    }
                    current=J->etudiants;
                    if (T->next) {
                        T=T->next;//si il y a une autre tourelle on continue sinon on arrete ici la boucle while
                    }
                    else {
                        break;
                    }
                }
                else {
                    if (current->next) { // si il reste des zombies à verifier (explication de pourquoi ne pas laisser la boucle while )
                        current=current->next;
                    }
                    else if (T->next){ // si plus fin de la liste chaînée des zombies et qu'il reste des tourelles à regarder
                        T=T->next;
                        current=J->etudiants; // on recommence la liste des zombies au début
                    }
                    else {
                        break;
                    }
                }
            }

            break;

        }
        Etudiant *av=J->etudiants;
        avance_zombie(av, J->tourelles, J->tour, tab); // on avance les zombies pas avancer
        int i=0;
        Etudiant *E=J->etudiants;
        while (E) { // si un zombie est en position inférieur à 1 pour empecher un bug d'affichage
            if (E->position<1) {
                i=1;
            }
            E=E->next;
        }
        if (i){break;} 
        reinit_use(av); // on remet les use des etudiants a 0 pour pouvoir commencer le nouveau tour peut etre faire %2
        for (int i=0; i<7; i++) { // on reinitialise le tableau pour pouvoir le remplir avec les informations du nouveau tour
            for (int j=0; j<15; j++) {
                tab[i][j]='.';
            }
        }

        Tourelle *ta=J->tourelles;
        while (ta) {//on remplit le tableau avec les emplacements des tourelles
            if (ta->type==1) {
                tab[ta->ligne-1][ta->position-1]='T';
            }
            else if (ta->type==2) {
                tab[ta->ligne-1][ta->position-1]='W';
            }
            else if (ta->type==3) {
                tab[ta->ligne-1][ta->position-1]='M';
            }
            else if (ta->type==4) {
                tab[ta->ligne-1][ta->position-1]='S';
            }
            else if (ta->type==5) {
                tab[ta->ligne-1][ta->position-1]='R';
            }
            else {
                tab[ta->ligne-1][ta->position-1]='C';
            }
            ta=ta->next;
        }
        J->tour+=1;

        printf("\033[2J");
        printf("\033[H");


        printf("-----------------------------------------------\n");
        tour_affichage(J, tab);// on affiche le tableau
        printf("\n");

        T=J->tourelles; //on reinitialise T au debut de la liste chaînée pour commencer le tour suivant
    }
    if (win(J->etudiants)) {
        printf("\n");
        printf("\033[1;32m");
        printf( "  ________    _____    ________  _______  ___________ \n"
            " /  _____/   /  _  \\  /  _____/  \\      \\ \\_   _____/ \n"
            "/   \\  ___  /  /_\\  \\/   \\  ___  /   |   \\ |    __)_ \n"
            "\\    \\_\\  \\/    |    \\    \\_\\  \\/    |    \\|        \\ \n"
            "\\______  /\\____|__  /\\______  /\\____|__  /_______  / \n"
            "        \\/         \\/        \\/         \\/        \\/ \033[0m\n\n");
        libere_tab(tab);
        return 1;
    }
    if (loose(J->etudiants)) {
        printf("\n");
        printf("\033[1;31m");
        printf( " _______________________________________   ____ ___\n"
            "\\______   \\_   _____/\\______   \\______ \\ |    |   \\ \n"
            "|     ___/|    __)_  |       _/|    |  \\|    |   / \n"
            "|    |    |        \\ |    |   \\|    `   \\    |  / \n"
            "|____|   /_______  / |____|_  /_______  /______/  \n"
            "                 \\/         \\/        \\/          \n\033[0m\n\n");
        libere_tab(tab);
        return 0;
    }
    libere_tab(tab);
    sleep(2);
    printf("\033[2J");
    printf("\033[H");
    return 1;
}
/*

████████  ██████  ██    ██ ██████       ██████ ██   ██  █████  ██████   ██████  ███████     
   ██    ██    ██ ██    ██ ██   ██     ██      ██   ██ ██   ██ ██   ██ ██       ██          
   ██    ██    ██ ██    ██ ██████      ██      ███████ ███████ ██████  ██   ███ █████       
   ██    ██    ██ ██    ██ ██   ██     ██      ██   ██ ██   ██ ██   ██ ██    ██ ██          
   ██     ██████   ██████  ██   ██      ██████ ██   ██ ██   ██ ██   ██  ██████  ███████     

*/

int tour_gen_charge(Jeu *J) {               //même fonction que tour_gen mais sans le placement des tourelles car c'est une partie chargée
    if (!J) {
        printf("Erreur le jeu est nul");
        return 0;
    }
    int **tab=malloc(7*sizeof(int*));
    for (int i=0; i<7; i++) {
        tab[i]=malloc(15*sizeof(int));
        for (int j=0; j<15; j++) {
            tab[i][j]=0;
        }
    }
    tab[0][0]=0;
    tab[0][1]=0;
    Tourelle *ta=J->tourelles;
    while (ta) {//on remplit le tableau avec les emplacements des tourelles
        if (ta->type==1) {
            tab[ta->ligne-1][ta->position-1]='T';
        }
        else if (ta->type==2) {
            tab[ta->ligne-1][ta->position-1]='W';
        }
        else if (ta->type==3) {
            tab[ta->ligne-1][ta->position-1]='M';
        }
        else if (ta->type==4) {
            tab[ta->ligne-1][ta->position-1]='S';
        }
        else if (ta->type==5) {
            tab[ta->ligne-1][ta->position-1]='R';
        }
        else {
            tab[ta->ligne-1][ta->position-1]='K';
        }
        ta=ta->next;
    }
    printf("\033[2J");
    printf("\033[H");
    printf("Etat de votre ancienne partie:\n\n");
    printf("-----------------------------------------------\n");
    for (int i=0; i<7; i++) { // on reinitialise le tableau pour pouvoir le remplir avec les informations du nouveau tour
        for (int j=0; j<15; j++) {
            if (tab[i][j]==0) {
                tab[i][j]='.';
            }
        }
    }
    tour_affichage(J, tab);
    sleep(4);
    printf("\033[2J");     // Efface tout le terminal
    printf("\033[H");      // Replace le curseur en haut à gauche
    affichage_compte_a_rebours();
    int f=0;
    reinit_use(J->etudiants);
    while (!win(J->etudiants) && !loose(J->etudiants)) { //pas la bonne condition
        int n;
        if (f){
        	do {
            		printf("\n\nVoulez-vous sauvegardez votre partie ? Tapez 1 pour oui et 0 pour non.\n");
            		if (scanf("%d",&n)!=1) {
            		    printf("\nErreur de saisie.\n");
            		    exit(1);
            		}
        	} while (n!=1 && n!=0);
        	if (n==1) {
            		libere_tab(tab);
            		sauvegarde(J);
            		return 10;
        	}
        }
        f=1;




        Tourelle *T= J->tourelles;

        while(T){ //boucle qui s'assure de la validité de tourelle (quelle ne soit pas NULL)
            Etudiant *current = J->etudiants;
            while (current && current->tour<=J->tour){ //boucle qui s'assure que les zombies sont ceux du tour et ceux des tours précédents
                if (T->ligne == current->ligne && T->position== current->position-1) {

                    if (T->type!=6 && (char) current->type!='K') {
                        current->pointsDeVie-=T->degats;
                        T->pointsDeVie-=current->degats;
                    }
                    else if (T->use%2==0){
                        current->pointsDeVie-=T->degats;
                        if (current->prev_line && current->prev_line->tour<=J->tour) {
                            current->prev_line->pointsDeVie-=T->degats/2; //pas de problème pour la suppression des zombies suppr_etudiants supprime tous les etudiants qui sont morts
                        }
                    }
                    else {
                        T->pointsDeVie-=current->degats;
                    }
                    current->use=1;

                    if (est_mort(current->pointsDeVie)) {
                        J->U->score+=current->score;
                        if ((char) current->type== 'K') {
                            if ((current->position - T->position)<=2) {
                                T->pointsDeVie-=current->degats;
                            }
                        }
                        J->etudiants = suppr_etudiants(J->etudiants);

                    }
                    if (T->pointsDeVie<=0) {
                        J->tourelles = suppr_tourelles(J->tourelles);

                    }
                    if (T->next) { //s'il y a une autre tourelle on continue sinon on s'arrete la
                        T=T->next;
                    }
                    else {
                        break;
                    }
                    current=J->etudiants;
                }
                else if (T->ligne == current->ligne && T->position!= current->position){
                    if (T->type!=6 && T->type!=5 && T->type!=3) {
                        current->pointsDeVie-=T->degats;
                    }
                    else if (T->type==6 && T->use%2==0) {
                        current->pointsDeVie-=T->degats;
                        T->use+=1;
                        if (current->prev_line && current->prev_line->tour<=J->tour) {
                            current->prev_line->pointsDeVie-=T->degats/2; //pas de problème pour la suppression des zombies suppr_etudiants supprime tous les etudiants qui sont morts
                        }
                    }
                    else if (T->type == 6 && T->use%2==1) {
                        T->use+=1;
                    }
                    if (T->type!=5 && ralentir_sur_la_ligne(T, T->ligne, J->tour)) {
                        if (current->next_line){ //inutile je pense
                            if (current->position - current->vitesse > current->next_line->position && current->use==0) {
                                if ((char) current->type!='A') {
                                    current->position-=current->vitesse;
                                    current->use=1;
                                }
                            }
                        }
                        else {
                            if (pos_max_T(T, T->ligne)<current->position-current->vitesse && current->use==0) {
                                if ((char) current->type!='A') {
                                    current->position-=current->vitesse;
                                    current->use=1;
                                }
                            }
                        }
                    }
                    else if (T->type==5){
                        if (T->use%2==0){
                            current->pointsDeVie-=T->degats;
                            if ((char) current->type!='A') {
                                current->use=1;
                            }
                        }
                        else {
                            if (pos_max_T(T, T->ligne)<current->position-current->vitesse && current->use==0) {
                                if ((char) current->type!='A') {
                                    current->position-=current->vitesse;
                                    current->use=1;
                                }
                            }
                        }

                        T->use+=1;
                    }
                    if (est_mort(current->pointsDeVie)) {
                        J->U->score+=current->score;
                        if ((char) current->type== 'K') {
                            if ((current->position - T->position)<=2) {
                                T->pointsDeVie-=current->degats;
                            }
                        }
                        J->etudiants = suppr_etudiants(J->etudiants);

                    }
                    if (T->pointsDeVie<=0) {            //rajouté car kamikaze avant on avait pas besoin
                        J->tourelles = suppr_tourelles(J->tourelles);
                    }
                    current=J->etudiants;
                    if (T->next) {
                        T=T->next;//si il y a une autre tourelle on continue sinon on arrete ici la boucle while
                    }
                    else {
                        break;
                    }
                }
                else {
                    if (current->next) {
                        current=current->next;
                    }
                    else if (T->next){
                        T=T->next;
                        current=J->etudiants;
                    }
                    else {
                        break;
                    }
                }
            }

            break;

        }
        Etudiant *av=J->etudiants;
        avance_zombie(av, J->tourelles, J->tour, tab);
        int i=0;
        Etudiant *E=J->etudiants;
        while (E) {
            if (E->position<1) {
                i=1;
            }
            E=E->next;
        }
        if (i){break;}
        reinit_use(av); // on remet les use des etudiants a 0 pour pouvoir commencer le nouveau tour peut etre faire %2
        for (int i=0; i<7; i++) { // on reinitialise le tableau pour pouvoir le remplir avec les informations du nouveau tour
            for (int j=0; j<15; j++) {
                tab[i][j]='.';
            }
        }
        //affiche_liste_Z(J->etudiants);
        Tourelle *ta=J->tourelles;
        while (ta) {//on remplit le tableau avec les emplacements des tourelles
            if (ta->type==1) {
                tab[ta->ligne-1][ta->position-1]='T';
            }
            else if (ta->type==2) {
                tab[ta->ligne-1][ta->position-1]='W';
            }
            else if (ta->type==3) {
                tab[ta->ligne-1][ta->position-1]='M';
            }
            else if (ta->type==4) {
                tab[ta->ligne-1][ta->position-1]='S';
            }
            else if (ta->type==5) {
                tab[ta->ligne-1][ta->position-1]='R';
            }
            else {
                tab[ta->ligne-1][ta->position-1]='C';
            }
            ta=ta->next;
        }
        J->tour+=1;

        printf("\033[2J");     // Efface tout le terminal
        printf("\033[H");      // Replace le curseur en haut à gauche


        printf("-----------------------------------------------\n");
        tour_affichage(J, tab);// on affiche le tableau
        printf("\n");

        T=J->tourelles; //on reinitialise T au debut de la liste chaînée pour commencer le tour suivant
    }
    if (win(J->etudiants)) {
        printf("\n");
        printf("\033[1;32m");
        printf( "  ________    _____    ________  _______  ___________ \n"
            " /  _____/   /  _  \\  /  _____/  \\      \\ \\_   _____/ \n"
            "/   \\  ___  /  /_\\  \\/   \\  ___  /   |   \\ |    __)_ \n"
            "\\    \\_\\  \\/    |    \\    \\_\\  \\/    |    \\|        \\ \n"
            "\\______  /\\____|__  /\\______  /\\____|__  /_______  / \n"
            "        \\/         \\/        \\/         \\/        \\/ \033[0m\n\n");
        libere_tab(tab);
        return 1;
    }
    if (loose(J->etudiants)) {
        printf("\n");
        printf("\033[1;31m");
        printf( " _______________________________________   ____ ___\n"
            "\\______   \\_   _____/\\______   \\______ \\ |    |   \\ \n"
            "|     ___/|    __)_  |       _/|    |  \\|    |   / \n"
            "|    |    |        \\ |    |   \\|    `   \\    |  / \n"
            "|____|   /_______  / |____|_  /_______  /______/  \n"
            "                 \\/         \\/        \\/          \n\033[0m\n\n");
        libere_tab(tab);
        return 0;
    }
    libere_tab(tab);
    sleep(2);
    printf("\033[2J");
    printf("\033[H");
    return 1;
}



int main() {
    activerCodesANSI();
    affichage_intro();
    Jeu *j=creation_jeu();
    if (j==NULL) {
        return 0;
    }
    if (repere_charge) {
        if (tour_gen_charge(j)==10) {
            return 0;
        }
    }
    else {
        if (tour_gen(j)==10) {
        return 0;
        }
    }
    calcul_nb_etoiles_et_score_final(j);
    meilleurs_scores(j->U);
    affichage_score();
    affichage_outro(j);
    libere_memoire_jeu(j);
    return 0;
}

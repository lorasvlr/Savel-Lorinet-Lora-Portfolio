/* 
@brief Programme d'un serpent 
Ce programme fais en sorte que le serpent composé de X et de O avance vers la droite jusqu'à ce que l'utilisateur est effectué une action sur la touche a

@date 26 Octobre 2024 (fini)

@version 1.0

@author Savel-Lorinet Lora
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

/* @def TAILLE
@brief taille maximum du serpent*/
#define TAILLE 10

/*@def LARGEUR
@brief largeur maximum du serpent*/
#define LARGEUR 40

/*@def HAUTEUR
@brief hauteur maximum du serpent*/
#define HAUTEUR 20

/* @def TETESERPENT
@brief définit la tête du serpent*/
#define TETESERPENT 'O'

/* @def CORPSSERPENT
@brief définit le corps du serpent*/
#define CORPSSERPENT 'X'

/*@def POSITIONX
@brief position initiale en x du serpent*/
#define POSITIONX 40

/*@def POSITIONY
@brief position initiale en y du serpent*/
#define POSITIONY 20


/*@def DIRECTIOND
@brief direction vers la droite du serpent*/
#define DIRECTIOND 'd'

/*@def DIRECTIONQ
@brief direction vers la gauche du serpent*/
#define DIRECTIONQ 'q'

/*@def DIRECTIONZ
@brief direction vers le haut du serpent*/
#define DIRECTIONZ 'z'

/*@def DIRECTIONS
@brief direction vers le bas du serpent*/
#define DIRECTIONS 's'

/*@def LARGEURPLATEAU
@brief largeur maximum du plateau*/
#define LARGEURPLATEAU 80

/*@def HAUTEURPLATEAU
@brief hauteur maximum du plateau*/
#define HAUTEURPLATEAU 40

/*@def TAILLEPAVE
@brief taille maximum d'un pavé*/
#define TAILLEPAVE 5

/*@def NOMBREPAVE
@brief taille maximum d'un pavé*/
#define NOMBREPAVE 4

/*@def CADREPLATEAU
@brief définit le cadre du plateau*/
#define CADREPLATEAU '#'

/*@def FONDPLATEAU
@brief définit le fond du plateau*/
#define FONDPLATEAU ' '

/*@def POMME
@brief définit le symbole représentant la pomme*/
#define POMME '6'

/*@def MAXPOMME
@brief définit le symbole représentant la pomme*/
#define MAXPOMME 10

/*@brief Déclarations de toutes les procédures */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[], int taille);
void progresser(int lesX[], int lesY[], char direction, char plateau[HAUTEURPLATEAU][LARGEURPLATEAU], bool *collision, bool *pomme, int *serpenttaille);
int kbhit();
void gotoXY(int x, int y);
void disableEcho();
void enableEcho();
void initPlateau();
void dessinerPlateau();
bool detectionCollision(int x, int y, char plateau[HAUTEURPLATEAU][LARGEURPLATEAU], int lesX[], int lesY[], int serpenttaille);
char plateau[HAUTEURPLATEAU][LARGEURPLATEAU];
int positionpave();
void ajouterPomme(char plateau[HAUTEURPLATEAU][LARGEURPLATEAU], int *serpenttaille, int lesX[], int lesY[]);

int main() 
{
    int lesX[1000];
    int lesY[1000];
    char touche = DIRECTIOND;
    char direction = DIRECTIOND;
    int x = POSITIONX;
    int y = POSITIONY;
    bool pomme = false;
    bool collision = false;
    int serpenttaille = 5;

    srand(time(NULL));
    system("clear");
    disableEcho();
    initPlateau();
    ajouterPomme(plateau, &serpenttaille, lesX, lesY);
    dessinerPlateau();

    // Initialise les coordonnées du serpent
    for (int i = 0; i < TAILLE; i++) {
        lesX[i] = x - i; // Positionnement des anneaux 
        lesY[i] = y;
    }

    /*@fn while, déplacement du serpent */
    while (touche != 'a') 
    {
        effacer(lesX[serpenttaille - 1], lesY[serpenttaille - 1]);
        progresser(lesX, lesY, direction, plateau, &collision, &pomme, &serpenttaille);      
        dessinerSerpent(lesX, lesY, serpenttaille); 

        if (collision)
        {
            enableEcho();
            printf("Attention, il y a eu une collision !");
            break;
        }

        if (pomme) 
        {
            pomme = false; // Réinitialiser l'état après avoir mangé une pomme

            // Vérifier si la taille du serpent atteint la limite
            if (serpenttaille >= TAILLE + 10) 
            {
                printf("\nBravo ! Partie terminée, vous avez gagné avec une taille de %d anneaux ! :)\n", serpenttaille);
                break;
            }
        }

        usleep(200000); // Tempo de 200ms

        if (kbhit()) {
            touche = getchar(); // Vérifie si une touche a été pressée

            if (((touche == DIRECTIONS) && (direction != DIRECTIONZ)) || 
                ((touche == DIRECTIONZ) && (direction != DIRECTIONS)) ||
                ((touche == DIRECTIOND) && (direction != DIRECTIONQ)) ||
                ((touche == DIRECTIONQ) && (direction != DIRECTIOND)))
            {
                direction = touche;
            }
        }
    }
    enableEcho();
    printf("Votre serpent s'est arrêté. :) \n");
    return 0;
}

/*@fn afficher, Affichage d'un caractère à une position (x, y)*/ 
void afficher(int x, int y, char c) 
{
    gotoXY(x, y);
    printf("%c", c);
}

/*@fn effacer, Efface un caractère à une position (x, y)*/ 
void effacer(int x, int y) 
{
    gotoXY(x, y);
    printf(" ");
}

/*@fn dessinerSerpent, dessine le serpent*/
void dessinerSerpent(int lesX[], int lesY[], int serpenttaille) 
{
    afficher(lesX[0], lesY[0], TETESERPENT);            // Tête
    for (int i = 1; i < serpenttaille; i++) 
    {
        if(lesX[i] > 0 && lesX[i] < LARGEURPLATEAU -1 &&
           lesY[i] > 0 && lesY[i] < HAUTEURPLATEAU - 1)
        {
            afficher(lesX[i], lesY[i], CORPSSERPENT);   // affiche de nouveaux anneaux
        } 
    }
}

/*@fn progresser, fais progresser le serpent vers la droite*/
void progresser(int lesX[], int lesY[], char direction, char plateau[HAUTEURPLATEAU][LARGEURPLATEAU], bool *collision, bool *pomme, int *serpenttaille) 
{
    effacer(lesX[*serpenttaille - 1], lesY[*serpenttaille - 1]);

    for (int i = *serpenttaille - 1; i > 0; i--) 
    {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }
    switch(direction)  // test les différents cas d'appui d'une touche et agit en conséquences
    {
        case DIRECTIONZ: 
            lesY[0]--;
            break;
        case DIRECTIONS: 
            lesY[0]++;
            break;
        case DIRECTIONQ: 
            lesX[0]--;
            break;
        case DIRECTIOND: 
            lesX[0]++;
            break;
    }

        // Téléportation lorsque le serpent traverse un trou
    if (lesY[0] == 1 && lesX[0] == LARGEURPLATEAU / 2) 
    {
        // Trou en haut, réapparaît en bas
        lesY[0] = HAUTEURPLATEAU - 2;
    } 
    else if (lesY[0] == HAUTEURPLATEAU - 1 && lesX[0] == LARGEURPLATEAU / 2) 
    {
        // Trou en bas, réapparaît en haut
        lesY[0] = 2;
    } 
    else if (lesX[0] == 1 && lesY[0] == HAUTEURPLATEAU / 2) 
    {
        // Trou à gauche, réapparaît à droite
        lesX[0] = LARGEURPLATEAU - 2;
    } 
    else if (lesX[0] == LARGEURPLATEAU - 1 && lesY[0] == HAUTEURPLATEAU / 2) 
    {
        // Trou à droite, réapparaît à gauche
        lesX[0] = 2;
    }

    if(lesX[0] <= 0)                     // Limite gauche 
    {
        lesX[0] = 1; 
    }
    if(lesX[0] >= LARGEURPLATEAU - 1)    //Limite droite
    {
        lesX[0] = LARGEURPLATEAU - 2;
    }
    if(lesY[0] <= 0)                     // Limite du haut
    {
        lesY[0] = 1;
    }
    if (lesY[0] >= HAUTEURPLATEAU - 1)  // Limite du bas
    {
        lesY[0] = HAUTEURPLATEAU - 2;
    }

    *collision = detectionCollision(lesX[0], lesY[0], plateau, lesX, lesY, TAILLE); // vérification d'une collision

    if (plateau[lesY[0]][lesX[0]] == POMME) 
    {
        *pomme = true; // Marquer que la pomme a été mangée
        (*serpenttaille)++;
        plateau[lesY[0]][lesX[0]] = FONDPLATEAU; // Efface la pomme du plateau
        ajouterPomme(plateau, serpenttaille, lesX, lesY); // Ajouter une nouvelle pomme
    }
}

bool detectionCollision(int x, int y, char plateau[HAUTEURPLATEAU][LARGEURPLATEAU], int lesX[], int lesY[], int serpenttaille)
{
    if (x <= 0 || x >= LARGEURPLATEAU - 1 || y <= 0 || y >= HAUTEURPLATEAU - 1)
    {
        return true;
    }

    if (plateau[y][x] == CADREPLATEAU)
    {
        return true;
    }

    for (int i = 1; i < serpenttaille; i++)
    {
        if(lesX[i] == x && lesY[i] == y)
        {
            return true;
        }
    }
    return false;
}

/*@fn initPlateau, initialise le plateau*/
void initPlateau()
{  
    for (int i = 0; i < HAUTEURPLATEAU; i++) 
    {
        for (int j = 0; j < LARGEURPLATEAU; j++) 
        {
            // Bordures horizontales avec un trou d'un caractère au centre
            if ((i == 1 || i == HAUTEURPLATEAU - 1) && j != LARGEURPLATEAU / 2) 
            {
                plateau[i][j] = CADREPLATEAU;
            } 
            // Bordures verticales avec un trou d'un caractère au centre
            else if ((j == 1 || j == LARGEURPLATEAU - 1) && i != HAUTEURPLATEAU / 2)
            {
                plateau[i][j] = CADREPLATEAU;
            }
            // Fond du plateau
            else 
            {
                plateau[i][j] = FONDPLATEAU;
            }
        }
    }

    // Placement des pavés
    for (int i = 0; i < NOMBREPAVE; i++)        
    {
        int position = positionpave();
        int x = (position >> 16) & 0xFFFF;      // Extraire la position x 
        int y = position & 0xFFFF;              // Extraire la position y 

        for (int dx = 0; dx < TAILLEPAVE; dx++)
        {
            for (int dy = 0; dy < TAILLEPAVE; dy++)
            {
                if (x + dx < LARGEURPLATEAU - 1 && y + dy < HAUTEURPLATEAU - 1)
                {
                    plateau[y + dy][x + dx] = CADREPLATEAU;
                }
            }
        }
    }
}

/*@fn positionpave, se charge de faire en sorte que les pavés n'apparaissent pas à la position initiale du serpent */
int positionpave()
{
    int x, y;
    bool position_valide;

    do
    {
        x = rand() % (LARGEURPLATEAU - TAILLEPAVE - 1) + 1;     // Générer des positions aléatoires pour le pavé
        y = rand() % (HAUTEURPLATEAU - TAILLEPAVE - 1) + 1;

        position_valide = true;

        for (int dx = 0; dx < TAILLEPAVE; dx++)                 // Vérifier si le pavé chevauche la position initiale du serpent
        {
            for (int dy = 0; dy < TAILLEPAVE; dy++)
            {
                if ((x + dx == POSITIONX) && (y + dy == POSITIONY))
                {
                    position_valide = false;
                    break;
                }
            }
            if (!position_valide) break;
        }
    } while (!position_valide);

    return (x << 16) | y;       // Retourne les positions 
}

/*@fn dessinerPlateau, dessine le plateau*/
void dessinerPlateau()
{
    system("clear");
    printf("\033[H");
    for(int i = 1; i < HAUTEURPLATEAU + 1; i++)
    {
        for(int j = 1; j < LARGEURPLATEAU + 1; j++)
        {
            printf("%c", plateau[i][j]);
        }
        printf("\n");
    }
}
void ajouterPomme(char plateau[HAUTEURPLATEAU][LARGEURPLATEAU], int *serpenttaille, int lesX[], int lesY[]) 
{
    // Vérifier si une pomme existe déjà
    for (int i = 0; i < HAUTEURPLATEAU; i++) 
    {
        for (int j = 0; j < LARGEURPLATEAU; j++) 
        {
            if (plateau[i][j] == POMME) 
            {
                return; // Une pomme existe déjà, ne rien faire
            }
        }
    }

    // Chercher une position vide pour placer une nouvelle pomme
    int x, y;
    bool pommeSurSerpent = false;
    do 
    {
        x = rand() % (LARGEURPLATEAU - 2) + 1; // Assure que la pomme est dans les limites (pas dans les bords)
        y = rand() % (HAUTEURPLATEAU - 2) + 1;

        pommeSurSerpent = false;
        for (int i = 0; i < *serpenttaille; i++) 
        {
            if (lesX[i] == x && lesY[i] == y) 
            {
                pommeSurSerpent = true;
                break;
            }
        }
    } 
    while (plateau[y][x] != FONDPLATEAU || pommeSurSerpent);

    plateau[y][x] = POMME; // Place la nouvelle pomme
    afficher(x, y, POMME); // Montre la pomme
}


/*************************************************************************************************************************
*                                                                                                                        *
*                                                Procédures préfaites                                                    *
*                                                                                                                        *
************************************************************************************************************************ */

/*
@fn Fonction pour vérifier si une touche a été pressée
la fonction retourne :
1 si un caractere est present
0 si pas de caractere present
*/ 
int kbhit()
{
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

// Fonction pour déplacer le curseur à (x, y)
void gotoXY(int x, int y) 
{
    printf("\033[%d;%df", y, x);
}

void disableEcho() 
{
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() 
{
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

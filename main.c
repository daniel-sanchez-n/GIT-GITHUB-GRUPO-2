#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
// Preprocessats Exercici 1
#define MAXCARACTERSTWORD 16 // Fins a 15 caràcters + '\0'
#define MAXPARAULESTSEARCH 15
#define MAXSIZEGRID 50
// Preprocesados EJERCICIO 5
#define MAXROWS 50
#define MAXCOLUMNS 50
#define MAXLENGTH 15
#define POINT '.'

// Estructuras EJERCICIO 5
typedef struct {
    char letters[MAXROWS][MAXCOLUMNS];
    int nRows;
    int nColumns;
} tLetterSoup;

typedef struct {
    char letters[MAXLENGTH];
    int length;
} tString;

// EJERCICIO 1
// Definició de les possibles orientacions de les paraules
typedef enum {
    HORITZONTAL,
    VERTICAL,
    DIAGONAL
} tOrientation;

// Tipus de dades per representar una posició dins la sopa de lletres
typedef struct {
    int x;  // Coordenada horitzontal
    int y;  // Coordenada vertical
    tOrientation orientacio; // Orientació de la paraula
} tPosition;

// Tipus de dades per representar una paraula dins la sopa
typedef struct {
    char paraula[MAXCARACTERSTWORD];  
    bool trobada;      // Indica si la paraula ha estat trobada
    tPosition posicio; // Posició de la paraula dins la sopa
} tWord;

// Definició dels estats de la sopa
typedef enum {
    BUIDA,
    OMPLERTA_ALEATORI,
    OMPLERTA_PARAULES
} tSoupState;

// Tipus de dades per representar la sopa de lletres
typedef struct {
    char grid[MAXSIZEGRID][MAXSIZEGRID]; // La quadrícula pot ser entre 15x15 i 50x50
    int amplada;       // Amplada de la sopa
    int alcada;        // Alçada de la sopa
    tSoupState estat;  // Estat de la sopa
} tSoup;

// Tipus de dades per emmagatzemar les paraules a buscar
typedef struct {
    tWord paraules[MAXPARAULESTSEARCH]; // Fins a 15 paraules
    int numParaules;    // Nombre de paraules a cercar
} tSearch;

// Tipus de dades per representar una partida de sopa de lletres
typedef struct {
    tSoup sopa;     // La sopa de lletres
    tSearch cerca;  // Les paraules a buscar
} tGame;

// Main y extras
tGame initGame(int rows, int cols, int numWords);
int main() {
    srand(time(NULL));
    int rows, cols, numWords;
    printf("Introdueix la quantitat de files: ");
    scanf("%d",&rows);
    printf("Introdueix la quantitat de columnes: ");
    scanf("%d",&cols);
    printf("Introdueix el numero de paraules: ");
    scanf("%d", &numWords);
    tGame game = initGame(rows, cols, numWords);
    playGame(&game);
    return 0;
}
tGame initGame(int rows, int cols, int numWords) {
    tGame game;
    initSoup(rows, cols, &game.sopa);
    initSearch(&game.cerca);
    int i;
    
    for (i = 0; i < numWords; i++) {
    	printf("Introdueix la paraula %i: ", i + 1);
        tWord word;
        scanf("%s", word.paraula);
        addWordToSearch(&game.cerca, word);
        hideWordIntoSoup(word, rows, cols, &game.sopa);
    }
    return game;
}

void playGame(tGame *game) {
    char input[16];
    int found = 0;
    
    printf("Sopa de lletres:\n");
    displaySoup(game->sopa);
    
    while (found < game->cerca.numParaules) {
        printf("Introdueix una paraula que hagis trobat: ");
        scanf("%s", input);
        int i;
        bool wordFound = false;
        for (i = 0; i < game->cerca.numParaules; i++) {
            if (!game->cerca.paraules[i].trobada && strcmp(input, game->cerca.paraules[i].paraula) == 0) {
                game->cerca.paraules[i].trobada = true;
                wordFound = true;
                found++;
                printf("Correcte! %d/%d paraules trobades.\n", found, game->cerca.numParaules);
                break;
            }
        }
        if (!wordFound) {
            printf("Paraula no trobada. Torna a intentar-ho:(\n");
        }
    }
    printf("Enhorabona! Has trobat totes les paraules.\n");
}

void displaySoup(tSoup soup) {
	int i,j;
    for (i = 0; i < soup.alcada; i++) {
        for (j = 0; j < soup.amplada; j++) {
            printf("%c ", soup.grid[i][j]);
        }
        printf("\n");
    }
}
// EJERCICIO 2
// Funció per obtenir el caràcter en una posició específica de la sopa de lletres
char getCharacter(tSoup sopa, int fila, int columna) {
    if (fila >= 0 && fila < sopa.alcada && columna >= 0 && columna < sopa.amplada) {
        return sopa.grid[fila][columna];
    }
    return '\0';  // Retorna caràcter nul si la posició està fora de rang
}

// Funció per establir un caràcter en una posició específica de la sopa
void setCharacter(tSoup *sopa, int fila, int columna, char car) {
    if (fila >= 0 && fila < sopa->alcada && columna >= 0 && columna < sopa->amplada) {
        sopa->grid[fila][columna] = car;
    }
}

// Funció per establir una paraula en una posició determinada de la sopa amb una orientació
void setWord(tSoup *sopa, tWord *paraula, int fila, int columna, tOrientation orientacio) {
    paraula->posicio.x = columna;
    paraula->posicio.y = fila;
    paraula->posicio.orientacio = orientacio;
    paraula->trobada = false;  // Marca la paraula com no trobada
    
    // Col·loca la paraula en la sopa segons l'orientació
    int i;
    for (i = 0; i < strlen(paraula->paraula); i++) {
        switch (orientacio) {
            case HORITZONTAL:
                setCharacter(sopa, fila, columna + i, paraula->paraula[i]);
                break;
            case VERTICAL:
                setCharacter(sopa, fila + i, columna, paraula->paraula[i]);
                break;
            case DIAGONAL:
                setCharacter(sopa, fila + i, columna + i, paraula->paraula[i]);
                break;
        }
    }
}
// Funció per llegir una paraula des de l'entrada estàndard
tWord readWord() {
    tWord novaParaula;
    printf("Introdueix una paraula (fins a 15 caràcters): ");
    scanf("%s", novaParaula.paraula);
    
    // Inicialitza la paraula com no trobada i sense posició
    novaParaula.trobada = false;
    novaParaula.posicio.x = -1;
    novaParaula.posicio.y = -1;
    novaParaula.posicio.orientacio = HORITZONTAL;  // Orientació per defecte (pot canviar després)
    
    return novaParaula;
}

// EJERCICIO 3
// Definicions prèvies (tipus de dades com tSoup, tSearch, tWord ja definits)

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

char getRandomCharacter() {
    return 'A' + getRandomNumber(0, 25);  // Lletres aleatòries entre 'A' i 'Z'
}

void fillWithRandomCharacters(int n, int m, tSoup *soup) {
	int i;
    for (i = 0; i < n; i++) {
    	int j;
        for (j = 0; j < m; j++) {
            soup->grid[i][j] = getRandomCharacter();
        }
    }
}

void hideWordIntoSoup(tWord word, int n, int m, tSoup *soup) {
    int i, j, ori, len, k;
    len = strlen(word.paraula);
    ori = getRandomNumber(1, 3); // 1 = vertical, 2 = horitzontal, 3 = diagonal
    
    // PARTE EJERCICIO 4
	/* // Aleatorització per decidir si la paraula es posa normal o invertida
    if (getRandomNumber(0, 1) == 1) {
        reverseWord(word); // Inverteix la paraula aleatòriament
    } */
    if (ori == 1) { // VERTICAL
        i = getRandomNumber(0, n - len);
        j = getRandomNumber(0, m - 1);
        for (k = 0; k < len; k++) {
            soup->grid[i + k][j] = word.paraula[k];
        }
    } else if (ori == 2) { // HORITZONTAL
        i = getRandomNumber(0, n - 1);
        j = getRandomNumber(0, m - len);
        int k;
        for (k = 0; k < len; k++) {
            soup->grid[i][j + k] = word.paraula[k];
        }
    } else { // DIAGONAL
        i = getRandomNumber(0, n - len);
        j = getRandomNumber(0, m - len);
        int k;
        for (k = 0; k < len; k++) {
            soup->grid[i + k][j + k] = word.paraula[k];
        }
    }
}

void addWordToSearch(tSearch *search, tWord word) {
    search->paraules[search->numParaules] = word;
    search->numParaules++;
}

void initSearch(tSearch *search) {
    search->numParaules = 0;
}

void initSoup(int n, int m, tSoup *soup) {
    soup->amplada = m;
    soup->alcada = n;
    soup->estat = BUIDA;  // Inicialitzem la sopa com buida

    // Omplim la sopa amb lletres aleatòries
    fillWithRandomCharacters(n, m, soup);
}

void fillSoup() {
    int n, m, w, i;
    tSoup soup;
    tSearch search;
    tWord word;

    // Llegeix les dimensions de la sopa i el nombre de paraules
    scanf("%d %d", &n, &m);
    initSoup(n, m, &soup);
    initSearch(&search);

    // Llegeix el nombre de paraules
    scanf("%d", &w);

    // Llegeix cada paraula i afegeix-la a la llista de cerca
    for (i = 0; i < w; i++) {
        scanf("%s", word.paraula);
        addWordToSearch(&search, word);
        hideWordIntoSoup(word, n, m, &soup);
    }

    // Mostra la sopa final
    writeSoup(n, m, soup);
}

// EJERCICIO 4
void reverseWord(tWord *word) {
    int len = strlen(word->paraula);
    int i;
    for (i = 0; i < len / 2; i++) {
        char temp = word->paraula[i];
        word->paraula[i] = word->paraula[len - i - 1];
        word->paraula[len - i - 1] = temp;
    }
}

// EJERCICIO 5
// Funció per llegir un caràcter en majúscules
bool isUpperCaseLetter(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Llegeix la sopa de lletres des de l'entrada estàndard
void readInputSoup(tLetterSoup *soup) {
    char c;
    int i, j;
    for (i = 0; i < soup->nRows; i++) {
        for (j = 0; j < soup->nColumns; j++) {
            do {
                c = getchar();
            } while (!isUpperCaseLetter(c));
            soup->letters[i][j] = c;
        }
    }
}

// Inicialitza la sopa de sortida (amb punts)
void initOutputSoup(tLetterSoup *soup) {
	int i, j;
    for (i = 0; i < soup->nRows; i++) {
        for (j = 0; j < soup->nColumns; j++) {
            soup->letters[i][j] = POINT;
        }
    }
}

// Funció per llegir una paraula
tString readString() {
    tString string;
    char c;
    int index = 0;
    do {
        c = getchar();
    } while (!isUpperCaseLetter(c));
    while (isUpperCaseLetter(c)) {
        string.letters[index] = c;
        index++;
        c = getchar();
    }
    string.length = index;
    return string;
}

// Funció per buscar la paraula en la sopa en diferents direccions
void lookForWord(tLetterSoup inputSoup, tString word, int x, int y, int *dirX, int *dirY, bool *found) {
    *found = false;
    *dirX = -1;
    while (*dirX <= 1 && !*found) {
        *dirY = -1;
        while (*dirY <= 1 && !*found) {
            checkWord(inputSoup, word, x, y, *dirX, *dirY, found);
            (*dirY)++;
        }
        (*dirX)++;
    }
}

// Funció per comprovar si la paraula coincideix en la direcció donada
void checkWord(tLetterSoup soup, tString currentWord, int x, int y, int dirX, int dirY, bool *found) {
    bool match = true;
    bool inBounds = (x >= 0) && (x < soup.nRows) && (y >= 0) && (y < soup.nColumns);
    int i = 0;
    
    while (i < currentWord.length && inBounds && match) {
        match = (currentWord.letters[i] == soup.letters[x][y]);
        x += dirX;
        y += dirY;
        inBounds = (x >= 0) && (x < soup.nRows) && (y >= 0) && (y < soup.nColumns);
        i++;
    }
    
    *found = match && (i == currentWord.length);
}

// Funció per col·locar una paraula trobada a la sopa de sortida
void putWord(tLetterSoup *soup, tString currentWord, int x, int y, int dirX, int dirY) {
	int i;
    for (i = 0; i < currentWord.length; i++) {
        soup->letters[x][y] = currentWord.letters[i];
        x += dirX;
        y += dirY;
    }
}

// Funció per escriure la sopa de lletres amb les paraules trobades
void writeSoup(tLetterSoup soup) {
	int i, j;
    printf("%d %d\n", soup.nRows, soup.nColumns);
    for (i = 0; i < soup.nRows; i++) {
        for (j = 0; j < soup.nColumns; j++) {
            printf("%c ", soup.letters[i][j]);
        }
        printf("\n");
    }
}


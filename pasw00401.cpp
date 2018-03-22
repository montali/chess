/*-------------------------------------
Title: pasw00401.cpp
Author: Simone Montali (288144), Guido Soncini (285140)
Version: 0.1
Description: This software saves the possible moves of a chess game on a text file
--------------------------------------*/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef enum {
    pawn,
    rook,
    bishop,
    knight,
    queen,
    king,
    noPiece
}
Pezzo;

typedef enum {
    white,
    black,
    noColor
}
Colore;

const char * pezzi[] = {
    "pawn",
    "rook",
    "bishop",
    "knight",
    "queen",
    "king",
    "noPiece"
};

const char * colori[] = {
    "white",
    "black",
    "noColor"
};

struct Cella {
    Pezzo pezzo;
    Colore colore;
};

typedef Cella Scacchiera[8][8];

// coordinate scacchistiche
struct Ccoord {
    char col; // 'a' .. 'h'	da sinistra
    int riga; // 1 .. 8 		dal "basso"
};

// coordinate matrice
struct Mcoord {
    int riga; // 0 .. 7		dall'alto
    int col; // 0 .. 7 		da sinistra
};

// inserisce celle vuote in tutta la scacchiera
void vuotaScacchiera(Scacchiera s);

// inserisce i vari pezzi nella scacchiera
void inserisciPezzi(Scacchiera s);

// visualizza la scacchiera (formato di visualizzazione e a scelta)
void visualizzaScacchiera(Scacchiera s);

Cella convertiInCella(char p);

// converte in char un pezzo p rappresentato in formato Cella
char convertiInChar(Cella p);

// converte coordinate matrice in coordinate scacchistiche
Ccoord converti(Mcoord);

// converte coordinate scacchistiche in coordinate matrice
Mcoord converti(Ccoord);

void indexChange(int & a, int & b, Pezzo pezzo, bool & finished); // Funzione che gestisce gli indici di spostamento sulla scacchiera

int main() {
    Scacchiera chess;
    vuotaScacchiera(chess);
    inserisciPezzi(chess);
    visualizzaScacchiera(chess);
    int rowIncrement, columnIncrement;
    std::ofstream output; //Creiamo lofstream per l'output su file
    output.open("mosse.txt");
    output << "Soncini Guido 285140 Montali Simone 288144" << endl;
    // inserimento coordinate cella (formato scacchistico es. b,1)
    // se si tratta di un alfiere bianco o nero visualizza tutte le sue
    //                               possibili mosse
    Mcoord current, move; // Queste due variabili le usiamo per andare a stampare le coordinate scacchistiche nel file
    Ccoord convertedCurrent, convertedMove; // Vedi sopra
    bool madeMove = false; // Questo boolean ci servirà per il "no move" nel file
    for (int i = 7; i >= 0; i--) {
        for (int c = 0; c < 8; c++) {
            madeMove = false;
            current.riga = i;
            current.col = c;
            convertedCurrent = converti(current);
            if (chess[i][c].colore == white) { //Cominciamo coi pezzi bianchi, dall'alto verso il basso
                output << pezzi[chess[i][c].pezzo] << " - " << colori[chess[i][c].colore] << " - " << convertedCurrent.col << convertedCurrent.riga << endl;
                switch (chess[i][c].pezzo) {
                case noPiece: // Chiaramente se non c'è nulla, non facciamo nulla
                case pawn: // Per il pawn, verifichiamo i tre casi: in caso possa mangiare in diagonale, ed in caso si muova e basta
                    if (chess[i - 1][c - 1].colore == black && i - 1 < 8 && i - 1 >= 0 && c - 1 < 8 && c - 1 >= 0) {
                        move.riga = i - 1;
                        move.col = c - 1;
                        convertedMove = converti(move);
                        cout << "[" << i << ", " << c << "]  *[" << i - 1 << ", " << c - 1 << "]" << endl;
                        output << "*" << pezzi[chess[i - 1][c - 1].pezzo] << " - " << convertedMove.col << convertedMove.riga << endl;
                        madeMove = true;
                    }
                    if (chess[i - 1][c + 1].colore == black && i - 1 < 8 && i - 1 >= 0 && c + 1 < 8 && c + 1 >= 0) {
                        move.riga = i - 1;
                        move.col = c + 1;
                        convertedMove = converti(move);
                        cout << "[" << i << ", " << c << "]  *[" << i - 1 << ", " << c + 1 << "]" << endl;
                        output << "*" << pezzi[chess[i + 1][c + 1].pezzo] << " - " << convertedMove.col << convertedMove.riga << endl;
                        madeMove = true;
                    }
                    if (chess[i - 1][c].colore == noColor && i - 1 >= 0 && i - 1 < 8) {
                        move.riga = i;
                        move.col = c;
                        convertedMove = converti(move);
                        cout << "[" << i << ", " << c << "]  [" << i - 1 << ", " << c << "]" << endl;
                        output << convertedMove.col << convertedMove.riga << endl;
                        madeMove = true;
                    }
                    break;

                default:
                    //variabili dell'incremento della posizione
                    if (chess[i][c].pezzo == rook) {

                        rowIncrement = 1;
                        columnIncrement = 0;
                        rowIncrement = 1;
                        columnIncrement = 1;
                    } else if (chess[i][c].pezzo == knight) {
                        rowIncrement = 2;
                        columnIncrement = 1;
                    } else {
                        rowIncrement = 1;
                        columnIncrement = 1;
                    }

                    // posizioni , x e y, iniziali dell'alfiere che verranno incrementate
                    int pieceRow = i;
                    int pieceColumn = c;

                    bool control = false; // variabile aggiuntiva per poter terminare il ciclo
                    do {
                        // controllo che l'incremento delle variabile non porti ad una situazione impossibile(ex pieceRow = -1)
                        if (pieceRow + rowIncrement < 0 || pieceRow + rowIncrement > 7 || pieceColumn + columnIncrement < 0 || pieceColumn + columnIncrement > 7) {
                            indexChange(rowIncrement, columnIncrement, chess[i][c].pezzo, control); // procedure per scambiare i due numeri e porne uno negativo in modo che almeno uno dei due numeri abbia un segno diverso

                            //resetto ogni volta la poszione iniziale dell'alfiere per poter conrollare una direzione diversa ogni volta
                            pieceRow = i;
                            pieceColumn = c;
                        }
                        //controllo che nella posizione icrementata non ci sia un'altra pedina se si controllo se si possa mangiare o meno
                        else if (chess[pieceRow + rowIncrement][pieceColumn + columnIncrement].colore != noColor) {
                            if (chess[pieceRow + rowIncrement][pieceColumn + columnIncrement].colore != chess[i][c].colore) {
                                move.riga = pieceRow + rowIncrement;
                                move.col = pieceColumn + columnIncrement;
                                convertedMove = converti(move);
                                cout << "[" << i << ", " << c << "]  *[" << pieceRow + rowIncrement << ", " << pieceColumn + columnIncrement << "]" << endl;
                                output << "*" << pezzi[chess[pieceRow + rowIncrement][pieceColumn + columnIncrement].pezzo] << " - " << convertedMove.col << convertedMove.riga << endl;
                                madeMove = true;
                            }
                            indexChange(rowIncrement, columnIncrement, chess[i][c].pezzo, control);
                            pieceRow = i;
                            pieceColumn = c;
                        } else //in questo caso la posizione esiste nella scacchiera e non ci sono pedine nel punto; quindi la stampo e passo alla posizione successiva
                        {
                            pieceRow += rowIncrement;
                            pieceColumn += columnIncrement;
                            move.riga = pieceRow;
                            move.col = pieceColumn;
                            convertedMove = converti(move);
                            cout << "[" << i << ", " << c << "]  [" << pieceRow << ", " << pieceColumn << "]" << endl;
                            output << convertedMove.col << convertedMove.riga << endl;
                            madeMove = true;
                            if (chess[i][c].pezzo == king || chess[i][c].pezzo == knight)
                                indexChange(rowIncrement, columnIncrement, chess[i][c].pezzo, control);
                        }

                    } while (!control);
                    break;
                }
                if (madeMove == false) {
                    output << "no move" << endl;
                }
            }
        }
    }
    for (int i = 0; i < 8; i++) { // Questa parte è uguale alla precedente, ma procede dall'alto verso il basso e verifica solo i pezzi neri
        for (int c = 8; c >= 0; c--) {
            current.riga = i;
            current.col = c;
            madeMove = false;
            convertedCurrent = converti(current);
            if (chess[i][c].colore == black) {
                output << pezzi[chess[i][c].pezzo] << " - " << colori[chess[i][c].colore] << " - " << convertedCurrent.col << convertedCurrent.riga << endl;
                switch (chess[i][c].pezzo) {
                case noPiece:
                case pawn:
                    if (chess[i + 1][c - 1].colore == white && i + 1 < 8 && i + 1 >= 0 && c - 1 < 8 && c - 1 >= 0) {
                        move.riga = i + 1;
                        move.col = c - 1;
                        convertedMove = converti(move);
                        cout << "[" << i << ", " << c << "]  *[" << i + 1 << ", " << c - 1 << "]" << endl;
                        output << "*" << pezzi[chess[i + 1][c - 1].pezzo] << " - " << convertedMove.col << convertedMove.riga << endl;
                        madeMove = true;
                    }
                    if (chess[i + 1][c + 1].colore == white && i + 1 < 8 && i + 1 >= 0 && c + 1 < 8 && c + 1 >= 0) {
                        move.riga = i + 1;
                        move.col = c + 1;
                        convertedMove = converti(move);
                        cout << "[" << i << ", " << c << "]  *[" << i + 1 << ", " << c + 1 << "]" << endl;
                        output << "*" << pezzi[chess[i + 1][c + 1].pezzo] << " - "  << convertedMove.col << convertedMove.riga << endl;
                        madeMove = true;
                    }
                    if (chess[i + 1][c].colore == noColor && i + 1 >= 0 && i + 1 < 8) {
                        move.riga = i;
                        move.col = c;
                        convertedMove = converti(move);
                        cout << "[" << i << ", " << c << "]  [" << i + 1 << ", " << c << "]" << endl;
                        output  << convertedMove.col << convertedMove.riga << endl;
                        madeMove = true;
                    }
                    break;

                default:
                    //variabili dell'incremento della posizione
                    if (chess[i][c].pezzo == rook) {

                        rowIncrement = 1;
                        columnIncrement = 0;
                        rowIncrement = 1;
                        columnIncrement = 1;
                    } else if (chess[i][c].pezzo == knight) {
                        rowIncrement = 2;
                        columnIncrement = 1;
                    } else {
                        rowIncrement = 1;
                        columnIncrement = 1;
                    }

                    // posizioni , x e y, iniziali dell'alfiere che verranno incrementate
                    int pieceRow = i;
                    int pieceColumn = c;

                    bool control = false; // variabile aggiuntiva per poter terminare il ciclo
                    do {
                        // controllo che l'incremento delle variabile non porti ad una situazione impossibile(ex pieceRow = -1)
                        if (pieceRow + rowIncrement < 0 || pieceRow + rowIncrement > 7 || pieceColumn + columnIncrement < 0 || pieceColumn + columnIncrement > 7) {
                            indexChange(rowIncrement, columnIncrement, chess[i][c].pezzo, control); // procedure per scambiare i due numeri e porne uno negativo in modo che almeno uno dei due numeri abbia un segno diverso

                            //resetto ogni volta la poszione iniziale dell'alfiere per poter conrollare una direzione diversa ogni volta
                            pieceRow = i;
                            pieceColumn = c;
                        }
                        //controllo che nella posizione icrementata non ci sia un'altra pedina se si controllo se si possa mangiare o meno
                        else if (chess[pieceRow + rowIncrement][pieceColumn + columnIncrement].colore != noColor) {
                            if (chess[pieceRow + rowIncrement][pieceColumn + columnIncrement].colore != chess[i][c].colore) {
                                move.riga = pieceRow + rowIncrement;
                                move.col = pieceColumn + columnIncrement;
                                convertedMove = converti(move);
                                cout << "[" << i << ", " << c << "]  *[" << pieceRow + rowIncrement << ", " << pieceColumn + columnIncrement << "]" << endl;
                                output << "*" << pezzi[chess[pieceRow + rowIncrement][pieceColumn + columnIncrement].pezzo] << " - " << convertedMove.col << convertedMove.riga << endl;
                                madeMove = true;
                                //printed = true;
                            }
                            indexChange(rowIncrement, columnIncrement, chess[i][c].pezzo, control);
                            pieceRow = i;
                            pieceColumn = c;
                        } else //in questo caso la posizione esiste nella scacchiera e non ci sono pedine nel punto; quindi la stampo e passo alla posizione successiva
                        {
                            pieceRow += rowIncrement;
                            pieceColumn += columnIncrement;
                            move.riga = pieceRow;
                            move.col = pieceColumn;
                            convertedMove = converti(move);
                            cout << "[" << i << ", " << c << "]  [" << pieceRow << ", " << pieceColumn << "]" << endl;
                            output << convertedMove.col << convertedMove.riga << endl;
                            madeMove = true;
                            //printed = true;
                            if (chess[i][c].pezzo == king || chess[i][c].pezzo == knight)
                                indexChange(rowIncrement, columnIncrement, chess[i][c].pezzo, control);
                        }

                    } while (!control);
                    break;
                }
                if (madeMove == false) {
                    output << "no move" << endl;
                }
            }
        }
    }
    return 0;
}

// inserisce celle vuote in tutta la scacchiera
void vuotaScacchiera(Scacchiera s) {
    Cella vuota;
    vuota.pezzo = noPiece;
    vuota.colore = noColor;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            s[r][c] = vuota;
}

// inserisce i vari pezzi nella scacchiera
void inserisciPezzi(Scacchiera s) {
    ifstream chessLoading;
    chessLoading.open("scacchiera.txt");
    //  la codifica dei pezzi bianchi e' la seguente:
    // Pedone 	p
    // Torre	r
    // Cavallo	n
    // Alfiere	b
    // Regina	q
    // Re		k
    // per i pezzi neri stessi caratteri ma maiuscoli
    string sitIniziale = "";
    string readLine;
    while (getline(chessLoading, readLine)) {
        sitIniziale += readLine;
    }
    chessLoading.close();
    // recupero dei caratteri dalla stringa
    // e inserimento pezzi corrispondenti in scacchiera
    for (int i = 0; i < sitIniziale.size(); i++) {
        if (sitIniziale[i] != ' ') {
            if (isupper(sitIniziale[i]))
                s[i / 8][i % 8].colore = black;
            else if (islower(sitIniziale[i]))
                s[i / 8][i % 8].colore = white;

            switch (toupper(sitIniziale[i])) {
            case 'P':
                s[i / 8][i % 8].pezzo = pawn;
                break;
            case 'R':
                s[i / 8][i % 8].pezzo = rook;
                break;
            case 'N':
                s[i / 8][i % 8].pezzo = knight;
                break;
            case 'B':
                s[i / 8][i % 8].pezzo = bishop;
                break;
            case 'Q':
                s[i / 8][i % 8].pezzo = queen;
                break;
            case 'K':
                s[i / 8][i % 8].pezzo = king;
                break;
            }
        }
    }
}

// visualizza la scacchiera (formato di visualizzazione a scelta)
void visualizzaScacchiera(Scacchiera s) {
    char print;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            switch (s[r][c].pezzo) {
            case pawn:
                print = 'p';
                break;
            case rook:
                print = 'r';
                break;
            case knight:
                print = 'n';
                break;
            case bishop:
                print = 'b';
                break;
            case queen:
                print = 'q';
                break;
            case king:
                print = 'k';
                break;
            default:
                print = '#';
                break;
            }

            if (s[r][c].colore == black) {
                print = toupper(print);
            }

            cout << print << ' ';

        }
        cout << endl;
    }
}

// converte coordinate matrice in coordinate scacchistiche
Ccoord converti(Mcoord m) {
    Ccoord c;
    c.col = 'a' + (m.col);
    c.riga = 8 - m.riga;
    return c;
}

// converte coordinate scacchistiche in coordinate matrice
Mcoord converti(Ccoord c) {
    Mcoord m;
    m.col = c.col - 'a';
    m.riga = 8 - c.riga;
    return m;
}

void indexChange(int & a, int & b, Pezzo pezzo, bool & finished) {
    switch (pezzo) {
    case knight:
        //inverto sempre il segno di b ma quando è negativo lo scambio
        if (b < 0) {
            int c = a;
            a = b;
            b = c;
        }
        b = -b;
        if (a == 1 && b == -2)
            finished = true;
        else if (a == -1 && b == -2) {
            a = -a;
            b = -b;
        }
        break;
    case king:
        if (b != -1 && a != -1)
            b--;
        else if ((b != 0) && a != -1) {
            b = -b;
            a--;
        } else
            finished = true;
        break;
    case queen:
        if (b != -1 && a != -1)
            b--;
        else if ((b != 0) && a != -1) {
            b = -b;
            a--;
        } else
            finished = true;
        break;
    default:
        int d;
        d = a;
        a = -b;
        b = d;
        if ((a == 1 && b == 1) || (a == 1 && b == 0))
            finished = true;
        break;
    }

}

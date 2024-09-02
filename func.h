/* BĂNESCU Ema-Ioana, grupa 311CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INT_MAX 2147483647
// Structurile pentru taskul 1

typedef struct {
    int nr_tronsoane; // nr_costuri(task2)
    float *trons; // vector costuri(pt task 2)
    // vector cu tronsoane care va avea pe post de
    // elemente practic gradele de uzura (nr de tip float)
    float *copie_trons;
    // vector cu n tronsoane care va avea ca elemente copii
    // ale elementelor lui trons,
    int *ok;
    // ok e vector care contine pe pozitia i 0 sau 1,
    // in caz ca un element al tronsonului este
    // injumatatit sau nu, la actualizarea gradelor de uzura.
}TRuta;

typedef struct celarc {
    char oras_dest[101];   // destinatie arc
    TRuta muchie;
    // ceea ce am pe muchie adica structura cu tronsoanele
    struct celarc *urm;
    // adresa urmatorului adiacent
} TCelArc, *AArc;

typedef struct {
    char *oras1;
    char *oras2;
    int R;
} TOras;
// aici retin orasele si numarul de rute citite de la tastatura.

typedef struct {
    int n;
    // numar noduri
    int R;
    AArc* x;
    // vector adrese liste arce
    char **tag;
  // tag: vector de char* adica vector de nume
  // de orase sursa, practic fiecare nod va avea un tag,
  // pentru a stii care este denumirea nodului sursa
  // Vectorul tag reprezinta numele nodurilor de la care duc eu
  // practic restul de noduri adiacente cu respectivele.
} TGL;
TGL Init_graf(int R);
int cauta_index_nod(TGL* graf, char* city);
void adauga_arc(TGL* graf, char* oras1, char* oras2,
                int nr_tronsoane, float* trons);
void afiseaza_graf(TGL* graf, FILE *output, TOras* nume_orase);
void actualizeaza_uzura(TGL *graf, int K);
void modifca_grade(TGL *graf);
int pastrare_rute(TGL *graf, float L, int *v, TOras *nume_orase);
void elibereaza_graf(TGL* graf, int R, TOras* nume_orase);
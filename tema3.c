/* BĂNESCU Ema-Ioana, grupa 311CB */
#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include <string.h>

void task1(FILE *input, FILE *output) {
    int R, K, i;
    float L;
    fscanf(input, "%d %d %f", &R, &K, &L);
    // citesc numarul de rute, numarul de ani, si gradul de uzura acceptabil.
    TGL graf;
    graf = Init_graf(R);
    if (!graf.x || !graf.tag) {
        fclose(input);
        fclose(output);
    }
    TOras* nume_orase = (TOras*)malloc(R * sizeof(TOras));
    if (!nume_orase) {
        // in caz ca esueaza alocarea memoriei pentru nume_orase
        elibereaza_graf(&graf, R, NULL);
        // am pus NULL acolo ca parametru, pentru ca eu inca nu
        // mi am alocat si vectorul de structuri, TOras*.
        fclose(input);
        fclose(output);
        return;
    }

    for (i = 0; i < R; i++) {
        char oras1[101], oras2[101];
        int nr_tronsoane;
        fscanf(input, "%s %s %d", oras1, oras2, &nr_tronsoane);
        nume_orase[i].oras1 = strdup(oras1);
        nume_orase[i].oras2 = strdup(oras2);
        nume_orase->R = R;
        if (!nume_orase[i].oras1 || !nume_orase[i].oras2) {
            elibereaza_graf(&graf, R, nume_orase);
            fclose(input);
            fclose(output);
            return;
        }
        float* trons = (float*)malloc(nr_tronsoane * sizeof(float));
        if (!trons) {
            elibereaza_graf(&graf, R, nume_orase);
            fclose(input);
            fclose(output);
        }
        int j;
        for (j = 0; j < nr_tronsoane; j++) {
            fscanf(input, "%f", &trons[j]);
        }

        if (cauta_index_nod(&graf, oras1) == -1) {
            // ii aloc spatiu si copiez numele oras1, in tagul grafului
            // pe pozitia graf->n
            // care va tot creste ce masura ce tot citesc si adaug in graf.
            graf.tag[graf.n] = strdup(oras1);
            if (!graf.tag[graf.n]) {
                // in caz ca nu a reusit alocarea la asignarea lui oras1
                free(trons);
                elibereaza_graf(&graf, R, nume_orase);
                fclose(input);
                fclose(output);
                return;
            }
            graf.n++;
            // cresc si numarul de noduri, doar daca nodul cu numele oras1
            // nu exista deja in graf
        }
        //  adaug arcul care are ca oras sursa oras1
        //  si ca destinatie oras2, in graf.
        adauga_arc(&graf, oras1, oras2, nr_tronsoane, trons);
        free(trons);
    }

    actualizeaza_uzura(&graf, K);
    afiseaza_graf(&graf, output, nume_orase);
    // am actualizat si apoi am afisat graful
    // cu noile grade de uzura ale tronsoanelor.
    int v[101];
    int dim = pastrare_rute(&graf, L, v, nume_orase);
    for (i = 0; i < dim; i++) {
        fprintf(output, "%d ", v[i]);
    }
    elibereaza_graf(&graf, R, nume_orase);
    // inchid fisierele.
    fclose(input);
    fclose(output);
}

int main(int argc, char* argv[])  {
    int task;
    task = atoi(argv[1]);
    // preiau numarul task-ului pe care vreau sa il rezolv.
    FILE* input = fopen("tema3.in", "r");
    if (!input) {
        printf("Eroare la deschidere\n");
        return 1;
    }

    FILE* output = fopen("tema3.out", "w");
    if (!output) {
        printf("Eroare la deschidere\n");
        fclose(input);
        return 1;
    }
    if (task == 1 && argc == 2) {
        task1(input, output);
    }
    return 0;
}

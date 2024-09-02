/* BĂNESCU Ema-Ioana, grupa 311CB */
#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include <string.h>

// functie care imi initiaza graful.
TGL Init_graf(int R) {
    int i;
    TGL graf;
    // setez numarul de noduri la zero
    graf.n = 0;
    // numarul de rute la R.
    graf.R = R;
    // aloc vectorul x de adrese liste arce, precum si vectorul de siruri tag.
    graf.x = (AArc*)malloc(R * 2 * sizeof(AArc));
    graf.tag = (char**)malloc(R * 2 * sizeof(char*));
    for (i = 0; i < R * 2; i++) {
        graf.x[i] = NULL;
        graf.tag[i] = NULL;
    }
    // returnez graful astfel initiat.
    return graf;
}
// functie care imi intoarce indexul unui nod cu un anumit tag.
int cauta_index_nod(TGL* graf, char* city) {
    int i;
    for (i = 0; i < graf->n; i++) {
        if (strcmp(graf->tag[i], city) == 0) {
            // daca il gasesc, returnez, l-am gasit pe indexul i, altfel,
            // intorc -1, si practic voi crea un nou nod,
            // adica un alt element in vectorul de adrese liste
            // arce ulterior, cu o lista de adicaenta
            // care va fi initial setata la NULL.
            return i;
        }
    }
    return -1;
}

// functie care imi adauga un arc (o muchie) in graf.
void adauga_arc(TGL* graf, char* oras1, char* oras2,
                int nr_tronsoane, float* trons) {
    AArc arc_nou = (AArc)malloc(sizeof(TCelArc));
    strcpy(arc_nou->oras_dest, oras2);
    // aloc campurile structurii TRuta, ale muchiei.
    arc_nou->muchie.nr_tronsoane = nr_tronsoane;
    arc_nou->muchie.trons = (float*)malloc(nr_tronsoane * sizeof(float));
    arc_nou->muchie.copie_trons = (float*)malloc(nr_tronsoane * sizeof(float));
    arc_nou->muchie.ok = (int*)malloc(nr_tronsoane * sizeof(int));
    int j;
    for (j = 0; j < nr_tronsoane; j++) {
        arc_nou->muchie.trons[j] = trons[j];
        arc_nou->muchie.copie_trons[j] = trons[j];
        // copiez in copie_trons, elementele din trons.
        arc_nou->muchie.ok[j] = 0;
        // initializez vectorul ok cu zerouri.
    }

    int index_oras = cauta_index_nod(graf, oras1);
     // verific daca lista de adiacenta este goala.
    if (graf->x[index_oras] == NULL) {
        // dacă lista este goală, noul arc il pun pe prima pozitie in lista.
        arc_nou->urm = NULL;
        graf->x[index_oras] = arc_nou;
    } else {
        // parcurg lista până la ultimul element
        AArc arc_curent = graf->x[index_oras];
        while (arc_curent->urm != NULL) {
            arc_curent = arc_curent->urm;
        }
        // leg noul arc la finalul listei
        arc_nou->urm = NULL;
        arc_curent->urm = arc_nou;
    }
}

// funcție pentru a afișa graful.
void afiseaza_graf(TGL* graf, FILE *output, TOras* nume_orase) {
    int k, i;
    // iterez prin fiecare ruta retinuta in vectorul nume_orase, pe care le-am
    // citit din fisierul de intrare in main.
    for (k = 0; k < graf->R; k++) {
        // iterez pana la numarul de noduri din graf
        for (i = 0; i < graf->n; i++) {
            // imi iau pointer la inceputul listei de adiacenta a fiecarui nod
            AArc arc = graf->x[i];
            if (strcmp(nume_orase[k].oras1, graf->tag[i]) == 0) {
                while (arc != NULL) {
                    int j;
                    if (strcmp(arc->oras_dest, nume_orase[k].oras2) == 0) {
                        // practic, daca orasul destinatie din nume_orase coincide
                        // cu tagului nodului de pe ,,pozitia" i si daca orasul sursa coincide
                        // cu orasul destinatie al arcului
                        fprintf(output, "%s ", graf->tag[i]);
                        fprintf(output, "%s ", arc->oras_dest);
                        fprintf(output, "%d ", arc->muchie.nr_tronsoane);
                        for (j = 0; j < arc->muchie.nr_tronsoane; j++) {
                            fprintf(output, "%.2f ", arc->muchie.trons[j]);
                        }
                        fprintf(output, "\n");
                    }
                arc = arc->urm;
                }
            }
        }
    }
}

// funcția de actualizare a uzurii
void actualizeaza_uzura(TGL *graf, int K) {
    int i;
    int an;
    // iterez pentru fiecare an
    for (an = 0; an < K; an++) {
        // reinitializez ok-urile pentru fiecare muchie din graf.
        for (i = 0; i < graf->n; i++) {
            AArc arc = graf->x[i];
            while (arc) {
                int j;
                for (j = 0; j < arc->muchie.nr_tronsoane; j++) {
                    arc->muchie.ok[j] = 0;
                }
                arc = arc->urm;
            }
        }
        for (i = 0; i < graf->n; i++) {
            AArc arc = graf->x[i];
            if(arc) {
                int j;
                char tag_arc[101];
                strcpy(tag_arc, graf->tag[i]);
                while (arc) {
                    int nr_tronsoane = arc->muchie.nr_tronsoane;
                    for (j = 0; j < nr_tronsoane; j++) {
                        if (arc->muchie.trons[j] == 0) {
                            if (j > 0 && j < arc->muchie.nr_tronsoane - 1) {
                                float grad_uzura_vecin = 0.0;
                                    if (arc->muchie.trons[j - 1] > grad_uzura_vecin) {
                                        grad_uzura_vecin = arc->muchie.trons[j - 1];
                                    }
                                    if (arc->muchie.trons[j + 1] > grad_uzura_vecin) {
                                        grad_uzura_vecin = arc->muchie.trons[j + 1];
                                    }
                                        arc->muchie.copie_trons[j] = grad_uzura_vecin / 2;
                                        arc->muchie.ok[j] = 1;
                    // vreau sa ii pun LA FINAL valoarea asta impartita la doi.
                            } else if (j == 0 && arc->muchie.nr_tronsoane != 1) {
                    // daca ar fi doar un tronson,
                    // as vrea sa intre doar pe ramura cealalta.
                                int k;
                                float grad_uzura_vecin = 0.00;
                                if (arc->muchie.trons[j + 1] > grad_uzura_vecin) {
                                    grad_uzura_vecin = arc->muchie.trons[j + 1];
                                }
                                for (k = 0; k < graf->n; k++) {
                                    AArc arc_vec = graf->x[k];
                                    char arc_vec_tag[101];
                                    strcpy(arc_vec_tag,graf->tag[k]);
                                    while (arc_vec) {
                    // daca nodul destinatie al unei muchii coincide
                    // cu nodul sursa(tag) al arcului curent 
                                        if (strcmp(arc_vec->oras_dest, tag_arc) == 0) {
                                            if (arc_vec->muchie.trons[arc_vec->muchie.nr_tronsoane - 1] > grad_uzura_vecin) {
                                                grad_uzura_vecin = arc_vec->muchie.trons[arc_vec->muchie.nr_tronsoane - 1];
                                            }
                                        }
                    // daca nodul sursa(tag) al unei alte rute coincide
                    // cu nodul sursa(tag) al rutei curente. 
                                        if (strcmp(arc_vec_tag, tag_arc) == 0) {
                                            if (arc_vec->muchie.trons[0] > grad_uzura_vecin) {
                                                grad_uzura_vecin = arc_vec->muchie.trons[0];
                                            }
                                        }
                                        arc_vec = arc_vec->urm;
                                    }
                                }
                                arc->muchie.copie_trons[j] = grad_uzura_vecin/2;
                                arc->muchie.ok[j] = 1; 

                            } else if (j == arc->muchie.nr_tronsoane - 1) {
                                int k;
                                float grad_uzura_vecin = 0.00;
                                if (arc->muchie.nr_tronsoane > 1 && arc->muchie.trons[j - 1] > grad_uzura_vecin) {
                                    grad_uzura_vecin = arc->muchie.trons[j - 1];
                                }
                                for (k = 0; k < graf->n; k++) {
                                    AArc arc_vec = graf->x[k];
                                    char nume_sursa[101];
                                    strcpy(nume_sursa, graf->tag[k]);
                                    while (arc_vec) {
                            // daca gasesc vreun oras sursa care sa
                            // coincida cu cel destinatie al rutei curente.
                                        if (strcmp(nume_sursa, arc->oras_dest) == 0) {
                                            if (arc_vec->muchie.trons[0] > grad_uzura_vecin) {
                                                grad_uzura_vecin = arc_vec->muchie.trons[0];
                                            }
                                        }
                            // daca gasesc vreun oras destinatie care sa
                            // coincida cu cel destinatie al rutei curente.
                                        if (strcmp(arc_vec->oras_dest, arc->oras_dest) == 0) {
                                            if (arc_vec->muchie.trons[arc_vec->muchie.nr_tronsoane - 1] > grad_uzura_vecin) {
                                                grad_uzura_vecin = arc_vec->muchie.trons[arc_vec->muchie.nr_tronsoane - 1];
                                            }
                                        }
                                        arc_vec = arc_vec->urm;
                                    }
                                }
                                arc->muchie.copie_trons[j] = grad_uzura_vecin / 2;
                                arc->muchie.ok[j] = 1;
                    // vreau sa ii pun la final valoarea asta impartita la doi.
                            }
                        }
                    }
                    arc = arc->urm;
                }
            }
        }
        modifca_grade(graf);
    }
}
// functie in care modific gradele tronsoanelor.
void modifca_grade(TGL *graf) {
    int i;
    for (i = 0; i < graf->n; i++) {
        AArc arc = graf->x[i];
        while (arc) {
            int j;
            for (j = 0; j < arc->muchie.nr_tronsoane; j++) {
                if (arc->muchie.ok[j] == 0) {
                    arc->muchie.trons[j] *= 2;
                    if (arc->muchie.trons[j] > 100.0) {
                        arc->muchie.trons[j] = 100.0;
                        // limitez uzura maximă la 100%
                    }
                } else if (arc->muchie.ok[j] == 1) {
                    arc->muchie.trons[j] = arc->muchie.copie_trons[j];
                        // pun acea valoare grad_uzura_vecinu/2
                }
            }
            arc = arc->urm;
        }
    }
}

// functie in care selectez ce rute vreau sa pastrez, din graf.
int pastrare_rute(TGL *graf, float L, int *v, TOras *nume_orase) {
    int ct = 0, i, k;
    for (k = 0; k < graf->R; k++) {
        // iterez prima data prin numele de orase oras1-oras2,
        // pe care le-am parsat la citire in structura TOras.
        // am avut nevoie de aceasta structura suplimentara,
        // pentru a respecta ordinea rutelor
        // si la afisare.
        for (i = 0; i < graf->n; i++) {
            AArc arc = graf->x[i];
            // iterez prin orasele pe post de destinatie din graful construit.
            if (strcmp(nume_orase[k].oras1, graf->tag[i]) == 0) {
                // compar numele nodului(tag) sursa cu oras1.
                while (arc != NULL) {
                    if (strcmp(arc->oras_dest, nume_orase[k].oras2) == 0) {
                        float suma = 0;
                        // inițializez suma cu 0 pentru fiecare arc
                        int j;
                        for (j = 0; j < arc->muchie.nr_tronsoane; j++) {
                            // calculez media
                            suma += arc->muchie.trons[j];
                        }
                        float medie = suma / arc->muchie.nr_tronsoane;
                        if (medie < L) {
                            v[ct++] = k + 1;
                            // asignez pe pozitia ct, valoarea k + 1.
                        }
                    }
                    arc = arc->urm;
                }
            }
        }
    }
    return ct;
}
// functie in care eliberez memoria pusa la dispozitie pentru graf, dar si
// pentru vectorul structuri de tip TOras, daca e posibil.
void elibereaza_graf(TGL* graf, int R, TOras* nume_orase) {
    int i;
    if (graf) {
        // iterez prin noduri, prin capurile de lista de adiacenta
        for (i = 0; i < graf->n; i++) {
            AArc curr = graf->x[i];
            // pointerul curr este pointer la inceputul listei
            // de adiacenta de pe pozitia i.
            while (curr) {
                AArc p = curr->urm;
                free(curr->muchie.trons);
                free(curr->muchie.copie_trons);
                free(curr->muchie.ok);
                free(curr);
                curr = p;
            }
        }
        free(graf->x);
        // am eliberat si vectorul de adrese liste arce.
        // eliberez si vectorul tag
        for (i = 0; i < graf->n; i++) {
            if (graf->tag[i]) {
                free(graf->tag[i]);
            }
        }
        if (graf->tag != NULL) {
            free(graf->tag);
        }
    }
    // eliberez si fiecare element din vectorul de structuri
    // de tip TOras, impreuna cu campurile asociate.
    if (nume_orase) {
        for (i = 0; i < R; i++) {
            free(nume_orase[i].oras1);
            free(nume_orase[i].oras2);
        }
        free(nume_orase);
    }
}
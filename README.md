## Functii folosite:
# 1. Functia Init_graf:

Are rolul de a initializa graful, dandu-se ca parametru numerul de rute R(vezi comentarii)

# 2. Functia cauta_index_nod:

Functia imi intoarce un index asociat pozitiei, lui graf->tag de pe o anumita pozitie care sa coincida cu orasul city,
pe care voi vrea sa il inserez ulterior, in main, evident, daca nu a mai fost inca o data inserat, pentru ca
intr-un graf nu pot exista noduri duplicate practic. Daca nodul cu tagul la fel cu orasul respectiv nu exista
inca in graf, se intoarce -1.
    OBS : Cand ma refer la pozitia unui nod in graf, ma refer la al catelea este, daca ma gandesc cumva ca primul inserat e pe pozitia 0,
al doilea pe pozitia 2, etc. si tagul are scopul de a retine numele nodului de pe o anumita pozitie.

# 3. Functia adauga_arc:

Aceasta functie are ca scop sa adauge o ruta noua, o muchie, intre 2 orase date ca parametru, in graf.
Numele orasului sursa va fi retinut in oras1, iar cel destinatie, unde se termina ruta, va fi in oras2. Functia mai primeste
ca parametrii, numarul de tronsoane si vectorul de numere reale ce va contine gradele de uzura, specifice fiecarui tronson.
Mi am luat un pointer la celula de arc, arc_nou, caruia i-am copiat in campul oras_dest, orasul destinatie, apoi i-am populat
campurile din structura TRuta, adica de pe muchie.
      I-am initializat numarul de tronsoane, am alocat vectorul de tronsoane, care va contine gradele de uzura,
am alocat si o copie a acelui vector de tronsoane, pentru ca ma va ajuta considerabil in functia ActualizeazaUzura,
si am mai alocat un vector ok, care va contine elemente asignate cu 1 daca in cadrul functiei de uzura, un anumit grad care va fi 0,
va fi influentat de vecini, si va trebui actualizat,
respectiv 0, in caz contrar.
    Dupa ce am facut aceste alocari, am populat elementele din vectorii precedenti descrisi, cele din vectorul ok, fiind initial 0.
Apoi gasesc indexul orasului de plecare, cu ajutorul functiei find_city_index (returneaza al catelea nod din graf este, sau -1
in caz ca inca nu este in graf).
Daca lista de adiacenta pentru orasul de plecare este goala, voi pune ca prim element in lista pointerul la celula din
lista TCelArc arc_nou, si ii setez campul urmator la NULL. Altfel, parcurg lista de adiacenta pana la ultimul 
element si adaug arc_nou dupa ultimul element din lista.
  
# 4. Functia afiseaza_graf:
Iterez cu o bucla for prin rutele citite din fisierul de intrare(numarul de rute l-am asignat in graf->R).
M-am folosit de vectorul de structuri TOras, care imi retine pentru fiecare ruta citita de la fisier, orasul sursa,
respectiv destinatie pentru fiecare ruta.
    Pentru fiecare element din vectorul nume_orase(vector de structuri), verific care dintre nodurile din graf, coincid din punct de vedere
al orasului sursa si destinatie, cu oras1 si oras2 din structura TOras de pe pozitia k din vectorul de nume_orase.
M-am ajutat de pointerul la lista de adiacenta, arc, pe care l-am initializat pentru fiecare nod dion graf cu capul de lista corespunzator
listei de adiacenta de pe pozitia i din vectorul de adrese de liste de adiacenta din graf.  Apoi, daca coincid, insamna
ca am gasit muchia care ma intereseaza si afisez nodul sursa, nodul destinatie, numarul de tronsoane, precum si
fiecare grad de uzura, si avansez cu pointerul la lista respectiva.
    Repet rationamentul pana termin cu tot graful.

# 5. Functia actualizeaza_uzura

  Aceasta este functia care in final imi va modifica gradele de uzura, dupa K ani care trec de la momentul initial.
  Pentru fiecare an, fac urmatoarele lucruri:
    -> Reinitializez vectorul ok, pentru fiecare muchie din graf, iterand prin vectorul de adrese de liste arce, si prin fiecare lista
        din vectorul de liste x de pe fiecare pozitie. 
    -> Iterez cu o bucla for i prin fiecare nod din graf, si imi iau pointerul arc, care va puncta la inceputul listei de pe
       pozitia i din vectorul de liste x.
    -> Cat timp exista elemente in lista curenta:
        .Iterez cu un for_j prin vectorul de tronsoane si ma intereseaza prima data daca gradul unui anumit tronson este 0. In acest caz,
           am identificat 3 cazuri: (in toate 3, initializez grad_uzura_vecin cu 0.00, care imi va stoca cel mai mare grad dintre
          gradele vecinilor)
        I.  Daca ma aflu pe pozitia j, si aceasta e mai mare ca 0 si mai mica ca nr_trosnoane -1 (adica daca nu sunt
         in capatul din stanga sau din dreapta al vectorului de tronsoane):
            . Verific vecinul din stanga, respectiv din dreapta (stiu ca sigur exista), si la final in grad_uzura_vecin voi avea valoare
            maxima intre gradul vecinului din stanga si a celui din dreapta.
            . Voi modifica gradul de pe pozitia j, din COPIA VECTORULUI DE TRONSOANE, la grad_uzura_vecin/2, si actualizez
             de asemenea si elementul de pe pozitia j din vectorul ok, cu 1. (ca voi vrea sa ii pun valoarea din copie,
             imediat inainte de a avansa la alt an.)
        OBS! Am avut neaparat nevoie de copie_trons, deoarece daca modificam direct pe gradele din vectorul original de tronsoane,
            cand aveam de exemplu mai multe tronsoane cu grad 0, consecutive, pentru cel de-al doilea de exemplu
            trebuia sa iau ca grad al vecinului pe cel initial, nu pe cel injumatatit... 
        II. Daca ma aflu pe pozitia j = 0 (primul tronson, iar numarul de tronsoane sa fie mai mare ca 1, am pus aceasta restrictie aici,
pentru ca daca eram in cazul cu un singur tronson, mi ar fi mers si pe cazul II, cat si pe III, si nu era corect).
Verific vecinul din dreapta, si dupa va fi putin diferit fata de cazul I:
            . Iterez prin fiecare nod din graf si verifc daca nodul destinatie al unei muchii e acelasi ca nodul sursa (tag) arcului curent,
            si astfel ma uit la gradul ultimului element din vectorul de tronsoane, si il compar cu maximul(grad_uzura_vecin),  SAU
            verific daca nodul sursa al unei muchii(rute) coincide cu nodul sursa(tag_arc) al rutei curente, si astfel ma uit
            la gradul tronsonului de pe prima pozitie, si compar la fel, cu grad_uzura_vecin.
            . Tot avansez in lista de adiacenta a fiecarui nod, apoi prin restul de noduri si la fel ca in cazul I, actualizez
             COPIA vectorului de tronsoane, cat si ok-ul.
        III. Dca ma aflu pe pozitia j = arc->muchie.nr_tronsoane - 1(adica daca ma aflu la ultimul tronson), atunci:
            . Iterez prin fiecare nod din graf si incerc sa gasesc un oras sursa care sa coincida cu cel destinatie al rutei curente
            si astfel ma uit la gradul primului tronson din vectorul de tronsoane SAU incerc sa gasesc vreun oras destinatie care
            sa coincida cu cel destinatie al rutei curente si astfel ma uit la gradul ultimului tronson.
            . Avansez prin lista de adiacenta a fiecarui nod, cat si prin fiecare nod din graf si apoi actualizez elementul de pe pozitia j,
             din COPIA VECTORULUI DE TRONSOANE, cat si vectorul ok cu 1.
    -> Dupa ce am terminat de iterat cu bucla for_i prin toate nodurile din graf, apelez functia modifica_grade(graf).
  
# 6. Functia modifica_grade(graf):

Iterez prin fiecare nod din graf si apoi prin fiecare lista din vectorul de adrese de liste de adiacenta arce,
si pentru fiecare tronson al rutei respective, verific daca elementul din vectorul ok, de pe pozitia aferenta
gradului unui tronson este 0, atunci o sa dublez valoarea respectiva de pe pozitia aceea si ii voi trunchia valoarea la 100.00,
(maximul posibil), altfel, daca in ok este valoarea 1, inseamna ca intentionez sa ii pun acea valoare injumatatita,
calculata in COPIA vectorului de tronsoane si asta voi face.
  
# 7. Functia pastrare_rute(TGL *graf, float L, int*v, TOras *nume_orase)
      
In cadrul acestei functii, imi selectez rutele pe care vreau sa le pastrez in graf, pe baza mediei gradelor tronsoanelor,
si voi pastra in vectorul de inturi, v, transmis ca parametru, indicii rutelor pe care le voi pastra.
    Am iterat prima data cu bucla for_k prin numele de orase oras1-oras2, pe care le-am parsat la citire in structura TOras
(structura aceea care ma ajuta sa respect ordinea rutelor din fisierul de intrare, pentru a afisa in mod corespunzator)
    Apoi, am iterat cu o bucla for_i prin nodurile din graf, apoi daca orasul sursa de pe arcul i (retinut in graf->tag[i])
coincide cu orasul sursa al rutei de pe pozitia k din nume_orase, atunci am iterat prin lista de adiacenta a orasului respectiv,
si dupa a trebuit sa gasesc arcul astfel incat si orasul destinatei al arcului respectiv sa coincida cu arcul destinatie
din vectorul nume_orase de pe pozitia k.
    Odata reperata muchia, imi ramane decat sa calculez suma gradelor din vectorul de tronsoane, sa o impart la numarul de
tronsoane si apoi media daca este mai mica ca parametrul L, sa sa pun in vectorul v pe
pozitia aferenta, k + 1 (indexul rutei respective din vectorul nume_orase, adunat cu unu, deoarece, evident, stim ca
indexarea incepe de la 0.)
    La final returnez (ct) numarul de rute pastrate. (dar voi avea si vectorul obtinut, prin efect lateral)    
        
# 8. Functia elibereaza_graf
    
Aceasta functie are rolul de a elibera memoria folosita pentru lucrul cu graful meu, dar si cu vectorul de structuri de
tip TOras, evident, daca nu e vid. Daca pointerul la TGL, graf nu e NULL, atunci iterez prin vectorul de adrese liste arce,
eliberez fiecare element din lista de adiacenta de pe pozitia i, cu tot cu vectorul de tronsoane, copia vectorului
de tronsoane, vectorul ok. Apoi dupa ce am iterat prin fiecare lista din vectorul de liste, eliberez vectorul de liste insusi,
eliberez si vectorul tag(char**). Si de asemenea, eliberez si memoria alocata pentru vectorul de structuri de tip TOras,
daca acesta exista, adica nu e NULL.

# Main
  
Mi-am creat functia task1, unde voi face toate cele necesare taskului 1, iar in main mi-am extras argumentul din linia
de comanda aferent taskului pe care vreau sa il realizez, il convertesc la cifra si deschid fisierele: de intrare si de iesire,
cu tot cu gestionarea erorilor. Daca argumentul aferent este 1, atunci apelez functia task1.
    Astfel, in functia task1, citesc din fisierul de input, valorile pentru R, K si L, apoi initiez graful cu ajutorul functiei Init_graf.
    Imi aloc, de asemenea si vectorul nume_orase, cu elemente de tip TOras si tratez in caz de esuare a memoriei.
    Apoi, cu un for pana la numarul de rute, voi citi din fisier numele celor doua orase care formeaza respectiva ruta,
precum si numarul de tronsoane si voi seta campurile elementului de pe pozitia i din nume_orase. Aloc si vectorul de tronsoane,
tratez in caz de esec al alocarii memoriei si pentru repsectiva ruta si citesc fiecare tronson(grad de uzura), de tip float.
    Daca oras1 nu exista in graf, il voi adauga, pun pe pozitia graf.n din vectorul tag al grafului oras1(cu alocare si copiere)
si cresc numarul de noduri.
    Apelez apoi functia actualizeaza_uzura, precum si afisare_graf, care imi va afisa graful cu tronsoanele actualizate,
apoi in variabila dim voi memora rezultatul functiei pastrare_rute, care ne va da numarul de rute pe care le pot pastra din graf,
si voi afisa elementele din vectorul ce contine indecsii rutelor de pastrat, obtinut de functia pastrare_rute, prin efect lateral.
    In final, eliberez memoria cu functia elibereaza_graf si inchid si fisierele.

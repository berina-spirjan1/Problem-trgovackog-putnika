#ifndef POMOCNA_CPP
#define POMOCNA_CPP
#include "Pomocna.h"
#include <fstream>
#include <iostream>
#include <cmath>

// citanje podataka o grafu iz fajla
Graf* ucitajGrafIzDatoteke(const string& putanjaDatoteke) {
    // Prvo trebamo pregledati strukturu fajla
    ifstream datoteka(putanjaDatoteke, ios_base::in);// otvorimo fajl sa flagom za čitanje
    string ime;
    getline(datoteka, ime);// Učitamo prvu liniju fajla, ona sadrži, između ostalog, pravo ime grafa
    unsigned long pozicijaRazdvajaca = ime.find(':');// nađemo gdje se tačno nalazi ":" u liniji
    if(ime[pozicijaRazdvajaca + 1] == ' ') {
        pozicijaRazdvajaca++;// Ovo radimo da ime grafa nema prazno mjesto na početku, nego da bude umjesto " graf" -> "graf"
    }
    ime.erase(0, pozicijaRazdvajaca + 1);// Ovim brišemo sve do prvog slova imena
    string spremnik;

    // Ovaj dio radimo da bismo podržali format datoteka sa stranice odakle se uzimaju primjeri grafova
    getline(datoteka, spremnik); // Ucitaj liniju i idi dalje, jer nam nije potrebna
    getline(datoteka, spremnik); // isto
    getline(datoteka, spremnik); // isto
    getline(datoteka, spremnik); // Iz ove linije cemo ekstraktovati dimenziju grafa

    pozicijaRazdvajaca = spremnik.find(':');// Kao i sa imenom grafa, tražimo gdje se nalazi ":"
    if(spremnik[pozicijaRazdvajaca + 1] == ' ') {
        pozicijaRazdvajaca++;
    }// preskačemo prazno mjesto ako je tu, a jeste
    spremnik.erase(0, pozicijaRazdvajaca + 1);// Brisemo sve do same dimenzije grafa

    int dimenzija = stoi(spremnik);// Pretvara string u int

    // Ovo radimo da se ispoštuje format datoteke
    getline(datoteka, spremnik); // Ucitaj liniju i idi dalje, jer nam nije potrebna
    getline(datoteka, spremnik); // isto

    // Da bismo kreirali graf, potrebno je učitati Euklidske koordinate svih cvorova iz datoteke
    // Da bismo te tačke iskorstili za kreiranje grafa, potrebno ih je spremiti u neku strukturu
    // Struktura za jeste matrica dimenzija n*2, pri cemu je n broj cvorova, a 2, jer svaka tacka ima dvije koordinate
    // Koristimo auto, zbog kraćeg koda
    // auto**koordinate je pokazivac na niz
    // Potrebno je kreirati matricu koja ce cuvati za svaku tacku njene dvije koordinate
    auto** koordinate = new double*[dimenzija]{};// alociramo n pokazivaca na pokazivac koji pokazuje na double, pri cemu je n = dimenzija
    for (int i = 0; i < dimenzija; i++) {
        koordinate[i] = new double[2]{};// za svaki taj pokazivac, kreiramo niz od dva elementa, sto je dovoljno za dvije koordinate tačaka
    }
    for (int i = 0; i < dimenzija; i++) {// za sve tacke, potrebno je ucitati koordinate iz datoteke
        int index;
        double longituda, latituda;
        datoteka >> index >> longituda >> latituda;// ucitavanje indexa tačke/cvora te koordinata ovog cvora
        index--;// Posto u datoteci indexi krecu od 1, a u memoriji od 0, umanjili smo index za jedan
        koordinate[index][0] = longituda;
        koordinate[index][1] = latituda;// Zapisivanje ovih vrijednosti u matricu prethodno spomenutu
    }
    // Kreira se graf sa brojem cvorova = dimenzija
    auto *graf = new Graf(ime, dimenzija);
    // Sada iskorištavamo učitane podatke iz datoteke i računamo euklidske udaljenosti svih tačaka međusobno
    for (int i = 0; i < dimenzija; i++) {
        for (int j = 0; j < dimenzija; ++j) {
            // Dvostruka petlja nam je potrebna da bismo izračunali od svake tačke udaljenost do druge neke
            double x1=koordinate[i][0],
            x2=koordinate[j][0],
            y1=koordinate[i][1],
            y2=koordinate[j][1];// uzimamo koordinate dvije tacke
            graf->matricaSusjedstva[i][j] = (int)round(sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))));
            // U matricu susjedstva upisujemo euklidsku udaljenost ove dvije tacke koju zaokružujemo na najbliži cijeli broj
        }
    }

    // Sve resurse koji nisu više potrebni, treba osloboditi
    for (int i = 0; i < dimenzija; i++) {
        delete[] koordinate[i];
    }
    delete[] koordinate;
    datoteka.close();// Zatvaramo fajl
    return graf;
}


// Da bismo imali brzi pristup medjurezultatima, trebamo pripremiti mjesto za spasavanje tih medjurezultata
// bitset je kljuc mape i on predstavlja lanac duzine i pri cemu i ide od 0 do broja cvorova u grafu
// kljuc moze da bude bilo koji lanac, u ovom slucaju ih ima 2 ^ N pri cemu je N broj cvorova
// pocetno rjesenje je vektor vrijednosti udaljenosti svakog tog lanca od nekog cvora
// na pocetku je to postavljeno na INT_MAX i to nam predstavlja beskonacnost
void inicijaliziraj(unordered_map<bitset<LIMIT>, vector<int>> &rjesenja, bitset<LIMIT> obidjeni, int trenutni,
                    const vector<int> &pocetno, int brojCvorova) {
    if (trenutni == brojCvorova) {
        return;
    }
    auto prvi = obidjeni;
    auto drugi = obidjeni;
    prvi.set(trenutni);
    rjesenja[prvi] = pocetno;
    rjesenja[drugi] = pocetno;
    inicijaliziraj(rjesenja, prvi, trenutni + 1, pocetno, brojCvorova);
    inicijaliziraj(rjesenja, drugi, trenutni + 1, pocetno, brojCvorova);
}




void pripremiZaAlgoritam(const Graf *graf, unordered_map<bitset<32>, vector<int>> &rjesenja, bitset<32> &sviCvorovi,
                         bitset<32> &obidjeni) {
    vector<int> podrjesenja(graf->brojCvorova);
    fill(podrjesenja.begin(), podrjesenja.end(), INT_MAX); // duzina puta za svaki grad je beskonacna
    inicijaliziraj(rjesenja, bitset<LIMIT>(), 0, podrjesenja, graf->brojCvorova);
    obidjeni.set(0); // postavljamo da je prvi grad obidjen i on se stavlja u lanac
    for (int i = 0; i < graf->brojCvorova; i++) {
        sviCvorovi.set(i);
    }
}

int tsp(
        Graf *graf, //graf svi podaci o grafu. Matrica susjedstva i velicina grafa
        unordered_map<bitset<LIMIT>, vector<int>> &rjesenja, //rjesenja ovo su kesirana rjesenja koja koristimo kao medjurezultate
        bitset<LIMIT> &sviCvorovi, //sviCvorovi ovo je skupina bita gdje su jedinice postavljene za svaki grad
        bitset<LIMIT> obidjeni, //obidjeni ovo je skupina bita gdje su jedinice postavljene za gradove koji su posjeceni
        int pocetniCvor //pocetniCvor ovo je indeks pocetnog grada ili cvora
) {
    if (obidjeni == sviCvorovi) { // ukoliko smo obisli sve cvorove, vracamo duzinu do polazeceg cvora
        return graf->matricaSusjedstva[pocetniCvor][0];
    }
    int rezultat = INT_MAX; // duzina puta je beskonacna
    auto it = rjesenja.find(obidjeni); // trazimo kesirano rjesenje za lanac obidjenih gradova
    if (it != rjesenja.end()) { // ako postoji zapis o tom lancu
        auto podrjesenja = it->second;
        auto podrjesenje = podrjesenja[pocetniCvor];
        if (podrjesenje != INT_MAX) { // ako je udaljenost tog lanca od pocetnog cvora razlicita od beskonacno
            return podrjesenje;// onda je to duzina puta kroz lanac do tog cvora
        }
    }
    // ukoliko se desi da nemamo kesirano rjesenje
    // trazimo standardnom metodom kao sto je to prezentovano u knjizi
    for (int i = 0; i < graf->brojCvorova; i++) {
        // za svaki grad koji nismo obisli
        if (obidjeni[i] == 0) {
            // racunamo duzinu puta kroz lanac u taj grad
            obidjeni.set(i); // stavljamo taj grad u lanac
            // ovo je po formuli iz knjige
            // udaljenost pocetnog cvora do grada i
            // plus duzina lanca
            int duzinaPutanje =
                    graf->matricaSusjedstva[pocetniCvor][i] + tsp(graf, rjesenja, sviCvorovi, obidjeni, i);
            if (duzinaPutanje < rezultat) {
                rezultat = duzinaPutanje;
            }
            // da bismo sve slucajeve preostale radili na ovo nivou kardinalnosti
            // potrebno je da iz lanca gradova izbacimo ovaj zadnji sto je ubacen
            obidjeni.set(i, false);
        }
    }
    // rjesenje koje smo izracunali kesiramo u nasu mapu
    it->second[pocetniCvor] = rezultat;

    return rezultat;
}

int rijesi(Graf* graf) {
    unordered_map<bitset<32>, vector<int>> rjesenja; // kesirana rjesenja koja se koriste za ubrzavanje
    bitset<32> sviCvorovi;// ovo je skupina bita gdje su jedinice postavljene za svaki grad
    bitset<32> obidjeni;// ovo je skupina bita gdje su jedinice postavljene za obidjene gradove kojih na pocetku nema
    pripremiZaAlgoritam(graf, rjesenja, sviCvorovi, obidjeni);
    int rjesenje = tsp(graf, rjesenja, sviCvorovi, obidjeni, 0);
    return rjesenje;
}
#endif // POMOCNA_CPP

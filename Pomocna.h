#ifndef POMOCNA_H
#define POMOCNA_H

#include "Graf.h"
#include <string>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <climits>
#define LIMIT 32

using namespace std;

Graf* ucitajGrafIzDatoteke(const string& putanjaDatoteke);

void inicijaliziraj(unordered_map<bitset<LIMIT>, vector<int>> &rjesenja, bitset<LIMIT> obidjeni, int trenutni,
                    const vector<int> &pocetno, int brojCvorova);

void pripremiZaAlgoritam(const Graf *graf, unordered_map<bitset<32>, vector<int>> &rjesenja, bitset<32> &sviCvorovi,
                         bitset<32> &obidjeni);

int tsp(
        Graf *graf,
        unordered_map<bitset<LIMIT>, vector<int>> &rjesenja,
        bitset<LIMIT> &sviCvorovi,
        bitset<LIMIT> obidjeni,
        int pocetniCvor
);


int rijesi(Graf* graf);

#endif // POMOCNA_H

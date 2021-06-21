#include <iostream>
#include "Pomocna.h"
#include <chrono>
using namespace std;
int main() {
    auto *graf = ucitajGrafIzDatoteke("data/forTest22Nodes.tsp");// ucitamo graf iz datoteke
    cout << "Naziv grafa: " << graf->ime << endl
         << "Dimenzija grafa: " << graf->brojCvorova << endl;
    chrono::steady_clock::time_point pocetak;// mjerenje vremena
    chrono::steady_clock::time_point kraj;
    pocetak = chrono::steady_clock::now();
    int rjesenje = rijesi(graf);
    kraj = chrono::steady_clock::now();
    cout << "Duzina najkraceg puta je: " << rjesenje << endl;
    cout << "Vrijeme izvrsavanja: "
              << chrono::duration_cast<chrono::milliseconds>(kraj - pocetak).count() << " [ms]"
              << endl;
    delete graf;
    return 0;
}

#ifndef GRAF_CPP
#define GRAF_CPP

#include "Graf.h"

#include <utility>

// Konstruktor koji kreira praznu matricu susjedstva
Graf::Graf(string ime, int brojCvorova)  : ime(move(ime)), brojCvorova(brojCvorova) {
    matricaSusjedstva = new int*[brojCvorova]{};
    for (int i = 0; i < brojCvorova; i++) {
        matricaSusjedstva[i] = new int[brojCvorova]{};
    }
}

// Destruktor koji oslobodi memoriju nakon sto objekat prestane postojati
Graf::~Graf() {
    for (int i = 0; i < brojCvorova; i++) {
        delete[] matricaSusjedstva[i];
    }
    delete[] matricaSusjedstva;
}



#endif // GRAF_CPP

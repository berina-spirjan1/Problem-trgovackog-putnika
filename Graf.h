#ifndef GRAF_H
#define GRAF_H
#include<string>

using namespace std;

struct Graf {
    string ime;
    int brojCvorova;
    int** matricaSusjedstva;

    Graf(string ime, int brojCvorova);
    ~Graf();
};
#endif // GRAF_H

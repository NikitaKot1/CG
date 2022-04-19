#ifndef STRUCT_H
#define STRUCT_H

#include <vector>

struct dot{
    int x;
    int y;
};
using namespace std;
typedef vector <dot> vv;

struct figure{
    vv body;
    int size;
};

struct section{
    dot d1;
    dot d2;
};

struct active_sect{
    double start_x;
    int start_y;
    int ny;
    double dx;
};

struct active_sects{
    vector <section> act;
};

struct active_sects2{
    vector <active_sect> act;
};

struct horisont_sect{
    int y;
    int x1;
    int x2;
};

#endif // STRUCT_H

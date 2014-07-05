#ifndef TYPESAPP_H
#define TYPESAPP_H
#include <vector>
#include <iostream>

#define __pow__(a)((a)*(a))
#define PI 3.14159265
#define EPS 1e-9

//#define USE_CAMERA_EXTERNA
#ifdef USE_CAMERA_EXTERNA
    #define __device "http://admin:admin@192.168.1.200/GetData.cgi?CH=2?resolution=800x592&req_fps=30&.mjpg"
#else
    #define __device 0
#endif

namespace cores {
    const std::string AZUL = "AZUL";
    const std::string AMARELO = "AMARELO";
    const std::string VERDE = "VERDE";
    const std::string ROSA = "ROSA";
    const std::string LARANJA = "LARANJA";
}

enum ThreadType
{
    CAL_FIRST, CAL_FINISH
};

struct _corcalibra {
    //MAX: 0..2, MIN: 3..5
    int _cores[6];
};

typedef std::pair<int, int> pii;
typedef std::vector<pii> vp;

int cmp(double x, double y);

#endif // TYPESAPP_H

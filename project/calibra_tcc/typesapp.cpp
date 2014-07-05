#include "camerathread.h"

int cmp(double x, double y)
{
    return ((x <= y + EPS) ? (x + EPS < y) ? -1 : 0 : 1);
}

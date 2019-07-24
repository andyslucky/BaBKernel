#include "math.h"
unsigned int pow(int b, int exp){
    int i = 0;
    unsigned int val = 1;
    for(;i < exp; i++){
        val *= b;
    }
    return val;
}


#define N_ITERATIONS 10

#include "stdio.h"

#ifndef FLOATING
#include "fixed.h"
#endif

int main() {
    #ifdef FLOATING
    double x = 7.0;
    double y = 2.0;
    #else
    mad_fixed_t x = mad_f_fromint((unsigned int) 7);
    mad_fixed_t y = mad_f_fromint((unsigned int) 2);
    printf("x, y = %f, %f\n", mad_f_todouble(x), mad_f_todouble(y));
    #endif

    for(long long i = 0; i < N_ITERATIONS; i++) {
        #ifdef FLOATING
        x = x / y;
        #else
        x = mad_f_div(x, y);
        #endif
    }

    #ifdef FLOATING
    printf("The result is = %f\n", x);
    #else
    printf("The result is = %f\n", mad_f_todouble(x));
    printf("x + y = %f\n", mad_f_todouble(x + y));
    #endif

    return 0;
}

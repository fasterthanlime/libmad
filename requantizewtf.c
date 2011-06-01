
#include "stdio.h"

#include "fixed.h"
#include "layer3.h"

#include "math.h"

#define CHAR_BIT 8

/*
 * fractional powers of two
 * used for requantization and joint stereo decoding
 *
 * root_table[3 + x] = 2^(x/4)
 */


/*
 * table for requantization
 *
 * rq_table[x].mantissa * 2^(rq_table[x].exponent) = x^(4/3)
 */
static
struct fixedfloat {
  unsigned long mantissa  : 27;
  unsigned short exponent :  5;
} const rq_table[8207] = {
# include "rq_table.dat"
};

double our_requantize(unsigned int value, signed int exp) {
  //return 0.125 * pow((double) value, 4.0d / 3.0d) * pow(2.0d, (double) exp);
   signed int frac;

   double requantized = 0.0;

   frac = exp % 4;  /* assumes sign(frac) == sign(exp) */
   exp /= 4;

   requantized = pow((double) value, 4.0 / 3.0);
   requantized *= pow(2.0, (double) (exp / 4.0));
   
   if(frac) {
     requantized *= pow(2.0, ((double) frac / 4.0));
   }
   
   if(exp < 0) {
     requantized /= pow(2.0, -exp * (3.0 / 4.0));
   }

   printf("exp = %5d | ", exp);
   return requantized;
}

mad_fixed_t III_requantize(unsigned int value, signed int exp);
 
int main() {
    unsigned int value;
    signed int exp;
    FILE *f = fopen("req_params.txt", "rb");
    
    int counter = 0;
    while(feof(f) == 0) {
        fscanf(f, "%u %d\n", &value, &exp);
    //value = 1;
    //for(exp = -28; exp > -150; exp -= 4) {
        double mads = mad_f_todouble(III_requantize(value, exp));
        double ours = our_requantize(value, exp);
        //printf("value = %5u, exp = %5d, mad's %5.5f | ours %5.5f | x^(4/3) = %5.5f | x * 2^exp = %5.5f | ours/mad = %5.5f\n",
        //    value, exp, mads, ours, pow((double) value, 4.0d / 3.0d), (double) value * pow(2.0d, (double) exp), ours/mads);
            
        printf("value = %5u, exp = %5d, mad's %5.5f | ours %5.5f | ours - mad = %5.5f\n",
            value, exp, mads, ours, ours - mads);
    }
    fclose(f);
    
    return 0;
}

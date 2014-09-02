#include "random.h"
#include <cstdlib>
#include <cmath>

double random_normal()  // normal distribution, centered on 0, std dev 1
{
    double r1 = (rand() + 1.0) / (RAND_MAX + 1.0); // (0...1] uniform dist
    double r2 = (rand() + 1.0) / (RAND_MAX + 1.0); // (0...1] uniform dist
    return sqrt( -2.0 * log(r1) ) * cos( 2.0 * M_PI * r2 );
}

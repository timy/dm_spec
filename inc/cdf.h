#ifndef CDF_H_
#define CDF_H_

void cdf_read( const char* name, int count,
               void (*sscanf_st)(char*, int, struct parameters*),
               struct parameters* ps );

#endif

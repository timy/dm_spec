#ifndef DEBUG_H_
#define DEBUG_H_

////////////////////////////////////////////////////////////
// some handy DEBUG routines
////////////////////////////////////////////////////////////
#include <iostream>
#define print(x) {                                              \
    std::cout << "PRINT: "                                      \
              << " (" << __FILE__ << ", L" << __LINE__ << ")"   \
              << "\t" << #x << " = " << x << std::endl; }

// pointer to ps is needed here
#define error( ps, fmt, ... ) {                                 \
    fprintf( (ps)->file->item[para_file::LOG]->f[0]->fptr,      \
             "ERROR %s (L%d): " fmt "\n",                       \
             __FILE__, __LINE__, ## __VA_ARGS__ );              \
  }

#include <fstream>
#include <typeinfo>
#include <cstring>
#include <cstdlib>

#define printa(x, n) {                                          \
    for (int idump = 0; idump < n; idump++) {                   \
      std::cout << x[idump] << std::endl;                       \
    }                                                           \
    std::cout << "PRINTA "                                      \
              << __FILE__ << " L" << __LINE__ << std::endl; }

#define plot(x, n) {std::ofstream file_dump;                    \
    file_dump.open("dump.dat");                                 \
    if (strstr(typeid(x).name(), "complex") != NULL) {          \
      for (int idump = 0; idump < n; idump++) {                 \
        file_dump << real(x[idump]) << " " << imag(x[idump])    \
                  << std::endl;                                 \
      }                                                         \
    } else {                                                    \
      for (int idump = 0; idump < n; idump++) {                 \
        file_dump << x[idump] << std::endl;                     \
      }                                                         \
    }                                                           \
    file_dump.close();                                          \
    system("python dump.py");                                   \
    std::cout << "PLOT " << __FILE__ << " L" << __LINE__        \
              << std::endl;                                     \
  }

#endif

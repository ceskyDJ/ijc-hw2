// htab_hash_function.c
// Source: http://www.fit.vutbr.cz/study/courses/IJC/public/DU2.html

#include <stdint.h>
#include "htab_private.h"

/**
 * Counts hash for a provided string
 * @param str The provided string to count hash from
 * @return Counted hash
 */
size_t htab_hash_function(const char *str) {
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}

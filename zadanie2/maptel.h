#ifndef MAPTEL_H
#define MAPTEL_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Creates new dictionary and returns a natural number being its id.
unsigned long maptel_create();

// Deletes the dictionary with the id.
void maptel_delete(unsigned long id);

// Inserts into the dictionary with the id a new entry on change the number
// tel_src to the number tel_dst. If any appropriate entry already exists, 
// overwrites it.
void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst);

// If there is an entry on change number tel_src stored in the dictionary
// with the id, removes it. Otherwise, it does nothing.
void maptel_erase(unsigned long id, char const *tel_src);

// Checks whether the dictionary with the id stores an information about the
// number tel_src. Follows the sequence of changes. Saves the changed number 
// into tel_dst. If there is no change or changes form a cycle, it saves 
// tel_src into tel_dst. The value len is the size of an allocated memory 
// pointed by tel_dst.
void maptel_transform(unsigned long id, char const *tel_src, 
                      char *tel_dst, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef __cplusplus
static const size_t TEL_NUM_MAX_LEN = 22U;
#else /* __cplusplus */
#define TEL_NUM_MAX_LEN ((size_t)22U)
#endif /* __cplusplus */

#endif /* MAPTEL_H */


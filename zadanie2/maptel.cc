#include <iostream>
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <utility>
#include "maptel.h"

#ifdef NDEBUG
const bool DEBUG = false;
#else
const bool DEBUG = true;
#endif

using namespace std;

typedef unordered_map<string, string> maptel;

namespace {
    // Counter of the created dictionaries;
    unsigned long maptel_counter = 0UL;

    // Application of the 'Construct on first use' idiom; 
    // prevents a static initialization order fiasco.
    unordered_map<unsigned long, maptel>& maptel_map() {
        static unordered_map<unsigned long, maptel> maptel_map_inst;
        return maptel_map_inst;
    }

    // Checks if there is a dictionary with the id.
    bool map_exist(unsigned long id) {
        return maptel_map().find(id) != maptel_map().end();
    }

    // Validates the correctness of the number tel.
    bool valid_tel(char const *tel) {
        if (!tel) return false;
        size_t i;
        for (i = 0; *tel != '\0' && i != TEL_NUM_MAX_LEN; i++)
            if (!isdigit(tel[i])) return false;
        if (*tel != '\0') return false;
        if (i == 0) return false;
        return true;
    }
} /*Anonymous namespace*/


// Creates new dictionary and returns a natural number being its id.
unsigned long maptel_create() {
    if (DEBUG)
        cerr << "maptel: maptel_create()" << endl;

    maptel new_maptel;
    pair<unsigned long, maptel> new_entry = make_pair(maptel_counter, new_maptel);
    maptel_map().insert(new_entry);

    if (DEBUG)
        cerr << "maptel: maptel_create: new map id = " << maptel_counter << endl;

    return maptel_counter++;
}


// Deletes the dictionary with the id.
void maptel_delete(unsigned long id) {
    assert(map_exist(id));

    if (DEBUG)
        cerr << "maptel: maptel_delete(" << id << ")" << endl;

    maptel_map().erase(id);

    if (DEBUG)
        cerr << "maptel: maptel_delete: map " << id << " deleted" << endl;	
}


// Inserts into the dictionary with the id a new entry on change the number
// tel_src to the number tel_dst. If any appropriate entry already exists, 
// overwrites it.
void maptel_insert(unsigned long id, char const *tel_src, 
                   char const *tel_dst) {
    assert(map_exist(id));
    assert(valid_tel(tel_src));
    assert(valid_tel(tel_dst));

    if (DEBUG)
        cerr << "maptel: maptel_insert(" << id << ", " << tel_src << ", " 
             << tel_dst << ")" << endl;

    maptel_map()[id][tel_src] = tel_dst;

    if (DEBUG)
        cerr << "maptel: maptel_insert: inserted" << endl;
}


// If there is an entry on change number tel_src stored in the dictionary
// with the id, removes it. Otherwise, it does nothing.
void maptel_erase(unsigned long id, char const *tel_src) {
    assert(map_exist(id));
    assert(valid_tel(tel_src));

    if (DEBUG)
        cerr << "maptel: maptel_erase(" << id << ", " << tel_src << ")" << endl;

    if (maptel_map()[id].find(tel_src) == maptel_map()[id].end()) {
        if (DEBUG)
            cerr << "maptel: maptel_erase: nothing to erase" << endl;
        return;
    }
    maptel_map()[id].erase(tel_src);

    if (DEBUG)
        cerr << "maptel: maptel_erase: erased" << endl;
}


// Checks whether the dictionary with the id stores an information about the
// number tel_src. Follows the sequence of changes. Saves the changed number 
// into tel_dst. If there is no change or changes form a cycle, it saves 
// tel_src into tel_dst. The value len is the size of an allocated memory 
// pointed by tel_dst.
void maptel_transform(unsigned long id, char const *tel_src, 
                      char *tel_dst, size_t len) {
    assert(map_exist(id));
    assert(valid_tel(tel_src));

    if (DEBUG)
        cerr << "maptel: maptel_transform(" << id << ", " << tel_src 
             << ", " << static_cast<const void *>(tel_dst) << ", " 
             << len << ")" << endl;

    maptel::const_iterator it1;
    it1 = maptel_map()[id].find(tel_src);

    if (it1 == maptel_map()[id].end()) {
        assert(len > strlen(tel_src));
        strncpy(tel_dst, tel_src, len);
    }
    else {
        const char *last_num = it1->second.c_str();
        maptel::const_iterator it2 = maptel_map()[id].find(it1->second);

        while (it2 != maptel_map()[id].end() && it1 != it2) {
            it1 = maptel_map()[id].find(it1->second);
            last_num = it2->second.c_str();
            it2 = maptel_map()[id].find(it2->second);
            if (it2 != maptel_map()[id].end()) {
                if(it1 == it2) break;
                last_num = it2->second.c_str();
                it2 = maptel_map()[id].find(it2->second);
            }
        }

        if (it2 == maptel_map()[id].end()) {
            assert(len > strlen(last_num));
            strncpy(tel_dst, last_num, len);
        }
        else {
            if(DEBUG)
                cerr << "maptel: maptel_transform: cycle detected" << endl;
            assert(len > strlen(tel_src));
            strncpy(tel_dst, tel_src, len);
        }
    }

    if (DEBUG)
        cerr << "maptel: maptel_transform: " << tel_src << " -> " << tel_dst 
             << "," << endl;
}

#include "../Headers/HashMap.h"
#include "../../../System/Utils.h"
#include "../../../Unit Test/CuTest/CuTest.h"
#include "limits.h"


unsigned int hashMapFHashCal(int (*hashFun)(const void *), void *key, unsigned int length);


unsigned int hashMapSHashCal(int (*hashFun)(const void *), void *key, unsigned int bPrime);


unsigned int calIndex(unsigned int fHash, unsigned int sHash, unsigned int index, unsigned int length);


int calBPrime(int length);


int getNextPrime(int num);


/** The freeing entry function.
 */

void freeEntry(void *entry) {
    Entry *entryToFree = (Entry *) entry;
    entryToFree->freeItemFun(entryToFree->item);
    entryToFree->freeKeyFun(entryToFree->key);
    free(entryToFree);

}


/** This function will take the freeing item function address, the freeing key function address, the key comparator function, and the hash function as a parameters,
 * then it will initialize a new hash map,
 * then the function will return the address of the hash map.
 */

HashMap *hashMapInitialization(
        void (*freeKey)(void *), void (*freeItem)(void *), int (*keyComp)(const void *, const void *),
        int (*hashFun)(const void *)
) {

    if (freeKey == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "free key function pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    } else if (freeItem == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "free item function pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    } else if (keyComp == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key comparator function pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    } else if (hashFun == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "hash function pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = FAILED_ALLOCATION;
        return NULL;
#else
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "hash map", "hash map data structure");
        exit(FAILED_ALLOCATION);
#endif

    }

    map->length = getNextPrime(10); //the length of the map array should always be a prime number.
    map->arr = (Entry **) calloc(sizeof(Entry *), map->length);
    if (map->arr == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = FAILED_ALLOCATION;
        return NULL;
#else
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "hash map entries array", "hash map data structure");
        exit(FAILED_ALLOCATION);
#endif

    }

    map->count = 0;
    map->bPrime = calBPrime(map->length);
    map->freeItemFun = freeItem;
    map->freeKeyFun = freeKey;
    map->keyComp = keyComp;
    map->hashFun = hashFun;

    return map;

}


/** This function will take the map address, the key address, and the item address as a parameters,
 * then it will insert the item in the map.
 *
 * Note: if the key is already in the map then the map will override the data and free the old item and it's key.
 */

void hashMapInsert(HashMap *map, void *key, void *item) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    } else if (key == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    } else if (item == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "item pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    if (map->count == map->length) {
        map->length = getNextPrime(map->length * 2); //the length of the map array should always be a prime number.
        map->arr = (Entry **) realloc(map->arr, sizeof(Entry *) * map->length);
        if (map->arr == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
            ERROR_TEST->errorCode = FAILED_REALLOCATION;
            return;
#else
            fprintf(stderr, FAILED_REALLOCATION_MESSAGE, "entries array", "hash map data structure");
            exit(FAILED_REALLOCATION);
#endif

        }

        for (int i = map->count; i < map->length; i++)
            map->arr[i] = NULL;

        map->bPrime = calBPrime(map->length);
    }

    unsigned int fHash = hashMapFHashCal(map->hashFun, key, map->length),
            sHash = hashMapSHashCal(map->hashFun, key, map->bPrime);

    unsigned int pHashIndex = 0;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);

    while (map->arr[index] != NULL) {

        if (map->keyComp(key, map->arr[index]->key) == 0) {
            map->freeItemFun(map->arr[index]->item);
            map->freeKeyFun(map->arr[index]->key);
            map->arr[index]->key = key;
            map->arr[index]->item = item;
            return;
        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    }

    Entry *newEntry = (Entry *) malloc(sizeof(Entry));
    if (newEntry == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = FAILED_ALLOCATION;
        return;
#else
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "new entry", "hash map data structure");
        exit(FAILED_ALLOCATION);
#endif

    }

    newEntry->key = key;
    newEntry->item = item;
    newEntry->freeItemFun = map->freeItemFun;
    newEntry->freeKeyFun = map->freeKeyFun;
    map->arr[index] = newEntry;
    map->count++;

}


/** This function will take the map address, and the key address as a parameters,
 * then it will return (1) if the key is in the map,
 * other wise it will return zero (0).
 *
 * Note: this function will not free the passed key.
 */

int hashMapContains(HashMap *map, void *key) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return -1;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    } else if (key == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return -1;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    unsigned int fHash = hashMapFHashCal(map->hashFun, key, map->length),
            sHash = hashMapSHashCal(map->hashFun, key, map->bPrime);

    unsigned int pHashIndex = 0;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0)
                return 1;

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return 0;

}


/** This function will take the map address, and the key address as a parameter,
 * then it will return the item address if the key existed,
 * other wise it will return NULL.
 *
 * Note: this function will not free the passed key.
 */

void *hashMapGet(HashMap *map, void *key) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    } else if (key == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    unsigned int fHash = hashMapFHashCal(map->hashFun, key, map->length),
            sHash = hashMapSHashCal(map->hashFun, key, map->bPrime);

    unsigned int pHashIndex = 0;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0)
                return map->arr[index]->item;

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}


/** This function will take the map address, and the key address as a parameter,
 * then it will return the key address if the key existed,
 * other wise it will return NULL.
 *
 * Note: this function will not free the passed key.
 */

void *hashMapGetKey(HashMap *map, void *key) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    } else if (key == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    unsigned int fHash = hashMapFHashCal(map->hashFun, key, map->length),
            sHash = hashMapSHashCal(map->hashFun, key, map->bPrime);

    unsigned int pHashIndex = 0;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0)
                return map->arr[index]->key;

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}


/** This function will take the map address, and the key address as a parameters,
 * then it will delete and free the key and the item that linked to the key.
 *
 * Note: if the key didn't found in the hash map, then the function will do nothing.
 */

void hashMapDelete(HashMap *map, void *key) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    } else if (key == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    unsigned int fHash = hashMapFHashCal(map->hashFun, key, map->length),
            sHash = hashMapSHashCal(map->hashFun, key, map->bPrime);

    unsigned int pHashIndex = 0;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0) {
                freeEntry(map->arr[index]);
                map->arr[index] = NULL;
                map->count--;
                return;
            }

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

}


/** This function will take the map address, and the key address as a parameters,
 * then it will delete and free the key without freeing the item that linked to that key.
 *
 * Note: if the key didn't found in the hash map, then the function will do nothing.
 */

void *hashMapDeleteWtoFr(HashMap *map, void *key) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    } else if (key == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    unsigned int fHash = hashMapFHashCal(map->hashFun, key, map->length),
            sHash = hashMapSHashCal(map->hashFun, key, map->bPrime);

    unsigned int pHashIndex = 0;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0) {
                void *returnItem = map->arr[index]->item;
                map->freeKeyFun(map->arr[index]->key);
                free(map->arr[index]);
                map->arr[index] = NULL;
                map->count--;
                return returnItem;
            }

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}


/** This function will take the map address, and the key address as a parameters,
 * then it will delete the entry without freeing the key and the item that linked to that key.
 *
 * Note: if the key didn't found in the hash map, then the function will do nothing.
 */

Entry *hashMapDeleteWtoFrAll(HashMap *map, void *key) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    } else if (key == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        exit(INVALID_ARG);
#endif

    }

    unsigned int fHash = hashMapFHashCal(map->hashFun, key, map->length),
            sHash = hashMapSHashCal(map->hashFun, key, map->bPrime);

    unsigned int pHashIndex = 0;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0) {
                Entry *returnEntry = map->arr[index];
                map->arr[index] = NULL;
                map->count--;
                return returnEntry;
            }

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}


/** This function will take the map address as a parameter,
 * then it will return double void pointer that has a copy of all the items in the map.
 */

void **hashMapToArray(HashMap *map) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    }

    void **arr = (void **) malloc(sizeof(void *) * map->count);
    if (arr == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = FAILED_ALLOCATION;
        return NULL;
#else
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "to array", "hash map data structure");
        exit(FAILED_ALLOCATION);
#endif

    }

    for (int i = 0, index = 0; i < map->length; i++) {

        if (map->arr[i] != NULL)
            arr[index++] = map->arr[i]->item;

    }

    return arr;

}


/** This function will take the map address as a parameter,
 * then it will return an entry array that contains a copy of all the items and it's key in the map.
 *
 * Note: the returned Entries will not be a really copy of the keys and items, it will reference to the same addresses.
 */

Entry **hashMapToEntryArray(HashMap *map) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    }

    Entry **arr = (Entry **) malloc(sizeof(Entry *) * map->count);
    if (arr == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = FAILED_ALLOCATION;
        return NULL;
#else
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "to entries array", "hash map data structure");
        exit(FAILED_ALLOCATION);
#endif

    }

    for (int i = 0, index = 0; i < map->length; i++) {
        if (map->arr[i] != NULL)
            arr[index++] = map->arr[i];

    }

    return arr;

}


/** This function will take the map address as a parameter,
 * then it will return the number of items in the map.
 */

int hashMapGetLength(HashMap *map) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return -1;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    }

    return map->count;

}


/** This function will take the map address as a parameter,
 * then it will return one (1) if the map is empty,
 * other wise it will return zero (0).
 */

int hashMapIsEmpty(HashMap *map) {
    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return -1;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    }

    return map->count == 0;

}


/** This function will take the map address as a parameter,
 * then it will clear and free all the items and it's key from the map without freeing the map.
 */

void clearHashMap(HashMap *map) {

    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    }

    for (int i = 0; i < map->length; i++) {
        if (map->arr[i] != NULL) {
            freeEntry(map->arr[i]);
            map->arr[i] = NULL;
        }

    }

    map->count = 0;

}


/** This function will take the map address as a parameter,
 * then it will destroy and free the map and all it's entries.
 */

void destroyHashMap(void *map) {

    if (map == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map", "hash map data structure");
        exit(NULL_POINTER);
#endif

    }

    clearHashMap(map);
    free(((HashMap *) map)->arr);
    free(map);

}


/** This function will take the hash function pointer, the key pointer, and the hash map array length as a parameters,
 * then it will return the first hash of this key.
 *
 * Note: this function should only be called from the hash map functions.
 */

unsigned int hashMapFHashCal(int (*hashFun)(const void *), void *key, unsigned int length) {
    return (hashFun(key) % length);
}


/** This function will take the hash function pointer, the key address, and the biggest prime number,
 * that smaller than the set array length as a parameters, then it will return the second hash of this key.
 *
 * Note: this function should only be called from the hash map functions.
 */

unsigned int hashMapSHashCal(int (*hashFun)(const void *), void *key, unsigned int bPrime) {
    return (bPrime - (hashFun(key) % bPrime));
}


/** This function will take the first hash of the key, the second hash of the key, the index, and the length of the map array
 * as a parameters, then it will return the next index that should be available.
 *
 * Note: this function should only be called from the hash map functions.
 */

unsigned int calIndex(unsigned int fHash, unsigned int sHash, unsigned int index, unsigned int length) {
    return ((fHash + (index * sHash)) % length);
}


/** This function will take the length of the map array as a parameter,
 * then it will return the biggest prime number that is smaller than the length.
 */

int calBPrime(int length) {
    for (int i = length - 1; i >= 1; i--) {
        for (int j = i / 2; j > 1; j--) {
            if (i % j == 0)
                break;
            else if (i % j != 0 && j == 2)
                return i;

        }
    }

    return 1;

}


/** This function will take an integer number as a parameter,
 * then it will return the next prime number that bigger than or equal to the passed parameter,
 * other wise it will return 1 if the function didn't found any prime numbers.
 */

int getNextPrime(int num) {
    for (int i = num; i <= INT_MAX; i++) {
        for (int j = i / 2; j > 1; j--) {

            if (i % j == 0)
                break;
            else if (j == 2 && i % j != 0)
                return i;

        }

    }

    return 1;

}
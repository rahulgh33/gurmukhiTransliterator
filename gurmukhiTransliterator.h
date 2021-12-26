#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <stdbool.h>
#include <locale.h>
#include <wchar.h>

#define MAXCHARS 57

typedef struct  {
    char *gurmukhi;
    long gurmukhiCode;
    char *english;
} gurmukhiTableEntry[MAXCHARS];
gurmukhiTableEntry * makeTable(char *fileName);
void transliterateFile(char *fileName, gurmukhiTableEntry *table);
char *transliterateWord(const wchar_t *word, gurmukhiTableEntry *table);
bool isTwoBytes(long gurmukhiCode);
bool isThreeBytes(long gurmukhiCode);
bool isFourBytes(long gurmukhiCode);
bool isNukta(long gurmukhiCode);
bool isLetter(long gurmukhiCode);
bool isHalant(long gurmukhiCode);
bool isNasalDiacritic(long gurmukhiCode);
bool isAddak(long gurmukhiCode);
bool isGuttural(long gurmukhiCode);
bool isPalatal(long gurmukhiCode);
bool isRetroflex(long gurmukhiCode);
bool isDental(long gurmukhiCode);
bool isLabial(long gurmukhiCode);
bool isNasal(long gurmukhiCode);
bool isPlaceholderVowel(long gurmukhiCode);
char *getEnglish(gurmukhiTableEntry *table, long gurmukhiCode);


#include "gurmukhiTransliterator.h"



int main () {
	FILE *cfPtr;
	gurmukhiTableEntry *transliterationTable;
	transliterationTable = makeTable("improvedGurmukhiChart.csv");
	transliterateFile("sampletext.txt", transliterationTable);
	return 0;
}

bool isTwoBytes(long gurmukhiCode) {
	return gurmukhiCode == 2566 || gurmukhiCode == 2568 || gurmukhiCode == 2584 || gurmukhiCode == 2589 ||gurmukhiCode == 2599 || gurmukhiCode == 2605 ||  gurmukhiCode == 2570 || gurmukhiCode == 2576 || gurmukhiCode == 2580 || gurmukhiCode == 2582 || gurmukhiCode == 2587 || gurmukhiCode == 2590 || gurmukhiCode == 2597 || gurmukhiCode == 2603 || gurmukhiCode == 2614 || gurmukhiCode == 2622 || gurmukhiCode == 2624 || gurmukhiCode == 2626 || gurmukhiCode == 2632 || gurmukhiCode == 2636 || gurmukhiCode == 2650;
}

bool isThreeBytes(long gurmukhiCode){
	return gurmukhiCode == 2585 || gurmukhiCode == 2591 ||  gurmukhiCode == 2593 ||  gurmukhiCode == 2595 || gurmukhiCode == 2652; 
}

bool isFourBytes(long gurmukhiCode){
	return gurmukhiCode == 2592 || gurmukhiCode == 2594;
}

bool isNukta(long gurmukhiCode) {
	return gurmukhiCode == 2620;
}

bool isLetter(long gurmukhiCode) {
	long start = 2581;
	long end = 2617;
	return start <= gurmukhiCode && gurmukhiCode <= end || gurmukhiCode == 2652;
}

bool isHalant(long gurmukhiCode) {
	return gurmukhiCode == 2637;
}

bool isNasalDiacritic(long gurmukhiCode) { 
	return gurmukhiCode == 2672 || gurmukhiCode == 2562;
}

bool isAddak(long gurmukhiCode) {
	return gurmukhiCode == 2673;
}

bool isGuttural(long gurmukhiCode) {
	long start = 2581;
	long end = 2584;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isPalatal(long gurmukhiCode) {
	long start = 2586;
	long end = 2589;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isRetroflex(long gurmukhiCode) {
	long start = 2591;
	long end = 2594;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isDental(long gurmukhiCode) {
	long start = 2596;
	long end = 2599;
	return start <= gurmukhiCode && gurmukhiCode <= end || gurmukhiCode == 2616;
}

bool isLabial(long gurmukhiCode) {
	long start = 2602;
	long end = 2605;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isNasal(long gurmukhiCode) {
	return gurmukhiCode == 2600 || gurmukhiCode == 2606;
}

bool isPlaceholderVowel(long gurmukhiCode) {
	long start = 2565;
	long end = 2580;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

char *getEnglish(gurmukhiTableEntry *table, long gurmukhiCode) {
	char *english;
	for(int i = 0; i < MAXCHARS; i++){
		if(table[i]->gurmukhiCode == gurmukhiCode){
			english = malloc(sizeof(table[i]->english + 1));
			strcpy(english, table[i]->english);
			return english;	
		}
	}
	return "";
}


gurmukhiTableEntry * makeTable(char *fileName){
	gurmukhiTableEntry * transliterationTable = calloc(sizeof(gurmukhiTableEntry), MAXCHARS);
	FILE *cfPtr;
	if((cfPtr = fopen(fileName,"r")) == NULL){
		perror("File could not be opened.\n");
		return NULL;
	} else {
		char line[100];
		int i=0;
		fgets(line, 100, cfPtr);
        	while(fgets(line, 100, cfPtr)){
        		char * temp = strtok(line, ",");
			transliterationTable[i]->gurmukhi=malloc(sizeof(temp));
			strcpy(transliterationTable[i]->gurmukhi, temp);

			temp = strtok(NULL, "\n");
			transliterationTable[i]->english=malloc(sizeof(temp));
			strcpy(transliterationTable[i]->english, temp);

			transliterationTable[i]->gurmukhiCode = strtol(transliterationTable[i]->gurmukhi, NULL, 16);
			i++;
        	}
        	fclose ( cfPtr );
	}
	return transliterationTable;
}

void transliterateFile(char *fileName, gurmukhiTableEntry *table){
	FILE *cfPtr;
	setlocale(LC_CTYPE, "en_US.UTF-8");
	if((cfPtr = fopen(fileName,"rb")) == NULL){
		perror("File could not be opened.\n");
	} else {
		wchar_t line[100];
		while(fgetws(line, 100, cfPtr)!= NULL){
			wchar_t *buffer;
			wchar_t *word = wcstok(line, L" ", &buffer);
			while(word != NULL) {
				printf("%s ", transliterateWord(word, table));
				word = wcstok(NULL, L" ", &buffer);
			}
			printf("\n");
		}
		fclose(cfPtr);
	}
}
char *transliterateWord(const wchar_t *word, gurmukhiTableEntry *table){
	char *result = malloc(2*wcslen(word)+1);
	if(result == NULL){
		perror("Out of memory");	
	} else{
		int i = 0;
		int j = 0;
		while(i < wcslen(word)){
			result[j] = '\0';
			char *transliteratedLetters = getEnglish(table, (word[i]));
			if(isTwoBytes(word[i])) {
				if(isNukta(word[i+1])){
					if(word[i] == 2582) {
						const char *x = "x";
						strcat(result, x);
						j++;
						if(isLetter(word[i+2])){
							result[j] = '\0';
							const char *a = "a";
							strcat(result, a);
							j++;
							i+=2;
						} else {
							i+=2;
						}
					} else if(word[i] == 2603) {
						const char *f = "f";
						strcat(result, f);
						j++;
						if(isLetter(word[i+2])){
							result[j] = '\0';
							const char *a = "a";
							strcat(result, a);
							j++;
							i+=2;
						} else {
							i+=2;
						}
					} else{
						i++;
					}
				} else {
					strcat(result, transliteratedLetters);
					j+=2;
					if(isLetter(word[i])){
						if(isLetter(word[i+1]) || isAddak(word[i+1]) || isNasalDiacritic(word[i+1]) || isPlaceholderVowel(word[i+1])){
							result[j] = '\0';
							const char *a = "a";
							strcat(result, a);
							j++;
							i++;
						} else{
							i++;
						}
					} else{
						i++;
					}
				}
			} else if(isThreeBytes(word[i])) {
				strcat(result, transliteratedLetters);
				j+=3;
				if(isLetter(word[i])){
					if(isLetter(word[i+1])){
						result[j] = '\0';
						const char *a = "a";
						strcat(result, a);
						j++;
						i++;
					} else{
						i++;
					}
				} else{
					i++;
				}
			} else if(isFourBytes(word[i])) {
				strcat(result, transliteratedLetters);
				j+=4;
				if(isLetter(word[i])){
					if(isLetter(word[i+1])){
						result[j] = '\0';
						const char *a = "a";
						strcat(result, a);
						j++;
						i++;
					} else{
						i++;
					}
				} else{
					i++;
				}
			} else{
				if(isHalant(word[i])) {
					i++;				
				} else if(isNasalDiacritic(word[i])){
					if(isGuttural(word[i+1])){
						const char *ng = "ṅ";
						strcat(result, ng);
						j+=3;
						i++;
					} else if(isPalatal(word[i+1])){
						const char *ny = "ñ";
						strcat(result, ny);
						j+=2;
						i++;
					} else if(isRetroflex(word[i+1])){
						const char *retroN = "ṇ";
						strcat(result, retroN);
						j+=3;
						i++;
					} else if(isDental(word[i+1])){
						const char *n = "n";
						strcat(result, n);
						j+=1;
						i++;
					} else if(isLabial(word[i+1])){
						const char *m = "m";
						strcat(result, m);
						j+=1;
						i++;
					} else if(isNasal(word[i+1])){
						char *nextTransliteratedLetters = getEnglish(table, (word[i+1]));
						strcat(result, nextTransliteratedLetters);
						i++;
						j+=1;
					} else { 
						const char *nasal = "ṁ";
						strcat(result, nasal);
						j+=3;
						i++;
					}
				} else if(isAddak(word[i])){
					result[j] = '\0';		
					char *nextTransliteratedLetters = getEnglish(table, (word[i+1]));
					strcat(result, nextTransliteratedLetters);
					i++;
					j+=(strlen(nextTransliteratedLetters)-1);
				} else {
					if(isNukta(word[i+1])){
						if(word[i] == 2616) {
							const char *sh = "sh";
							strcat(result, sh);
							j+=2;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else if(word[i] == 2588) {
							const char *z = "z";
							strcat(result, z);
							j+=1;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else if(word[i] == 2583) {
							const char *gh = "gh";
							strcat(result, gh);
							j+=2;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else if(word[i] == 2610) {
							const char *l = "ḷ";
							strcat(result, l);
							j+=3;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else {
							i++;
						}
					} else{
						result[j] = '\0';
						strcat(result, transliteratedLetters);
						j+=1;
						if(isLetter(word[i+1]) || isAddak(word[i+1]) || isNasalDiacritic(word[i+1]) || isPlaceholderVowel(word[i+1])){
							if(isLetter(word[i])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								i++;
								j++;
							} else {
								i++;
							}
						} else {
							i++;
						}
					}
				}
			}
		}
		result[j] = '\0';
	}
	return result;
}


bool isGuttural(long gurmukhiCode) {
	long start = 2581;
	long end = 2584;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isPalatal(long gurmukhiCode) {
	long start = 2586;
	long end = 2589;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isRetroflex(long gurmukhiCode) {
	long start = 2591;
	long end = 2594;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isDental(long gurmukhiCode) {
	long start = 2596;
	long end = 2599;
	return start <= gurmukhiCode && gurmukhiCode <= end || gurmukhiCode == 2616;
}

bool isLabial(long gurmukhiCode) {
	long start = 2602;
	long end = 2605;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

bool isNasal(long gurmukhiCode) {
	return gurmukhiCode == 2600 || gurmukhiCode == 2606;
}

bool isPlaceholderVowel(long gurmukhiCode) {
	long start = 2565;
	long end = 2580;
	return start <= gurmukhiCode && gurmukhiCode <= end;
}

char *getEnglish(gurmukhiTableEntry *table, long gurmukhiCode) {
	char *english;
	for(int i = 0; i < MAXCHARS; i++){
		if(table[i]->gurmukhiCode == gurmukhiCode){
			english = malloc(sizeof(table[i]->english + 1));
			strcpy(english, table[i]->english);
			return english;	
		}
	}
	return "";
}


gurmukhiTableEntry * makeTable(char *fileName){
	gurmukhiTableEntry * transliterationTable = calloc(sizeof(gurmukhiTableEntry), MAXCHARS);
	FILE *cfPtr;
	if((cfPtr = fopen(fileName,"r")) == NULL){
		perror("File could not be opened.\n");
		return NULL;
	} else {
		char line[100];
		int i=0;
		fgets(line, 100, cfPtr);
        	while(fgets(line, 100, cfPtr)){
        		char * temp = strtok(line, ",");
			transliterationTable[i]->gurmukhi=malloc(sizeof(temp));
			strcpy(transliterationTable[i]->gurmukhi, temp);

			temp = strtok(NULL, "\n");
			transliterationTable[i]->english=malloc(sizeof(temp));
			strcpy(transliterationTable[i]->english, temp);

			transliterationTable[i]->gurmukhiCode = strtol(transliterationTable[i]->gurmukhi, NULL, 16);
			i++;
        	}
        	fclose ( cfPtr );
	}
	return transliterationTable;
}

void transliterateFile(char *fileName, gurmukhiTableEntry *table){
	FILE *cfPtr;
	setlocale(LC_CTYPE, "en_US.UTF-8");
	if((cfPtr = fopen(fileName,"rb")) == NULL){
		perror("File could not be opened.\n");
	} else {
		wchar_t line[100];
		while(fgetws(line, 100, cfPtr)!= NULL){
			wchar_t *buffer;
			wchar_t *word = wcstok(line, L" ", &buffer);
			while(word != NULL) {
				printf("%s ", transliterateWord(word, table));
				word = wcstok(NULL, L" ", &buffer);
			}
			printf("\n");
		}
		fclose(cfPtr);
	}
}
char *transliterateWord(const wchar_t *word, gurmukhiTableEntry *table){
	char *result = malloc(2*wcslen(word)+1);
	if(result == NULL){
		perror("Out of memory");	
	} else{
		int i = 0;
		int j = 0;
		while(i < wcslen(word)){
			result[j] = '\0';
			char *transliteratedLetters = getEnglish(table, (word[i]));
			if(isTwoBytes(word[i])) {
				if(isNukta(word[i+1])){
					if(word[i] == 2582) {
						const char *x = "x";
						strcat(result, x);
						j++;
						if(isLetter(word[i+2])){
							result[j] = '\0';
							const char *a = "a";
							strcat(result, a);
							j++;
							i+=2;
						} else {
							i+=2;
						}
					} else if(word[i] == 2603) {
						const char *f = "f";
						strcat(result, f);
						j++;
						if(isLetter(word[i+2])){
							result[j] = '\0';
							const char *a = "a";
							strcat(result, a);
							j++;
							i+=2;
						} else {
							i+=2;
						}
					} else{
						i++;
					}
				} else {
					strcat(result, transliteratedLetters);
					j+=2;
					if(isLetter(word[i])){
						if(isLetter(word[i+1]) || isAddak(word[i+1]) || isNasalDiacritic(word[i+1]) || isPlaceholderVowel(word[i+1])){
							result[j] = '\0';
							const char *a = "a";
							strcat(result, a);
							j++;
							i++;
						} else{
							i++;
						}
					} else{
						i++;
					}
				}
			} else if(isThreeBytes(word[i])) {
				strcat(result, transliteratedLetters);
				j+=3;
				if(isLetter(word[i])){
					if(isLetter(word[i+1])){
						result[j] = '\0';
						const char *a = "a";
						strcat(result, a);
						j++;
						i++;
					} else{
						i++;
					}
				} else{
					i++;
				}
			} else if(isFourBytes(word[i])) {
				strcat(result, transliteratedLetters);
				j+=4;
				if(isLetter(word[i])){
					if(isLetter(word[i+1])){
						result[j] = '\0';
						const char *a = "a";
						strcat(result, a);
						j++;
						i++;
					} else{
						i++;
					}
				} else{
					i++;
				}
			} else{
				if(isHalant(word[i])) {
					i++;				
				} else if(isNasalDiacritic(word[i])){
					if(isGuttural(word[i+1])){
						const char *ng = "ṅ";
						strcat(result, ng);
						j+=3;
						i++;
					} else if(isPalatal(word[i+1])){
						const char *ny = "ñ";
						strcat(result, ny);
						j+=2;
						i++;
					} else if(isRetroflex(word[i+1])){
						const char *retroN = "ṇ";
						strcat(result, retroN);
						j+=3;
						i++;
					} else if(isDental(word[i+1])){
						const char *n = "n";
						strcat(result, n);
						j+=1;
						i++;
					} else if(isLabial(word[i+1])){
						const char *m = "m";
						strcat(result, m);
						j+=1;
						i++;
					} else if(isNasal(word[i+1])){
						char *nextTransliteratedLetters = getEnglish(table, (word[i+1]));
						strcat(result, nextTransliteratedLetters);
						i++;
						j+=1;
					} else { 
						const char *nasal = "ṁ";
						strcat(result, nasal);
						j+=3;
						i++;
					}
				} else if(isAddak(word[i])){
					result[j] = '\0';		
					char *nextTransliteratedLetters = getEnglish(table, (word[i+1]));
					strcat(result, nextTransliteratedLetters);
					i++;
					j+=(strlen(nextTransliteratedLetters)-1);
				} else {
					if(isNukta(word[i+1])){
						if(word[i] == 2616) {
							const char *sh = "sh";
							strcat(result, sh);
							j+=2;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else if(word[i] == 2588) {
							const char *z = "z";
							strcat(result, z);
							j+=1;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else if(word[i] == 2583) {
							const char *gh = "gh";
							strcat(result, gh);
							j+=2;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else if(word[i] == 2610) {
							const char *l = "ḷ";
							strcat(result, l);
							j+=3;
							if(isLetter(word[i+2])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								j++;
								i+=2;
							} else {
								i+=2;
							}
						} else {
							i++;
						}
					} else{
						result[j] = '\0';
						strcat(result, transliteratedLetters);
						j+=1;
						if(isLetter(word[i+1]) || isAddak(word[i+1]) || isNasalDiacritic(word[i+1]) || isPlaceholderVowel(word[i+1])){
							if(isLetter(word[i])){
								result[j] = '\0';
								const char *a = "a";
								strcat(result, a);
								i++;
								j++;
							} else {
								i++;
							}
						} else {
							i++;
						}
					}
				}
			}
		}
		result[j] = '\0';
	}
	return result;
}

/* -----------------------------------------------------------------------

CS 211 - PROGRAMMING PRACTICUM
PROJECT 03 SPELLING BEE GAME AND SOLVER 
DAVINA TITUS 
UIN - 652284919

This program is a generalized version of the NYT Spelling Bee game.
It allows the user to either manually enter a hive of letters or 
generate one randomly, then play the game and/or solve it automatically.

The program has three main components:

* Build the Hive  - set up the letters used in the game
* Play the Game   - user interactively guesses valid words
* Solve the Game  - program finds all valid words for the hive

* ----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

const int MIN_HIVE_SIZE = 2;
const int MAX_HIVE_SIZE = 12;
const int MIN_WORD_LENGTH = 4;

typedef struct WordList_struct {
    char** words;
    int numWords;
    int capacity;
} WordList;

WordList* createWordList() {
    WordList* newList = malloc(sizeof(WordList));
    newList->capacity = 4;
    newList->numWords = 0;
    newList->words = malloc(newList->capacity * sizeof(char*));

    return newList;
}

/* appendWord()
-ADDS A NEW WORD TO THE END OF A WORDLIST.
-IF THE ARRAY IS FULL, DOUBLES THE CAPACITY 
IF THE LIST IS AT CAPACITY, A NEW ARRAY WITH DOUBLE THE SIZE IS
ALLOCATED, THE OLD CONTENTS ARE COPIED OVER, AND THE OLD ARRAY IS FREED.
THE NEW WORD IS MALLOC'D AND COPIED (NOT JUST POINTER COPIED) INTO THE LIST.
*/

void appendWord(WordList* thisWordList, char* newWord) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-A): Write appendWord
    - adds newWord to the end of the thisWordList (malloc'ing memory and copying the string; do NOT just copy the input pointer)
    - increments thisWordList's numWords. If there is no space for the new word, reallocate with double the capacity
    - by malloc'ing new memory for the words array and freeing the old memory
    */

    if (thisWordList->numWords==thisWordList->capacity){
        int newCap = thisWordList->capacity*2;

        char** newWords = (char**)malloc(newCap*sizeof(char*));

        for (int i=0; i < thisWordList->numWords;i++){
            newWords[i] = thisWordList->words[i];
        }
        
        free(thisWordList->words);

        thisWordList->words = newWords;
        thisWordList->capacity=newCap;
    }

    thisWordList->words[thisWordList->numWords] = (char*)malloc((strlen(newWord)+1)*sizeof(char));
    strcpy(thisWordList->words[thisWordList->numWords], newWord);

    thisWordList->numWords++;
}

/* buildDictionary()
    OPENS THE FILE filename, READS EACH WORD, AND ADDS WORDS OF AT LEAST
   minLength CHARACTERS TO dictionaryList USING appendWord().
   RETURNS THE LENGTH OF THE LONGEST VALID WORD, OR -1 IF THE FILE
   COULD NOT BE OPENED OR NO VALID WORDS WERE FOUND */

int buildDictionary(char* filename, WordList* dictionaryList, int minLength) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-B): Write buildDictionary
    - Opens a valid file and adds all words that are at least minLength long to the dictionaryList
    - Returns the length of the longest valid word if all goes well, -1 otherwise
    */

    FILE* fp = fopen(filename,"r");

    char buff[256];
    int longestWrd = 0;

    if (fp==NULL){
        return -1;
    }

    while (fscanf(fp,"%s", buff)==1){
        int lenWord = strlen(buff);

        if (lenWord>= minLength){
            appendWord(dictionaryList,buff);

            if (lenWord>longestWrd){
                longestWrd=lenWord;
            }
        }
    }

    fclose(fp); //this line is here so that the starter code compiles; replace/modify this line

    if (dictionaryList->numWords==0){
        return -1;
    }

    return longestWrd;
}

/* freeWordList()
    FREES ALL MEMORY USED BY A WORDLIST:
    FIRST FREES EACH INDIVIDUAL WORD STRING,
    THEN FREES THE words POINTER ARRAY,
    THEN FREES THE WORDLIST STRUCT ITSELF */

void freeWordList(WordList* list) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-C): Write freeWordList
    - Frees the memory used by the WordList, both the words' char* arrays and the char** words array itself
    */

    for (int i=0; i < list->numWords;i++){
        free(list->words[i]);
    }

    free(list->words);
    free(list);
}

/* findLetter()
SEARCHES FOR CHARACTER aLet IN STRING str.
RETURNS THE INDEX OF THE FIRST OCCURANCE IF FOUND,
OR -1 IF THE CHARACTER IS NOT IN THE STRING */

int findLetter(char* str, char aLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 2-A): Write findLetter
    - Returns the index of aLet in str, if it exists
    - Otherwise returns -1
    */
    
    for (int i=0;str[i]!='\0';i++){
        if (str[i]==aLet){
            return i;
        }
    }
    return -1;
}

/* buildHive()
BUILDS AN ALPHABETICALLY SORTED HIVE FROM THE INPUT STRING str.
EACH UNIQUE CHARACTER FROM str IS INSERTED INTO hive IN SORTED ORDER.
DUPLICATE CHARACTERS ARE SKIPPED.
hive IS NULL TERMINATED AT THE END */

void buildHive(char* str, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 2-B): Write buildHive
    - take each character of the string and add it to the hive in alphabetical order
    - no letter should be added twice
    - ensure that hive is null-terminated at the end
      (findLetter is helpful here)
    */

    hive[0]='\0';
    int lenHive =0;

    for (int i=0; str[i]!='\0';i++){
        char c = str[i];

        if (findLetter(hive,c)!= -1){
            continue;
        }

        int insert = lenHive;
        for (int j =0;j< lenHive;j++){
            if (c< hive[j]){
                insert=j;
                break;
            }
        }

        for (int j= lenHive; j > insert;j--){
            hive[j]= hive[j-1];
        }

        hive[insert]=c;
        lenHive++;
        hive[lenHive]='\0';
    }
   
}

/* countUniqueLetters()
    COUNTS AND RETURNS THE NUMBER OF UNIQUE CHARACTERS IN str.
   WORKS FOR ANY CHARACTERS, NOT JUST LETTERS.
   FOR EACH CHARACTER, CHECKS ALL PREVIOUS CHARACTERS TO SEE
   IF IT HAS ALREADY BEEN COUNTED */

int countUniqueLetters(char* str) {
    //---------------------------------------------------------------------
    /* TODO (Task 3-A): Write countUniqueLetters
    - Counts and returns the number of unique letters in the input string 
      (note: this only needs to work for lower-case alpha characters, i.e. letters, for this project, 
            but it should work generally for all characters)
    */

    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        bool seen = false;
        for (int j = 0; j < i; j++) {
            if (str[j] == str[i]) {
                seen = true;
                break;
            }
        }
        if (!seen) count++;
    } 
    return count;

}

/* WordList()
CREATES AND RETURNS A WORDLIST OF ALL "FIT WORDS" FROM dictionaryList.
A FIT WORD HAS EXACTLY hiveSize UNIQUE LETTERS.
USES countUniqueLetters() TO CHECK EACH WORD */

WordList* findAllFitWords(WordList* dictionaryList, int hiveSize) {
    WordList* fitWords = createWordList();
    //---------------------------------------------------------------------
    /* TODO (Task 3-B): Write findAllFitWords
    - Creates a WordList and adds all fit words from dictionaryList to the new WordList 
      (A fit word has exactly hiveSize unique letters)
    */

    for (int i =0; i < dictionaryList->numWords;i++){
        if (countUniqueLetters(dictionaryList->words[i])==hiveSize){
            appendWord(fitWords,dictionaryList->words[i]);
        }
    }
    return fitWords;
}

/* isValidWord()
RETURNS true IF word IS VALID FOR THE SPELLING BEE HIVE:
- EVERY LETTER IN word MUST BE IN hive
- word MUST CONTAIN reqLet AT LEAST ONCE 
CONVERTS EACH CHARACTER TO LOWERCASE BEFORE CHECKING */

bool isValidWord(char* word, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-A, part 1 of 2): Write isValidWord 
    - Returns true if word 
      (a) uses ONLY letters that are in the hive, and 
      (b) MUST use the required letter. 
    - Returns false otherwise
    */

    bool hasreqLet = false;
    for (int i =0; word[i]!='\0';i++){
        char c = tolower(word[i]);
        if (findLetter(hive,c)==-1){
            return false;
        } 
        if (c==reqLet){
            hasreqLet=true;
        }
    }
    return hasreqLet;
}

/* isPangram()
RETURNS true IS str IS A PANGRAM - MEANING IT USES EVERY LETTER
IN hive AT LEAST ONCE 
CHECKS EACH HIVE LETTER AGAINST THE STRING */

bool isPangram(char* str, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-B): Write isPangram
    - Returns true if str is a pangram (uses all the letters in hive at least once)
    - Returns false otherwise
    */

    for (int i =0; hive[i]!='\0';i++){
        if (findLetter(str,hive[i])== -1){
            return false;
        }
    }
    return true;
}

/* printHive()
PRINTS THE GIVE STRING WITH A CARET (^) POINTING TO THE 
REQUIRED LETTER AT reqLet , ALONG WITH A LABEL */

void printHive(char* hive, int reqLetInd) {
    printf("  Hive: \"%s\"\n", hive);
    printf("         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^");
    for (int i = reqLetInd + 1; i < strlen(hive); i++) {
        printf(" ");
    }
    printf(" (all words must include \'%c\')\n\n", hive[reqLetInd]);
}

/* printList()
PRINTS ALL WORDS IN thisWordList WITH THEIR SCORES 
PERFECT PANGRAMS ( EXACTLY hiveSize LETTERS ) ARE MARKED WITH ***
REGULAR PANGRAMS ARE MARKED WITH *
SCORING :
4-LETTER WORDS=1 PT, LONGER WORDS = LENGTH PTS,PANGRAMS GET +hiveSize BONUS
PRINTS TOTAL SCORE AT BOTTOM */

void printList(WordList* thisWordList, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-C): Write printList
    - Outputs the words in the list 
    - Marks pangrams with asterisk and perfect pangrams with three asterisks
    - Displays score for each word as well as total score at the bottom
    See example shown below.
    Word List:
    *** (16) computer
        ( 7) compute
        ( 1) mute
     *  (17) recompute
        ( 5) comet
    Total Score: 46    
    */

    printf("  Word List:\n");
    int totScore = 0;
    int hiveSize= strlen(hive);

    for (int i=0; i < thisWordList->numWords;i++){
        char* word = thisWordList->words[i];
        int len = strlen(word);

        int score= (len==4)? 1: len;
        bool pg = isPangram(word,hive);
        bool perfpg = pg && (len== hiveSize);
        if (pg){
            score+= hiveSize;
        }
        totScore+= score;

        if (perfpg){
            printf(" *** (%2d) %s\n", score, word);
        } else if (pg) {
            printf("  *  (%2d) %s\n", score, word);
        } else {
            printf("     (%2d) %s\n", score, word);
        }  
    }
    printf("  Total Score: %d\n", totScore);
}

/* bruteForceSolve() 
BRUTE FORCE SOLVER- CHECKS EVERY WORD IN dictionaryList AGAINST THE HIVE.
ADDS ANY WORD THAT PASSES isValidWord() TO solvedList
THIS IS AN O(N) LINEAR SEARCH THROUGH THE ENTIRE DICTIONARY */

void bruteForceSolve(WordList* dictionaryList, WordList* solvedList, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 5-A): Write bruteForceSolve
    - Goes through each word in the dictionary and determines if that is a solution to the puzzle
    - Adds the word to solvedList if it is a solution
    */

    for (int i=0; i< dictionaryList->numWords;i++){
        if(isValidWord(dictionaryList->words[i],hive,reqLet)){
            appendWord(solvedList, dictionaryList->words[i]);
        }
    }
}

/* isPrefix()
RETURNS true IF partWord IS A PREFIX (ROOT) OF fullWord
partWord MUST MATCH THE BEGINNING OF fullWord CHARACTER BY CHARACTER
BOTH EQUAL LENGTH MATCHES & SHORTER partWord ARE CONSIDERED VALID PREFIXES.
RETURNS false IF partWord IS LONGER THAN fullWord OR ANY CHARACTER MISMATCHES */

bool isPrefix(char* partWord, char* fullWord) {
    //---------------------------------------------------------------------
    /* TODO (Task 6-A): Write isPrefix
    - checks if partWord is the root/prefix of fullWord, such that 
        partWord is a perfect match to the beginning (arbitrary length) of fullWord
    - returns true is partWord is a root/prefix of fullWord; returns false otherwise 
    */
    int i=0;
    for (int i=0; partWord[i]!='\0';i++){
        if(fullWord[i]=='\0'){
            return false;
        }

        if (partWord[i]!= fullWord[i]) return false;
    }
    return true; //this line is here so that the starter code compiles; replace/modify this line
}

/*findWord()
RECURSIVE BINARY SEARCH ON thisWordList FOR aWord
RETURNS - >=0: INDEX OF EXACT MATCH IN words ARRAY
-1 : aWord IS A PREFIX OF AT LEAST ONE WORD (PARTIAL MATCH)
-99 : aWord NOT FOUND N IS NOT A PREFIX OF ANY WORD ( NO MATCH AT ALL)
CHECKS BOTH loInd AND loInd-1 FOR PREFIX MATCHES WHEN SEARCH RANGE IS EXHAUSTED  */

int findWord(WordList* thisWordList, char* aWord, int loInd, int hiInd) {
    /* TODO (Task 6-B): Complete findWord
    * - Fix all the if statements that say 'if(true)' with the correct binary search cases
    */

    if (hiInd < loInd) { // Base case 2: aWord not found in words[]

        if (loInd < thisWordList->numWords && thisWordList->words != NULL && isPrefix(aWord, thisWordList->words[loInd])) { //TODO: fix the second part of this if statement
            return -1; //words match this root (partial match)
        }
        if (loInd-1 >+0 && thisWordList->words[loInd - 1] != NULL && isPrefix(aWord, thisWordList->words[loInd - 1])) {
            return -1; //no more words matching this root (no match)
        }
        return -99;
    }

    int mdInd = (hiInd + loInd) / 2;

    if (strcmp(aWord, thisWordList->words[mdInd])==0) { // Base case 1: found tryWord at midInd
        return mdInd;
    }

    if (strcmp(aWord, thisWordList->words[mdInd])>0) { // Recursive case: search upper half
        return findWord(thisWordList, aWord, mdInd + 1, hiInd);
    }

    // Recursive case: search lower half
    return findWord(thisWordList, aWord, loInd, mdInd - 1);
}

/* findAllMatches()
OPTIMISED SOLVER 
USING BINARY SEARCH TO FIND ALL VALID WORDS 
- EXACT MATCH (>=0): ADD WORD IF VALID, THEN EXTEND WITH FIRST HIVE LETTER
- PREFIX MATCH (-1): EXTEND WITH FIRST HIVE LETTER WITHOUT ADDING 
- NO MATCH (-99): DIAL LAST LETTER TO NEXT HIVE LETTER , OR BACKTACK IF LAST */

void findAllMatches(WordList* dictionaryList, WordList* solvedList, char* tryWord, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 6-C): Complete findAllMatches
    */
    int curLen = strlen(tryWord);
    int index = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1);
    if (index>=0){
        if (isValidWord(tryWord,hive,reqLet)&& curLen >= MIN_WORD_LENGTH) {
            appendWord(solvedList,tryWord);
        }

        tryWord[curLen] = hive[0];
        tryWord[curLen+1] ='\0';
        findAllMatches(dictionaryList, solvedList, tryWord,hive,reqLet);

    } else if (index == -1) {

        tryWord[curLen]=hive[0];
        tryWord[curLen+1]='\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);

    } else {
        if (curLen==0){
            return;
        }

        char lastLet = tryWord[curLen-1];
        int hiveInd = findLetter(hive, lastLet);

        while (curLen>0){
            if (hive[hiveInd+1]!='\0'){
                tryWord[curLen-1]= hive[hiveInd+1];
                findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
                return;
            } else {
                tryWord[curLen-1]='\0';
                curLen--;
                if (curLen==0) return;
                lastLet = tryWord[curLen-1];
                hiveInd = findLetter(hive, lastLet);
            }
        }
    }
    //call recursive function here ONLY if tryWord is not empty
}

/* bool setSettings 
PARSES COMMAND LINE ARGMUMENTS AND SETS PROGRAM CONFIGURATION FLAGS.
-r : ENABLES RANDOM MODE W GIVEN HIVE SIZE 
-d: SETS DICTIONARY FILE 
-s: SETS RANDOM SEED FOR REPRODUCIBLE RANDOM HIVE GENERATION
-p: ENABLES PLAY MODE SO USER CAN GUESS WORDS INTERACTIVELY 
-o: USES OPTIMISED SOLVER INSTEAD OF BRUTE FORCE 
RETURNS true IF ALL ARGMUMENTS ARE VALID , false OTHERWISE */

// Provided for you, to determine the program settings based on parameters
bool setSettings(int argc, char* argv[], bool* pRandMode, int* pNumLets, char dictFile[100], bool* pPlayMode, bool* pBruteForceMode, bool* pSeedSelection) {
    *pRandMode = false;
    *pNumLets = 0;
    strcpy(dictFile, "dictionary.txt");
    *pPlayMode = false;
    *pBruteForceMode = true;
    *pSeedSelection = false;
    srand((int)time(0));
    //--------------------------------------
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pRandMode = true;
            *pNumLets = atoi(argv[i]);
            if (*pNumLets < MIN_HIVE_SIZE || *pNumLets > MAX_HIVE_SIZE) {
                return false;
            }
        }
        else if (strcmp(argv[i], "-d") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            strcpy(dictFile, argv[i]);
            FILE* filePtr = fopen(dictFile, "r");
            if (filePtr == NULL) {
                return false;
            }
            fclose(filePtr);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pSeedSelection = true;
            int seed = atoi(argv[i]);
            srand(seed);
        }
        else if (strcmp(argv[i], "-p") == 0) {
            *pPlayMode = true;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            *pBruteForceMode = false;
        }
        else {
            return false;
        }
    }
    return true;
}

// printONorOFF()
//PRINTS ON IF MODE IS true, OFF OTHERWISE

void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}

/* printYESorNO()
PRINTS "YES" IF mode IS true, "NO" OTHERWISE */
void printYESorNO(bool mode) {
    if (mode) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
}


int main(int argc, char* argv[]) {

    printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");

    bool randMode = false;
    int hiveSize = 0;
    char dict[100] = "dictionary.txt";
    bool playMode = false;
    bool bruteForce = true;
    bool seedSelection = false;
    char hive[MAX_HIVE_SIZE + 1] = {};
    hive[0] = '\0';
    int reqLetInd = -1;
    char reqLet = '\0';


    if (!setSettings(argc, argv, &randMode, &hiveSize, dict, &playMode, &bruteForce, &seedSelection)) {
        printf("Invalid command-line argument(s).\nTerminating program...\n");
        return 1;
    }
    else {
        printf("Program Settings:\n");
        printf("  random mode = ");
        printONorOFF(randMode);
        printf("  play mode = ");
        printONorOFF(playMode);
        printf("  brute force solution = ");
        printONorOFF(bruteForce);
        printf("  dictionary file = %s\n", dict);
        printf("  hive set = ");
        printYESorNO(randMode);
        printf("\n\n");
    }

    // build word array (only words with desired minimum length or longer) from dictionary file
    printf("Building array of words from dictionary... \n");
    WordList* dictionaryList = createWordList();
    int maxWordLength = buildDictionary(dict, dictionaryList, MIN_WORD_LENGTH);
    if (maxWordLength == -1) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of valid words.\n");
        printf("Terminating program...\n");
        return 0;
    }
    printf("   Word array built!\n\n");


    printf("Analyzing dictionary...\n");

    if (dictionaryList->numWords < 0) {
        printf("  Dictionary %s not found...\n", dict);
        printf("Terminating program...\n");
        return 0;
    }

    // end program if file has zero words of minimum desired length or longer
    if (dictionaryList->numWords == 0) {
        printf("  Dictionary %s contains insufficient words of length %d or more...\n", dict, MIN_WORD_LENGTH);
        printf("Terminating program...\n");
        return 0;
    }
    else {
        printf("  Dictionary %s contains \n  %d words of length %d or more;\n", dict, dictionaryList->numWords, MIN_WORD_LENGTH);
    }


    if (randMode) {
        printf("==== SET HIVE: RANDOM MODE ====\n");
        //find number of words in words array that use hiveSize unique letters
        WordList* fitWords = findAllFitWords(dictionaryList, hiveSize);
        int numFitWords = fitWords->numWords;
        //pick one at random
        int pickOne = rand() % numFitWords;
        char* chosenFitWord = fitWords->words[pickOne];

        //and alaphabetize the unique letters to make the letter hive
        buildHive(chosenFitWord, hive);
        freeWordList(fitWords);

        reqLetInd = rand() % hiveSize;
        reqLet = hive[reqLetInd];

    }
    else {
        printf("==== SET HIVE: USER MODE ====\n");

        printf("  Enter a single string of lower-case,\n  unique letters for the letter hive... ");

        //---------------------------------------------------------------------
        /* TODO (Task 2-C): Get hive input (part 1 of 2)
         - Write code for randMode OFF, where user enters the hive; use the print statements below in the exact order you see them
         - ask the user to enter a string, following these requirements:
             - must be lowercase letters
             - string length must not be below MIN_HIVE_SIZE or above MAX_HIVE_SIZE
             - must not have any duplicate letters (you might find a helper function useful for this part)
         - If a word is invalid, display the appropriate error below and repeat until there is valid input
         - You may use buildHive() to help with this task
         (Task 2-C is continued a few lines below, getting the required letter)
        */

        char strInput[100];
        bool valHive = false;

        while (!valHive) {
            if (scanf("%s", strInput) != 1) break;

            int len = strlen(strInput);

            bool invalidChar = false;
            for (int i = 0; strInput[i] != '\0'; i++) {
                if (!islower(strInput[i])) {
                    invalidChar = true;
                    break;
                }
            }

            if (len < MIN_HIVE_SIZE || len > MAX_HIVE_SIZE) {
                printf("  HIVE ERROR: \"%s\" has invalid length;\n  valid hive size is between %d and %d, inclusive\n\n", strInput, MIN_HIVE_SIZE, MAX_HIVE_SIZE);
                printf("  Enter a single string of lower-case,\n  unique letters for the letter hive... ");
            } else if (invalidChar) {
                printf("  HIVE ERROR: \"%s\" contains invalid letters;\n  valid characters are lower-case alpha only\n\n", strInput);
                printf("  Enter a single string of lower-case,\n  unique letters for the letter hive... ");
            } else if (countUniqueLetters(strInput) != len) {
                printf("  HIVE ERROR: \"%s\" contains duplicate letters\n\n", strInput);
                printf("  Enter a single string of lower-case,\n  unique letters for the letter hive... ");
            } else {
                buildHive(strInput, hive);
                valHive = true;
            }
        }
     
        hiveSize = strlen(hive);
        reqLetInd = -1;
        reqLet = '\0';


        printf("  Enter the letter from \"%s\"\n  that is required for all words: ", hive);
        //---------------------------------------------------------------------
        /* TODO (Task 2-C): Get hive input (part 2 of 2)
        - Ask the user to enter the required letter, using the print statement above
        - If the letter is not in the hive, display the error message below and repeat the prompt until input is valid
        - If the letter the user enters is within the hive, then find the index of that letter in the hive,
        set that value you found for reqLetInd, and change reqLet to the required letter
        - Note that uppercase letters ARE valid here, unlike earlier; you will find it helpful to make them lowercase in code
        */
        
        while (reqLetInd == -1) {
            char reqInput[10];
            if (scanf("%s", reqInput) != 1) break;
            char reqChar = tolower(reqInput[0]);

            int idx = findLetter(hive, reqChar);
            if (idx == -1) {
                printf("  HIVE ERROR: \"%s\" does not contain the character \'%c\'\n\n", hive, reqChar);
                printf("  Enter the letter from \"%s\"\n  that is required for all words: ", hive);
            } else {
                reqLetInd = idx;
                reqLet = reqChar;
            }
        }
    }
    
    printHive(hive, reqLetInd);

    if (playMode) {
        printf("==== PLAY MODE ====\n");

        char* userWord = (char*)malloc((maxWordLength + 1) * sizeof(char));
        strcpy(userWord, "default");

        WordList* userWordList = createWordList();

        printf("............................................\n");
        printHive(hive, reqLetInd);


        printf("  Enter a word (enter DONE to quit): ");
        scanf("%s", userWord);
        printf("\n");


        while (strcmp(userWord, "DONE")) {

            //---------------------------------------------------------------------
            /* TODO (Task 4-A, part 2 of 2): Validate input from the user (for play mode)
            - userWord is the input from the user (their attempted word)
            - If the word is not valid (it's too short, not in the dictionary, not valid for the hive), 
                or it has already been entered, display the appropriate error message below.
            - If the word is valid, add it to userWordList
            
            - The end of the while loop is already given to you, make sure not to remove that
            - As always, helper functions will benefit you greatly here
            */
            
            for (int i=0;userWord[i]!='\0';i++){
                userWord[i]= tolower(userWord[i]);
            }

            if (strcmp(userWord,"done")==0){
                break;
            }

            if((int)strlen(userWord)< MIN_WORD_LENGTH){
                printf(" !!! \"%s\" is TOO SHORT (min length = %d) !!!\n", userWord, MIN_WORD_LENGTH);
            } else {
                bool inDict = false;
                for (int i=0;i < dictionaryList->numWords;i++){
                    if(strcmp(dictionaryList->words[i],userWord)==0){
                        inDict = true;
                        break;
                    }
                }

                if (!inDict){
                    printf(" !!! \"%s\" is NOT a valid word in the dictionary !!!\n", userWord);
                } else if (!isValidWord(userWord, hive, reqLet)) {
                    printf(" !!! \"%s\" is NOT valid for the hive !!!\n", userWord);
                } else {
                    bool duplicate = false;
                    for (int i =0; i < userWordList->numWords;i++){
                        if (strcmp(userWordList->words[i], userWord) == 0) {
                            duplicate = true;
                            break;
                        }
                    }

                    if (duplicate){
                        printf(" !!! \"%s\" is already in your word list !!!\n", userWord);
                    } else {
                        appendWord(userWordList, userWord);
                    }
                }
            }
            
            // Keep the code below this comment, which prints the list and the hive, and gets the next input
            printf("\n");
            printList(userWordList, hive);
            printf("............................................\n");
            printHive(hive, reqLetInd);

            printf("  Enter a word (enter DONE to quit): ");
            scanf("%s", userWord);
            printf("\n");

        }

        // Do until user enters quit request
        //  Display Hive & Word List & Current Score
        //  X Allow user to enter a word
        //  X Check if word is in dict
        //  X Check if word is a duplicate
        //  Check if word is valid for the hive
        //  If all valid, add it to word list, report score  

        freeWordList(userWordList);
        free(userWord);
    }

    printf("==== SPELLING BEE SOLVER ====\n");

    printf("  Valid words from hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^\n");

    WordList* solvedList = createWordList();

    if (bruteForce) { //find all words that work... (1) brute force
        bruteForceSolve(dictionaryList, solvedList, hive, reqLet);
    }
    else {
        char* tryWord = (char*)malloc(sizeof(char) * (maxWordLength + 1));

        tryWord[0] = hive[0];
        tryWord[1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        free(tryWord);

    }

    int longestSolvedWordLen = 0;
    for (int i = 0; i < solvedList->numWords; i++) {
        if (strlen(solvedList->words[i]) > longestSolvedWordLen) {
            longestSolvedWordLen = strlen(solvedList->words[i]);
        }
    }

    // Helpful variables
    int numValidWords = 0;
    int numPangrams = 0;
    int numPerfectPangrams = 0;
    int totScore = 0;
    int score = 0;
    bool isBingo = true;

    //---------------------------------------------------------------------
    /* TODO (Task 5-B): Display solver results
    - Display list of words solver found, noting pangrams and perfect pangrams with asterisks like printList
    - Calculate the number of valid words, number of pangrams, number of perfect pangrams, 
          total possible score, and whether there is a bingo. Save those values in the corresponding
          variables, or update the printf statements below to dispay those values
    */
    isBingo = true;

    for (int i=0; i < solvedList->numWords;i++){
        char* word = solvedList->words[i];
        int len = strlen(word);

        int score = (len == 4) ? 1: len;
        bool pg = isPangram(word,hive);
        bool perfpg = pg && (len==hiveSize);
        if (pg) {
            score+= hiveSize;
        }

        totScore+= score;
        numValidWords++;

        if (pg) numPangrams++;
        if (perfpg) numPerfectPangrams++;

        if (perfpg){
            printf("  *** (%2d) %s\n", score, word);
        } else if (pg){
            printf("   *  (%2d) %s\n", score, word);
        } else {
            printf("      (%2d) %s\n", score, word);
        }
    }

    for (int i=0; hive[i]!= '\0';i++){
        bool found = false;
        for (int j=0;j < solvedList->numWords;j++){
            if (solvedList->words[j][0]==hive[i]){
                found = true;
                break;
            }
        }

        if (!found){
            isBingo = false;
            break;
        }
    }


    
    // Additional results are printed here:
    printf("\n");
    printf("  Total counts for hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^\n");
    printf("    Number of Valid Words: %d\n", numValidWords);
    printf("    Number of ( * ) Pangrams: %d\n", numPangrams);
    printf("    Number of (***) Perfect Pangrams: %d\n", numPerfectPangrams);
    printf("    Bingo: ");
    printYESorNO(isBingo);
    printf("    Total Score Possible: %d\n", totScore);

    //---------------------------------------------------------------------
    /* TODO Extra credit: Display frequency table
    - Display a table showing the number of words starting with each letter and duration in a table.
    - For example, with the hive abcde and required letter e:
             Frequency Table:
             let  4  5  6  7  8
             -------------------
              a:  3  1  1  1  0
              b:  3  1  4  0  0
              c:  3  1  0  0  1
              d:  3  0  3  0  0
              e:  0  1  0  0  0
       The table shows that there are 3 words that start with a and are 4 letters long, 
       and 1 each that are 5, 6, and 7 letters long and start with a.
       Note that the lengths 4-8 are shown here because the longest word is 8 letters long,
       but that will change depending on the hive
    */

    int minlen = MIN_WORD_LENGTH;
    int maxlen = longestSolvedWordLen;

    int lenhive = strlen(hive);
    int numcols = maxlen - minlen +1;

    int** freq = (int**)malloc(lenhive*sizeof(int*));
    for (int i=0; i < lenhive;i++){
        freq[i]=(int*)calloc(numcols,sizeof(int));
    }

    for (int i=0; i < solvedList->numWords;i++){
        char* word = solvedList->words[i];

        int len = strlen(word);
        char firstlet = word[0];
        int row = findLetter(hive, firstlet);
        int col = len-minlen;

        if (row>=0 && col >=0 && col < numcols){
            freq[row][col]++;
        }
    }

    printf("    Frequency Table:\n");
    printf(     "let");
    for (int len=minlen;len<=maxlen;len++){
        printf(" %2d",len);
    }
    printf("\n");

    printf("    ");
    int divlen =3+(numcols*3)+1;
    for (int i=0;i < divlen;i++){
        printf("-");
    }
    printf("\n");

    for (int i=0;i<lenhive;i++){
        printf("    %c:",hive[i]);
        for (int j=0; j < numcols;j++){
            printf(" %2d",freq[i][j]);
        }
        printf("\n");
    }

    for (int i=0; i < lenhive;i++){
        free(freq[i]);
    }
    free(freq);


    freeWordList(dictionaryList);
    freeWordList(solvedList);
    printf("\n\n");
    return 0;
}


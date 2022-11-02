#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// 100 MB = 100,000 KB = 100,000,000 byte 
// char는 1 byte 변수이므로 "MAX_LENGTH"를 100,000,001로 setting합니다. (최대 사용 용량 50MB)
#define MAX_LENGTH 100000001

/* 지역변수로 선언 시 스택에 생성되어 "MAX_LENGTH"일 때 Runtime Error가 생길 수 있지만,
 * 전역변수로 선언 시 힙 영역에 생성되어 제한으로부터 자유롭기에 전역변수로 선언하였습니다. */
char *string;
char *pat;
char *t; // "string"내에 등장하는 "pat"의 가장 앞을 가르키는 pointer

// 1. char *strstr(char *s, char *pat) (slide 1 in lecture 4)
/* The strstr function is built-in in the string.h header file */
int strstrfind (char *string, char *pat) {
    int counter = 0; // variable for count "pat" in "string"
    
    // strstr함수는 "string"에서 "pat"의 첫 번째로 등장하는 자리를 char pointer로 retrun합니다.
    t = strstr(string, pat);
    // "string"에 "pat"이 없을 때까지 반복합니다.
    while(t != NULL) {
        /* Case of find "pat" in "string" */
        counter++;
        // "pat"을 찾은 pointer의 다음 pointer부터 다시 "pat"을 찾습니다.
        t = strstr(t+1, pat);
    } // end while

    return counter;
} // end strstrfind function

// 2. nfind() in slide 3 in lecture 4
int nfind (char *string, char *pat) {
    int i, j; // variable for using the "for" statement
    int start = 0; // first index of "string" to compare
    int lasts = strlen(string)-1; // last index of "string"
    int lastp = strlen(pat)-1; // last index of "pat"
    int endmatch = lastp; // 비교할 "string"의 마지막 index

    int counter = 0; // variable for count "pat" in "string"
    
    // i는 "string"의 idx, j는 "pat"의 idx
    for (i=0 ; endmatch<=lasts ; endmatch++,start++) {
        /* if endmatch > lasts, exit the "for" statement.
         * else "endmatch"++ and "start"++ */
        if (string[endmatch] == pat[lastp]){
            /* "string"의 endmatch번째 문자와 "pat"의 lastp번째 문자가 같은 경우
             * string[start]와 pat[0]부터 pat[lastp-1]까지 문자를 모두 비교합니다 */
            for (j=0,i=start ; j<lastp&&string[i]==pat[j] ; i++,j++)
                ; // end for

            if (j == lastp) {
                /* string[start]와 pat[0]부터 string[endmatch]와 pat[lastp]까지 모두 같은 경우 */
                counter++;
            } // end if
        } // end if
    } // end for

    return counter;
} // end nfind function

// 3. Fast pattern matching in slide 5 in lecture 4
/* failure function value array */
int failure[MAX_LENGTH];

/* fail function is that sets the failure function value to the failure array. */
void fail (char *pat) {
    int i, j; // variable for using the "for" statement
    int n = strlen(pat); // variable for length of "pat"
    failure[0] = -1; // Let set failure[0] = -1

    for (j = 1; j < n; j++){
        /* Set the failure array from 1 to n-1. */
        i = failure[j-1];
        while (pat[j] != pat[i+1] && i>=0)
            /* pat[j]와 pat[i+1]가 같을 때 exit합니다. i = -1가 될 때도 exit합니다.*/
            i = failure[i]; // end while
        if (pat[j] == pat[i+1])
            /* 위의 while문을 pat[j]와 pat[i+1]가 같아서 빠져나온 case
             * (pat[j]와 pat[0]이 같아서 빠져나온 case도 포함) */
            failure[j] = i+1; // end if
        else
            /* 위의 while문을 i = -1가 되어서 빠져나왔는데, pat[j]와 pat[0]가 다른 문자인 경우 */
            failure[j] = -1; // end else
    } // end for
} // end fail function

/* pmatch function is that counts the number of "pat" in the "string" using the failure array. */
int pmatch (char* string, char* pat) {
    int i=0; int j=0; // variable for using the "while" statement
    int counter = 0; // variable for count "pat" in "string"
    int lens=strlen(string); int lenp=strlen(pat);

    // i는 "string"의 idx, j는 "pat"의 idx
    while (i < lens) {
        if (string[i] == pat[j]) { 
            i++;
            j++;
        } // end if
        else if (j == 0)
            i++; // end else if
        else
            j = failure[j-1]+1; // end else

        // j == lenp라면 pattern을 string에서 찾았다는 것입니다.
        if(j == lenp) {
            // 따라서 counter에 1을 더해준 다음
            counter++;
            // j에 failure[j-1]+1을 넣어주며 탐색을 계속합니다.
            j = failure[j-1]+1;
        } // end if
    } // end while
    return counter;
} // end pmatch function

/* 'a'부터 'z'까지의 random character을 return해주는 함수 */
char getRandomCharacter() {
    return 'a' + (rand()%26);
} // end getRandomCharacter function

/* 특정한 크기의 random string을 만들고 "string.txt"에 입력하는 함수 */
void makeStringFile(int sizeidx) {
    int len; // variable for using the "for" statement
    int MB = (int)1e6;
    // "string"의 size가 차례대로 1MB, 5MB, 10MB, 20MB, 30MB, 40MB, 50MB로 만들어집니다.
    // ex) sizeidx = 3 -> Set string array size to 20MB
    int stringSize[7] = {1*MB, 5*MB, 10*MB, 20*MB, 30*MB, 40*MB, 50*MB};    
    
    // 위에서 정해진 string size 크기만큼의 random string을 만듭니다.
    for(len=0 ; len < stringSize[sizeidx] ; len++) {
        string[len] = getRandomCharacter();
    } // end for

    // "string.txt" file을 만든 후, 그 파일에 위에서 만든 string을 입력합니다.
    FILE *writeStringFile = fopen("./string.txt","w");
    fprintf(writeStringFile, string);
    fclose(writeStringFile); // Close the file pointer.
} // end makeStringFile

/* 특정한 크기의 random string을 만들고 "pat.txt"에 입력하는 함수 */
void makePatFile(int sizeidx) {
    int len; // variable for using the "for" statement
    // string의 size가 차례대로 10byte, 50byte, 100byte, 200byte, 300byte, 400byte, 500byte로 만들어집니다.
    // ex) sizeidx = 2 -> Set string array size to 100byte
    int patSize[7] = {10,50,100,200,300,400,500};

    // 위에서 정해진 "pat" size 크기만큼의 random pat을 만듭니다.
    for(len=0 ; len < patSize[sizeidx] ; len++) {
        pat[len] = getRandomCharacter();
    } // end for

    // "pat.txt" file을 만든 후, 그 파일에 위에서 만든 pat을 입력합니다.
    FILE *writePatFile = fopen("./pat.txt","w");
    fprintf(writePatFile, pat);
    fclose(writePatFile); // Close the file pointer.
} // end makePatFile function

/* "string.txt"파일을 읽고 string 문자열에 넣어주는 함수 */
void readString() {
    // Open the "string.txt" file in the same folder in r(read) mode.
    FILE *readStringFile = fopen("./string.txt","r");
    /* 파일에 있는 문자열을 MAX_LENGTH만큼 읽어서 string(char pointer array)에 넣어줍니다. */
    if(readStringFile == NULL) { // Exception handling
        /* Case of "string.txt" file does not exist */
        printf("Could not find file \"string.txt\".\n");
        exit(1); // End the program
    } // end if    
    fread(string, 1, MAX_LENGTH, readStringFile);
    fclose(readStringFile); // close file pointer
} // end readString function

/* "pat.txt"파일을 읽고 pat 문자열에 넣어주는 함수 */
void readPat() {
    // Open the "pat.txt" file in the same folder in r(read) mode.
    FILE *patFile = fopen("./pat.txt","r");
    /* 파일에 있는 문자열을 MAX_LENGTH만큼 읽어서 pat(char pointer array)에 넣어줍니다. */
    if(patFile == NULL) { // Exception handling
        /* Case of "pat.txt" file does not exist */
        printf("Could not find file \"pat.txt\".\n");
        exit(1); // End the program
    } // end if
    fread(pat, 1, MAX_LENGTH, patFile);
    fclose(patFile); // close file pointer
} // end readPat function

int main(void) {
    // MAX_LENGTH * 1byte 크기의 동적메모리 할당
    string = (char *)malloc(sizeof(char) * MAX_LENGTH);
    pat = (char *)malloc(sizeof(char) * MAX_LENGTH);
    t = (char *)malloc(sizeof(char) * MAX_LENGTH);

    srand(time(NULL));

    // Display about each case
    int stringcase = 2; int patcase = 6; // user가 원하는 string and pat의 길이를 넣어준다.

    char stringsize[15], patsize[15];
    switch (stringcase) {
        case 0:
            strcpy(stringsize, "1MB (1e6개)");
            break;
        case 1:
            strcpy(stringsize, "5MB (5e6개)");
            break;
        case 2:
            strcpy(stringsize, "10MB (1e7개)");
            break;
        case 3:
            strcpy(stringsize, "20MB (2e7개)");
            break;
        case 4:
            strcpy(stringsize, "30MB (3e7개)");
            break;
        case 5:
            strcpy(stringsize, "40MB (4e7개)");
            break;
        case 6:
            strcpy(stringsize, "50MB (5e7개)");
            break;
        default:
            break;
    } // end switch
    switch (patcase) {
        case 0:
            strcpy(patsize, "10byte");
            break;
        case 1:
            strcpy(patsize, "50byte");
            break;
        case 2:
            strcpy(patsize, "100byte");
            break;
        case 3:
            strcpy(patsize, "200byte");
            break;
        case 4:
            strcpy(patsize, "300byte");
            break;
        case 5:
            strcpy(patsize, "400byte");
            break;
        case 6:
            strcpy(patsize, "500byte");
            break;
        default:
            break;
    } // end switch
    
    int number; // variable for using the "for" statement
    int count = 0; // variable for string에 있는 pat의 개수
    double averageCount = 0; // 10회 측정 중 string에 있는 pat의 개수를 평균 내기위해 선언된 변수
    
    // 시간 측정에 사용되는 variable
    time_t start, end; // variable checked when code start and end
    double strstrTime = 0, nfindTime = 0, pmatchTime = 0;

    /* 총 10회 측정 후 측정시간을 평균낸다. */
    for(number = 0; number < 10 ; number++) {
        makeStringFile(stringcase); // random string을 생성 후, "string.txt"에 입력합니다.
        makePatFile(patcase); // random pat을 생성 후, "pat.txt"에 입력합니다.

        readString(); // "string.txt"에 있는 문자열을 string에 넣어줍니다.
        readPat(); // "pat.txt"에 있는 문자열을 pat에 넣어줍니다.

        // 1. strstr code 
        start = clock();
        count = strstrfind(string, pat); // Find the number of "pat" in "string" using the strstr function.
        end = clock();
        strstrTime += (double)(end - start);
        //printf("pattern count by strstr  : %d\n", count);
        //printf("strstr 소요시간: %.0lfms\n", (double)(end - start));

        // 2. nfind code
        start = clock();
        count = nfind(string, pat); // Find the number of "pat" in "string" using the nfind function.
        end = clock();
        nfindTime += (double)(end - start);
        //printf("pattern count by nfind   : %d\n", count);
        //printf("nfind  소요시간: %.0lfms\n", (double)(end - start));

        // 3. pmatch code
        start = clock();
        fail(pat); // make failure array
        count = pmatch(string, pat); // Find the number of "pat" in "string" using the pmatch function.
        end = clock();
        pmatchTime += (double)(end - start);
        //printf("pattern count by pmatch : %d\n", count);
        //printf("pmatch 소요시간: %.0lfms\n", (double)(end - start));

        averageCount += count;
        //puts(""); // spacing
    } // end for

    // Display the result
    printf("Case %d-%d) string size: %s / patsize : %s.\n", stringcase+1, patcase+1, stringsize, patsize);
    printf("Pattern count in string: %.1lf\n", averageCount/10);
    printf("strstr 평균 소요시간: %.1lfms\n", strstrTime/10);
    printf("nfind  평균 소요시간: %.1lfms\n", nfindTime/10);
    printf("pmatch 평균 소요시간: %.1lfms\n", pmatchTime/10);
    puts("");

    // Frees the allocated memory.
    free(string);
    free(pat);
    free(t);

} // end main function

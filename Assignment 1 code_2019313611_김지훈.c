#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// 100 MB = 100,000 KB = 100,000,000 byte 
// char�� 1 byte �����̹Ƿ� "MAX_LENGTH"�� 100,000,001�� setting�մϴ�. (�ִ� ��� �뷮 50MB)
#define MAX_LENGTH 100000001

/* ���������� ���� �� ���ÿ� �����Ǿ� "MAX_LENGTH"�� �� Runtime Error�� ���� �� ������,
 * ���������� ���� �� �� ������ �����Ǿ� �������κ��� �����ӱ⿡ ���������� �����Ͽ����ϴ�. */
char *string;
char *pat;
char *t; // "string"���� �����ϴ� "pat"�� ���� ���� ����Ű�� pointer

// 1. char *strstr(char *s, char *pat) (slide 1 in lecture 4)
/* The strstr function is built-in in the string.h header file */
int strstrfind (char *string, char *pat) {
    int counter = 0; // variable for count "pat" in "string"
    
    // strstr�Լ��� "string"���� "pat"�� ù ��°�� �����ϴ� �ڸ��� char pointer�� retrun�մϴ�.
    t = strstr(string, pat);
    // "string"�� "pat"�� ���� ������ �ݺ��մϴ�.
    while(t != NULL) {
        /* Case of find "pat" in "string" */
        counter++;
        // "pat"�� ã�� pointer�� ���� pointer���� �ٽ� "pat"�� ã���ϴ�.
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
    int endmatch = lastp; // ���� "string"�� ������ index

    int counter = 0; // variable for count "pat" in "string"
    
    // i�� "string"�� idx, j�� "pat"�� idx
    for (i=0 ; endmatch<=lasts ; endmatch++,start++) {
        /* if endmatch > lasts, exit the "for" statement.
         * else "endmatch"++ and "start"++ */
        if (string[endmatch] == pat[lastp]){
            /* "string"�� endmatch��° ���ڿ� "pat"�� lastp��° ���ڰ� ���� ���
             * string[start]�� pat[0]���� pat[lastp-1]���� ���ڸ� ��� ���մϴ� */
            for (j=0,i=start ; j<lastp&&string[i]==pat[j] ; i++,j++)
                ; // end for

            if (j == lastp) {
                /* string[start]�� pat[0]���� string[endmatch]�� pat[lastp]���� ��� ���� ��� */
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
            /* pat[j]�� pat[i+1]�� ���� �� exit�մϴ�. i = -1�� �� ���� exit�մϴ�.*/
            i = failure[i]; // end while
        if (pat[j] == pat[i+1])
            /* ���� while���� pat[j]�� pat[i+1]�� ���Ƽ� �������� case
             * (pat[j]�� pat[0]�� ���Ƽ� �������� case�� ����) */
            failure[j] = i+1; // end if
        else
            /* ���� while���� i = -1�� �Ǿ �������Դµ�, pat[j]�� pat[0]�� �ٸ� ������ ��� */
            failure[j] = -1; // end else
    } // end for
} // end fail function

/* pmatch function is that counts the number of "pat" in the "string" using the failure array. */
int pmatch (char* string, char* pat) {
    int i=0; int j=0; // variable for using the "while" statement
    int counter = 0; // variable for count "pat" in "string"
    int lens=strlen(string); int lenp=strlen(pat);

    // i�� "string"�� idx, j�� "pat"�� idx
    while (i < lens) {
        if (string[i] == pat[j]) { 
            i++;
            j++;
        } // end if
        else if (j == 0)
            i++; // end else if
        else
            j = failure[j-1]+1; // end else

        // j == lenp��� pattern�� string���� ã�Ҵٴ� ���Դϴ�.
        if(j == lenp) {
            // ���� counter�� 1�� ������ ����
            counter++;
            // j�� failure[j-1]+1�� �־��ָ� Ž���� ����մϴ�.
            j = failure[j-1]+1;
        } // end if
    } // end while
    return counter;
} // end pmatch function

/* 'a'���� 'z'������ random character�� return���ִ� �Լ� */
char getRandomCharacter() {
    return 'a' + (rand()%26);
} // end getRandomCharacter function

/* Ư���� ũ���� random string�� ����� "string.txt"�� �Է��ϴ� �Լ� */
void makeStringFile(int sizeidx) {
    int len; // variable for using the "for" statement
    int MB = (int)1e6;
    // "string"�� size�� ���ʴ�� 1MB, 5MB, 10MB, 20MB, 30MB, 40MB, 50MB�� ��������ϴ�.
    // ex) sizeidx = 3 -> Set string array size to 20MB
    int stringSize[7] = {1*MB, 5*MB, 10*MB, 20*MB, 30*MB, 40*MB, 50*MB};    
    
    // ������ ������ string size ũ�⸸ŭ�� random string�� ����ϴ�.
    for(len=0 ; len < stringSize[sizeidx] ; len++) {
        string[len] = getRandomCharacter();
    } // end for

    // "string.txt" file�� ���� ��, �� ���Ͽ� ������ ���� string�� �Է��մϴ�.
    FILE *writeStringFile = fopen("./string.txt","w");
    fprintf(writeStringFile, string);
    fclose(writeStringFile); // Close the file pointer.
} // end makeStringFile

/* Ư���� ũ���� random string�� ����� "pat.txt"�� �Է��ϴ� �Լ� */
void makePatFile(int sizeidx) {
    int len; // variable for using the "for" statement
    // string�� size�� ���ʴ�� 10byte, 50byte, 100byte, 200byte, 300byte, 400byte, 500byte�� ��������ϴ�.
    // ex) sizeidx = 2 -> Set string array size to 100byte
    int patSize[7] = {10,50,100,200,300,400,500};

    // ������ ������ "pat" size ũ�⸸ŭ�� random pat�� ����ϴ�.
    for(len=0 ; len < patSize[sizeidx] ; len++) {
        pat[len] = getRandomCharacter();
    } // end for

    // "pat.txt" file�� ���� ��, �� ���Ͽ� ������ ���� pat�� �Է��մϴ�.
    FILE *writePatFile = fopen("./pat.txt","w");
    fprintf(writePatFile, pat);
    fclose(writePatFile); // Close the file pointer.
} // end makePatFile function

/* "string.txt"������ �а� string ���ڿ��� �־��ִ� �Լ� */
void readString() {
    // Open the "string.txt" file in the same folder in r(read) mode.
    FILE *readStringFile = fopen("./string.txt","r");
    /* ���Ͽ� �ִ� ���ڿ��� MAX_LENGTH��ŭ �о string(char pointer array)�� �־��ݴϴ�. */
    if(readStringFile == NULL) { // Exception handling
        /* Case of "string.txt" file does not exist */
        printf("Could not find file \"string.txt\".\n");
        exit(1); // End the program
    } // end if    
    fread(string, 1, MAX_LENGTH, readStringFile);
    fclose(readStringFile); // close file pointer
} // end readString function

/* "pat.txt"������ �а� pat ���ڿ��� �־��ִ� �Լ� */
void readPat() {
    // Open the "pat.txt" file in the same folder in r(read) mode.
    FILE *patFile = fopen("./pat.txt","r");
    /* ���Ͽ� �ִ� ���ڿ��� MAX_LENGTH��ŭ �о pat(char pointer array)�� �־��ݴϴ�. */
    if(patFile == NULL) { // Exception handling
        /* Case of "pat.txt" file does not exist */
        printf("Could not find file \"pat.txt\".\n");
        exit(1); // End the program
    } // end if
    fread(pat, 1, MAX_LENGTH, patFile);
    fclose(patFile); // close file pointer
} // end readPat function

int main(void) {
    // MAX_LENGTH * 1byte ũ���� �����޸� �Ҵ�
    string = (char *)malloc(sizeof(char) * MAX_LENGTH);
    pat = (char *)malloc(sizeof(char) * MAX_LENGTH);
    t = (char *)malloc(sizeof(char) * MAX_LENGTH);

    srand(time(NULL));

    // Display about each case
    int stringcase = 2; int patcase = 6; // user�� ���ϴ� string and pat�� ���̸� �־��ش�.

    char stringsize[15], patsize[15];
    switch (stringcase) {
        case 0:
            strcpy(stringsize, "1MB (1e6��)");
            break;
        case 1:
            strcpy(stringsize, "5MB (5e6��)");
            break;
        case 2:
            strcpy(stringsize, "10MB (1e7��)");
            break;
        case 3:
            strcpy(stringsize, "20MB (2e7��)");
            break;
        case 4:
            strcpy(stringsize, "30MB (3e7��)");
            break;
        case 5:
            strcpy(stringsize, "40MB (4e7��)");
            break;
        case 6:
            strcpy(stringsize, "50MB (5e7��)");
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
    int count = 0; // variable for string�� �ִ� pat�� ����
    double averageCount = 0; // 10ȸ ���� �� string�� �ִ� pat�� ������ ��� �������� ����� ����
    
    // �ð� ������ ���Ǵ� variable
    time_t start, end; // variable checked when code start and end
    double strstrTime = 0, nfindTime = 0, pmatchTime = 0;

    /* �� 10ȸ ���� �� �����ð��� ��ճ���. */
    for(number = 0; number < 10 ; number++) {
        makeStringFile(stringcase); // random string�� ���� ��, "string.txt"�� �Է��մϴ�.
        makePatFile(patcase); // random pat�� ���� ��, "pat.txt"�� �Է��մϴ�.

        readString(); // "string.txt"�� �ִ� ���ڿ��� string�� �־��ݴϴ�.
        readPat(); // "pat.txt"�� �ִ� ���ڿ��� pat�� �־��ݴϴ�.

        // 1. strstr code 
        start = clock();
        count = strstrfind(string, pat); // Find the number of "pat" in "string" using the strstr function.
        end = clock();
        strstrTime += (double)(end - start);
        //printf("pattern count by strstr  : %d\n", count);
        //printf("strstr �ҿ�ð�: %.0lfms\n", (double)(end - start));

        // 2. nfind code
        start = clock();
        count = nfind(string, pat); // Find the number of "pat" in "string" using the nfind function.
        end = clock();
        nfindTime += (double)(end - start);
        //printf("pattern count by nfind   : %d\n", count);
        //printf("nfind  �ҿ�ð�: %.0lfms\n", (double)(end - start));

        // 3. pmatch code
        start = clock();
        fail(pat); // make failure array
        count = pmatch(string, pat); // Find the number of "pat" in "string" using the pmatch function.
        end = clock();
        pmatchTime += (double)(end - start);
        //printf("pattern count by pmatch : %d\n", count);
        //printf("pmatch �ҿ�ð�: %.0lfms\n", (double)(end - start));

        averageCount += count;
        //puts(""); // spacing
    } // end for

    // Display the result
    printf("Case %d-%d) string size: %s / patsize : %s.\n", stringcase+1, patcase+1, stringsize, patsize);
    printf("Pattern count in string: %.1lf\n", averageCount/10);
    printf("strstr ��� �ҿ�ð�: %.1lfms\n", strstrTime/10);
    printf("nfind  ��� �ҿ�ð�: %.1lfms\n", nfindTime/10);
    printf("pmatch ��� �ҿ�ð�: %.1lfms\n", pmatchTime/10);
    puts("");

    // Frees the allocated memory.
    free(string);
    free(pat);
    free(t);

} // end main function

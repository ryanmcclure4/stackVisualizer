#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_BYTES   1
#define SHORT_BYTES  2
#define INT_BYTES    4
#define LONG_BYTES   4
#define FLOAT_BYTES  4
#define DOUBLE_BYTES 8

#define BUF_LEN 40
#define PRINTBUF 25
#define PRINTBUF_WORD 22
#define STACKSIZE 50

#define CHAR   "char"
#define SHORT  "short"
#define INT    "int"
#define LONG   "long"
#define FLOAT  "float"
#define DOUBLE "double"

#define WORD           "|       %c       |"
#define HALF_HALF      "|   %c   |   %c   |"
#define HALF_BYTE_BYTE "|   %c   | %c | %c |"
#define BYTE_HALF_BYTE "| %c |   %c   | %c |"
#define BYTE_BYTE_HALF "| %c | %c |   %c   |"
#define FOUR_BYTES     "| %c | %c | %c | %c |"


char * generateStackFrame (char stackVals[], long word) {
    //printf ("word: %ld", word);
    char * buffer;
    buffer = malloc(sizeof(char)*BUF_LEN);
    int index = 0;
    // 8 bytes
    if (word) {
        snprintf (buffer, PRINTBUF_WORD, WORD, stackVals[0]);
    // 4 bytes + padding
    } else {
        for (index = 0; index < 4; index++) {
            if (stackVals[index] == '\0')
                stackVals[index] = 'X';
        }
        if (stackVals[3] == stackVals[2] && stackVals[3] != 'X') {
            if (stackVals[1] == stackVals[0] && stackVals[1] != 'X') {
                snprintf (buffer, PRINTBUF, HALF_HALF, stackVals[3], stackVals[1]);

            } else {
                snprintf (buffer, PRINTBUF, HALF_BYTE_BYTE, stackVals[3],
                          stackVals[1], stackVals[0]);
            }
        } else if (stackVals[1] == stackVals[2] && stackVals[1] != 'X') {
            snprintf (buffer, PRINTBUF, BYTE_HALF_BYTE, stackVals[3],
                      stackVals[2], stackVals[0]);

        } else if (stackVals[1] == stackVals[0] && stackVals[1] != 'X') {
            if (stackVals[3] == stackVals[2] && stackVals[3] != 'X') {
                snprintf (buffer, PRINTBUF, HALF_HALF,
                          stackVals[3], stackVals[1]);
            } else {
                snprintf (buffer, PRINTBUF, BYTE_BYTE_HALF, stackVals[3],
                          stackVals[2], stackVals[0]);
            }

        } else {
            snprintf (buffer, PRINTBUF, FOUR_BYTES,
                      stackVals[3], stackVals[2], stackVals[1], stackVals[0]);
        }
    }

    return buffer;
}

int main (int argc, char * argv[]) {
    int availableBytes = 4;
    int stackCount     = 0;
    int stackIndex     = 0;
    int numVars        = 0;
    int index          = 0;

    long filled = 0;
    long word   = 0;

    char line[BUF_LEN];
    char * inputString;
    char * endPtr;
    char * finalStack[STACKSIZE] = {""};

    char stackVals[4] = {'\0'};
    char varName;

    printf ("Number of variables to initialize : ");
    scanf ("%d\n", &numVars);

    for (index = 0; index < numVars; index++) {
        filled = 0;
        inputString = fgets (line, BUF_LEN, stdin);
        if (inputString == NULL) {
            // error; no input
        }
        // extract variable name
        endPtr = strchr (inputString, '\n');
        endPtr--;
        varName = endPtr[0];

        // check for CHAR
        if (strstr (inputString, CHAR)) {
            if (availableBytes == 4){
                stackVals[0] = varName;
                availableBytes = 3;
                filled = 1;
            } else if (availableBytes == 3) {
                stackVals[1] = varName;
                availableBytes = 2;
                filled = 1;
            } else if (availableBytes == 2) {
                stackVals[2] = varName;
                availableBytes = 1;
                filled = 1;
            } else if (availableBytes == 1) {
                stackVals[3] = varName;
                availableBytes = 0;
                filled = 1;
            }

        // check for SHORT
        } else if (strstr (inputString, SHORT)) {
            if (availableBytes == 4) {
                stackVals[0] = varName;
                stackVals[1] = varName;
                availableBytes = 2;
                filled = 1;
            } else if (availableBytes == 3) {
                stackVals[2] = varName;
                stackVals[3] = varName;
                availableBytes = 0;
                filled = 1;
            } else if (availableBytes == 2) {
                stackVals[2] = varName;
                stackVals[3] = varName;
                availableBytes = 0;
                filled = 1;
            } else if (availableBytes == 1) {
                // save previous stack frame
                finalStack[stackCount++] = generateStackFrame (stackVals, word);
                // prep frame for current var
                stackVals[0] = varName;
                stackVals[1] = varName;
                stackVals[2] = '\0';
                stackVals[3] = '\0';
                availableBytes = 2;
                filled = 1;
            }

        // check for INT, LONG, or FLOAT
        } else if (strstr (inputString, INT)  ||
                   strstr (inputString, LONG) ||
                   strstr (inputString, FLOAT)) {
            if (availableBytes <= 3) {
                // save previous stack frame
                finalStack[stackCount++] = generateStackFrame (stackVals, word);
            }
            stackVals[0] = varName;
            stackVals[1] = varName;
            stackVals[2] = varName;
            stackVals[3] = varName;
            word = 1;
            filled = 1;
            availableBytes = 0;

        // check for DOUBLE
        } else if (strstr (inputString, DOUBLE)) {
            if (availableBytes <= 3) {
                // save previous stack frame
                finalStack[stackCount++] = generateStackFrame (stackVals, word);
            }
            word = 1;
            stackVals[0] = varName;
            stackVals[1] = varName;
            stackVals[2] = varName;
            stackVals[3] = varName;
            finalStack[stackCount++] = generateStackFrame (stackVals, word);
            finalStack[stackCount++] = generateStackFrame (stackVals, word);
            stackVals[0] = '\0';
            stackVals[1] = '\0';
            stackVals[2] = '\0';
            stackVals[3] = '\0';
            word = 0;
            filled = 1;
            availableBytes = 4;
        }

        // save stack frame
        if (!availableBytes || !filled || (availableBytes!=4 && index == numVars-1)) {
            finalStack[stackCount++] = generateStackFrame (stackVals, word);
            if (word) word = 0;
            stackVals[0] = '\0';
            stackVals[1] = '\0';
            stackVals[2] = '\0';
            stackVals[3] = '\0';
            availableBytes = 4;
        }
    }

    // display stack frames
    stackIndex = -(4 * stackCount);
    printf ("\t _______________________________\n");
    printf ("\t|        |             |        |\n");
    printf ("\t| OFFSET |    STACK    | MEMORY |\n");
    printf ("\t|________|_____________|________|\n");
    printf ("\t|                               |\n");
    printf ("\t|\t ---------------   LOW  |\n");
    for (index = stackCount-1; index >= 0; index--) {
        printf ("\t|  %d\t%s   ^   |\n", stackIndex, finalStack[index]);
        stackIndex += 4;
    }
    printf ("\t|  %%fp -> --------------   HIGH |\n");
    printf ("\t|_______________________________|\n");

    for (index = 0; index < stackCount; index++) {
        free (finalStack[index]);
    }
 
    return 0;
}

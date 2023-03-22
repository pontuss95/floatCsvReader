#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "floatCsvReader.h"

unsigned int formatString(char *S)
{
    // Formats a row. Removes all invalid chars.
    // Valid characters are 0-9 . , and -
    // Spaces, tabs and non-valid characters is removed.
    size_t strLen = 0;

    strLen = strlen(S);

    for (int i = 0; i < strLen; i++)
    {

        if (!(((S[i] >= '0') && (S[i] <= '9')) || (S[i] == '.') || (S[i] == ',') || (S[i] == '-')))
        {
            // Element i is unwanted. Shift all elements in front of it one step back.
            // Including null terminator
            for (int j = i + 1; j < (strLen + 1); j++)
            {
                S[j - 1] = S[j];
            }
            i--;      // Needs to check right shifted element.
            strLen--; // String is now one element shorter.
        }
    }

    // count number of commas;
    unsigned int numCommas = 0;
    for (int i = 0; i < strLen; i++)
    {
        if (S[i] == ',')
        {
            numCommas++;
        }
    }

    return numCommas;
}
Array *initArray()
{
    Array *arr = malloc(sizeof(Array));
    arr->numCols = 0;
    arr->numRows = 0;
}




void doubleAmountOfRows(Array *arr)
{
    unsigned int oldRowSize = arr->numRows;

    // Expand array
    if (arr->numRows == 0)
    {
        arr->Array = malloc(sizeof(double *) * 10);
        arr->numRows = 10;
    }
    else
    {
        arr->numRows *= 2;
        arr->Array = realloc(arr->Array, sizeof(double *) * arr->numRows);
    }

    // Allocate each row
    for (int i = oldRowSize; i < arr->numRows; i++)
    {
        arr->Array[i] = malloc(sizeof(double) * arr->numCols);
    }
}

void shrinkArray(Array *arr, unsigned int nRows)
{
    for (int i = nRows; i < arr->numRows; i++)
    {
        free(arr->Array[i]);
    }
    arr->Array = realloc(arr->Array, sizeof(double *)*nRows);
    arr->numRows = nRows;
}

void freeArray(Array *arr)
{
    for (int i = 0; i < arr->numRows; i++)
    {
        free(arr->Array[i]);
    }
    free(arr->Array);
    free(arr);
}

Array *readCsv(FILE *f)
{

    unsigned int numCommas = 0;
    char *line = (char *)malloc(sizeof(char));

    line[0] = '\0';
    size_t buffSize = 1;
    unsigned int numCommasOld = 0;
    char init = 1;

    Array *arr = initArray();

    unsigned int rowNum = 0;
    unsigned int colNum = 0;
    while (0 < getline(&line, &buffSize, f))
    {

        numCommas = formatString(line);
        if (init)
        {
            init = 0;
            numCommasOld = numCommas;
            arr->numCols = numCommas + 1;
        }
        else
        {
            rowNum++;
            if (numCommasOld != numCommas)
            {
                printf("Error colum width is not consistent\n Line %d in csv file\n", rowNum+1);
                freeArray(arr);
                return NULL;
            }
        }

        if (rowNum == arr->numRows)
        {
            doubleAmountOfRows(arr);
        }
        size_t strLen = 0;
        strLen = strlen(line);

        char *lineTmp = line;
        for (int colNum = 0; colNum < (numCommas + 1); colNum++)
        {

            if (!sscanf(lineTmp, "%lf", &arr->Array[rowNum][colNum]))
            {
                printf("Error in csv format did not find float at ro x col y");
            }

            if (colNum != numCommas)
            {
                for (; *(lineTmp) != ','; lineTmp++)
                {
                    if (lineTmp - line >= strLen)
                    {
                        printf("Buffer overflow\n");
                        freeArray(arr);
                        return NULL;
                    }
                }
                lineTmp++;
            }
        }
    }

   shrinkArray(arr, (rowNum + 1));

    return arr;
}


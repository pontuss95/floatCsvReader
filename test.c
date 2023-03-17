#include <stdio.h>
#include <stdlib.h>
#include "floatCsvReader.h"

int main()
{
    printf("asd\n");
    FILE *f = fopen("test.csv", "r");

    Array *arr = readCsv(f);
    if (arr)
    {
        printf("Array with %d rows and %d cols\n", arr->numRows, arr->numCols);

        for (int i = 0; i < arr->numRows; i++)
        {
            for (int j = 0; j < arr->numCols; j++)
            {
                if (j == 0)
                {
                    printf("%lf", arr->Array[i][j]);
                }
                else
                {

                    printf(", %lf", arr->Array[i][j]);
                }
            }
            printf("\n");
        }
    }
}
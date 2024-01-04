#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
 

int main(int argc, char **argv){
    FILE* file = fopen(argv[1],"r");

    int p = 0;
    int n = 0;
    int exponent = 0;

    fscanf(file, "%d", &p);

    int *matrix = malloc(p*p*sizeof(int)); 
    int *prod = malloc(p*p*sizeof(int)); 
    for (int row = 0; row < p; row++){
        for (int col = 0; col < p; col++){
            fscanf(file, "%d", &n);
            matrix[row*p + col] = n;
            prod[row*p + col] = n;
        }
    }

    fscanf(file, "%d", &exponent);
    for (int i = 0; i < exponent-1; i++){
        int *size = malloc(p*p*sizeof(int));
            for (int row = 0; row < p; row++){
                for (int col = 0; col < p; col++){
                    size[row*p + col] = 0;
                    for (int i = 0; i < p; i++){
                        size[row*p + col] += (matrix[row*p + i] * prod[i*p + col]);
                    }
                }
            }
        int *calculated = size;
        for (int i = 0; i < p * p; i++){
            prod[i] = calculated[i];
            }

        free(calculated);
    }

    for (int row = 0; row < p; row++){
        
        for (int col = 0; col < p; col++){
            printf("%d",prod[row*p + col]);
            if (col != p-1) printf(" ");
        }
        printf("\n");
    }

    free(matrix);
    free(prod);

    return 0;
}

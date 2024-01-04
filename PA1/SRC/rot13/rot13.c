#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv){

    if(argc == 2){
        int i;

        for(i = 0; argv[1][i] != '\0'; i++){
            int p = argv[1][i];
            if(isalpha(p)){
                if(p >= 65 && p <= 77){
                    putchar(p + 13);
                }
                else if(p > 77 && p < 90){
                    putchar(p - 13);
                }
                else if(p >= 97 && p <= 109){
                    putchar(p + 13);
                }
                else{
                    putchar(p - 13);
                }
            }
            else{
                printf("%c", argv[1][i]);
            }

        }

        printf("\n");
    }
    else{
        printf("Not the correct amount of arguments\n");
    }

  return EXIT_SUCCESS;
}

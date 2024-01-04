#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    if(argc > 1){
        int i = 0;
        for(i = 1; i < argc - 1; i++){
            for(int j = 1; j < argc - 1; j++){
                int compare = strcmp(argv[j], argv[j+1]);

                if(compare > 0){
                    char *temp = argv[j+1];
                    argv[j+1] = argv[j];
                    argv[j] = temp;
                }

            }
            
        }
        for(int x = 1; x < i; x++){
            printf("%s\n", argv[x]);
        }
        printf("%s\n", argv[i]);
    }





    

    return EXIT_SUCCESS;
}

/*
    printf("argv1 before swap: %s\n", argv[1]);
    printf("argv2 before swap: %s\n", argv[2]);
        char *temp = argv[1];
        argv[1] = argv[2];
        argv[2] = temp;
    printf("%s\n", temp);
    printf("Argv1 after swap: %s\n", argv[1]);
    printf("Argv2 after swap: %s\n", argv[2]);
*/

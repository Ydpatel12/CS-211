#include <stdio.h>
#include <stdlib.h>

struct Matrix {
    double **data;
    int row;
    int col;
};



int CompareStrings(char *p1, char *p2){
    char char1, char2;
    while(char1 == char2){
        char1 = *p1++;
		char2 = *p2++;
		if(char1 == '\0'){
			return char1 - char2;
        }
    }
	return char1 - char2;
}

struct Matrix MakeMatrix(int row, int col){
    struct Matrix newMatrix;
    newMatrix.col = col;
    newMatrix.row = row;
    newMatrix.data = malloc(sizeof(double*) * row);

    for(int i = 0; i < row; ++i) {
        newMatrix.data[i] = malloc(sizeof(double) * col);
    }
    return newMatrix;
}

void MatrixSizes(FILE **train, int *attributes, int *numofhouses, struct Matrix *X, struct Matrix *Y){
    if(fscanf(*train, "%i", attributes) == EOF){
        exit(EXIT_FAILURE);
    }

    if(fscanf(*train, "%i", numofhouses) == EOF){
        exit(EXIT_FAILURE);
    }

    *X = MakeMatrix(*numofhouses, *attributes + 1);
    *Y = MakeMatrix(*numofhouses, 1);
}

void PopulateMatricies(FILE **fp, int attributes, int numofhouses, struct Matrix *X, struct Matrix *Y){
    double value;
    for(int i = 0; i < numofhouses; ++i) {
        for(int j = 0; j < (attributes + 2); ++j) {
            if(j == 0)
                X->data[i][j] = 1;
            else {
                fscanf(*fp, "%lf", &value);

                if((j+1) == (attributes+2))
                    Y->data[i][0] = value;
                else
                    X->data[i][j] = value;
            }
        }
    }
}


struct Matrix MultiplyMatrix(struct Matrix X, struct Matrix Y){

    int col;
    double result;

    struct Matrix final = MakeMatrix(X.row, Y.col);
    for(int rrow = 0; rrow < X.row; rrow++) {
        for(int rcol = 0; rcol < Y.col; rcol++) {
            result = col = 0;
            for(int row = 0; row < Y.row; row++) {
                result += X.data[rrow][col] * Y.data[row][rcol];
                col++;
            }
            final.data[rrow][rcol] = result;
        }
    }
    return final;
}

char **sortFiles(char **file1, char **file2){
    char **files = malloc(2 * sizeof(char*));
    files[0] = *file1;
    files[1] = *file2;
    int inputread, trainread;
    for(int i = 0; i < 2; ++i){
        FILE *fp = fopen(files[i], "r");
        char FileType[6];

        if(fscanf(fp, "%s",FileType) == EOF) {
            exit(EXIT_FAILURE);
        }

        if(!CompareStrings("data", FileType)){
            inputread = 1;
            if(i == 1)
		break;

            files[0] = *file2;
            files[1] = *file1;
            i = -1;
        }

        if(!CompareStrings("train", FileType)){
            trainread = 1;

            files[0] = *file2;
            files[1] = *file1;
        }
        fclose(fp);
    }

    if(!inputread || !trainread){
        exit(EXIT_FAILURE);
    }

    return files;
}

void FreeMatrix(struct Matrix A){
    for(int i = 0; i < A.row; ++i){
        free(A.data[i]);
    }

    free(A.data);
}

int main(int argc, char **argv){

    char **files = sortFiles(&argv[1], &argv[2]);
    char *train = files[0];
    char *data = files[1];
    
    int attributes, numofhouses;
    struct Matrix matrixX, matrixY, matrixW;

    FILE *fp = fopen(train, "r");
    char buffer[6];
    fscanf(fp, "%s", buffer);

    MatrixSizes(&fp, &attributes, &numofhouses, &matrixX, &matrixY);
    PopulateMatricies(&fp, attributes, numofhouses, &matrixX, &matrixY);
    
    struct Matrix XT = MakeMatrix(matrixX.col, matrixX.row);
    for(int i = 0; i < matrixX.row; ++i) {
        for(int j = 0; j < matrixX.col; ++j){
            XT.data[j][i] = matrixX.data[i][j];
        }
        
    }

    struct Matrix XTX = MultiplyMatrix(XT, matrixX);
    FreeMatrix(matrixX);

    struct Matrix InverXTX = MakeMatrix(XTX.row, XTX.col);

    for(int i = 0; i < InverXTX.row; ++i) {
        for(int j = 0; j < InverXTX.col; ++j){ 
            InverXTX.data[i][j] = (i == j) ? 1 : 0;
        }

    }

    for(int i = 0; i < XTX.row; ++i) {
        if(XTX.data[i][i] != 1) {
           double reduce = XTX.data[i][i];
            for(int j = 0; j < XTX.col; ++j) {
               XTX.data[i][j] = XTX.data[i][j] / reduce;
               InverXTX.data[i][j] = InverXTX.data[i][j] / reduce;
            }
        }

        if((i+1) == XTX.row){
            break;
        }

        for(int j = i + 1; j < XTX.row; ++j) {
            double multi = XTX.data[j][i];
            for(int k = 0; k < XTX.col; ++k) { 
                XTX.data[j][k] -= (multi * XTX.data[i][k]);
                InverXTX.data[j][k] -= (multi * InverXTX.data[i][k]);
            }
        }
    }


    for(int i = (XTX.row - 1); i > 0; i--) {
        for(int j = i - 1; j >= 0; j--) {
            double multiply = XTX.data[j][i];
            for(int k = (XTX.col - 1); k >= 0; k--) {
                XTX.data[j][k] -= (multiply * XTX.data[i][k]);
                InverXTX.data[j][k] -= (multiply * InverXTX.data[i][k]);
            }
        }
    }

    FreeMatrix(XTX);

    struct Matrix InXTX_XT = MultiplyMatrix(InverXTX, XT);
    FreeMatrix(InverXTX);
    FreeMatrix(XT);

    matrixW = MultiplyMatrix(InXTX_XT, matrixY);
    FreeMatrix(InXTX_XT);
    FreeMatrix(matrixY);

    fp = fopen(data, "r");
    fscanf(fp, "%s", buffer);

    fscanf(fp, "%d", &attributes);
    fscanf(fp, "%d", &numofhouses);

    matrixX = MakeMatrix(numofhouses, attributes + 1);

    double value;
    for(int i = 0; i < numofhouses; ++i) {
        for(int j = 0; j < (attributes + 1); ++j) {
            if(j == 0)
                matrixX.data[i][j] = 1;

            else {
                fscanf(fp, "%lf", &value);
                matrixX.data[i][j] = value;
            }
        }
    }

    matrixY = MultiplyMatrix(matrixX, matrixW);

    for(int i = 0; i < matrixY.row; ++i) {
        for(int j = 0; j < matrixY.col; ++j){
            printf("%.0f\n", matrixY.data[i][j]);
        }
    }

    FreeMatrix(matrixW);
    FreeMatrix(matrixX);
    FreeMatrix(matrixY);
    free(files);
    
    return EXIT_SUCCESS ;
}


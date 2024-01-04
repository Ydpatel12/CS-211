#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


typedef enum { NOT, AND, OR, NAND, NOR, XOR, DECODER, MULTIPLEXER, PASS } gateType;

char *gateName[9] = {"NOT", "AND", "OR", "NAND", "NOR", "XOR", "DECODER", "MULTIPLEXER", "PASS"};

typedef struct variable {
    int out;
    char *name; 
    int boolean; 
}Variable;

typedef struct gate {
    gateType type;
    int *paramaters;
    int size; 
}Gate;

typedef struct circuit {
    int numOfgateList;
    int numOfOutputs;
    int numOfInputs; 
    int *inputs; 
    int *outputs;
    Gate **gateList;
    int numOfVariables; 
    Variable **arrVarStructs; 
}Circuit;

void freeVariables(Variable *var){

    if (var != NULL){
        if (var->name != NULL){
            free(var->name);
        } 
        
    free(var);
    }

}

void gateListFreed(Gate *gate){

    if (gate != NULL){
        if (gate->paramaters != NULL) free(gate->paramaters);
            
    free(gate);
    }
}

void freeTheCircuit(Circuit *circuitry){

    if (circuitry != NULL){
        if (circuitry->numOfInputs && circuitry->inputs != NULL) free(circuitry->inputs);
        if (circuitry->numOfOutputs && circuitry->outputs != NULL) free(circuitry->outputs);
        
        if (circuitry->arrVarStructs != NULL){

            for (int i = 0; i < circuitry->numOfVariables; i++){
                freeVariables(circuitry->arrVarStructs[i]);
            } 
            
            free(circuitry->arrVarStructs);
        }

        if (circuitry->gateList != NULL){

            for (int i = 0; i < circuitry->numOfgateList; i++){
                gateListFreed(circuitry->gateList[i]);
            } 
                
            free(circuitry->gateList);
        }
        
        free(circuitry);
    }
}

int variableSearch(char *var, Circuit *circuitry){

    int index = -1;
    for (int i = 0; i < circuitry->numOfVariables && index == -1; i++){

        if (!strcmp(var, circuitry->arrVarStructs[i]->name))
            index = i;
    }

    return index;
}

Variable *newVar(char *name, Circuit *circuitry){

    Variable *var = (Variable *) malloc(sizeof(Variable));
    

    var->name = (char *) malloc(strlen(name) + 1);

    strcpy(var->name, name); 
    var->boolean = -1; 
    var->out = 0; 

    return var;
}

int includeVariable(char *var_name, Circuit *circuitry){

    int n;
    Variable **arrVarStructs;
    n = variableSearch(var_name, circuitry);

    if (n == -1){

        n = circuitry->numOfVariables;
        if (n == 0) arrVarStructs = (Variable **) malloc(sizeof(Variable *));

        else arrVarStructs = (Variable **) realloc(circuitry->arrVarStructs, (n + 1) * sizeof(Variable *));

        circuitry->arrVarStructs = arrVarStructs;
        circuitry->arrVarStructs[n] = newVar(var_name, circuitry);
        circuitry->numOfVariables++;

    }

    return n; 
}

Circuit *createNewCircuit(){

    int i;
    Circuit *circuitry = (Circuit *) malloc(sizeof(Circuit));

    memset(circuitry, 0, sizeof(Circuit));
    i = includeVariable("0", circuitry); 

    circuitry->arrVarStructs[i]->boolean = 0; 
    i = includeVariable("1", circuitry); 

    circuitry->arrVarStructs[i]->boolean = 1; 
    includeVariable("_", circuitry); 

    return circuitry;
}

Gate *newGate(gateType type, int n, int size, Circuit *circuitry){

    Gate *gate = (Gate *) malloc(sizeof(Gate));

    gate->type = type;
    gate->paramaters = (int *) malloc(n *sizeof(int *));
    gate->size = size;

    return gate;
}

void includeGate(Gate *gate, Circuit *circuitry){

    int n;
    n = circuitry->numOfgateList;
    Gate **gateList;

    if (n == 0){
        gateList = (Gate **) malloc(sizeof(Gate *));
    }

    else{
        gateList = (Gate **) realloc(circuitry->gateList, (n + 1) * sizeof(Gate *));
    }

    circuitry->gateList = gateList;
    circuitry->gateList[n] = gate; 
    circuitry->numOfgateList++;
}

int getBooleanValue(int start, int end, Gate *gate, Circuit *circuitry){

    int i;
    int exponent = 0;
    int val = 0;

    for (i = end, exponent = 0; i >= start && val != -1; i--, exponent++){

        if (circuitry->arrVarStructs[gate->paramaters[i]]->boolean == -1){
            val = -1;
        }

        else{
             val += circuitry->arrVarStructs[gate->paramaters[i]]->boolean << exponent;
        }
    }

    return val;

}

int computeGate(Gate *gate, Circuit *circuitry){
    
    int output = -1;
    int a;
    int b;
    int i;
    int holder;
    
    a = circuitry->arrVarStructs[gate->paramaters[0]]->boolean;
    b = circuitry->arrVarStructs[gate->paramaters[1]]->boolean;

    switch (gate->type){

    case PASS:

        output = a;
        circuitry->arrVarStructs[gate->paramaters[1]]->boolean = output;

        break;

    case AND:

        if (a != -1 && b != -1){
            output = a & b;
        }

        circuitry->arrVarStructs[gate->paramaters[2]]->boolean = output;

        break;

    case NAND:

        if (a != -1 && b != -1) output = (~(a & b)) & 1;
        circuitry->arrVarStructs[gate->paramaters[2]]->boolean = output;

        break;

    case NOR:

        if (a != -1 && b != -1) output = (~(a | b)) & 1;
        circuitry->arrVarStructs[gate->paramaters[2]]->boolean = output;

        break;

    case OR:

        if (a != -1 && b != -1) output = a | b;
        circuitry->arrVarStructs[gate->paramaters[2]]->boolean = output;

        break;

    case XOR:

        if (a != -1 && b != -1) output = a ^ b;
        circuitry->arrVarStructs[gate->paramaters[2]]->boolean = output;

        break;

    case NOT:

        if (a != -1) output = (~a) & 1;
        circuitry->arrVarStructs[gate->paramaters[1]]->boolean = output;

        break;

    case DECODER:

        holder = getBooleanValue(0, gate->size - 1, gate, circuitry);

        if (holder == -1){
            for (i = 0; i < (1 << gate->size); i++){
                circuitry->arrVarStructs[gate->paramaters[gate->size + i]]->boolean = -1;
            }     
        }

        else{

            for (i = 0; i < (1 << gate->size); i++){
                circuitry->arrVarStructs[gate->paramaters[gate->size + i]]->boolean = 0;
            }

            circuitry->arrVarStructs[gate->paramaters[gate->size + holder]]->boolean = 1;
            output = 1;
        }

        break;

    case MULTIPLEXER:

        holder = getBooleanValue(1 << gate->size, (1 << gate->size) + gate->size - 1, gate, circuitry);

        if (holder == -1){
            circuitry->arrVarStructs[gate->paramaters[(1 << gate->size) + gate->size]]->boolean = -1;
        }

        else{

            output = circuitry->arrVarStructs[gate->paramaters[holder]]->boolean;
            circuitry->arrVarStructs[gate->paramaters[(1 << gate->size) + gate->size]]->boolean = output;

        }

        break;
    }

    return output;

}

int nextInput(Circuit *circuitry){
    int num = 1;

    for (int i = circuitry->numOfInputs - 1; i >= 0 && num == 1; i--){

        if (circuitry->arrVarStructs[circuitry->inputs[i]]->boolean == 0){
            circuitry->arrVarStructs[circuitry->inputs[i]]->boolean = 1;
            num = 0;

        }

        else{

            circuitry->arrVarStructs[circuitry->inputs[i]]->boolean = 0;
            num = 1; 
        }
    }

    return num;
}

void printingInputs(Circuit *circuitry){
    for (int i = 0; i < circuitry->numOfInputs; i++){
        printf("%d ", circuitry->arrVarStructs[circuitry->inputs[i]]->boolean);
    }

}

void printingOutputs(Circuit *circuitry){
    for (int i = 0; i < circuitry->numOfOutputs; i++){
        printf(" %d", circuitry->arrVarStructs[circuitry->outputs[i]]->boolean);
    }
}

int correctOutputs(Circuit *circuitry){
    for (int i = 0; i < circuitry->numOfOutputs; i++){
        if (circuitry->arrVarStructs[circuitry->outputs[i]]->boolean == -1){
            return 0;
        }
    }
    
    return 1;
}

int needOuputs(Circuit *circuitry){

    for (int i = 3; i < circuitry->numOfVariables; i++)

        if (circuitry->arrVarStructs[i]->out){
            circuitry->arrVarStructs[i]->boolean = -1;
        }
    
    return 0;
}

int compute(Circuit *circuitry){

    int i;
    int num = 0;

    for (i = 0; i < circuitry->numOfInputs; i++)

        circuitry->arrVarStructs[circuitry->inputs[i]]->boolean = 0;

    while (!num){

        printingInputs(circuitry);
        needOuputs(circuitry);

        while (!correctOutputs(circuitry)){ 
            for (i = 0; i < circuitry->numOfgateList; i++) 
                computeGate(circuitry->gateList[i], circuitry); 
        }

        printf("|");
        printingOutputs(circuitry);
        printf("\n");

        num = nextInput(circuitry); 

    }

    return 0;
}



int main(int argc, char **argv){

    Circuit *circuitry;
    FILE *read;

    char temp[20];

    int n;
    int i;
    int j;
    int size;

    Gate *gate;
    gateType type;

    int outi;
    int nouts;

    read = fopen(argv[1], "r"); 
    circuitry = createNewCircuit();
    fscanf(read, "%16s %d", temp, &circuitry->numOfInputs);
    circuitry->inputs = (int *) malloc(circuitry->numOfInputs*sizeof(int));


    for (i = 0; i < circuitry->numOfInputs; i++){

        fscanf(read, "%16s", temp); 
        n = includeVariable(temp, circuitry);
        circuitry->inputs[i] = n; 
    }

    fscanf(read, "%16s %d", temp, &circuitry->numOfOutputs); 
    circuitry->outputs = (int *) malloc(circuitry->numOfOutputs*sizeof(int)); 

    for (i = 0; i < circuitry->numOfOutputs; i++){

        fscanf(read, "%16s", temp); 
        n = includeVariable(temp, circuitry);
        circuitry->outputs[i] = n;
        circuitry->arrVarStructs[circuitry->outputs[i]]->out = 1;

    }

    while (!feof(read)){

        if(fscanf(read, "%16s", temp) == 1){
            nouts = 1;
            outi = 2;

            if (!strcmp(temp, "PASS")){
                size = 0;
                n = 2;
                outi = 1;
                nouts = 1;
                type = PASS;
            }
            
            if (!strcmp(temp, "NOT")){
                size = 0;
                n = 2;
                outi = 1;
                nouts = 1;
                type = NOT;
            }

            else if (!strcmp(temp, "AND")){
                size = 0;
                n = 3;
                type = AND;
            }

            else if (!strcmp(temp, "OR")){
                size = 0;
                n = 3;
                type = OR;
            }

            else if (!strcmp(temp, "XOR")){
                size = 0;
                n = 3;
                type = XOR;
            }

            else if (!strcmp(temp, "NAND")){
                size = 0;
                n = 3;
                type = NAND;
            }

            else if (!strcmp(temp, "NOR")){
                size = 0;
                n = 3;
                type = NOR;
            }

            else if (!strcmp(temp, "DECODER")){
                fscanf(read, "%d", &size); 
                n = size + (1 << size); 
                outi = size; 
                nouts = 1 << size;
                type = DECODER; 
            }

            else if (!strcmp(temp, "MULTIPLEXER")){

                fscanf(read, "%d", &size); 

                n = size + (1 << size) + 1;
                outi = size + (1 << size); 
                nouts = 1; 

                 type = MULTIPLEXER;
            }

            gate = newGate(type, n, size, circuitry);

            for (int z = 0; z < n; z++){

                fscanf(read, "%16s", temp);
                j = includeVariable(temp, circuitry);

                gate->paramaters[z] = j;

                if (z >= outi && z <= outi + nouts - 1) circuitry->arrVarStructs[j]->out = 1;
            }

            includeGate(gate, circuitry);
        }
    }

    compute(circuitry);
    freeTheCircuit(circuitry);

    return 0;
}

# include <stdio.h>

int main(){

    float *v;
    int num;

    printf("Digite a quantidade de elementos que deseja armazenar de forma dinâmica: \n");
    scanf("%d", &num);

    //printf("int: %d \nfloat: %d \nchar: %d \n", sizeof(int), sizeof(float), sizeof(char));

    v = (float *) malloc(num * sizeof(float));

    printf("Preencha o vetor dinâmico: \n");

    for(int i = 0; i < num; i++){
        scanf("%f", &v[i]);
    }

    for(int i = 0; i < num; i++){
        printf("Posição %d, valor %f \n", i, v[i]);
    }

    free(v);

    return 0;

}

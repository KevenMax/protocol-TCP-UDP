#include <stdio.h>

int main(){

    struct aluno{
        char nome[70];
        int idade;
        int matricula;
    };

    struct aluno a1;

    printf("Digite as informações do aluno: \n\n");

    printf("Nome: \n");
    scanf(" %[^\n]s", a1.nome);

    printf("Idade: \n");
    scanf("%d", &a1.idade);

    printf("Matricula: \n");
    scanf("%d", &a1.matricula);

    printf("Resumo: \nNome: %s \nIdade: %d \nMatricula: %d", a1.nome, a1.idade, a1.matricula);

}

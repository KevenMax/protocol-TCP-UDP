#include <stdio.h>

int main(){
    int *v;
    v = primos();
    printf("%d", v);
    //printf("%d", v[0]);
}

int primos() {
   int *v;
   v[0] = 1009; v[1] = 1013; v[2] = 1019;
    return v;
}

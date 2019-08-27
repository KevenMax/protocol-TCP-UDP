#include <stdio.h>

int main(){
    int v[3];
    primos(v);
    printf("%d", v[0]);
}

void primos(int v[]) {
   v[0] = 1009; v[1] = 1013; v[2] = 1019;
}

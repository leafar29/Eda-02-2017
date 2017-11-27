#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
    double valor;
    double nivel;

    scanf("%lf", &valor);

    if(valor == 0)
        nivel = 0;
    else if(valor == 1)
        nivel = 1;
    else
        nivel = log(valor)/log(2) + 1;
    
    printf("%d", (int)nivel)  ;


    return 0;
}
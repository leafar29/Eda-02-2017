#include<stdio.h>
#include<stdlib.h>

typedef struct arv{
    int val;
    struct arv *esq;
    struct arv *dir;
}Arv;

int arv_vazia(Arv* a){
    return (a == NULL);
}



 int main(){

    return 0;
 }
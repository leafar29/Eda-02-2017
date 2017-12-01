#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int hash(char* v, int M){
    int i, h = v[0];
    for(i = 1; v[i] != '\0'; i++)
        h = (h * 251 + v[i]) %M;
    return h;
}


void ler_nome(char *s){
    char c;
    c = getchar();
    while(c!='\n'){
        *s = c;
        c=getchar();
        s++;
    }
}


int main(){
    int n =0 ,m=0;
    int i,j;
    char s[50];
    char *v;
    int *h;
    int aux, cont = 0;

    scanf("%d ", &m);
    scanf("%d", &n);    
    
    h = malloc(n * sizeof(int));
    
    for(i = 0; i < n; i++){    
        //s[0] = '\0';
        scanf("%s", s);
        v = (char*)malloc(strlen(s) * sizeof(char));
        strcpy(v,s);
        h[i] = hash(v,m);
        free(v);
        for(j = i-1; j >= 0; j--){
            aux = h[i];
            if(aux == h[j])
                cont++;
        }
    }

    printf("Colisões: %d\n", cont);

    return 0;
}
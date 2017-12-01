#include<stdio.h>
#include<stdlib.h>

/* Rotação para a direita */
void rot_dir(No* p){
  No *q, *temp;
  
  q = p->esq;
  temp = q->dir
  q->dir = p;
  p->esq = temp;
  p = q;
}

/*Rotação para a esquerda */
void rot_esq(No* p){
  No *q, *temp;
  
  q = p->dir;
  temp = q->esq;
  q->esq = p;
  p->dir = temp;
  p = q;
}

/*ROtação Esquerda direita */
void rot_esq_dir(No* p){
  rot_esq(p->esq);
  rot_dir(p);
}

/*Rotação direita esquerda */
void rot_di_esq(No* p){
  rot_dir(p->dir);
  rot_esq(p);
}

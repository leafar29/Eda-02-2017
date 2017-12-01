//Rafael Makaha Gomes Ferreira - 16042369
//Renan Welz Schadt - 160143403

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LIMITE_INPUTS 300
#define NAO_ACHOU -1
#define CADASTRADO 1
#define EXCLUIDO 0
#define NOME 0
#define MATRICULA 1

typedef struct  {
	char nome[30];
	int matr;
	int status;
} Registro;
typedef struct elemento {
	int indice;
	int mat_aluno;
	char letra_aluno;
	struct elemento  *prox;
} Elemento;

void leia_string(char* input){
	char c;
	int i = 0;

	c = getchar();
	if(c == '\n')
		c = getchar();
	while(c != '\n'){
		input[i] = c;
		c = getchar();
		i++;
	}
	input[i] = '\0';
}

Elemento* insere_vimatnome(Elemento *l, int ind_alu, char pl_alu, int mat_alu, int tipoI) {
	Elemento* novo = (Elemento*)malloc(sizeof(Elemento));
	Elemento* aux;
	Elemento* aux_ant;

	novo->letra_aluno = pl_alu;
	novo->indice = ind_alu;
	novo->mat_aluno = mat_alu;
	novo->prox = NULL;

	if(tipoI){
		if(l == NULL){ /*checa se o primeiro elemento é nulo*/
			l = novo;
		}else{
			if(l->prox == NULL)
				l->prox = novo;
			else{
				aux_ant = l;
				aux = l->prox;
				
				while(aux->prox != NULL) {
					if(novo->letra_aluno < aux->letra_aluno) {
							novo->prox = aux;
							aux_ant->prox = novo;
							break;
					}
				aux_ant = aux_ant->prox;
				aux = aux->prox;
				}
				if(aux->prox == NULL){
					aux->prox = novo;
				}
			}
		}
	}else{
		if(l == NULL){ /*checa se o primeiro elemento é nulo*/
			l = novo;
		}else{
			if(l->prox == NULL)
				l->prox = novo;
			else{
				aux_ant = l;
				aux = l->prox;
				while(aux->prox != NULL) {
					if(novo->mat_aluno < aux->mat_aluno) {
							novo->prox = aux;
							aux_ant->prox = novo;
							break;
					}
				aux_ant = aux_ant->prox;
				aux = aux->prox;
				}
				if(aux->prox == NULL){
					aux->prox = novo;
				}
			}
		}

	}

	return (l);
}

Elemento * exclui_vimatnome(Elemento *l, int ind_alu) {
	Elemento *aux;
	Elemento *aux_ant;
	

	if(l == NULL)
		printf("Base vazia");
	
	
	else {
        	aux_ant = l;
        	aux = l->prox;
	        if(l->prox == NULL){
			l==NULL;
		}else{
			while(aux->prox != NULL) {
        		        if(ind_alu == aux->indice) {
        			        aux_ant->prox = aux->prox;
					free(aux);
                			break;
                		}
                	aux_ant = aux_ant->prox;
	                aux = aux->prox;
			}
	    	}
	}
}

int existe_registro_na_base(Registro *turma, int mat_alu, int limite_superior) {
	int i, indiceR;
	i=0; indiceR = NAO_ACHOU;
	while (i<limite_superior) {
		if (turma[i].status == CADASTRADO && turma[i].matr == mat_alu){
			indiceR = i;
			i = limite_superior + 1;
		}
		i++;
	}
	return (indiceR);
}

void imprime_ordenado_matnome(Elemento *l, Registro *turma) {
	Elemento  *p;
	int i;
	p=l;
	while (p!=NULL) {
		i = p->indice;
	printf("%s, %d", turma[i].nome, turma[i].matr);
	if (p->prox != NULL)
		printf("; ");
	p=p->prox;
	} 
}

void imprime_na_ordem_de_input(Registro *turma, int limite_superior) {
	int i;

	for(i =0; i < limite_superior; i++){
		if(turma[i].status){
			printf("%s,%d; ",turma[i].nome, turma[i].matr);
		}
	}
}



void main(void){
	char opcao[2], nom_alu[30];
	int qtd_inputs, indice, ind_exclusao, mat_alu;
	Registro turmalu[LIMITE_INPUTS];
	Elemento *lmat=NULL, *lnome=NULL;

	indice = 0;
	qtd_inputs = 0;

	while (qtd_inputs < LIMITE_INPUTS) {
		leia_string(opcao);
		leia_string(nom_alu);
		scanf("%d",&mat_alu);
		if (opcao[0] == 'I') {
			strcpy(turmalu[indice].nome, nom_alu);
			turmalu[indice].matr   = mat_alu;
			turmalu[indice].status = CADASTRADO;
			lnome = insere_vimatnome(lnome, indice, turmalu[indice].nome[0], turmalu[indice].matr, NOME);
			lmat  = insere_vimatnome(lmat,  indice, turmalu[indice].nome[0], turmalu[indice].matr, MATRICULA);
			if (opcao[1] == 'N') {
				imprime_ordenado_matnome(lnome, turmalu);
			} else {
				imprime_ordenado_matnome(lmat, turmalu);
			}
		indice++; /* esse indice sempre indica o limite superior do vetor */
		} else {
			ind_exclusao = existe_registro_na_base(turmalu, mat_alu, indice);
			if (ind_exclusao == NAO_ACHOU)
				printf("ENTRADA INVALIDA\n");
			else {
				turmalu[ind_exclusao].status=EXCLUIDO;
				lmat  = exclui_vimatnome(lmat, ind_exclusao);
				lnome = exclui_vimatnome(lnome,ind_exclusao);
				imprime_na_ordem_de_input(turmalu, indice);
			} 
		} 
	qtd_inputs++;
	} /* fim while */
}

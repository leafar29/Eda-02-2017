/*********************************************************************************************
* EDA 2017/2 - ESTRUTURAS DE DADOS E ALGORITMOS (Prof. Fernando W. Cruz)
* Projeto  - Arvores e funcoes de hash
* Verifica corretude de palavras de um arquivo-texto segundo um dicionario carregado em RAM.
 *********************************************************************************************/
 #include <ctype.h>
 #include <stdio.h>
 #include <sys/resource.h>
 #include <sys/time.h>
 #include <stdbool.h>
 #include <stdlib.h>
 #include <string.h>

/* Tamanho maximo de uma palavra do dicionario */
#define TAM_MAX 45
/* dicionario default */
#define NOME_DICIONARIO "dicioPadrao"

/* retornos desse programa */
#define SUCESSO                 0
#define NUM_ARGS_INCORRETO      1
#define ARQDICIO_NAOCARREGADO   2
#define ARQTEXTO_ERROABERTURA   3
#define ARQTEXTO_ERROLEITURA    4
#define ERRO_DICIO_NAOCARREGADO 5


bool *TabHash; //ponteiro para a tabela hash
unsigned int tam_hash = 0; //variavel que guarda o tamanho da tabela
unsigned int num_palavra = 0; //variavel que guarda o numero de palavras
int colisao = 0;// variavel que guarda o numero de colisoes

unsigned int FNVHash(const char* str, unsigned int len) {
  const unsigned int fnv_prime = 0x811C9DC5;
  unsigned int hash= 0;
  unsigned int i= 0;
  for(i = 0; i < len; str++, i++) {
    hash *= fnv_prime;
    hash ^= (*str);
  }
  return hash%1000000000;/*Divisao de resto por 1 bilhão, maior numero que conseguimos sem que o programa dê erro
                                                                            quanto maior este num menos colisões aparecerão*/
} /* End Of FNVHash Function */



/* Retorna true se a palavra estah no dicionario. Do contrario, retorna false */
bool conferePalavra(const char *palavra) {
	char Palavra[TAM_MAX];
	unsigned int hash, i;
	for(i = 0; i <= strlen(palavra); i++){
		if(palavra[i] != '\''){
			Palavra[i] = tolower(palavra[i]); //coloca caracter a caracter em caixa baixa (ctype.h)
		}
		else
			Palavra[i] = palavra[i];
	}

  hash = FNVHash(Palavra, strlen(Palavra)); //define o codigo hash da palavra a ser comparada

  if(TabHash[hash] == 1){ // Se o codigo hash é uma posiçao do vetor ocupada com true, a palavra é válida
		return true;
	}

  return false; // Se o codigo hash é uma posiçao do vetor que armazena false, a palavra não está no dicionario
} /* fim-conferePalavra */

/* Carrega dicionario na memoria. Retorna true se sucesso; senao retorna false. */
bool carregaDicionario(const char *dicionario) {
	unsigned int hashPalavra, i;
  char palavra[TAM_MAX];
	FILE* arq = fopen(dicionario, "r");

  if(arq == NULL) //Se o arquivo estiver vazio, retorna false
		return false;

	while(!feof(arq)){ //Enquanto não chega ao final do arquivo
    fscanf(arq, "%s", palavra);
  	hashPalavra = FNVHash(palavra, strlen(palavra)); //Define um hash para cada palavra lida

    /*Compara o hash da ultima palavra lida com o maior hash da lista (+100), se o novo hash é maior, ele realoca o espaço da
    tabela, para que a posição do novo hash seja válida*/
		if(hashPalavra > tam_hash){
			TabHash = realloc(TabHash, (hashPalavra+1)*sizeof(bool));
      tam_hash = hashPalavra + 100;
		}

    //Tratamento de colisoes, Enderaçamento Aberto - Sondagem Linear
    // Em caso de colisão, este método coloca "true" na próxima posição "false" que encontrar (hashPalavra++)
    if(TabHash[hashPalavra] == 1){
      do{
        hashPalavra++;
        if(TabHash[hashPalavra] == 0){
          TabHash[hashPalavra] = 1;
          break;
        }
      } while(TabHash[hashPalavra] == 1);

      colisao++;
    }

    //A TabHash é um vetor de bool, hashPalavra são as posições, quando TabHash[x] = 1, alguma palavra obeteve o código hash 'x'
    TabHash[hashPalavra] = 1;
    num_palavra++;//Adiciona uma palavra a contagem do numero de palavras do dicionario
	}

  return true;
} /* fim-carregaDicionario */


/* Retorna qtde palavras do dicionario, se carregado; senao carregado retorna zero */
/*Palavras são contadas a medida que foram inseridas, para economizar desempenho*/
unsigned int contaPalavrasDic(void) {
  if(num_palavra == 0) //Dicionario sem conteudo
    return 0;
  printf("\n");
  printf("COLISOES TRATADAS: %d", colisao);//Numero de colisoes encontradas e tratadas
  return num_palavra;

} /* fim-contaPalavrasDic */

/* Descarrega dicionario da memoria. */
bool descarregaDicionario(void) {
	free(TabHash);
	return true;
} /* fim-descarregaDicionario */

/* Retorna o numero de segundos entre a e b */
double calcula_tempo(const struct rusage *b, const struct rusage *a) {
    if (b == NULL || a == NULL)
        return 0;
    else
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
} /* fim-calcula_tempo */


int main(int argc, char *argv[]) {
    struct rusage tempo_inicial, tempo_final; /* structs para dados de tempo do processo */
    double tempo_carga = 0.0, tempo_check = 0.0, tempo_calc_tamanho_dic = 0.0, tempo_limpeza_memoria = 0.0;
    /* determina qual dicionario usar; o default eh usar o arquivo dicioPadrao */
    char *dicionario = (argc == 3) ? argv[1] : NOME_DICIONARIO;
    int  indice, totPalErradas, totPalavras, c;
    char palavra[TAM_MAX+1];
    bool palavraErrada, descarga, carga;
    unsigned int qtdePalavrasDic;
    char *arqTexto;
    FILE *fd;

    /* Confere se o numero de argumentos de chamada estah correto */
    if (argc != 2 && argc != 3) {
        printf("Uso: %s [nomeArquivoDicionario] nomeArquivoTexto\n", argv[0]);
        return NUM_ARGS_INCORRETO;
    } /* fim-if */

    /* carrega o dicionario na memoria, c/ anotacao de tempos inicial e final */
    getrusage(RUSAGE_SELF, &tempo_inicial);
       carga = carregaDicionario(dicionario);
    getrusage(RUSAGE_SELF, &tempo_final);

    /* aborta se o dicionario nao estah carregado */
    if (!carga) {
        printf("Dicionario nao carregado!\n");
        return ARQDICIO_NAOCARREGADO;
    } /* fim-if */

    /* calcula_tempo para carregar o dicionario */
    tempo_carga = calcula_tempo(&tempo_inicial, &tempo_final);

    /* tenta abrir um arquivo-texto para corrigir */
    arqTexto = (argc == 3) ? argv[2] : argv[1];
    fd = fopen(arqTexto, "r");
    if (fd == NULL) {
        printf("Nao foi possivel abrir o arquivo de texto %s.\n", arqTexto);
        descarregaDicionario();
        return ARQTEXTO_ERROABERTURA;
    } /* fim-if */

    /* Reportando palavras erradas de acordo com o dicionario */
    printf("\nPALAVRAS NAO ENCONTRADAS NO DICIONARIO \n\n");

    /* preparando para checar cada uma das palavras do arquivo-texto */
    indice = 0, totPalErradas = 0, totPalavras = 0;

    /* checa cada palavra do arquivo-texto  */
    for (c = fgetc(fd); c != EOF; c = fgetc(fd)) {
        /* permite apenas palavras c/ caracteres alfabeticos e apostrofes */
        if (isalpha(c) || (c == '\'' && indice > 0)) {
            /* recupera um caracter do arquivo e coloca no vetor palavra */
            palavra[indice] = c;
            indice++;
            /* ignora palavras longas demais (acima de 45) */
            if (indice > TAM_MAX) {
                /* nesse caso, consome o restante da palavra e "reseta" o indice */
                while ((c = fgetc(fd)) != EOF && isalpha(c));
                indice = 0;
            } /* fim-if */
        } /* fim-if */
        /* ignora palavras que contenham numeros */
        else if (isdigit(c)) {
            /* nesse caso, consome o restante da palavra e "reseta" o indice */
            while ((c = fgetc(fd)) != EOF && isalnum(c));
            indice = 0;
        } /* fim-if */
        /* encontra uma palavra completa */
        else if (indice > 0) { /* termina a palavra corrente */
            palavra[indice] = '\0';
            totPalavras++;
            /* confere o tempo de busca da palavra */
            getrusage(RUSAGE_SELF, &tempo_inicial);
              palavraErrada = !conferePalavra(palavra);
            getrusage(RUSAGE_SELF, &tempo_final);
            /* atualiza tempo de checagem */
            tempo_check += calcula_tempo(&tempo_inicial, &tempo_final);
            /* imprime palavra se nao encontrada no dicionario */
            if (palavraErrada) {
                printf("%s\n", palavra);
                totPalErradas++;
            } /* fim-if */
            /* faz "reset" no indice para recuperar nova palavra no arquivo-texto*/
            indice = 0;
        } /* fim-if */
    } /* fim-for */

    /* verifica se houve um erro na leitura do arquivo-texto */
    if (ferror(fd)) {
        fclose(fd);
        printf("Erro de leitura %s.\n", arqTexto);
        descarregaDicionario();
        return ARQTEXTO_ERROLEITURA;
    } /* fim-if */

    /* fecha arquivo */
    fclose(fd);

    /* determina a quantidade de palavras presentes no dicionario, c anotacao de tempos */
    getrusage(RUSAGE_SELF, &tempo_inicial);
      qtdePalavrasDic = contaPalavrasDic();
    getrusage(RUSAGE_SELF, &tempo_final);

    /* calcula tempo p determinar o tamanho do dicionario */
    tempo_calc_tamanho_dic = calcula_tempo(&tempo_inicial, &tempo_final);

    /* limpa a memoria - descarrega o dicionario, c anotacao de tempos */
    getrusage(RUSAGE_SELF, &tempo_inicial);
      descarga = descarregaDicionario();
    getrusage(RUSAGE_SELF, &tempo_final);

    /* aborta se o dicionario nao estiver carregado */
    if (!descarga) {
        printf("Nao foi necessario fazer limpeza da memoria\n");
        return ERRO_DICIO_NAOCARREGADO;
    } /* fim-if */

    /* calcula tempo para descarregar o dicionario */
    tempo_limpeza_memoria = calcula_tempo(&tempo_inicial, &tempo_final);

    /* RESULTADOS ENCONTRADOS */
    printf("\n");
    printf("TOTAL DE PALAVRAS ERRADAS NO TEXTO    : %d\n",   totPalErradas);
    printf("TOTAL DE PALAVRAS DO DICIONARIO         : %d\n",   qtdePalavrasDic);
    printf("TOTAL DE PALAVRAS DO TEXTO              : %d\n",   totPalavras);
    printf("TEMPO GASTO COM CARGA DO DICIONARIO     : %.2f\n", tempo_carga);
    printf("TEMPO GASTO COM CHECK DO ARQUIVO        : %.2f\n", tempo_check);
    printf("TEMPO GASTO P CALCULO TAMANHO DICIONARIO: %.2f\n", tempo_calc_tamanho_dic);
    printf("TEMPO GASTO COM LIMPEZA DA MEMORIA      : %.2f\n", tempo_limpeza_memoria);
    printf("------------------------------------------------------\n");
    printf("T E M P O   T O T A L                   : %.2f\n\n",
     tempo_carga + tempo_check + tempo_calc_tamanho_dic + tempo_limpeza_memoria);
    printf("------------------------------------------------------\n");

   return SUCESSO;
} /* fim-main */

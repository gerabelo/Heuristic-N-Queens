/*
        Um sistema evolutivo que busca solucao para o Problema das Oito Rainhas
        escrito por Geraldo Rabelo <geraldo.rabelo@gmail.com>
        setembro 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define POPULACAO       100
//#define ELITE_POPULACAO 10
#define GERACOES        500
#define GENES           8 

/*
                Calculando o tamanho do grupo Elite com base no tamanho da população.

                Arranjo = n!/(n-p)!
                populacao = elite!/(elite-p)!
                
                p = 2   os novos individuos serão formados por arranjo entre pares.
                
                populacao*(elite-2)! = elite!
                populacao*(elite-2)! = elite*(elite-1)!
                populacao*(elite-2)! = elite*(elite-1)*(elite-2)!                
                populacao = elite*(elite-1)
                elite^2 - elite - populacao = 0
                elite = (-1+sqrt(1+4*populacao))/2                
*/

struct cromossomo {
        int genes[GENES];
        int aptidao;
} CROMOSSOMO;

typedef struct cromossomo Cromossomo;

Cromossomo individuos[POPULACAO];
Cromossomo *elite;

int populacao_atual     = 0;
int geracao             = 0;
int elite_da_populacao  = 0;

int total_de_rainhas_corretamente_posicionadas (int candidato) {       
        /*
                Esta função trata das Restrições a cerca do posicionamento das Queens
        */
        int linha                                               = 0;
        int numero_de_rainhas_posicionadas_corretamente         = 0;
        int vetor_colisoes[GENES]                               = {-1,-2,-3,-4,-5,-6,-7,-8};
        int contador_vetor_colisoes                             = 0;
        int dejavu_colisao                                      = 0;
        int colisao_flag                                        = 0;      
        
        
        for (int coluna = 0; coluna < GENES; coluna++) {
                
                linha = individuos[candidato].genes[coluna];
                
                colisao_flag = 0;
                
                        for (int c0 = coluna+1; c0 < GENES; c0++) {
                
                                
                        
                                for (int c1 = 0; c1 < 3; c1++) {
                                        // Três é o número máximo de linhas bloqueadas em cada coluna adjacente a uma já ocupada
                                
                                        if (c1 == 0 && individuos[candidato].genes[c0] == linha) {                                        
                                                dejavu_colisao = 0;
                                                for (int c2 = 0; c2 < GENES; c2++) {
                                                        if (vetor_colisoes[c2] == c0) { 
                                                                dejavu_colisao = 1; 
                                                        }
                                                }
                                                if (dejavu_colisao == 0) {
                                                        
                                                        vetor_colisoes[contador_vetor_colisoes] = c0;
                                                        contador_vetor_colisoes++;
                                                }
                                                colisao_flag = 1;

                                        } else if ((c1 == 1) && (individuos[candidato].genes[c0] == linha+c0-coluna) && (linha+c0-coluna < 8)) 
                                        { 

                                                dejavu_colisao = 0;
                                                for (int c2 = 0; c2 < GENES; c2++)
                                                {
                                                        if (vetor_colisoes[c2] == c0)
                                                        { 
                                                                dejavu_colisao = 1; 
                                                        }
                                                }
                                                if (dejavu_colisao == 0)
                                                {
                                                        vetor_colisoes[contador_vetor_colisoes] = c0;
                                                        contador_vetor_colisoes++;
                                                }
                                                colisao_flag = 1;

                                        } else if (c1 == 2 && (individuos[candidato].genes[c0] == linha-c0+coluna) && (linha-c0+coluna > -1)) 
                                        {

                                                dejavu_colisao = 0;
                                                for (int c2 = 0; c2 < GENES; c2++)
                                                {
                                                        if (vetor_colisoes[c2] == c0)
                                                        {
                                                                dejavu_colisao = 1; 
                                                        }
                                                }
                                                if (dejavu_colisao == 0)
                                                {
                                                        vetor_colisoes[contador_vetor_colisoes] = c0;
                                                        contador_vetor_colisoes++;
                                                }
                                                colisao_flag = 1;
                                        }
                                }
                        }

                if (colisao_flag == 1)
                {
                        dejavu_colisao = 0;
                        for (int c2 = 0; c2 < GENES; c2++) 
                        {
                                if (vetor_colisoes[c2] == coluna) 
                                { 
                                        dejavu_colisao = 1; 
                                }
                        }
                        if (dejavu_colisao == 0) 
                        {
                                vetor_colisoes[contador_vetor_colisoes] = coluna;
                                contador_vetor_colisoes++;
                        }                        
                }
        }
        
        for (int c2 = 0; c2 < GENES; c2++) 
        {
                if (vetor_colisoes[c2] < 0)
                { 
                        numero_de_rainhas_posicionadas_corretamente++; 
                }
        }                        
                        
        return numero_de_rainhas_posicionadas_corretamente;
}

void gerar_populacao_inical (int limite_populacional) {
        for (int individuo = 0; individuo < limite_populacional; individuo++) {
                individuos[individuo].aptidao = 0;
                for (int gene = 0; gene < GENES; gene++) {
                        individuos[individuo].genes[gene] = (rand() % GENES);
                }
        }
}

void reproducao(int pai) {
        
        for (int c0 = 0; c0 < (GENES/2); c0++) {
                individuos[populacao_atual].genes[c0] = individuos[pai].genes[c0];
        }
        
        for (int c1 = 0; c1 < elite_da_populacao; c1++) {

                for (int c2 = GENES/2; c2 < GENES; c2++) {
                        if (c1 != pai) {
                                individuos[populacao_atual].genes[c2] = individuos[c1].genes[c2];
                        }
                }
                
                //mutação
                
                for (int c0 = (rand()%GENES); c0 < (GENES-1); c0++)
                {
                        individuos[populacao_atual].genes[(rand() % GENES)] = (rand() % GENES);
                }
                
                individuos[populacao_atual].aptidao = 0;
                populacao_atual++;
        }
}

void gerar_nova_populacao () {
        populacao_atual = elite_da_populacao;
        for (int c0 = 0; c0 < elite_da_populacao; c0++) {
                individuos[c0].aptidao = elite[c0].aptidao;
                for (int c1 = 0; c1 < GENES; c1++) {
                        individuos[c0].genes[c1] = elite[c0].genes[c1];
                }
        }        
        for (int individuo = 0; individuo < elite_da_populacao; individuo++) {
                reproducao(individuo);
        }
}

void preservar_elite() {
        for (int c0 = 0; c0 < elite_da_populacao; c0++) {
                for (int c1 = 0; c1 < GENES; c1++) {
                        elite[c0].genes[c1] = individuos[c0].genes[c1];
                }
                elite[c0].aptidao = individuos[c0].aptidao;
        }
}

int calcula_aptidao(int candidato) {
        return total_de_rainhas_corretamente_posicionadas(candidato);
}

void ordenar_populacao_por_aptidao (int limite_populacional) {
        Cromossomo sujeito;
        int aptidao = 0;
        int houve_troca = 0;
        
        for (int c0 = 0; c0 < limite_populacional-1; c0++) {
                if (individuos[c0].aptidao < individuos[c0+1].aptidao) {
                        for (int c1 = 0; c1 < GENES; c1++) {
                                sujeito.genes[c1] = individuos[c0+1].genes[c1];
                                individuos[c0+1].genes[c1] = individuos[c0].genes[c1];
                                individuos[c0].genes[c1] = sujeito.genes[c1];
                        }
                        sujeito.aptidao = individuos[c0+1].aptidao;
                        individuos[c0+1].aptidao = individuos[c0].aptidao;
                        individuos[c0].aptidao = sujeito.aptidao;
                        
                        houve_troca = 1;                        
                }
        }
        
        if (houve_troca == 1) {
                ordenar_populacao_por_aptidao(POPULACAO);
        }
}

void imprimir_melhor_resultado_na_tela() {
        printf("\n\tgeração: %d; melhor resultado: %d [ ",geracao,individuos[0].aptidao);

        for (int c0 = 0; c0 < GENES; c0++) {                               
                printf("%c%d ",97+c0,elite[0].genes[c0]+1);
        }
        printf("]");
}

void imprimir_individuo_na_tela(int vencedor) {
        printf("\n\tgeração: %d; melhor resultado: %d [ ",geracao,individuos[vencedor].aptidao);
        
        for (int c0 = 0; c0 < GENES; c0++) {
                printf("%c%d ",97+c0,individuos[vencedor].genes[c0]+1);
        }

        printf("]\n\n");        
}


int main(int argc, char **argv) {  

        double start_t, end_t, total_t;
        start_t = clock();        
        
        
//        FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
//        char *arg = 0;
//        size_t size = 0;  

        
        geracao = 0;
        
        printf("\n");
        
        gerar_populacao_inical(POPULACAO);  
        elite_da_populacao = round((-1+sqrt(1+4*POPULACAO)/2));
        printf("\tPreservados %d de %d individuos -Elitismo-",elite_da_populacao,POPULACAO);
        elite = malloc(elite_da_populacao*sizeof(Cromossomo));
        
        while (geracao < GERACOES) {
                for (int individuo = 0; individuo < POPULACAO; individuo++) {
                        individuos[individuo].aptidao = calcula_aptidao(individuo);
                        if (individuos[individuo].aptidao == 8) {
                                imprimir_individuo_na_tela(individuo);
                                end_t = clock();                                
                                total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                                printf("\tTempo total: %f segundos\n", total_t  );
                                printf("\n");

                                return(0);
                        }
                }
                ordenar_populacao_por_aptidao(POPULACAO);
                preservar_elite();
                imprimir_melhor_resultado_na_tela();
                gerar_nova_populacao();
                geracao++;
        }
                        
        end_t = clock();        
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("Tempo total: %f segundos\n", total_t  );
        printf("\n");


//        while(getdelim(&arg, &size, 0, cpuinfo) != -1)
//        {
//                puts(arg);
//        }
//        free(arg);
//        fclose(cpuinfo);
        
        return(0);
}

/*
   Um sistema evolutivo que busca solucao para o Problema das N Rainhas
   escrito por Geraldo Rabelo <geraldo.rabelo@gmail.com>
   setembro 2017


        Compilando:     $gcc -o Nqueens Nqueens.c -lm
        
        Executando:     $./Nqueens
        

      O que faz?
        Roda indefinidamente começando em N igual a 8 e vai incrementando. Cria um arquivo txt para cada soluçao encontrada.
      

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h> //sleep

#define POPULACAO               800
#define TAXA_MUTACAO_ELITE      0.25

struct cromossomo {
   int *genes;
   int aptidao;
} CROMOSSOMO;
typedef struct cromossomo Cromossomo;

Cromossomo individuos[POPULACAO];
Cromossomo *elite;

int populacao_atual                             = 0;
int geracao                                     = 0;
int elite_da_populacao                          = 0;
int genes                                       = 8;
int linha                                       = 0;
int numero_de_rainhas_posicionadas_corretamente = 0;
int dejavu_colisao                              = 0;
int colisao_flag                                = 0;      
int coluna                                      = 0;
int individuo                                   = 0;
int aptidao                                     = 0;
int houve_troca                                 = 0;   

int contador_vetor_colisoes                     = 0;
int contador_mutacao                            = 0;
int contador                                    = 0;
int contador_coluna                             = 0;   
int contador_posicao                            = 0;
int contador_populacao                          = 0; 
int contador_genes                              = 0;

int total_de_rainhas_corretamente_posicionadas (int candidato) {       
   /*
      Esta função trata das Restrições a cerca do posicionamento das Queens
      verifica linha e diagonais
   */
   linha                                        = 0;
   numero_de_rainhas_posicionadas_corretamente  = 0;
   int *vetor_colisoes                          = malloc(genes*sizeof(int));
   contador_vetor_colisoes                      = 0;
   dejavu_colisao                               = 0;
   colisao_flag                                 = 0;

   for (contador = genes-1; contador >= 0; contador--) {
      vetor_colisoes[contador] = 1-(contador+2);
   }   

   for (coluna = 0; coluna < genes; coluna++) {      
      linha = individuos[candidato].genes[coluna];
      colisao_flag = 0;  
      for (contador_coluna = coluna+1; contador_coluna < genes; contador_coluna++) {    
        for (contador_posicao = 0; contador_posicao < 3; contador_posicao++) {// Três é o número máximo de linhas bloqueadas em cada coluna adjacente a uma já ocupada
           if (contador_posicao == 0 && individuos[candidato].genes[contador_coluna] == linha) {     
              dejavu_colisao = 0;
              for (contador_genes = 0; contador_genes < genes; contador_genes++) {
                 if (vetor_colisoes[contador_genes] == contador_coluna) { dejavu_colisao = 1; }
              }

              if (dejavu_colisao == 0) {
                 vetor_colisoes[contador_vetor_colisoes] = contador_coluna;
                 contador_vetor_colisoes++;
              }
              colisao_flag = 1;
           } else if ((contador_posicao == 1) && (individuos[candidato].genes[contador_coluna] == linha+contador_coluna-coluna) && (linha+contador_coluna-coluna < genes)) 
           { 
              dejavu_colisao = 0;
              for (contador_genes = 0; contador_genes < genes; contador_genes++)
              {
                 if (vetor_colisoes[contador_genes] == contador_coluna)
                 { 
                    dejavu_colisao = 1; 
                 }
              }

              if (dejavu_colisao == 0)
              {
                 vetor_colisoes[contador_vetor_colisoes] = contador_coluna;
                 contador_vetor_colisoes++;
              }
              colisao_flag = 1;
           } else if (contador_posicao == 2 && (individuos[candidato].genes[contador_coluna] == linha-contador_coluna+coluna) && (linha-contador_coluna+coluna > -1)) 
           {
              dejavu_colisao = 0;

              for (contador_genes = 0; contador_genes < genes; contador_genes++)
              {
                 if (vetor_colisoes[contador_genes] == contador_coluna)
                 {
                    dejavu_colisao = 1;
                 }
              }

              if (dejavu_colisao == 0)
              {
                 vetor_colisoes[contador_vetor_colisoes] = contador_coluna;
                 contador_vetor_colisoes++;
              }
              colisao_flag = 1;
            }
         }
      }

      if (colisao_flag == 1)
      {
         dejavu_colisao = 0;

         for (contador_genes = 0; contador_genes < genes; contador_genes++) 
         {
            if (vetor_colisoes[contador_genes] == coluna) 
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

   for (contador_genes = 0; contador_genes < genes; contador_genes++) 
    {
      if (vetor_colisoes[contador_genes] < 0)
      { 
         numero_de_rainhas_posicionadas_corretamente++; 
      }
   }            

   free(vetor_colisoes);
   return numero_de_rainhas_posicionadas_corretamente;
}

void gerar_populacao_inical () {
   for (contador_populacao = 0; contador_populacao < POPULACAO; contador_populacao++) {
      individuos[contador_populacao].aptidao = 0;

      for (contador_genes = 0; contador_genes < genes; contador_genes++) {
         individuos[contador_populacao].genes[contador_genes] = (rand() % genes);
      }
   }
}

void reproducao(int pai) {      
   for (int contador_populacao = 0; contador_populacao < elite_da_populacao; contador_populacao++) {
      for (int contador_genes = 0; contador_genes < (genes/2); contador_genes++) {
         individuos[populacao_atual].genes[contador_genes] = individuos[pai].genes[contador_genes];
      }      

      for (int contador_genes = genes/2; contador_genes < genes; contador_genes++) {
         if (contador_populacao != pai) {
            individuos[populacao_atual].genes[contador_genes] = individuos[contador_populacao].genes[contador_genes];
         }
      }

      individuos[populacao_atual].genes[(rand()%genes)] = (rand()%genes);
      individuos[populacao_atual].aptidao = 0;
      populacao_atual++;
   }   
}

void mutacao_global() {
    for (int contador_genes = (rand()%genes); contador_genes < (genes-1); contador_genes++)
    {
       individuos[(rand() % POPULACAO)].genes[rand()%genes] = (rand()%genes);
       contador_mutacao++;      
    }
}

void mutacao_local_elite() {
    for (int contador_genes = 0; contador_genes < TAXA_MUTACAO_ELITE*(rand()%genes); contador_genes++)
    {
       individuos[((rand() % elite_da_populacao))].genes[rand()%genes] = (rand()%genes);
       contador_mutacao++;      
    }
}

void mutacao_local_plebe() {
   for (int contador_genes = (rand()%genes); contador_genes < (genes-1); contador_genes++)
   {
      for (int contador_populacao = 0; contador_populacao < (rand()%elite_da_populacao); contador_populacao++)
      {
         individuos[(rand() % (POPULACAO-elite_da_populacao))+elite_da_populacao].genes[rand()%genes] = (rand()%genes);
         contador_mutacao++;      
      }
   }
}

void gerar_nova_populacao () {   
   contador_mutacao = 0;
   populacao_atual = elite_da_populacao;

   for (int contador_populacao = 0; contador_populacao < elite_da_populacao; contador_populacao++) {
      individuos[contador_populacao].aptidao = elite[contador_populacao].aptidao;
      for (int contador_genes = 0; contador_genes < genes; contador_genes++) {
         individuos[contador_populacao].genes[contador_genes] = elite[contador_populacao].genes[contador_genes];
      }
   }   

   for (int contador_populacao = 0; contador_populacao < elite_da_populacao; contador_populacao++) {
      reproducao(contador_populacao);
   }
}

void preservar_elite() {
   for (int contador_populacao = 0; contador_populacao < elite_da_populacao; contador_populacao++) {
      for (int contador_genes = 0; contador_genes < genes; contador_genes++) {
         elite[contador_populacao].genes[contador_genes] = individuos[contador_populacao].genes[contador_genes];
      }
      elite[contador_populacao].aptidao = individuos[contador_populacao].aptidao;
   }
}

int selecao_natural(int especime) {
   return total_de_rainhas_corretamente_posicionadas(especime);
}

void ordenar_populacao_por_aptidao (int limite_populacional) {
   Cromossomo sujeito;
   sujeito.genes = malloc(genes*sizeof(int));   

   aptidao = 0;
   houve_troca = 0;   

   for (int contador_populacao = 0; contador_populacao < limite_populacional-1; contador_populacao++) {
      if (individuos[contador_populacao].aptidao < individuos[contador_populacao+1].aptidao) {
         for (int contador_genes = 0; contador_genes < genes; contador_genes++) {
            sujeito.genes[contador_genes] = individuos[contador_populacao+1].genes[contador_genes];
            individuos[contador_populacao+1].genes[contador_genes] = individuos[contador_populacao].genes[contador_genes];
            individuos[contador_populacao].genes[contador_genes] = sujeito.genes[contador_genes];
         }
         sujeito.aptidao = individuos[contador_populacao+1].aptidao;
         individuos[contador_populacao+1].aptidao = individuos[contador_populacao].aptidao;
         individuos[contador_populacao].aptidao = sujeito.aptidao;
         houve_troca = 1;    
      }
   }

   if (houve_troca == 1) {
      ordenar_populacao_por_aptidao(POPULACAO);
   }
   free(sujeito.genes);
}

void imprimir_melhor_resultado_na_tela() {
   //executar a função de ordenação
   printf("\n\t%d\t%d/%d",geracao,elite[0].aptidao,genes);

/*
        printf("\n\tgeração: %d; raínhas: %d de %d [ ",geracao,elite[0].aptidao,genes);

        for (contador_genes = 0; contador_genes < genes; contador_genes++) {                               
                printf("%d ",elite[0].genes[contador_genes]+1);
        }
        printf("]");
*/
}

void imprimir_individuo_na_tela(int especime) {
   printf("\n\t%d\t%d/%d [ ",geracao,individuos[especime].aptidao,genes);

   for (contador_populacao = 0; contador_populacao < genes; contador_populacao++) {
      printf("%d ",individuos[especime].genes[contador_populacao]+1);   
   }
   printf("]\n");
}

int imprimir_melhor_resultado_em_arquivo(FILE *arquivo) {
   //executar a função de ordenação
   return fprintf(arquivo,"%d;%d;\n",geracao,elite[0].aptidao);
}

void imprimir_individuo_em_arquivo(int genes,int especime, float time) {
   FILE *arquivo = NULL;
   char *file = malloc(32*sizeof(char));
   memset(file,'\0',32);
   sprintf(file,"%d_queens.txt",genes);
   arquivo = fopen(file,"w+");   
   free(file);

   if (arquivo == NULL) {
      printf("\n\nNão foi possível criar o arquivo %d_queens.txt\n\n",genes);
      exit(1);
   }

/*
   fprintf(arquivo,"\\*\n\tAlgoritmo N-queens escrito por Geraldo Rabelo <geraldo.rabelo@gmail.com> em Setembro de 2017.\n*\\\n\n");   
   fprintf(arquivo,"número de raínhas: %d\nsolução: [",individuos[especime].aptidao);

   for (contador_genes = 0; contador_genes < genes-1; contador_genes++) {
      fprintf(arquivo,"%d,",individuos[especime].genes[contador_genes]+1);   
   }       
   fprintf(arquivo,"%d]\nresolvido em %d gerações (%f segundos)",individuos[especime].genes[contador_genes]+1,geracao, time);
   
*/
   for (contador_genes = 0; contador_genes < genes; contador_genes++) {
      fprintf(arquivo,"%d;",individuos[especime].genes[contador_genes]+1);
   }       


   fclose(arquivo);
}

void dimensionar_populacao()
{
   for (contador = 0;contador < POPULACAO;contador++) {
      free(individuos[contador].genes);
      individuos[contador].genes = malloc(genes*sizeof(int));
   }

   for (contador = 0;contador < elite_da_populacao;contador++) {
      free(elite[contador].genes);
      elite[contador].genes = malloc(genes*sizeof(int));
   }
}

int gerar_css() {
   FILE *arquivo = NULL;
   char *file = malloc(32*sizeof(char));
   memset(file,'\0',32);
   sprintf(file,"%d_queens.css",genes);

   arquivo = fopen(file,"w+");

//   fprintf(arquivo,"body { display: inline-block; margin: 0px auto; text-align: center;}");
   fprintf(arquivo,"body, div.progress { background-color: #ffc; }\n\n");
   fprintf(arquivo,"a.external:after { content: url(\"external.gif\"); }\n\n");
   fprintf(arquivo,"a:link, a:visited { color: #060; }\n\n");
   fprintf(arquivo,".colour, a:hover, dt, h1, h2, h3, h4, h5, h6, hr, th, div.progress { color: #030; }\n\n");
   fprintf(arquivo,"table.chessboard {\n");
//   fprintf(arquivo,"  margin: 0px auto;\n");
   fprintf(arquivo,"  border-spacing: 0;\n");
   fprintf(arquivo,"  border: 0.1em outset #660;\n");
   fprintf(arquivo,"  background-color: #9c9;\n");
   fprintf(arquivo,"  text-align: center;\n");
   fprintf(arquivo,"  font-size: 0.8em;\n");
   fprintf(arquivo,"  font-weight: bold;\n");
   fprintf(arquivo,"}\n\n");
   fprintf(arquivo,"table.chessboard tr { height: 1.2em; }\n\n");
   fprintf(arquivo,"table.chessboard td { width: 1.2em; padding: 0; border: 0; }\n\n");
   fprintf(arquivo,"table.chessboard td.light { background-color: #ffc; }\n\n");
   fprintf(arquivo,"div.chessboard table.chessboard { font-size: 1em; font-weight: normal; }\n\n");
   fprintf(arquivo,"div.chessboard table.big { border-width: 0.2em; }\n\n");
   fprintf(arquivo,"div.chessboard table.big tr { height: 3em; }\n\n");
   fprintf(arquivo,"div.chessboard table.big td { width: 3em; }\n\n");
   fprintf(arquivo,"div.progress { float: right; margin: 10px 0 1px 10px; border: 2px solid; padding: 10px; text-align: center; }\n\n");
   fprintf(arquivo,"div.progress p { font-size: 1.1em; margin-top: 0; margin-bottom: 10px; }\n\n");
   fprintf(arquivo,"div.progress p.progress { font-size: 2em; }\n\n\n");
   fprintf(arquivo,"/* Alternate Media */\n");
   fprintf(arquivo,"@media print {\n");
   fprintf(arquivo,"  table.chessboard td { border: 1px solid #660; border-width: 0 1px 1px 0; }\n");
   fprintf(arquivo,"}\n\n");
   fprintf(arquivo,".border { border-color: #030; }\n");
   fprintf(arquivo,"#content { width: %dpx; display: block; margin-top: 50px; margin-left: auto; margin-right: auto; }",25*genes);


   fclose(arquivo);
   free(file);
   return 1;
}

int exportar_html(int especime, float time) {
   FILE *arquivo = NULL;
   char *file = malloc(32*sizeof(char));
   memset(file,'\0',32);
   sprintf(file,"%d_queens.html",genes);
   arquivo = fopen(file,"w+");   

//   fprintf(arquivo,"<html>\n\t<head>\n\t<link rel=\"stylesheet\" href=\"Nqueens.css\" type=\"text/css\" />\n\t</head>\n\t<body>\n\t<table cellspacing=\"0\" class=\"chessboard\" style=\"width: 59.3em\">\n\t<caption>\"%d raínhas\"</caption>",genes);
   fprintf(arquivo,"<html>\n\t<head>\n\t<link rel=\"stylesheet\" href=\"%d_queens.css\" type=\"text/css\" />\n\t</head>\n\t<body>\n\t<div id=\"content\"><table cellspacing=\"0\" class=\"chessboard\">\n\t<caption>\"N-Queens %dx%d\"</caption>",genes,genes,genes);
   
   for (int row = 0; row < genes; row++) {
      fprintf(arquivo,"\n\t<tr>\n");
      for (int column = 0; column < genes; column++) {
         if (row % 2 == 0) {
                 if(individuos[especime].genes[row] == column) {
                    if (column % 2 == 0) {    
                       fprintf(arquivo,"\t\t<td><img src=\"queen.png\"></td>\n");
                    } else {
                       fprintf(arquivo,"\t\t<td class=\"light\"><img src=\"queen.png\"></td>\n");
                    
                    }
                 } else {
                    if (column % 2 == 0) {             
                      fprintf(arquivo,"\t\t<td>&nbsp;</td>\n");
                    } else {
                      fprintf(arquivo,"\t\t<td class=\"light\">&nbsp;</td>\n");
                    }
                 }         
         } else {
                 if(individuos[especime].genes[row] == column) {
                    if (column % 2 == 0) {    
                       fprintf(arquivo,"\t\t<td class=\"light\"><img src=\"queen.png\"></td>\n");
                    } else {
                       fprintf(arquivo,"\t\t<td><img src=\"queen.png\"></td>\n");
                    }
                 } else {
                    if (column % 2 == 0) {             
                      fprintf(arquivo,"\t\t<td class=\"light\">&nbsp;</td>\n");
                    } else {
                      fprintf(arquivo,"\t\t<td>&nbsp;</td>\n");
                    }
                 }  
         }      
      }
      fprintf(arquivo,"\t</tr>");
   }
   fprintf(arquivo,"\n\t</table></div>\n<!-- <p><a href=\"mailto:geraldo.rabelo@gmail.com?Subject=NQueens\" target=\"_top\"><b>by gerabelo</b></a></p><p>Solução encontrada em %f segundos.</p>-->\t</body>\n</html>",time);
   free(file);
   fclose(arquivo);
   gerar_css();
   
   return 1;
}


int main(int argc, char **argv) {  
   double start_t, end_t, total_t;
   start_t = clock();      

   for (contador = 0;contador < POPULACAO;contador++) {
      individuos[contador].genes = malloc(genes*sizeof(int));
   }   

   geracao = 0;   
   gerar_populacao_inical(POPULACAO);  
   elite_da_populacao = round((-1+sqrt(1+4*POPULACAO)/2));
   elite = malloc(elite_da_populacao*sizeof(Cromossomo));

   for (contador = 0;contador < elite_da_populacao;contador++) {
      elite[contador].genes = malloc(genes*sizeof(int));
   }   

   //FILE *log_file = NULL;
   //char *file = malloc(32*sizeof(char));

   while (1) {
      //memset(file,'\0',32);
      //sprintf(file,"%d_queens_log.txt",genes);
      //log_file = fopen(file,"w+");   
   
      for (int especime = 0; especime < POPULACAO; especime++) {
         individuos[especime].aptidao = total_de_rainhas_corretamente_posicionadas(especime);
         //sleep(0.5);

         if (individuos[especime].aptidao == genes) {
            imprimir_individuo_na_tela(especime);
            end_t = clock();       
            total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
            imprimir_individuo_em_arquivo(genes,especime,total_t);
            printf("\tTempo total: %f segundos\n", total_t);
            start_t = clock();
            exportar_html(especime);
            sleep(5);
            genes++;
            dimensionar_populacao();
            gerar_populacao_inical(POPULACAO);
            geracao = 0;
         }
      }

      ordenar_populacao_por_aptidao(POPULACAO);
      preservar_elite();
      imprimir_melhor_resultado_na_tela();
      //imprimir_melhor_resultado_em_arquivo(log_file);
      gerar_nova_populacao();
      mutacao_local_elite();
      mutacao_local_plebe();    
      geracao++;
      //free(log_file);
      
   }
   return(0);
}


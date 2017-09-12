# Computação Evolutiva 
Implementação em ANSI-C de um _Algoritmo Genético_ capaz de resolver o problema das N rainhas.  
 
 
:crown: :crown: :crown: :crown: :crown: :crown: :crown: :crown:  
## N Queens  
Este problema consiste, como próprio nome sugere, em posicionar N rainhas em um tableiro de xadrez, sem que elas ameassem umas as outras.  
  
    
## Como o algoritmo funciona?  
O algorimo cria algumas estruturas simples de dados e, com elas, tenta simular comportametos biológicos/naturais como o da evolução-mutação, o da seleção natural e o da reprodução.  
  
As estruturas:  
* cromossomo - composto por um conjunto de oito genes (cada um equivale a uma coluna do tabuleiro);
* individuo - possui um unico cromosso e um resulatdo chamado aptidao, que é calculado pelo algoritmo com base no numero de rainhas posicionadas corretamente;
* populacao -  um conjunto de cem indivíduos.  
    
    
## A Seleção :hocho:  
A cada geração um pequeno grupo formado pelos mais aptos é preservado -elitismo-. os demais são substituidos por combinações entre individuos do grupo sobrevivente.  
```
        elite_da_populacao = round((-1+sqrt(1+4*POPULACAO)/2));
        [...]
        void preservar_elite() {
           for (int contador_populacao = 0; contador_populacao < elite_da_populacao; contador_populacao++) {
              for (int contador_genes = 0; contador_genes < genes; contador_genes++) {
                 elite[contador_populacao].genes[contador_genes] = individuos[contador_populacao].genes[contador_genes];
              }
              elite[contador_populacao].aptidao = individuos[contador_populacao].aptidao;
           }
        }
```
O alfabeto que compoem os genes vai de 0 à 7 e representa a linha ocupada pela rainha naquela coluna/gene.
  
    
## A reprodução  :couple:  
Cada individuo do grupo sobrevivente tem seu cromossomo dividido em duas partes. Novos indivíduos sao criados preservando a primeira metade dos genes de um individuo pai enquanto a segunda é copiada de outro indivíduo mãe. Em seguida ocorre a mutação aleatória.
   
   
## A  mutação  :hear_no_evil:  
Alterações aleatorias ocorrerão em alguns dos genes da nova geração.
```
        for (int contador_populacao = (rand()%GENES); contador_populacao < (GENES-1); contador_populacao++)
        {
                individuos[populacao_atual].genes[(rand() % GENES)] = (rand() % GENES);
        }
```
    
## Conclusões  
Algoritmos Genéticos oferecem soluções heurísticas que podem, ou não, convergir para o resultado desejado.
  
![screenshot](https://c1.staticflickr.com/5/4332/36349900714_907c81669b_o.png)

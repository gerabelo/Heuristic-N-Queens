# Computação Evolutiva :+1:
Implementação em ANSI-C de um _Algoritmo Genético_ capaz de resolver o problema das oito rainhas.  
  
    
## 8 Queens  :crown::crown::crown::crown::crown::crown::crown::crown:
Este problema consiste, como próprio nome sugere, em posicionar oito rainhas em um tableiro de xadrez, sem que elas ameassem umas as outras.  
  
    
## Como o algoritmo funciona?  
O algorimo cria algumas estruturas simples de dados e, com elas, tenta simular comportametos biológicos como evolução/mutação, seleção e reprodução.  
  
As estruturas de dados são:  
* cromossomo - composto por um conjunto de oito genes (cada um equivale a uma coluna do tabuleiro);
* individuo - possui um unico cromosso e um resulatdo chamado aptidao, que é calculado pelo algoritmo com base no numero de rainhas posicionadas corretamente;
* populacao -  um conjunto de cem indivíduos.  
    
    
## A Seleção
A cada geração um pequeno grupo formado pelos mais aptos é preservado -elitismo-. os demais são substituidos por combinações entre individuos do grupo sobrevivente.  
```
        elite_da_populacao = round((-1+sqrt(1+4*POPULACAO)/2));
```
O alfabeto que compoem os genes vai de 0 à 7 e representa a linha ocupada pela rainha naquela coluna/gene.
  
    
## A reprodução  
Cada individuo do grupo sobrevivente tem seu cromossomo dividido em duas partes. Novos indivíduos sao criados preservando a primeira metade dos genes de um individuo pai enquanto a segunda é copiada de outro indivíduo mãe. Em seguida ocorre a mutação aleatória.
   
   
## A  mutação  
Alterações aleatorias ocorrerão em alguns dos genes da nova geração.
```
        for (int c0 = (rand()%GENES); c0 < (GENES-1); c0++)
        {
                individuos[populacao_atual].genes[(rand() % GENES)] = (rand() % GENES);
        }
```
    
## Conclusões  
Algoritmos Genéticos oferecem soluções heurísticas que podem, ou não, convergir para o resultado desejado.
  
![screenshot](https://c1.staticflickr.com/5/4400/36827028836_58d124ffe4_o.png)

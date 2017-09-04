# Computação Evolutiva
Implementação em ANSI-C de um _Algoritmo Genético_ capaz de resolver o problema das oito rainhas.  
  
    
## 8 Queens  
Este problema consiste, como próprio nome sugere, em posicionar oito rainhas em um tableiro de xadrez, sem que elas ameassem umas as outras.  
  
    
## Como o algoritmo funciona?  
O algorimo cria algumas estruturas simples de dados e, com elas, tenta simular comportametos biológicos como evolução/mutação, seleção e reprodução.  
  
As estruturas de dados são:  
* cromossomo - composto por um conjunto de oito genes (cada um equivale a uma coluna do tabuleiro);
* individuo - possui um unico cromosso e um resulatdo chamado aptidao, que é calculado pelo algoritmo com base no numero de rainhas posicionadas corretamente;
* populacao -  um conjunto de cem indivíduos.  
    
    
## A Seleção
A cada geração um grupo de 10% formado pelos mais aptos é preservado -elitismo-. os demais são substituidos por combinações entre individuos do grupo sobrevivente.  
O alfabeto que compoem os genes vai de 0 à 7 e representa a linha ocupada pela rainha naquela coluna/gene.
  
    
## A reprodução  
Cada individuo do grupo sobrevivente tem seu cromossomo dividido em duas partes. Novos indivíduos sao criados preservando a primeira metade dos genes de um individuo pai enquanto a segunda é copiada de outro indivíduo mãe. Em seguida ocorre a mutação aleatória.
   
   
## A  mutação  
Os 90% -novos indivíduos- terao entre um e tres genes substituidos aleatoriamentes. 
    
    
## Conclusões  
Algoritmos Genéticos oferecem soluções heurísticas que podem, ou não, convergir para o resultado desejado.

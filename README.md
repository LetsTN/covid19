# Covid 19 - problema Produtores/Consumidores

Esse problema foi proposto no trabalho 1 da disciplina de FPP.

Em problemas Produtores/Consumidores, processos compartilham um buffer de tamanho fixo, sendo que eles podem ser produtores ou consumidores.

Produtores inserem informação no buffer, enquando consumidores consomem informação. Isso nos leva ao problema de Sincronização de Threads.

## O problema (como o professor passou, literalmente)

Existem 6 processos nesse problema: 3 processos infectados e 3 processos laboratorios.

Cada um dos infectados vai fazer uma vacina e usa-la [a vacina eh uma porcaria e tem q ser reaplicada a todo momento que for possivel].

Para fazer uma vacina eh necessario virus-morto, injecao e insumo-secreto.

Cada infectado tem 1 dos 3 produtos.

Exemplo: 1 processo infectado tem virus-morto, outro tem injecao e o ultimo tem o insumo-secreto.

Cada laboratorio tem um diferente suprimento infinito de 2 dos 3 produtos.
* Exemplo:laboratorio1 tem injecao, virus-morto, laboratorio2 tem injecao e insumo-secreto e laboratorio3 tem insumo-secreto e virus-morto.

Cada laboratorio coloca 2 dos produtos a disposicao de quem quiser e todos os infectados correm pra pegar os dois que eles precisam pra fazer a vacina e somente renovam quando os SEUS produtos distribuidos forem consumidos. [o lab2 não renova se consumir a injeção do lab1 e o insumo secreto do lab3, por exemplo]


* Como garantir exclusao mutua, sem dead-lock e sem starvation ?

[dica, soh vai funcionar com uma mescla de semaforos e mutexes]

a entrada do codigo será um numero que vai ser o numero de vezes MINIMO que cada thread realizou seu objetivo primordial.

A saida deverá ser o numero de vezes que cada thread realizou seu objetivo primordial:
no caso, os infectados irao contabilizar o numero de vezes q cada um se injetou com a vacina e os laboratorios o numero de vezes que cada um renovou seus estoques:

## Como compilar os arquivos 

Por mais que já tenha o arquivo compilado (```covid19```), caso você mesmo queira compilar o arquivo, basta rodar:

```sh
gcc -Wall -pedantic -o covid19 covid19.c -lpthread
```

## Como executar

Para executar o código, basta rodar:

```sh
./covid19 n
```

Onde ```n``` é a quantidade mínima que cada um deve cumprir seu objetivo

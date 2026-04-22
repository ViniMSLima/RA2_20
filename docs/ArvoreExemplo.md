# Exemplo de Árvore Sintática e Rastreio LL(1)
Este documento demonstra a validade estrutural da linguagem através do rastreamento do processo de análise sintática e da representação da Árvore Sintática Abstrata (AST) resultante para o arquivo `teste1.txt`.

## 1. Código Fonte de Entrada (RPN)
O arquivo `teste1.txt` contém uma sequência de instruções em Notação Polonesa Reversa, cobrindo operações aritméticas, manipulação de memória e estruturas de controle.

```text
( START )
( 10.5 A MEM )
( 2.0 B MEM )
( A B + )
( A B - )
( A B * )
( A B | )
( 10 3 / )
( 10 3 % )
( 2 3 ^ )
( ( A B > ) ( 0 RES ) IF )
( ( B 10 < ) ( ( B 1 + ) B MEM ) WHILE )
( END )
```
## 2. Rastreamento (Trace) do Parser LL(1)
A tabela abaixo ilustra o funcionamento interno do analisador. Ela mostra o consumo dos tokens (LOCALIZADO), a evolução da PILHA sintática, o estado do BUFFER de entrada e a AÇÃO realizada de acordo com a tabela preditiva LL(1).

| LOCALIZADO | PILHA SINTÁTICA | BUFFER (Entrada Restante) | AÇÃO (Regra Aplicada) |
| :--- | :--- | :--- | :--- |
| | `programa $` | `( START ) ( 10.5 ... $` | Início da análise |
| | `( START ) proximo_bloco $` | `( START ) ( 10.5 ... $` | `programa -> ( START ) proximo_bloco` |
| `(` | `START ) proximo_bloco $` | `START ) ( 10.5 ... $` | Casamento de `PARENTE_ESQ` |
| `( START` | `) proximo_bloco $` | `) ( 10.5 ... $` | Casamento de `START` |
| `( START )` | `proximo_bloco $` | `( 10.5 ... $` | Casamento de `PARENTE_DIR` |
| `( START )` | `( corpo_instrucao $` | `( 10.5 ... $` | `proximo_bloco -> ( corpo_instrucao` |
| `( START ) (` | `corpo_instrucao $` | `10.5 A MEM ) ... $` | Casamento de `PARENTE_ESQ` |
| `( START ) (` | `expressao ) proximo_bloco $` | `10.5 A MEM ) ... $` | `corpo_instrucao -> expressao ) proximo_bloco` |


## 3. Árvore Sintática Abstrata (AST) Gerada
A estrutura hierárquica abaixo representa a organização lógica do programa após a análise. Esta árvore é percorrida em pós-ordem pelo gerador para produzir as instruções Assembly ARMv7.

-- [RAIZ] PROGRAMA_COMPLETO
  |-- [NUMERO] 10.5
  |-- [KEY_MEM] MEM
  |  |-- [VARIAVEL] A
  |-- [NUMERO] 2.0
  |-- [KEY_MEM] MEM
  |  |-- [VARIAVEL] B
  |-- [OP_ARITMETICO] +
  |  |-- [VARIAVEL] A
  |  |-- [VARIAVEL] B
  |-- [OP_ARITMETICO] -
  |  |-- [VARIAVEL] A
  |  |-- [VARIAVEL] B
  |-- [OP_ARITMETICO] *
  |  |-- [VARIAVEL] A
  |  |-- [VARIAVEL] B
  |-- [OP_ARITMETICO] |
  |  |-- [VARIAVEL] A
  |  |-- [VARIAVEL] B
  |-- [OP_ARITMETICO] /
  |  |-- [NUMERO] 10
  |  |-- [NUMERO] 3
  |-- [OP_ARITMETICO] %
  |  |-- [NUMERO] 10
  |  |-- [NUMERO] 3
  |-- [OP_ARITMETICO] ^
  |  |-- [NUMERO] 2
  |  |-- [NUMERO] 3
  |-- [KEY_IF] IF
  |  |-- [OP_RELACIONAL] >
  |  |  |-- [VARIAVEL] A
  |  |  |-- [VARIAVEL] B
  |  |-- [KEY_RES] RES
  |  |  |-- [NUMERO] 0
  |-- [OP_RELACIONAL] <
  |  |-- [VARIAVEL] B
  |  |-- [NUMERO] 10
  |-- [KEY_WHILE] WHILE
  |  |-- [OP_ARITMETICO] +
  |  |  |-- [VARIAVEL] B
  |  |  |-- [NUMERO] 1
  |  |-- [KEY_MEM] MEM
  |  |  |-- [VARIAVEL] B

Entrada da árvore

( START )
( 10.5 A MEM )
( 2.0 B MEM )
( A B + )
( A B - )
( A B * )
( A B | )
( 10 3 / )
( 10 3 % )
( 2 3 ^ )
( ( A B > ) ( 0 RES ) IF )
( ( B 10 < ) ( ( B 1 + ) B MEM ) WHILE )
( END )
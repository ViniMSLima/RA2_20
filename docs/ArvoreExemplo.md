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
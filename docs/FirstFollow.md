## Conjuntos FIRST e FOLLOW


### Conjuntos FIRST
* **FIRST(programa)** = { PARENTE_ESQ }
* **FIRST(proximo_bloco)** = { PARENTE_ESQ, ε }
* **FIRST(corpo_instrucao)** = { END, NUMERO, VARIAVEL, PARENTE_ESQ } U FIRST(operador_final)
* **FIRST(expressao)** = { NUMERO, VARIAVEL, PARENTE_ESQ } U FIRST(operador_final)
* **FIRST(lista_operandos)** = { NUMERO, VARIAVEL, PARENTE_ESQ, ε }
* **FIRST(operando)** = { NUMERO, VARIAVEL, PARENTE_ESQ }
* **FIRST(operador_final)** = { OP_ARITMETICO, OP_RELACIONAL, KEY_IF, KEY_WHILE, KEY_MEM, KEY_RES }

### Conjuntos FOLLOW
* **FOLLOW(programa)** = { EOF }
* **FOLLOW(proximo_bloco)** = { EOF }
* **FOLLOW(corpo_instrucao)** = { EOF }
* **FOLLOW(expressao)** = { PARENTE_DIR }
* **FOLLOW(lista_operandos)** = { FIRST(operador_final) }
* **FOLLOW(operando)** = { NUMERO, VARIAVEL, PARENTE_ESQ } U FIRST(operador_final)

- A gramática é comprovadamente LL(1) pois os conjuntos FIRST das produções alternativas de cada não-terminal são disjuntos, eliminando ambiguidades e permitindo a escolha preditiva com um lookahead de 1 símbolo.
# Documentação Teórica do Analisador Sintático (Fase 2)

**Responsabilidade:** Aluno 1  
**Objetivo:** Definição da Gramática, Conjuntos FIRST/FOLLOW e Tabela de Análise LL(1) sem conflitos (Fatorada à Esquerda).

---

## 1. Gramática EBNF (Fatorada)

A gramática abaixo foi estruturada e fatorada à esquerda no nível do escopo do programa para eliminar conflitos e garantir determinismo estrito LL(1).

    programa          -> PARENTE_ESQ START PARENTE_DIR proximo_bloco
    proximo_bloco     -> PARENTE_ESQ corpo_instrucao | ε
    corpo_instrucao   -> END PARENTE_DIR | expressao PARENTE_DIR proximo_bloco
    expressao         -> lista_operandos operador_final
    lista_operandos   -> operando lista_operandos | ε
    operando          -> NUMERO | VARIAVEL | PARENTE_ESQ expressao PARENTE_DIR
    operador_final    -> OP_ARITMETICO | OP_RELACIONAL | KEY_IF | KEY_WHILE | KEY_MEM | KEY_RES

---

## 4. Documentação da Nova Sintaxe 

Estrutura de Decisão (IF) em RPN:
O comando IF consome dois blocos de código (duas expressões) que estão no topo da pilha. O primeiro bloco resolvido é a condição, o segundo é a ação.
Sintaxe: ( ( Bloco_Condição ) ( Bloco_Ação ) IF )
Exemplo prático: ( ( 10 5 > ) ( X MEM ) IF ) -> "Se 10 for maior que 5, imprima X da memória."

Estrutura de Repetição (WHILE) em RPN:
Semelhante ao IF, mas o bloco de ação é repetido enquanto o bloco de condição for verdadeiro.
Sintaxe: ( ( Bloco_Condição ) ( Bloco_Ação ) WHILE )
Exemplo prático: ( ( X 10 < ) ( X 1 + ) WHILE ) -> "Enquanto X for menor que 10, adicione 1 a X."

Novos Operadores Relacionais:
 - > (Maior que), < (Menor que), = (Igualdade).


## 5. Estrutura da Árvore Sintática Abstrata (AST)

A Árvore Sintática Abstrata (AST) organiza a hierarquia de execução dos comandos lidos pelo Parser. Na notação pós-fixada (RPN), os operandos e blocos aninhados atuam como **Folhas** (ou sub-árvores) que são adotadas pelo **Operador** ou **Comando** (Nó Pai) que vem logo em seguida para fechar o escopo.

### 5.1. Anatomia do Nó de Decisão (`IF`)
O comando `IF` atua como um nó raiz que exige exatamente dois filhos: o primeiro bloco processado é a sub-árvore de condição, e o segundo é a sub-árvore de ação.

**Entrada RPN:** `( ( 10 5 > ) ( X MEM ) IF )`
**Hierarquia na AST:**
    [COMANDO] IF
      |
      |-- [CONDICAO] >
      |     |-- [NUMERO] 10
      |     |-- [NUMERO] 5
      |
      |-- [ACAO] MEM
            |-- [VARIAVEL] X

### 5.2. Anatomia do Nó de Repetição (`WHILE`)
Semelhante ao `IF`, o comando `WHILE` é um nó pai com dois galhos principais. A diferença ocorre apenas na Análise Semântica (Fase 3), onde este nó forçará a reavaliação cíclica.

**Entrada RPN:** `( ( X 10 < ) ( X 1 + ) WHILE )`
**Hierarquia na AST:**
    [COMANDO] WHILE
      |
      |-- [CONDICAO] <
      |     |-- [VARIAVEL] X
      |     |-- [NUMERO] 10
      |
      |-- [ACAO] +
            |-- [VARIAVEL] X
            |-- [NUMERO] 1
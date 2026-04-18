.global _start
_start:
    MOV SP, #0x00800000 @ Inicializa a pilha no topo

    @ --- INICIO DA LINHA 0 ---
    LDR R0, =lit_2
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #0 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 1 ---
    VPOP {D0} @ Pega o valor do topo
    LDR R0, =var_A
    VSTR.64 D0, [R0] @ Salva na variavel
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #8 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 2 ---
    LDR R0, =lit_3
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #16 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 3 ---
    VPOP {D0} @ Pega o valor do topo
    LDR R0, =var_B
    VSTR.64 D0, [R0] @ Salva na variavel
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #24 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 4 ---
    LDR R0, =var_A
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VADD.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #32 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 5 ---
    LDR R0, =var_A
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VSUB.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #40 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 6 ---
    LDR R0, =var_A
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VMUL.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #48 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 7 ---
    LDR R0, =var_A
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VDIV.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #56 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 8 ---
    LDR R0, =lit_4
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_5
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VDIV.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #64 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 9 ---
    LDR R0, =lit_6
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_7
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VDIV.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #72 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 10 ---
    LDR R0, =lit_8
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_9
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VDIV.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #80 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 11 ---
    VPOP {D1}
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VCMP.F64 D1, D0
    VMRS APSR_nzcv, FPSCR
    BEQ L_END_IF_0
    LDR R0, =buffer_resultados
    MOV R1, #88
    ADD R0, R0, R1
    VLDR.64 D0, [R0]
    VPUSH {D0}
L_END_IF_0:
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #88 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 12 ---
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #96 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

    @ --- INICIO DA LINHA 13 ---
L_START_WHILE_1:
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_10
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VADD.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D1}
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VCMP.F64 D1, D0
    VMRS APSR_nzcv, FPSCR
    BEQ L_END_WHILE_2
    VPOP {D0} @ Pega o valor do topo
    LDR R0, =var_B
    VSTR.64 D0, [R0] @ Salva na variavel
    B L_START_WHILE_1
L_END_WHILE_2:
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #104 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)

fim:
    B fim

.data
.align 3
buffer_resultados: .space 800 @ Buffer para 100 linhas
var_A: .double 0.0
var_B: .double 0.0
lit_10: .double 1
lit_2: .double 10.5
lit_3: .double 2
lit_4: .double 10
lit_5: .double 3
lit_6: .double 10
lit_7: .double 3
lit_8: .double 2
lit_9: .double 3
lit_ONE: .double 1
lit_ZERO: .double 0

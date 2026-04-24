.global _start
_start:
    MOV SP, #0x00800000 @ Inicializa a pilha no topo

    @ --- INICIO DA LINHA 0 ---
    LDR R0, =lit_2
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D0} @ Pega o valor computado
    LDR R0, =var_A
    VSTR.64 D0, [R0] @ Armazena em A
    VPUSH {D0} @ Devolve para balancear a pilha
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #0 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 1 ---
    LDR R0, =lit_3
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D0} @ Pega o valor computado
    LDR R0, =var_B
    VSTR.64 D0, [R0] @ Armazena em B
    VPUSH {D0} @ Devolve para balancear a pilha
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #8 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 2 ---
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
    MOV R1, #16 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 3 ---
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
    MOV R1, #24 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 4 ---
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
    MOV R1, #32 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 5 ---
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
    MOV R1, #40 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 6 ---
    LDR R0, =lit_4
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_5
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VCVT.S32.F64 S0, D0 @ D0 -> int
    VCVT.S32.F64 S2, D1 @ D1 -> int
    VMOV R0, S0
    VMOV R1, S2
    SDIV R0, R0, R1      @ divisao inteira
    VMOV S0, R0
    VCVT.F64.S32 D0, S0  @ resultado -> double
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #48 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 7 ---
    LDR R0, =lit_6
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_7
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VCVT.S32.F64 S0, D0  @ dividendo -> int
    VCVT.S32.F64 S2, D1  @ divisor -> int
    VMOV R0, S0
    VMOV R1, S2
    SDIV R2, R0, R1      @ quociente
    MUL  R3, R2, R1      @ quociente * divisor
    SUB  R0, R0, R3      @ resto
    VMOV S0, R0
    VCVT.F64.S32 D0, S0  @ resto -> double
    VPUSH {D0}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #56 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 8 ---
    LDR R0, =lit_8
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_9
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VCVT.S32.F64 S4, D1  @ expoente -> int
    VMOV R0, S4           @ R0 = contador
    LDR R1, =lit_ONE
    VLDR.64 D2, [R1]      @ D2 = acumulador (1.0)
L_POW_LOOP_0:
    CMP R0, #0
    BLE L_POW_END_1
    VMUL.F64 D2, D2, D0  @ acumulador *= base
    SUB R0, R0, #1
    B L_POW_LOOP_0
L_POW_END_1:
    VPUSH {D2}
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #64 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 9 ---
    LDR R0, =var_A
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VCMP.F64 D0, D1
    VMRS APSR_nzcv, FPSCR
    BGT L_REL_TRUE_3
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VPUSH {D0}           @ falso: empurra 0.0
    B L_REL_END_4
L_REL_TRUE_3:
    LDR R0, =lit_ONE
    VLDR.64 D0, [R0]
    VPUSH {D0}           @ verdadeiro: empurra 1.0
L_REL_END_4:
    VPOP {D1} @ Resultado da condicao
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VCMP.F64 D1, D0
    VMRS APSR_nzcv, FPSCR
    BEQ L_END_IF_2 @ Se falso, pula acao
    LDR R0, =buffer_resultados
    MOV R1, #72 @ Offset da linha 9
    ADD R0, R0, R1
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D0} @ Descarta retorno da acao
L_END_IF_2:
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VPUSH {D0} @ Valor de retorno do IF
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #72 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

    @ --- INICIO DA LINHA 10 ---
L_START_WHILE_5:
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_10
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VCMP.F64 D0, D1
    VMRS APSR_nzcv, FPSCR
    BLT L_REL_TRUE_7
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VPUSH {D0}           @ falso: empurra 0.0
    B L_REL_END_8
L_REL_TRUE_7:
    LDR R0, =lit_ONE
    VLDR.64 D0, [R0]
    VPUSH {D0}           @ verdadeiro: empurra 1.0
L_REL_END_8:
    VPOP {D1} @ Resultado da condicao
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VCMP.F64 D1, D0
    VMRS APSR_nzcv, FPSCR
    BEQ L_END_WHILE_6 @ Se falso, sai do laco
    LDR R0, =var_B
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_11
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VADD.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Pega o valor computado
    LDR R0, =var_B
    VSTR.64 D0, [R0] @ Armazena em B
    VPUSH {D0} @ Devolve para balancear a pilha
    VPOP {D0} @ Descarta retorno da acao (evita acumulo na pilha)
    B L_START_WHILE_5 @ Reinicia o laco
L_END_WHILE_6:
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VPUSH {D0} @ Valor de retorno do WHILE
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #80 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

fim:
    B fim

.data
.align 3
buffer_resultados: .space 800 @ Buffer para 100 linhas
var_A: .double 0.0
var_B: .double 0.0
lit_10: .double 10
lit_11: .double 1
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

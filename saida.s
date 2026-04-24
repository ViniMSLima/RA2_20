.global _start
_start:
    MOV SP, #0x00800000 @ Inicializa a pilha no topo

    @ --- INICIO DA LINHA 0 ---
    LDR R0, =lit_2
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_3
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VCMP.F64 D0, D1
    VMRS APSR_nzcv, FPSCR
    BEQ L_REL_TRUE_1
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VPUSH {D0}           @ falso: empurra 0.0
    B L_REL_END_2
L_REL_TRUE_1:
    LDR R0, =lit_ONE
    VLDR.64 D0, [R0]
    VPUSH {D0}           @ verdadeiro: empurra 1.0
L_REL_END_2:
    VPOP {D1} @ Resultado da condicao
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VCMP.F64 D1, D0
    VMRS APSR_nzcv, FPSCR
    BEQ L_END_IF_0 @ Se falso, pula acao
    LDR R0, =lit_4
    VLDR.64 D0, [R0]
    VPUSH {D0}
    LDR R0, =lit_5
    VLDR.64 D0, [R0]
    VPUSH {D0}
    VPOP {D1} @ Segundo operando
    VPOP {D0} @ Primeiro operando
    VMUL.F64 D0, D0, D1
    VPUSH {D0}
    VPOP {D0} @ Descarta retorno da acao
L_END_IF_0:
    LDR R0, =lit_ZERO
    VLDR.64 D0, [R0]
    VPUSH {D0} @ Valor de retorno do IF
    VPOP {D0} @ Pega o resultado da expressao
    LDR R0, =buffer_resultados
    MOV R1, #0 @ Offset (linha * 8 bytes)
    ADD R0, R0, R1
    VSTR.64 D0, [R0] @ Salva no historico
    VPUSH {D0} @ Devolve para a pilha

fim:
    B fim

.data
.align 3
buffer_resultados: .space 800 @ Buffer para 100 linhas
lit_2: .double 2
lit_3: .double 2
lit_4: .double 3
lit_5: .double 2
lit_ONE: .double 1
lit_ZERO: .double 0

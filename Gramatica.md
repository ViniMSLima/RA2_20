programa -> PARENTE_ESQ START PARENTE_DIR lista_comandos PARENTE_ESQ END PARENTE_DIR
lista_comandos -> comando lista_comandos | ε 
comando -> expressao_rpn | comando_especial | estrutura_controle
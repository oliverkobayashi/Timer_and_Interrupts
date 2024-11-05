// Variáveis globais
unsigned char ucContador = 0; // Armazena o valor da contagem
unsigned char tempoAtual = 1; // 1s ou 0,25s para a contagem

// Função de incremento para o display de 7 segmentos
void Incremento(unsigned char Contador) {
    switch (Contador) {
        case 0: LATD = 0b00111111; break; // 0
        case 1: LATD = 0b00000110; break; // 1
        case 2: LATD = 0b01011011; break; // 2
        case 3: LATD = 0b01001111; break; // 3
        case 4: LATD = 0b01100110; break; // 4
        case 5: LATD = 0b01101101; break; // 5
        case 6: LATD = 0b01111101; break; // 6
        case 7: LATD = 0b00000111; break; // 7
        case 8: LATD = 0b01111111; break; // 8
        case 9: LATD = 0b01101111; break; // 9
        default: LATD = 0x00; break; // Display apagado
    }
}

// Rotina de interrupção de alta prioridade
void Interrupt_HIGH() iv 0x0008 ics ICS_AUTO {
    if (INTCON.TMR0IF == 1) { // Verifica se a interrupção foi do TMR0
        Incremento(ucContador++); // Incrementa a contagem e atualiza o display

        if (ucContador > 9) { // Reseta a contagem ao ultrapassar 9
            ucContador = 0;
        }

        // Recarrega o TIMER0 de acordo com o período atual
        if (tempoAtual == 1) { // Período de 1s
            TMR0H = 0xCD;
            TMR0L = 0x0B;
        } else { // Período de 0,25s
            TMR0H = 0xF7;
            TMR0L = 0xC2;
        }

        INTCON.TMR0IF = 0; // Reseta a flag de interrupção
    }

    // Interrupção externa RB0
    if (INTCON.INT0IF == 1) {
        tempoAtual = 1; // Altera o período para 1s
        T0CON.TMR0ON = 1; // Liga o TIMER0 se estiver desligado
        INTCON.INT0IF = 0; // Reseta a flag de interrupção externa RB0
    }

    // Interrupção externa RB1
    if (INTCON3.INT1IF == 1) {
        tempoAtual = 0; // Altera o período para 0,25s
        T0CON.TMR0ON = 1; // Liga o TIMER0 se estiver desligado
        INTCON3.INT1IF = 0; // Reseta a flag de interrupção externa RB1
    }
}

// Configuração do MCU
void ConfigMCU() {
    ADCON1 |= 0x0F; // Configura todos os pinos como digitais
    TRISD = 0x00; // Configura PORTD como saída
    TRISB = 0x03; // Configura RB0 e RB1 como entradas
    LATD = 0x00; // Inicializa LATD (PORTD) desligado

    // Habilita os resistores de pull-up para RB0 e RB1
    INTCON2.RBPU = 0; // Desliga o pull-up global
}

// Configuração do TIMER0
void ConfigTIMER() {
    T0CON = 0b00000100; // Timer0 desligado (prescaler 1:32)
    TMR0H = 0xCD;       // Configuração inicial para 1s
    TMR0L = 0x0B;
    INTCON.TMR0IF = 0; // Zera a flag de interrupção
    T0CON.TMR0ON = 1;  // Liga o TIMER0
}

// Configuração das interrupções
void ConfigInterrupt() {
    INTCON.GIE = 1;     // Habilita interrupções globais
    INTCON.PEIE = 1;    // Habilita interrupções periféricas
    INTCON.TMR0IE = 1;  // Habilita interrupção do TIMER0
    INTCON.INT0IE = 1;  // Habilita interrupção externa em RB0
    INTCON3.INT1IE = 1; // Habilita interrupção externa em RB1
    INTCON.INT0IF = 0;  // Reseta a flag de interrupção de RB0
    INTCON3.INT1IF = 0; // Reseta a flag de interrupção de RB1
}

// Rotina principal
void main() {
    ConfigMCU();
    ConfigTIMER();
    ConfigInterrupt();

    while (1) {
        // O loop principal não precisa de lógica aqui,
        // porque as interrupções estão tratando a contagem.
    }
}

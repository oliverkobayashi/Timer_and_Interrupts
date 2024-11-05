// Variáveis globais
volatile unsigned int contador = 0;  // Armazena o valor do cronômetro
int TH;  // Valor alto para configuração do Timer0
int TL;  // Valor baixo para configuração do Timer0

// Configuração inicial do microcontrolador
void ConfigMCU() {
    #ifdef PIC18F45K22
        ANSELB = 0;  // Desativa funções analógicas em PORTB
        ANSELD = 0;  // Desativa funções analógicas em PORTD
        INTCON2.RBPU = 0; // Desabilita resistor de pull-up global
        WPUB.WPUB0 = 1;   // Ativa pull-up para RB0
        WPUB.WPUB1 = 1;   // Ativa pull-up para RB1
    #else
        ADCON1 |= 0x0F;  // Configura PORTA e PORTB como digitais
        INTCON2.RBPU = 0; // Desabilita resistor de pull-up global
    #endif
    
    TRISD = 0;         // Configura PORTD como saída
    PORTD = 0;         // Inicializa PORTD em 0
    TRISB.RB0 = 1;     // Configura RB0 como entrada
    TRISB.RB1 = 1;     // Configura RB1 como entrada
    PORTB.RB0 = 1;     // Inicializa RB0 em estado alto
    PORTB.RB1 = 1;     // Inicializa RB1 em estado alto
}

// Exibição de números no display de 7 segmentos
void display(int number) {
    if (number < 0 || number > 9) {
        LATD = 0x00;  // Desliga todos os segmentos se inválido
        return;
    }
    switch (number) {
        case 0:{LATD = 0b00111111; break;}//0
        case 1:{LATD = 0b00000110; break;}//1
        case 2:{LATD = 0b01011011; break;}//2
        case 3:{LATD = 0b01001111; break;}//3
        case 4:{LATD = 0b01100110; break;}//4
        case 5:{LATD = 0b01101101; break;}//5
        case 6:{LATD = 0b01111101; break;}//6
        case 7:{LATD = 0b00000111; break;}//7
        case 8:{LATD = 0b01111111; break;}//8
        case 9:{LATD = 0b01101111; break;}//9
    }
}

// Configurações das interrupções
void SetupGeneralInterrupts() {
    INTCON.GIEH = 1;   // Habilita interrupções de alta prioridade
    INTCON.GIEL = 1;   // Habilita interrupções de baixa prioridade
    RCON.IPEN = 1;     // Habilita priorização de interrupções

    // Configura interrupções do Timer0
    INTCON.TMR0IF = 0; // Limpa flag do Timer0
    INTCON.TMR0IE = 1; // Habilita interrupção do Timer0
    INTCON2.TMR0IP = 1;// Define Timer0 como alta prioridade

    // Configura interrupções externas
    INTCON.INT0IF = 0; // Limpa flag do INT0
    INTCON.INT1IF = 0; // Limpa flag do INT1
    INTCON.INT0IE = 1; // Habilita interrupção INT0
    INTCON3.INT1IE = 1; // Habilita interrupção INT1
    INTCON3.INT1IP = 1; // Define INT1 como alta prioridade

    // Define bordas de subida para as interrupções externas
    INTCON2.INTEDG0 = 1;
    INTCON2.INTEDG1 = 1;
}

// Rotina de interrupção de alta prioridade
void INTERRUPT_HIGH() iv 0x0008 ics ICS_AUTO {
    // Trata interrupção do Timer0
    if (INTCON.TMR0IF == 1) {
        contador++;  // Incrementa o contador
        if (contador >= 10) contador = 0; // Reseta se alcançar 10

        display(contador);  // Atualiza o display

        // Reinicializa o Timer0
        T0CON.TMR0ON = 0;   // Desativa temporariamente para configuração
        TMR0H = TH;         // Configura valor alto
        TMR0L = TL;         // Configura valor baixo
        T0CON.TMR0ON = 1;   // Reativa o Timer0

        INTCON.TMR0IF = 0;  // Limpa a flag de interrupção
    }

    // Trata interrupção do botão INT0 (RB0)
    if (INTCON.INT0IF == 1) {
        // Configura Timer0 para 1 segundo
        TH = 0x0B;
        TL = 0xDC;

        // Reinicializa o Timer0
        T0CON.TMR0ON = 0; 
        TMR0H = TH; 
        TMR0L = TL; 
        T0CON.TMR0ON = 1;
        INTCON.INT0IF = 0;  // Limpa a flag de interrupção
    }

    // Trata interrupção do botão INT1 (RB1)
    if (INTCON3.INT1IF == 1) {
        // Configura Timer0 para 0.25 segundo
        TH = 0xC2; 
        TL = 0xF7;
        
        // Reinicializa o Timer0
        T0CON.TMR0ON = 0;
        TMR0H = TH; 
        TMR0L = TL; 
        T0CON.TMR0ON = 1;
        INTCON3.INT1IF = 0; // Limpa a flag de interrupção
    }
}


void main() {
    
    ConfigMCU();  // Inicializa o microcontrolador com as configurações específicas

    T0CON = 0B00000100; //TIMER_OFF, MOD_16BITS, TIMER, PRES_1:32

    SetupGeneralInterrupts();  // Configura as interrupções gerais, incluindo INT0 e INT1

    // Loop principal do programa
    while(1) {
        // Neste loop, a maior parte do trabalho é feita pelas rotinas de interrupção
        // O microcontrolador pode executar outras tarefas não críticas aqui,
        // ou entrar em um estado de baixo consumo se aplicável.
    }
}
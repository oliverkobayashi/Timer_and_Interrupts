// Configuração inicial do microcontrolador
void ConfigurarMCU() {
  ADCON1 |= 0x0F;         // Define todos os pinos como digitais
  INTCON2.RBPU = 0;       // Desativa o resistor pull-up global
  TRISD = 0;              // Define PORTD como saída
  PORTD = 0;              // Inicializa PORTD com valor 0
  TRISB.RB0 = 1;          // Configura RB0 como entrada
  TRISB.RB1 = 1;          // Configura RB1 como entrada
  PORTB.RB0 = 1;          // Define estado inicial alto para RB0
  PORTB.RB1 = 1;          // Define estado inicial alto para RB1
}

// Exibição do display de 7 segmentos
void exibirNumero(int numero) {
  // Desliga os segmentos para valores inválidos
  if (numero < 0 || numero > 9) {
    LATD = 0x00;   
    return;
  }
  switch (numero) {
    case 0: LATD = 0b00111111; break;
    case 1: LATD = 0b00000110; break;
    case 2: LATD = 0b01011011; break;
    case 3: LATD = 0b01001111; break;
    case 4: LATD = 0b01100110; break;
    case 5: LATD = 0b01101101; break;
    case 6: LATD = 0b01111101; break;
    case 7: LATD = 0b00000111; break;
    case 8: LATD = 0b01111111; break;
    case 9: LATD = 0b01101111; break;
  }
}

// Configuração das interrupções
void ConfiguraInterrupcoes() {
  RCON.IPEN = 1;            // Ativa prioridade de interrupção
  INTCON.GIEH = 1;          // Habilita interrupções de alta prioridade
  INTCON.GIEL = 1;          // Habilita interrupções de baixa prioridade
  INTCON.TMR0IF = 0;        // Limpa flag do Timer0
  INTCON.TMR0IE = 1;        // Ativa interrupção do Timer0
  INTCON2.TMR0IP = 1;       // Define prioridade alta para Timer0
  INTCON.INT0IF = 0;        // Limpa flag de interrupção INT0
  INTCON.INT1IF = 0;        // Limpa flag de interrupção INT1
  INTCON.INT0IE = 1;        // Ativa interrupção de RB0 (INT0)
  INTCON3.INT1IE = 1;       // Ativa interrupção de RB1 (INT1)
  INTCON3.INT1IP = 1;       // Define prioridade alta para INT1
  INTCON2.INTEDG0 = 1;      // Configura borda de subida para INT0
  INTCON2.INTEDG1 = 1;      // Configura borda de subida para INT1
}

// Rotina de interrupção de alta prioridade
int TimerHigh, TimerLow;                // Configuração dos bytes altos e baixos
unsigned int contador = 0;              // Valor do cronômetro

void INTERRUPT_HIGH() iv 0x0008 ics ICS_AUTO {
  if (INTCON.TMR0IF) {                  // Timer0
    contador = (contador + 1) % 10;     // Incrementa e reinicia ao alcançar 10
    exibirNumero(contador);             // Atualiza o display com o valor do contador
    // Reinicia o Timer0
    T0CON.TMR0ON = 0;
    TMR0H = TimerHigh;
    TMR0L = TimerLow;
    T0CON.TMR0ON = 1;       // Reativa o Timer
    INTCON.TMR0IF = 0;      // Limpa a flag do Timer0
  }
  // Verifica a interrupção de RB0
  if (INTCON.INT0IF) {
    // Ajusta Timer0 para 1 segundo
    TimerHigh = 0x0B;       
    TimerLow = 0xDC;
    // Reinicia o Timer0
    T0CON.TMR0ON = 0;
    TMR0H = TimerHigh;
    TMR0L = TimerLow;
    T0CON.TMR0ON = 1;
    INTCON.INT0IF = 0;
  }
  // Verifica a interrupção de RB0
  if (INTCON3.INT1IF) {
    // Ajusta Timer0 para 0,25 segundo
    TimerHigh = 0xC2;       
    TimerLow = 0xF7;
    // Reinicia o Timer0
    T0CON.TMR0ON = 0;
    TMR0H = TimerHigh;
    TMR0L = TimerLow;
    T0CON.TMR0ON = 1;
    INTCON3.INT1IF = 0;
  }
}

void main() {
  ConfigurarMCU();          // Inicializa o microcontrolador
  T0CON = 0b00000100;       // Configura Timer0 (prescaler 1:32)
  ConfiguraInterrupcoes();  // Habilita interrupções gerais

  while (1) {
    // Loop principal do programa;
  }
}

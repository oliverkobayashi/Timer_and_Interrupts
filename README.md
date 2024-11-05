# Cronômetro Digital com Timer e Interrupções

## Integrantes
- André Jacob Suaide - **13864673**
- Henrique de Oliveira Araujo - **13863950**
- Oliver Kenzo Kobayashi - **13676930**
- Vítor Augusto Paiva de Brito - **13732303**

Este repositório contém o projeto para a disciplina de **Aplicação de Microprocessadores (SEL0614)**, no qual será desenvolvido um cronômetro digital utilizando o microcontrolador *PIC18F4550* programado em linguagem **C**.

---

## Visão Geral do Projeto
Este projeto busca desenvolver um cronômetro digital usando o microcontrolador *PIC18F4550*, programado em **C**. A ideia é fazer com que um display de 7 segmentos conte de 0 a 9 em *loop*, alterando o período de contagem (1 segundo ou 0,25 segundos) ao pressionar botões específicos. Para isso, serão utilizados temporizadores e interrupções para gerenciar a contagem de tempo e a interação com os botões. O projeto será implementado no compilador **MikroC PRO** for *PIC* e testado no **SimulIDE**, comparando essa abordagem em **C** com uma implementação anterior feita em Assembly para explorar as vantagens e desafios de cada método.

## Conceitos Envolvidos
A prática explora diversos conceitos de sistemas embarcados, como temporização, interrupções e interface de I/O. O **Timer0** é configurado para gerar interrupções periódicas que regulam o intervalo de contagem no display de 7 segmentos. Quando um botão na porta **RB0** é pressionado, a contagem ocorre a cada 1 segundo; quando o botão da porta **RB1** é ativado, a contagem muda para intervalos de 0,25 segundos. O uso de interrupções permite que o microcontrolador responda rapidamente a eventos externos, garantindo que a troca de períodos de contagem seja feita de forma eficiente e precisa.

A configuração do **Timer0** inclui a definição do prescaler e a ativação das interrupções globais e específicas do periférico. O acionamento do display de 7 segmentos, ligado à porta **D**, é controlado por meio da varredura dos pinos, assegurando que cada número de 0 a 9 seja exibido corretamente.

---

## Explicação do código

### Configuração inicial do microcontrolador (`ConfigurarMCU`)
```C
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
```

A função `ConfigurarMCU` é responsável pela configuração inicial dos pinos do microcontrolador. A instrução `ADCON1 |= 0x0F;` garante que todos os pinos sejam configurados como digitais, desativando funcionalidades analógicas. `INTCON2.RBPU = 0;` desativa o resistor pull-up global, o que permite que entradas externas controladas possam ser usadas. `TRISD = 0;` define o `PORTD` como saída, e `PORTD = 0;` inicializa esse porto com valor 0, apagando quaisquer sinais. Os pinos `RB0` e `RB1` são configurados c...

### Exibição do display de 7 segmentos (`exibirNumero`)
```C
void exibirNumero(int numero) {
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
```

A função `exibirNumero` é projetada para controlar um display de 7 segmentos, exibindo números de 0 a 9. Se o número passado estiver fora do intervalo válido, a instrução `LATD = 0x00;` apaga todos os segmentos do display. Utilizando uma estrutura `switch`, cada número de 0 a 9 é mapeado para os bits de `LATD` correspondentes, que acendem os segmentos apropriados do display para formar o número desejado.

### Configuração das interrupções (`ConfiguraInterrupcoes`)
```C
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
```

A função `ConfiguraInterrupcoes` é crucial para a configuração das interrupções do microcontrolador. `RCON.IPEN = 1;` ativa o sistema de prioridades de interrupção, permitindo uma distinção entre interrupções de alta e baixa prioridade. `INTCON.GIEH = 1;` e `INTCON.GIEL = 1;` habilitam interrupções de alta e baixa prioridade, respectivamente. As flags de interrupção de `Timer0`, `INT0`, e `INT1` são limpas com `INTCON.TMR0IF = 0;`, `INTCON.INT0IF = 0;`, e `INTCON.INT1IF = 0;`, para evitar a ativação aci...

### Rotina de interrupção de alta prioridade (`INTERRUPT_HIGH`)

Para calcular os valores de `TimerHigh` e `TimerLow`, é utilizada a seguinte equação:

$$
x = 65536 - \frac{\text{Perído (us)}}{0,5 \times 32}
$$

Em que os dois primeiros dígitos de **x** em hexadecimal serão atribuído a `TimerHigh` e os dois últimos dígitos atribuídos a `TimerLow`, com o período do cronômetro como variável da equação.

#### Explicação dos Termos

- **65536**: Um valor constante usado na fórmula, que representa o valor máximo para um contador de 16 bits.
- **tempo em us**: O tempo em microsegundos.
- **0,5 \* 32**: Uma constante de divisor para ajustar a unidade do tempo de acordo com a equação.

---

#### Cálculo para 1 segundo (1000000 us)

Substituindo o valor do tempo na fórmula:

$$
x = 65536 - \frac{1000000}{0.5 \times 32}
$$

##### Resultado para 1 segundo:
$$
x = 3036
$$

Em **hexadecimal**:

$$
x = 0x0BDC
$$

Portanto, `TimerHigh` será equivalente a `0B` e `TimerLow` será atribuído `DC`.

---

#### Cálculo para 0,25 segundos (250000 us)

Substituindo o valor do tempo na fórmula:

$$
x = 65536 - \frac{250000}{0.5 \times 32}
$$

##### Resultado para 0,25 segundos:
$$
x = 49911
$$

Em **hexadecimal**:

$$
x = 0xC2F7
$$

Portanto, `TimerHigh` será equivalente a `C2` e `TimerLow` será atribuído `F7`.

---

```C
void INTERRUPT_HIGH() iv 0x0008 ics ICS_AUTO {
  if (INTCON.TMR0IF) {                  // Timer0
    contador = (contador + 1) % 10;     // Incrementa e reinicia ao alcançar 10
    exibirNumero(contador);             // Atualiza o display com o valor do contador
    // Reinicializa o Timer0
    T0CON.TMR0ON = 0;
    TMR0H = TimerHigh;
    TMR0L = TimerLow;
    T0CON.TMR0ON = 1;
    INTCON.TMR0IF = 0;      // Limpa a flag de interrupção do Timer0
  }

  if (INTCON.INT0IF) {      // Botão RB0 (INT0)
    TimerHigh = 0x0B;       // Ajusta Timer0 para 1 segundo
    TimerLow = 0xDC;
    T0CON.TMR0ON = 0;
    TMR0H = TimerHigh;
    TMR0L = TimerLow;
    T0CON.TMR0ON = 1;
    INTCON.INT0IF = 0;      // Limpa a flag de interrupção INT0
  }

  if (INTCON3.INT1IF) {     // Botão RB1 (INT1)
    TimerHigh = 0xC2;       // Ajusta Timer0 para 0.25 segundo
    TimerLow = 0xF7;
    T0CON.TMR0ON = 0;
    TMR0H = TimerHigh;
    TMR0L = TimerLow;
    T0CON.TMR0ON = 1;
    INTCON3.INT1IF = 0;     // Limpa a flag de interrupção INT1
  }
}
```

A rotina de interrupção `INTERRUPT_HIGH` é projetada para lidar com interrupções de alta prioridade. Quando a flag `INTCON.TMR0IF` é detectada, indicando uma interrupção do `Timer0`, o contador é incrementado e reiniciado ao atingir 10. A função `exibirNumero` é chamada para atualizar o display com o valor atual do contador. O `Timer0` é então reinicializado com os valores `TimerHigh` e `TimerLow`. Se a flag de `INT0` for acionada (associada ao botão `RB0`), os registros do `Timer0` são configurados para ...

### Função principal (`main`)
```C
void main() {
  ConfigurarMCU();          // Inicializa o microcontrolador
  T0CON = 0b00000100;       // Configura Timer0 (prescaler 1:32)
  ConfiguraInterrupcoes();  // Habilita interrupções gerais

  while (1) {
    // Loop principal do programa;
  }
}
```

A função `main` é o ponto de entrada do programa e inicializa a execução. Primeiro, `ConfigurarMCU` é chamada para configurar o microcontrolador. A instrução `T0CON = 0b00000100;` define o `Timer0` com um prescaler de 1:32, ajustando o tempo de contagem. A função `ConfiguraInterrupcoes` é então chamada para habilitar as interrupções e configurá-las conforme necessário. O loop `while (1)` mantém o programa em execução contínua, esperando por interrupções que serão tratadas pelas rotinas associadas.

---

## O circuito

O circuito foi montado no **SimulIDE** de acordo com a figura abaixo, com as resistências para haver um controle da corrente e prevenir possível 'queimas' de componentes em um cenário de montagem do circuito físico.

![Circuito](assets/circuito.png)

### Funcionamento do circuito

O vídeo abaixo demonstra o funcionamento do circuito no software **SimulIDE**, apresentando os diferentes modos de operação de acordo com o pressionamento dos botões, alterando o período do cronômetro com 1 segundo ou 0,25 segundos.

<p align="center">
  <img width="460" height="300" src="assets/display.gif" alt="Circuito operando">
</p>

---

## Comparação com o Projeto em Assembly para o 8051
No projeto anterior, que usava Assembly para o microcontrolador *8051*, a programação envolvia maior detalhamento no controle de registradores e sub-rotinas. O código em Assembly exigia que o desenvolvedor gerenciasse manualmente cada aspecto das operações de delay, verificações de botão e manipulação de bits para o acionamento do display. Embora esse nível de controle permitisse uma compreensão aprofundada do funcionamento do hardware, ele tornava o desenvolvimento mais trabalhoso e sujeito a erros.

Por outro lado, a implementação atual em **C** para o *PIC18F4550* oferece uma abordagem mais intuitiva e eficiente. A linguagem **C** permite que o desenvolvedor trabalhe com funções e bibliotecas que simplificam a configuração de periféricos e o gerenciamento de interrupções, tornando o código mais legível e fácil de manter. Essa modularidade facilita a separação de blocos de código, como a configuração do timer e a lógica de exibição no display.

## Vantagens e Desvantagens
A principal vantagem da implementação em C para o *PIC18F* está na produtividade e clareza do código. A linguagem de médio nível permite que a programação seja mais rápida e menos suscetível a erros, especialmente com o suporte de *IDE*s e depuradores que ajudam na análise do funcionamento do programa. A desvantagem, no entanto, reside na dependência do compilador, que pode gerar código ligeiramente menos eficiente em termos de uso de memória quando comparado a um código Assembly cuidadosamente otimizado.

No projeto em Assembly com o *8051*, a vantagem estava na eficiência de memória e na possibilidade de otimizações finas, algo relevante em sistemas com recursos limitados. Contudo, a complexidade da programação em **Assembly** aumentava significativamente o tempo de desenvolvimento e dificultava a manutenção.

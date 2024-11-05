# Cronômetro Digital com Timer e Interrupções

## Integrantes
- André Jacob Suaide - **13864673**
- Henrique de Oliveira Araujo - **13863950**
- Oliver Kenzo Kobayashi - **13676930**
- Vítor Augusto Paiva de Brito - **13732303**

Este repositório contém o projeto para a disciplina de **Aplicação de Microprocessadores (SEL0614)**, no qual será desenvolvido um cronômetro digital utilizando o microcontrolador *PIC18F4550* programado em linguagem **C**.

## Visão Geral do Projeto
Este projeto busca desenvolver um cronômetro digital usando o microcontrolador *PIC18F4550*, programado em **C**. A ideia é fazer com que um display de 7 segmentos conte de 0 a 9 em *loop*, alterando o período de contagem (1 segundo ou 0,25 segundos) ao pressionar botões específicos. Para isso, serão utilizados temporizadores e interrupções para gerenciar a contagem de tempo e a interação com os botões. O projeto será implementado no compilador **MikroC PRO** for *PIC* e testado no **SimulIDE**, comparando essa abordagem em **C** com uma implementação anterior feita em Assembly para explorar as vantagens e desafios de cada método.

## Conceitos Envolvidos
A prática explora diversos conceitos de sistemas embarcados, como temporização, interrupções e interface de I/O. O **Timer0** é configurado para gerar interrupções periódicas que regulam o intervalo de contagem no display de 7 segmentos. Quando um botão na porta **RB0** é pressionado, a contagem ocorre a cada 1 segundo; quando o botão da porta **RB1** é ativado, a contagem muda para intervalos de 0,25 segundos. O uso de interrupções permite que o microcontrolador responda rapidamente a eventos externos, garantindo que a troca de períodos de contagem seja feita de forma eficiente e precisa.

A configuração do **Timer0** inclui a definição do prescaler e a ativação das interrupções globais e específicas do periférico. O acionamento do display de 7 segmentos, ligado à porta **D**, é controlado por meio da varredura dos pinos, assegurando que cada número de 0 a 9 seja exibido corretamente.

## O circuito

O circuito foi montado no **SimulIDE** de acordo com a figura abaixo, com as resistências para haver um controle da corrente e prevenir possível 'queimas' de componentes em um cenário de montagem do circuito físico.

![Circuito](assets/circuito.png)

### Funcionamento do circuito

O vídeo abaixo demonstra o funcionamento do circuito no software **SimulIDE**, apresentando os diferentes modos de operação de acordo com o pressionamento dos botões, alterando o período do cronômetro com 1 segundo ou 0,25 segundos.

![Circuito Funcionando](assets/display.gif)

## Comparação com o Projeto em Assembly para o 8051
No projeto anterior, que usava Assembly para o microcontrolador *8051*, a programação envolvia maior detalhamento no controle de registradores e sub-rotinas. O código em Assembly exigia que o desenvolvedor gerenciasse manualmente cada aspecto das operações de delay, verificações de botão e manipulação de bits para o acionamento do display. Embora esse nível de controle permitisse uma compreensão aprofundada do funcionamento do hardware, ele tornava o desenvolvimento mais trabalhoso e sujeito a erros.

Por outro lado, a implementação atual em **C** para o *PIC18F4550* oferece uma abordagem mais intuitiva e eficiente. A linguagem **C** permite que o desenvolvedor trabalhe com funções e bibliotecas que simplificam a configuração de periféricos e o gerenciamento de interrupções, tornando o código mais legível e fácil de manter. Essa modularidade facilita a separação de blocos de código, como a configuração do timer e a lógica de exibição no display.

## Vantagens e Desvantagens
A principal vantagem da implementação em C para o *PIC18F* está na produtividade e clareza do código. A linguagem de médio nível permite que a programação seja mais rápida e menos suscetível a erros, especialmente com o suporte de *IDE*s e depuradores que ajudam na análise do funcionamento do programa. A desvantagem, no entanto, reside na dependência do compilador, que pode gerar código ligeiramente menos eficiente em termos de uso de memória quando comparado a um código Assembly cuidadosamente otimizado.

No projeto em Assembly com o *8051*, a vantagem estava na eficiência de memória e na possibilidade de otimizações finas, algo relevante em sistemas com recursos limitados. Contudo, a complexidade da programação em **Assembly** aumentava significativamente o tempo de desenvolvimento e dificultava a manutenção.

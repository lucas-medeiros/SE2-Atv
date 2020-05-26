/*
Camada de Abstracao do Hardware (HAL) para execucao no Visual Studio
*/

#include "hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <conio.h>


// Variaveis da Camada de Abstracao do Hardware (HAL)
boolean pinTurnSignal_LEFT;		// Representa o estado do pino de saida do microcontrolador ligado ao sinalizador ESQUERDO
boolean pinTurnSignal_RIGHT;	// Representa o estado do pino de saida do microcontrolador ligado ao sinalizador DIREITO
boolean pinBreak_LEFT;			// Representa o estado do pino de saida do microcontrolador ligado a luz de freio do lado ESQUERDO
boolean pinBreak_RIGHT;			// Representa o estado do pino de saida do microcontrolador ligado a luz de freio do lado DIREITO
boolean pinBreak;				// Representa o estado do pino de entrada do microcontrolador ligado ao freio

tuCommand lastCommand;			// Armazena o estado atual dos sinalizadores. Veja tambem: tuCommand

//FILAS:
extern xQueueHandle queueCommands;
extern xQueueHandle queueBreak;

// Esta é a task que recebe os comandos do teclado e atualiza o valor de lastCommand
void task_Key(void *pParam){
	char key = ' ';
	while (1) {
		key = getch();
		switch (key) {
			case 'l':
				lastCommand.TurnCommands = command_Left;
				break;
			case 'r':
				lastCommand.TurnCommands = command_Right;
				break;
			case ' ':
				lastCommand.TurnCommands = command_None;
				break;
			case 'a':
				lastCommand.Alert = !lastCommand.Alert;
				break;
			case 'b':
				lastCommand.Break = !lastCommand.Break;
				pinBreak = !pinBreak;
				xQueueSendToBack(queueBreak, &pinBreak, 0);
				break;
			case 'i':
				lastCommand.Ignition = !lastCommand.Ignition;
				break;
			default:
				break;
		}
		xQueueSendToBack(queueCommands, &lastCommand, portMAX_DELAY);
		Show();
	}
}

// Inicializa a Camada de Abstracao de Hardware.
void InitHAL() {
	lastCommand.TurnCommands = command_None;
	lastCommand.Alert = 0;
	lastCommand.Break = 0;
	lastCommand.Ignition = 0;
	pinTurnSignal_LEFT = 0;
	pinTurnSignal_RIGHT = 0;
	pinBreak_LEFT = 0;
	pinBreak_RIGHT = 0;
	pinBreak = 0;
	xTaskCreate(task_Key, "task_Key", 1000, NULL, 1, NULL);
	Show();
}

// Metodo que retorna o estado da  alavanca de comando dos sinalizadores ("alavanca das setas junto ao volante"). Ver tamb�m tuCommand
tuCommand getTurnCommand() {
	return lastCommand;
}

/* Liga ou desliga o sinalizador direito.
- s: estado do sinalizador (TRUE = acende / FALSE = apaga)
*/
void TurnSignalRight(boolean s) {
	pinTurnSignal_RIGHT = s;
	Show();
}

/* Liga ou desliga o sinalizador esquerdo.
- s: estado do sinalizador (TRUE = acende / FALSE = apaga)
*/
void TurnSignalLeft(boolean s) {
	pinTurnSignal_LEFT = s;
	Show();
}

// Inverte o estado do sinalizador direito. Se estava apagado, acende. Se estava aceso, apaga.
void ToggleTurnSignalRight() {
	pinTurnSignal_RIGHT = !pinTurnSignal_RIGHT;
	Show();
}

// Inverte o estado do sinalizador direito. Se estava apagado, acende. Se estava aceso, apaga.
void ToggleTurnSignalLeft() {
	pinTurnSignal_LEFT = !pinTurnSignal_LEFT;
	Show();
}

// Inverte o estado da luz de freio. Se estava apagada, acende. Se estava acesa, apaga.
void ToggleBreak() {
	pinBreak_LEFT = !pinBreak_LEFT;
	pinBreak_RIGHT = !pinBreak_RIGHT;
	Show();
}

/* Liga ou desliga a luz de freio.
- s: estado do sinalizador (TRUE = acende / FALSE = apaga)
*/
void TurnSignalBreak(boolean s) {
	pinBreak_LEFT = s;
	pinBreak_RIGHT = s;
	Show();
}

//Fazer um m�todo show() pra mostrar na tela quando piscar (valor das vari�veis)
void Show() {
	printf("Pisca esquerdo: %d | Pisca direito: %d \tLuz de freio esquerda: %d | Luz de freio direita: %d \tIgnicao: %d\r", 
		pinTurnSignal_LEFT, pinTurnSignal_RIGHT, pinBreak_LEFT, pinBreak_RIGHT, lastCommand.Ignition);
}

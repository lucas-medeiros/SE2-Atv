/*
Camada de Abstracao do Hardware (HAL) para execucao no Visual Studio
*/

#include "hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <conio.h>

// IMPORTANTE: fique a vontade para criar outros metodos e variaveis

// Variaveis da Camada de Abstracao do Hardware (HAL)
boolean pinTurnSignal_LEFT;		// Representa o estado do pino de saida do microcontrolador ligado ao sinalizador ESQUERDO
boolean pinTurnSignal_RIGHT;	// Representa o estado do pino de saida do microcontrolador ligado ao sinalizador DIREITO
tuCommand lastCommand;			// Armazena o estado atual dos sinalizadores. Veja tambem: tuCommand

// Esta � a task que recebe os comandos do teclado e atualiza o valor de lastCommand
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
			default:
				break;
		}
		vTaskDelay(portTICK_RATE_MS); //delay de 1ms?
	}
}

// Inicializa a Camada de Abstracao de Hardware.
void InitHAL() {
	lastCommand.TurnCommands = command_None;
	lastCommand.Alert = 0;
	pinTurnSignal_LEFT = 0;
	pinTurnSignal_RIGHT = 0;
	xTaskCreate(task_Key, "task_Key", 1000, NULL, 1, NULL);
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

//Fazer um m�todo show() pra mostrar na tela quando piscar (valor das vari�veis)
void Show() {
	printf("Pisca esquerdo: %d | Pisca direito: %d\r", pinTurnSignal_LEFT, pinTurnSignal_RIGHT);
}

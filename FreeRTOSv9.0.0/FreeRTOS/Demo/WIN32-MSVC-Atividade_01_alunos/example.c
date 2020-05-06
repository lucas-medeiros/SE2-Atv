
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

// Demais includes
#include "basic_io.h"
#include "hal.h"
#include "sm.h"

// Esta será a sua máquina de estados
StateMachine smBlink;

//Input de comando
tuCommand lastCommand;

//Handle para deletar tasks
xTaskHandle handleLeft;
xTaskHandle handleRight;

//Protótipo das funções
void task_BlinkLeft(void* pParam);
void task_BlinkRight(void* pParam);
void stopSignal(void);


// Assinaturas dos estados da máquina de estados
	STATE(state_blinkR);
	STATE(state_blinkL);
	STATE(state_blinkAlert);
	STATE(state_blinkNone);

// TODO: crie aqui o código dos estados da máquina de estados
	STATE(state_blinkR) {
		lastCommand = getTurnCommand();
		if (FIRST) {
			stopSignal();
			xTaskCreate(task_BlinkRight, "task_BlinkRight", 1000, NULL, 2, &handleRight);
		}
		if (lastCommand.Alert == 1) {
			NEXT_STATE(state_blinkAlert);
			vTaskDelete(handleRight);
		}
		else if (lastCommand.TurnCommands == command_Left) {
			NEXT_STATE(state_blinkL);
			vTaskDelete(handleRight);
		}
		else if (lastCommand.TurnCommands == command_None) {
			NEXT_STATE(state_blinkNone);
			vTaskDelete(handleRight);
		}
	}

	STATE(state_blinkL) {
		lastCommand = getTurnCommand();
		if (FIRST) {
			stopSignal();
			xTaskCreate(task_BlinkLeft, "task_BlinkLeft", 1000, NULL, 2, &handleLeft);
		}
		if (lastCommand.Alert == 1) {
			NEXT_STATE(state_blinkAlert);
			vTaskDelete(handleLeft);
		} 
		else if (lastCommand.TurnCommands == command_Right) {
			NEXT_STATE(state_blinkR);
			vTaskDelete(handleLeft);
		}
		else if (lastCommand.TurnCommands == command_None) {
			NEXT_STATE(state_blinkNone);
			vTaskDelete(handleLeft);
		}
	}

	STATE(state_blinkAlert) {
		if (FIRST) {
			stopSignal();
			xTaskCreate(task_BlinkRight, "task_BlinkRight", 1000, NULL, 2, &handleRight);
			xTaskCreate(task_BlinkLeft, "task_BlinkLeft", 1000, NULL, 2, &handleLeft);
		}
		lastCommand = getTurnCommand();
		if (lastCommand.Alert == 0) {
			vTaskDelete(handleRight);
			vTaskDelete(handleLeft);
			if (lastCommand.TurnCommands == command_Left)
				NEXT_STATE(state_blinkL);
			else if (lastCommand.TurnCommands == command_Right)
				NEXT_STATE(state_blinkR);
			else if (lastCommand.TurnCommands == command_None)
				NEXT_STATE(state_blinkNone);
		}
	}

	STATE(state_blinkNone) {
		// TODO: adicione aqui o código de um dos estados
		lastCommand = getTurnCommand();
		if (FIRST) {
			stopSignal();
		}
		if (lastCommand.Alert == 1)
			NEXT_STATE(state_blinkAlert);
		else if (lastCommand.TurnCommands == command_Left)
			NEXT_STATE(state_blinkL);
		else if (lastCommand.TurnCommands == command_Right)
			NEXT_STATE(state_blinkR);
	}


// TODO: implemente aqui os codigos das Tasks
void task_BlinkLeft(void *pParam) {
	for (;;) {
		ToggleTurnSignalLeft();
		vTaskDelay(333 / portTICK_RATE_MS); //delay pra piscar a 1,5Hz
	}
	
}


void task_BlinkRight(void *pParam) {
	for (;;) {
		ToggleTurnSignalRight();
		vTaskDelay(333 / portTICK_RATE_MS); //delay pra piscar a 1,5Hz
	}
}


void stopSignal(void) { //função para parar os sinalizadores
	TurnSignalLeft(0);
	TurnSignalRight(0);
}


void vApplicationIdleHook(void) {
	EXEC(smBlink);
}


int main_(void) {
	printf("Iniciando sistema\n\n");
	printf("l - sinalizador para esquerda\nr - sinalizador para direita\na - liga/desliga pisca alerta\n<espaco> - desliga sinalizador\n\n");
	Show();
	InitHAL();
	INIT(smBlink, state_blinkNone);
	vTaskStartScheduler();
	//for (;;);
	return 0;
}


/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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

//Filas
xQueueHandle queueCommands;
xQueueHandle queueBreak;

//Protótipo das funções
void task_Blink(void* pParam);
void task_Break(void* pParam);
void stopSignal(void);


// Assinaturas dos estados da máquina de estados
	STATE(state_blinkR);
	STATE(state_blinkL);
	STATE(state_blinkAlert);
	STATE(state_blinkNone);

// TODO: crie aqui o código dos estados da máquina de estados
	STATE(state_blinkR) {
		//lastCommand = getTurnCommand();
		if (FIRST) {
			stopSignal();
			xTaskCreate(task_Blink, "task_BlinkRight", 1000, ToggleTurnSignalRight, 3, &handleRight);
		}
		xQueueReceive(queueCommands, &lastCommand, portMAX_DELAY);
		if (lastCommand.Alert == 1) {
			NEXT_STATE(state_blinkAlert);
			vTaskDelete(handleRight);
		}
		else if (lastCommand.TurnCommands == command_Left && lastCommand.Ignition) {
			NEXT_STATE(state_blinkL);
			vTaskDelete(handleRight);
		}
		else if (lastCommand.TurnCommands == command_None || !lastCommand.Ignition) {
			NEXT_STATE(state_blinkNone);
			vTaskDelete(handleRight);
		}
	}

	STATE(state_blinkL) {
		//lastCommand = getTurnCommand();
		if (FIRST) {
			stopSignal();
			xTaskCreate(task_Blink, "task_BlinkLeft", 1000, ToggleTurnSignalLeft, 3, &handleLeft);
		}
		xQueueReceive(queueCommands, &lastCommand, portMAX_DELAY);
		if (lastCommand.Alert == 1) {
			NEXT_STATE(state_blinkAlert);
			vTaskDelete(handleLeft);
		}
		else if (lastCommand.TurnCommands == command_Right && lastCommand.Ignition) {
			NEXT_STATE(state_blinkR);
			vTaskDelete(handleLeft);
		}
		else if (lastCommand.TurnCommands == command_None || !lastCommand.Ignition) {
			NEXT_STATE(state_blinkNone);
			vTaskDelete(handleLeft);
		}
	}

	STATE(state_blinkAlert) {
		if (FIRST) {
			stopSignal();
			xTaskCreate(task_Blink, "task_BlinkRight", 1000, ToggleTurnSignalRight, 3, &handleRight);
			xTaskCreate(task_Blink, "task_BlinkLeft", 1000, ToggleTurnSignalLeft, 3, &handleLeft);
		}
		//lastCommand = getTurnCommand();
		xQueueReceive(queueCommands, &lastCommand, portMAX_DELAY);
		if (lastCommand.Alert == 0) {
			vTaskDelete(handleRight);
			vTaskDelete(handleLeft);
			if (lastCommand.TurnCommands == command_Left && lastCommand.Ignition)
				NEXT_STATE(state_blinkL);
			else if (lastCommand.TurnCommands == command_Right && lastCommand.Ignition)
				NEXT_STATE(state_blinkR);
			else if (lastCommand.TurnCommands == command_None)
				NEXT_STATE(state_blinkNone);
		}
	}

	STATE(state_blinkNone) {
		// TODO: adicione aqui o código de um dos estados
		//lastCommand = getTurnCommand();
		if (FIRST) {
			stopSignal();
		}
		xQueueReceive(queueCommands, &lastCommand, portMAX_DELAY);
		if (lastCommand.Alert == 1)
			NEXT_STATE(state_blinkAlert);
		else if (lastCommand.TurnCommands == command_Left && lastCommand.Ignition)
			NEXT_STATE(state_blinkL);
		else if (lastCommand.TurnCommands == command_Right && lastCommand.Ignition)
			NEXT_STATE(state_blinkR);
	}


// TODO: implemente aqui os codigos das Tasks
void task_Blink(void *pParam) {
	void(*function)();
	function = pParam;
	for (;;) {
		(*function)();
		vTaskDelay(333 / portTICK_RATE_MS); //delay pra piscar a 1,5Hz
	}
}


void task_Break(void* pParam) {
	boolean pinBreak;
	for(;;){
		xQueueReceive(queueBreak, &pinBreak, portMAX_DELAY);
		TurnSignalBreak(pinBreak);
	}
}


void stopSignal(void) { //função para parar os sinalizadores
	TurnSignalLeft(0);
	TurnSignalRight(0);
}


void task_StateMachine(void* pParam) {
	for (;;) {
		EXEC(smBlink);
	}
}


int main_(void) {
	printf("Iniciando sistema\n\n");
	printf("l - sinalizador para esquerda\nr - sinalizador para direita\na - liga/desliga pisca alerta\n<espaco> - desliga sinalizador\n");
	printf("i - ignicao (liga/desliga o carro)\nb - freio (ativa/desativa o freio)\n\n");
	Show();
	InitHAL();
	queueCommands = xQueueCreate(1, sizeof(tuCommand));
	queueBreak = xQueueCreate(1, sizeof(boolean));
	xTaskCreate(task_StateMachine, "task_StateMachine", 1000, NULL, 1, NULL);
	xTaskCreate(task_Break, "task_Break", 1000, NULL, 3, NULL);
	INIT(smBlink, state_blinkNone);
	vTaskStartScheduler();
	//for (;;);
	return 0;
}

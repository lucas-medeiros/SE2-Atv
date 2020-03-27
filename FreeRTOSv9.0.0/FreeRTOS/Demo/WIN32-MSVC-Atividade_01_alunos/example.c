
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

// Demais includes
#include "basic_io.h"
#include "hal.h"
#include "sm.h"

// Esta ser� a sua m�quina de estados
StateMachine smBlink;

tuCommand lastCommand;


// TODO: defina aqui as assinaturas dos estados da m�quina de estados
	STATE(state_blinkR);
	STATE(state_blinkL);
	STATE(state_blinkAlert);
	STATE(state_blinkNone);

// TODO: crie aqui o c�digo dos estados da m�quina de estados
	STATE(state_blinkR) {
		// TODO: adicione aqui o c�digo de um dos estados
		lastCommand = getTurnCommand();
		if (lastCommand.Alert == 1)
			NEXT_STATE(state_blinkAlert);
		else if (lastCommand.TurnCommands == command_Left)
			NEXT_STATE(state_blinkL);
		else if (lastCommand.TurnCommands == command_None)
			NEXT_STATE(state_blinkNone);
	}

	STATE(state_blinkL) {
		// TODO: adicione aqui o c�digo de um dos estados
		lastCommand = getTurnCommand();
		if (lastCommand.Alert == 1)
			NEXT_STATE(state_blinkAlert);
		else if (lastCommand.TurnCommands == command_None)
			NEXT_STATE(state_blinkNone);
		else if (lastCommand.TurnCommands == command_Right)
			NEXT_STATE(state_blinkR);
	}

	STATE(state_blinkAlert) {
		// TODO: adicione aqui o c�digo de um dos estados
		lastCommand = getTurnCommand();
		if (lastCommand.Alert == 0) {
			if (lastCommand.TurnCommands == command_Left)
				NEXT_STATE(state_blinkL);
			else if (lastCommand.TurnCommands == command_Right)
				NEXT_STATE(state_blinkR);
			else if (lastCommand.TurnCommands == command_None)
				NEXT_STATE(state_blinkNone);
		}
	}

	STATE(state_blinkNone) {
		// TODO: adicione aqui o c�digo de um dos estados
		lastCommand = getTurnCommand();
		if (lastCommand.Alert == 1)
			NEXT_STATE(state_blinkAlert);
		else if (lastCommand.TurnCommands == command_Left)
			NEXT_STATE(state_blinkL);
		else if (lastCommand.TurnCommands == command_Right)
			NEXT_STATE(state_blinkR);
	}


// TODO: implemente aqui os codigos das Tasks
void task_BlinkLeft(void *pParam) {
	// TODO: adicione aqui o c�digo da task
	for (;;) {
		if (COMPARE(smBlink, state_blinkL) == 1 || COMPARE(smBlink, state_blinkAlert) == 1) {
			ToggleTurnSignalLeft();
			vTaskDelay(333 / portTICK_RATE_MS); //delay pra piscar a 1,5Hz
		}
		
	}
	
}

void task_BlinkRight(void *pParam) {
	// TODO: adicione aqui o c�digo da task
	for (;;) {
		if (COMPARE(smBlink, state_blinkR) == 1 || COMPARE(smBlink, state_blinkAlert) == 1) {
			ToggleTurnSignalRight();
			vTaskDelay(333 / portTICK_RATE_MS); //delay pra piscar a 1,5Hz
		}
	}
}

void task_StateMachine(void *pParam) {
	// TODO: adicione aqui o c�digo da task que executa a m�quina de estados
	for (;;) {
		//executa a maquina de estados
		EXEC(smBlink);

	}
}

int main_(void)
{
	// TODO: adicione aqui o c�digo que executa ao iniciar
	InitHAL();
	INIT(smBlink, state_blinkNone);
	xTaskCreate(task_BlinkLeft, "task_BlinkLeft", 1000, NULL, 1, NULL);
	xTaskCreate(task_BlinkRight, "task_BlinkRight", 1000, NULL, 1, NULL);
	xTaskCreate(task_StateMachine, "task_StateMachine", 1000, NULL, 1, NULL);
	vTaskStartScheduler();
	for (;;);
	return 0;
}


/* task_SM = pega last command e transicina os estados da SM -> NEXT_STATE 
	blink a 1,5Hz (sleep 333); */


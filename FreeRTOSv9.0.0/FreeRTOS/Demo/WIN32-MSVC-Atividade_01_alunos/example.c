
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

// Demais includes
#include "basic_io.h"
#include "hal.h"
#include "sm.h"

// Esta será a sua máquina de estados
StateMachine smBlink;


// TODO: defina aqui as assinaturas dos estados da máquina de estados
	STATE(state_blinkR);
	STATE(state_blinkL);
	STATE(state_blinkAlert);
	STATE(state_blinkNone);

// TODO: crie aqui o código dos estados da máquina de estados
	STATE(state_blinkR) {
		// TODO: adicione aqui o código de um dos estados
	}

	STATE(state_blinkL) {
		// TODO: adicione aqui o código de um dos estados
	}

	STATE(state_blinkAlert) {
		// TODO: adicione aqui o código de um dos estados
	}

	STATE(state_blinkNone) {
		// TODO: adicione aqui o código de um dos estados
	}


// TODO: implemente aqui os codigos das Tasks
void task_BlinkLeft(void *pParam) {
	// TODO: adicione aqui o código da task
	for (;;) {
		if (COMPARE(smBlink, state_blinkL) == 1) {
			ToggleTurnSignalLeft();
			vTaskDelay(333 / portTICK_RATE_MS); //delay pra piscar a 1,5Hz
		}
		
	}
	
}

void task_BlinkRight(void *pParam) {
	// TODO: adicione aqui o código da task
	for (;;) {
		if (COMPARE(smBlink, state_blinkR) == 1) {
			ToggleTurnSignalRight();
			vTaskDelay(333 / portTICK_RATE_MS); //delay pra piscar a 1,5Hz
		}
	}
}

void task_StateMachine(void *pParam) {
	// TODO: adicione aqui o código da task que executa a máquina de estados
	for (;;) {
		//executa a maquina de estados

	}
}

int main_(void)
{
	// TODO: adicione aqui o código que executa ao iniciar
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


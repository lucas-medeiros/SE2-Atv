
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

// Demais includes
#include "basic_io.h"
#include "hal.h"
#include "sm.h"

// Esta ser� a sua m�quina de estados
StateMachine smBlink;

// TODO: defina aqui as assinaturas dos estados da m�quina de estados
	STATE(...);
	STATE(...);

// TODO: crie aqui o c�digo dos estados da m�quina de estados
	STATE(...) {
		// TODO: adicione aqui o c�digo de um dos estados
	}

	STATE(...) {
		// TODO: adicione aqui o c�digo de um dos estados
	}

// TODO: implemente aqui os codigos das Tasks
void task_BlinkLeft(void *pParam) {
	// TODO: adicione aqui o c�digo da task
	ToggleTurnSignalLeft();
}

void task_BlinkRight(void *pParam) {
	// TODO: adicione aqui o c�digo da task
	ToggleTurnSignalRight();
}

void task_StateMachine(void *pParam) {
	// TODO: adicione aqui o c�digo da task que executa a m�quina de estados
}

int main_(void)
{
	// TODO: adicione aqui o c�digo que executa ao iniciar
	InitHAL();
	return 0;
}


/* task_SM = pega last command e transicina os estados da SM -> NEXT_STATE 
	blink a 1,5Hz (sleep 333); */


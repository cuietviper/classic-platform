/*
* Configuration of module: Os (Tasks.c)
*
* Created by:              
* Copyright:               
*
* Configured for (MCU):    MPC5602B
*
* Module vendor:           ArcCore
* Generator version:       2.0.34
*
* Generated by Arctic Studio (http://arccore.com) 
*/

#include "Os.h"

// #define USE_LDEBUG_PRINTF // Uncomment this to turn debug statements on.
#include "debug.h"
#include "Lin.h"
#include "LinIf.h"
#include "LinSM.h"
#include "PduR.h"
#include "Com.h"
uint32 valuerx1, valuerx2, valuerx3, valuerx4;
sint32 valuetx2,valuetx3 ;
uint8 valuetx1;

void ComM_BusSM_ModeIndication( NetworkHandleType Channel, ComM_ModeType ComMode ){}

// Simple application which drives one Tx message and receives two messages
void Task( void ) {


	   while (1) {
	      WaitEvent(EVENT_MASK_Event1);

	      LinIf_MainFunction();
	      Com_MainFunctionRx();
	      Com_MainFunctionTx();

	      ClearEvent(EVENT_MASK_Event1);

	      Com_ReceiveSignal(Signal_Rx_1, &valuerx1);
	      Com_ReceiveSignal(Signal_Rx_2, &valuerx2);
	      Com_ReceiveSignal(word_1, &valuerx3);
	      Com_ReceiveSignal(word_2, &valuerx4);
	      valuetx1++;
	      valuetx2--;
	      valuetx3 = valuetx2 ^ 0xFFFFFF ;

	      Com_SendSignal(Signal_Tx_1, &valuetx1);
	      Com_SendSignal(Signal_Tx_2, &valuetx2);
	      Com_SendSignal(Signal_Tx_3, &valuetx3);

	   }

}

void init( void ) {

	valuetx1 = valuetx2 = valuetx3 = 0;
	Lin_Init(NULL);
	LinIf_Init(NULL);
	PduR_Init(&PduR_Config);
	LinSM_Init(NULL);
	Com_Init(&ComConfiguration);
	LinSM_RequestComMode(0,COMM_FULL_COMMUNICATION);
	LinSM_ScheduleRequest(0,1);
	TerminateTask();

}


void OsIdle( void ) {
	while(1);
}

// Hooks
#define ERROR_LOG_SIZE 20

struct LogBad_s {
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
	TaskType taskId;
	OsServiceIdType serviceId;
	StatusType error;
};

void ErrorHook ( StatusType Error ) {

	TaskType task;
	static struct LogBad_s LogBad[ERROR_LOG_SIZE];
	static uint8_t ErrorCount = 0;
	GetTaskID(&task);
	OsServiceIdType service = OSErrorGetServiceId();
	
	LDEBUG_PRINTF("## ErrorHook err=%d\n",Error);

	/* Log the errors in a buffer for later review */
	LogBad[ErrorCount].param1 = os_error.param1;
	LogBad[ErrorCount].param2 = os_error.param2;
	LogBad[ErrorCount].param3 = os_error.param3;
	LogBad[ErrorCount].serviceId = service;
	LogBad[ErrorCount].taskId = task;
	LogBad[ErrorCount].error = Error;

	ErrorCount++;

	// Stall if buffer is full.
	while(ErrorCount >= ERROR_LOG_SIZE);

}

void PostTaskHook ( void ) {
	TaskType task;
	GetTaskID(&task);
	LDEBUG_PRINTF("## PreTaskHook, taskid=%d\n",task);
}

void PreTaskHook ( void ) {
	TaskType task;
	GetTaskID(&task);
	LDEBUG_PRINTF("## PreTaskHook, taskid=%d\n",task);
}

void ShutdownHook ( StatusType Error ) {
	LDEBUG_PRINTF("## ShutdownHook\n");
	while(1);
}

void StartupHook ( void ) {
	LDEBUG_PRINTF("## StartupHook\n");
}

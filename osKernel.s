			AREA | .text|, CODE, READONLY, ALIGN=2
			THUMB
			EXTERN currentPt
			EXPORT	SysTick_Handler
			EXPORT	osSchedulerLaunch


SysTick_Handler ; saves R0,R1,R2,R3,R12,LR,PC,PSR
		CPSID	I
		PUSH	{R4-R11}
		LDR 	R0,=currentPt; R0 points to currentPt
		LDR		R1,[R0]	; R1 = currentPt
		STR		SP,[R1]	; SP = currentPt
		LDR		R1,[R1,#4] ; R1 = currentPt->next
		STR		R1, [R0]	; currentPt = R1 = currentPt->next
		LDR		SP, [R1]	; SP = currentPt->stackPt
		POP		{R4-R11}
		CPSIE	I
		BX 		LR


osSchedulerLaunch
		LDR		R0,=currentPt
		LDR		R2,[R0];R2 = currentPt
		LDR		SP,[R2] ; SP = currentPt->stackPt
		POP		{R4-R11}
		POP		{R0-R3}
		POP		{R12}
		ADD		SP,SP,#4
		POP		{LR}
		ADD		SP,SP,#4
		CPSIE	I
		BX		LR

		ALIGN
		END



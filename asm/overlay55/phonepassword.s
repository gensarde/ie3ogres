
	.include "/macros/function.inc"
	.include "/include/overlay55.inc"

	.text
	arm_func_start FUN_ov55_0211d128
FUN_ov55_0211d128: ; 0x0211D128
	stmfd sp!, {r3, r4, r5, lr}
	mov r5, r0
	ldr r3, _0211D158 ; =0x0211DA5C
	mov r4, #0
	mov r0, r4
	add r1, r5, #0x10
	mov r2, #0x18
	str r3, [r5]
	bl MIi_CpuClearFast
	mov r0, r5
	str r4, [r5, #0x28]
	ldmfd sp!, {r3, r4, r5, pc}
_0211D158: .word ov55_0211DA5C
	arm_func_end FUN_ov55_0211d128

	arm_func_start _ZN14CPhonePasswordD1Ev
_ZN14CPhonePasswordD1Ev: ; 0x0211D15C
	stmfd sp!, {r4, lr}
	ldr r1, _0211D178 ; =0x0211DA5C
	mov r4, r0
	str r1, [r4]
	bl FUN_ov55_0211d1c4
	mov r0, r4
	ldmfd sp!, {r4, pc}
_0211D178: .word ov55_0211DA5C
	arm_func_end _ZN14CPhonePasswordD1Ev

	arm_func_start _ZN14CPhonePasswordD0Ev
_ZN14CPhonePasswordD0Ev: ; 0x0211D17C
	stmfd sp!, {r4, lr}
	ldr r1, _0211D1A0 ; =0x0211DA5C
	mov r4, r0
	str r1, [r4]
	bl FUN_ov55_0211d1c4
	mov r0, r4
	bl _ZdlPv
	mov r0, r4
	ldmfd sp!, {r4, pc}
_0211D1A0: .word ov55_0211DA5C
	arm_func_end _ZN14CPhonePasswordD0Ev

	arm_func_start _ZN14CPhonePassword8readFileEm
_ZN14CPhonePassword8readFileEm: ; 0x0211D1A4
	mov r2, r0
	str r1, [r2, #0x28]
	ldr r0, _0211D1BC ; =0x0211DA64
	ldr r12, _0211D1C0 ; = _ZN7Archive17ReadNewUncompressEPKcP9SFileData
	add r1, r2, #4
	bx r12
_0211D1BC: .word ov55_0211DA64
_0211D1C0: .word _ZN7Archive17ReadNewUncompressEPKcP9SFileData
	arm_func_end _ZN14CPhonePassword8readFileEm

	arm_func_start FUN_ov55_0211d1c4
FUN_ov55_0211d1c4: ; 0x0211D1C4
	stmfd sp!, {r4, lr}
	mov r4, r0
	add r0, r4, #4
	bl _ZN7Archive10DeallocateEP9SFileData
	mov r0, #0
	str r0, [r4, #4]
	str r0, [r4, #8]
	strb r0, [r4, #0xc]
	strb r0, [r4, #0xd]
	strb r0, [r4, #0xe]
	ldmfd sp!, {r4, pc}
	arm_func_end FUN_ov55_0211d1c4

	arm_func_start _ZN14CPhonePassword5checkEPcS0_
_ZN14CPhonePassword5checkEPcS0_: ; 0x0211D1F0
	stmfd sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
	sub sp, sp, #0x28
	mov r7, #0
	mov r9, r0
	mov r4, r1
	mov r8, r2
	str r7, [sp]
	str r7, [sp, #4]
	mov r5, r7
	add r6, sp, #8
_0211D218:
	mov r0, r7, lsl #1
	rsb r0, r0, #0xe
	strb r5, [r6]
	strb r5, [r6, #1]
	strb r5, [r6, #2]
	add r2, r8, r0
	ldrb r1, [r8, r0]
	ldrb r0, [r2, #1]
	cmp r7, #0
	strb r1, [r6]
	strb r0, [r6, #1]
	beq _0211D264
	ldr r0, [sp, #4]
	ldr r2, [sp]
	mov r1, r0, lsl #7
	orr r1, r1, r2, lsr #25
	mov r0, r2, lsl #7
	str r0, [sp]
	str r1, [sp, #4]
_0211D264:
	mov r0, r9
	mov r1, r6
	bl FUN_ov55_0211d4c0
	add r7, r7, #1
	ldr r2, [sp]
	and r0, r0, #0x7f
	ldr r1, [sp, #4]
	orr r2, r2, r0
	orr r3, r1, r0, asr #31
	str r2, [sp]
	str r3, [sp, #4]
	cmp r7, #8
	blt _0211D218
	mov r1, #0
	ldr r8, [r9, #0x28]
	ldr r6, _0211D4AC ; =0x6C078965
	ldr r7, _0211D4B0 ; =0x5D588B65
	ldr r2, _0211D4B4 ; =0x00269EC3
	mov r0, r1
	str r1, [r9, #0x14]
	str r8, [r9, #0x10]
	str r6, [r9, #0x18]
	str r7, [r9, #0x1c]
	str r2, [r9, #0x20]
	str r0, [r9, #0x24]
	add r1, sp, #0xc
_0211D2CC:
	ldr r6, [r9, #0x18]
	ldr r2, [r9, #0x10]
	ldr r3, [r9, #0x14]
	umull r7, r5, r6, r2
	mla r5, r6, r3, r5
	ldr r3, [r9, #0x1c]
	ldr r6, [r9, #0x20]
	mla r5, r3, r2, r5
	adds r6, r6, r7
	ldr r2, [r9, #0x24]
	str r6, [r9, #0x10]
	adc r2, r2, r5
	str r2, [r1, r0, lsl #2]
	add r0, r0, #1
	str r2, [r9, #0x14]
	cmp r0, #3
	blo _0211D2CC
	ldr r1, [sp, #0x10]
	ldr r2, [sp]
	ldr r3, [sp, #0xc]
	orr r1, r1, #0
	eor r2, r2, r1
	ldr r1, [sp, #4]
	orr r3, r3, #0
	eor r3, r1, r3
	add r0, sp, #0
	str r3, [sp, #4]
	add r1, r0, #1
	str r2, [sp]
	mov r0, #0
	mov r3, #1
_0211D348:
	ldr r2, [sp]
	ldr r5, [sp, #4]
	mov r6, r2, lsr r0
	rsb r2, r0, #0x20
	orr r6, r6, r5, lsl r2
	sub r2, r0, #0x20
	orr r6, r6, r5, lsr r2
	ldrb r7, [r1]
	and r5, r6, #1
	and r2, r7, #1
	bic r7, r7, #1
	orr r5, r7, r5
	strb r5, [r1], #1
	mvn r5, r3, lsl r0
	ldr r6, [sp]
	ldr r7, [sp, #4]
	and r6, r6, r5
	orr r12, r6, r2, lsl r0
	mov r2, r2, lsl r0
	and r5, r7, r5, asr #31
	orr lr, r5, r2, asr #31
	add r0, r0, #1
	str r12, [sp]
	str lr, [sp, #4]
	cmp r0, #6
	blt _0211D348
	ldr r6, _0211D4B8 ; =0x0000FFFF
	mov r8, r12, lsr #0x10
	and r0, r12, r6
	mov r2, r0, lsl #0x10
	mov r9, lr, lsr #0x10
	orr r8, r8, lr, lsl #16
	mov r7, r2, lsr #0x10
	ldr r0, _0211D4BC ; =gLogicThink
	add r1, sp, #0
	mov r2, #5
	str r8, [sp]
	str r9, [sp, #4]
	mov r5, #0
	bl FUN_02074854
	cmp r7, r0
	addne sp, sp, #0x28
	movne r0, r5
	ldmnefd sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
	ldr r0, [sp]
	ldr r1, [sp, #4]
	mov r2, r0, lsr #8
	orr r2, r2, r1, lsl #24
	and r0, r2, r6
	mov r1, r1, lsr #8
	mov r2, r2, lsr #0x10
	mov r0, r0, lsl #0x10
	mov r3, r1, lsr #0x10
	orr r2, r2, r1, lsl #16
	mov r6, r0, lsr #0x10
	add r1, sp, #0x18
	str r2, [sp]
	str r3, [sp, #4]
	mov r0, #4
_0211D434:
	strb r5, [r1]
	strb r5, [r1, #1]
	strb r5, [r1, #2]
	strb r5, [r1, #3]
	add r1, r1, #4
	subs r0, r0, #1
	bne _0211D434
	add r3, sp, #0x18
	mov r2, #8
_0211D458:
	ldrb r1, [r4]
	ldrb r0, [r4, #1]
	add r4, r4, #2
	subs r2, r2, #1
	strb r0, [r3, #1]
	strb r1, [r3], #2
	bne _0211D458
	ldr r2, [sp]
	ldr r1, _0211D4B8 ; =0x0000FFFF
	ldr r0, _0211D4BC ; =gLogicThink
	and r1, r2, r1
	mov r2, r1, lsl #0x10
	mov r4, r2, lsr #0x10
	add r1, sp, #0x18
	mov r2, #0x10
	bl FUN_02074854
	cmp r4, r0
	movne r6, r5
	mov r0, r6
	add sp, sp, #0x28
	ldmfd sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
_0211D4AC: .word 0x6C078965
_0211D4B0: .word 0x5D588B65
_0211D4B4: .word 0x00269EC3
_0211D4B8: .word 0x0000FFFF
_0211D4BC: .word gLogicThink
	arm_func_end _ZN14CPhonePassword5checkEPcS0_

	arm_func_start FUN_ov55_0211d4c0
FUN_ov55_0211d4c0: ; 0x0211D4C0
	stmfd sp!, {r4, r5, r6, r7, r8, lr}
	ldr r2, [r0, #8]
	mov r8, r1
	movs r7, r2, lsr #1
	ldr r5, [r0, #4]
	mov r6, #0
	beq _0211D50C
	mov r4, #2
_0211D4E0:
	mov r1, r8
	mov r2, r4
	add r0, r5, r6, lsl #1
	bl strncmp
	cmp r0, #0
	moveq r0, r6
	ldmeqfd sp!, {r4, r5, r6, r7, r8, pc}
	add r0, r6, #1
	and r6, r0, #0xff
	cmp r6, r7
	blo _0211D4E0
_0211D50C:
	mov r0, #0
	ldmfd sp!, {r4, r5, r6, r7, r8, pc}
	arm_func_end FUN_ov55_0211d4c0

	.section .init, 4
	arm_func_start FUN_ov55_0211d568
FUN_ov55_0211d568: ; 0x0211D568
	stmfd sp!, {r3, r4, r5, lr}
	mov r5, #0x2000
	mov r0, r5
	bl FX_Sqrt
	ldr r4, _0211D5B0 ; =0x0211DA3C
	str r0, [r4, #0x14]
	ldr r0, _0211D5B4 ; =0x00009CCD
	bl FX_Sqrt
	mov r1, #0x800
	str r0, [r4, #0x10]
	ldr r0, _0211D5B8 ; =0x0000EA3C
	str r1, [r4, #0xc]
	str r0, [r4, #8]
	add r0, r5, #0x4e0000
	str r0, [r4, #4]
	add r0, r1, #0x840000
	str r0, [r4]
	ldmfd sp!, {r3, r4, r5, pc}
_0211D5B0: .word ov55_0211DA3C
_0211D5B4: .word 0x00009CCD
_0211D5B8: .word 0x0000EA3C
	arm_func_end FUN_ov55_0211d568

	.section .sinit, 4
ov55_0211D5C0:
	.word FUN_ov55_0211d568

	.data
	.global ov55_0211DA3C
ov55_0211DA3C:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global ov55_0211DA5C
ov55_0211DA5C:
	.word _ZN14CPhonePasswordD1Ev
	.word _ZN14CPhonePasswordD0Ev
	.global ov55_0211DA64
ov55_0211DA64:
	.byte 0x2F, 0x64, 0x61, 0x74, 0x61, 0x5F, 0x69, 0x7A, 0x2F, 0x6C, 0x6F, 0x67
	.byte 0x69, 0x63, 0x2F, 0x70, 0x68, 0x6F, 0x6E, 0x65, 0x5F, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72
	.byte 0x64, 0x2E, 0x74, 0x78, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

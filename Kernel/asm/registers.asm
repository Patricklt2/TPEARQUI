GLOBAL getCurrentRAX
GLOBAL getCurrentRBX
GLOBAL getCurrentRCX
GLOBAL getCurrentRDX
GLOBAL getCurrentRBP
GLOBAL getCurrentRSP
GLOBAL getCurrentRDI
GLOBAL getCurrentRSI
GLOBAL getCurrentR15
GLOBAL getCurrentR14
GLOBAL getCurrentR13
GLOBAL getCurrentR12
GLOBAL getCurrentR11
GLOBAL getCurrentR10
GLOBAL getCurrentR9
GLOBAL getCurrentR8

section .text
; ------ MACRO -------
%macro getCurrent 1
	mov rax, %1
	ret
%endmacro

getCurrentRAX:
	getCurrent rax

getCurrentRBX:
	getCurrent rbx

getCurrentRCX:
	getCurrent rcx

getCurrentRDX:
	getCurrent rdx

getCurrentRBP:
	getCurrent rbp

getCurrentRSP:
	getCurrent rsp

getCurrentRDI:
	getCurrent rdi

getCurrentRSI:
	getCurrent rsi

getCurrentR15:
	getCurrent r15

getCurrentR14:
	getCurrent r14

getCurrentR13:
	getCurrent r13

getCurrentR12:
	getCurrent r12

getCurrentR11:
	getCurrent r11

getCurrentR10:
	getCurrent r10

getCurrentR9:
	getCurrent r9

getCurrentR8:
	getCurrent r8
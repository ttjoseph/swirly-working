; SH4 FPU implementation helper routines
[bits 32]

section .text

extern shfpu_sop0, shfpu_sop1, shfpu_sresult
extern shfpu_dop0, shfpu_dop1, shfpu_dresult

global shfpu_setContext, shfpu_sFLOAT

; void SHFpu_setContext(Dword *FR, Dword *XF, Dword *FPUL, Dword *FPSCR)
shfpu_setContext:
;	int 3
	push ebp
	mov ebp, esp
	
	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]
	mov ecx, [ebp + 16]
	mov edx, [ebp + 20]
	mov [FR], eax
	mov [XF], ebx
	mov [FPUL], ecx
	mov [FPSCR], edx	
	
	leave	
	ret
	
shfpu_sFADD:
	fld dword [shfpu_sop0]
	fld dword [shfpu_sop1]
	fmul st0, st1
	fstp st0
	fstp dword [shfpu_sresult]
	ret

; takes DRn and puts it in FPUL, converting double to float
;shfpu_FCNVDS	

; converts int in FPUL to float
shfpu_sFLOAT:
	mov eax, [FPUL]
	fild dword [eax]
	fstp dword [shfpu_sresult]
	ret

section .data	
FR dd 0 ; pointer!
XF dd 0 ; pointer!
FPUL dd 0 ; pointer!
FPSCR dd 0 ; pointer!

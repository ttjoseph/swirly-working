[bits 32]

section .text

global shcpu_DO_MACL

; shcpu_DO_MACL(Sdword rn, Sdword rm, Dword *mach, Dword *macl)
; this function doesn't increment rm and rn and does not check the s bit
; the function that calls this should inc rm and rn and do the saturation if needed
shcpu_DO_MACL:
	push ebp
	mov ebp, esp

	; multiply rm and rn
	fild dword [ebp + 8]
	fild dword [ebp + 12]
	fmulp st1

	; add the result to mach:macl
	mov eax, [ebp + 16] ; get mach
	mov ecx, [eax]
	mov ebx, [ebp + 20] ; get macl
	mov edx, [ebx]
	mov [g_tmpQword], edx ; low dword
	mov [g_tmpQword + 4], ecx ; high dword
	fild qword [g_tmpQword]
	faddp st1

	; copy answer into mach and macl
	fistp qword [g_tmpQword]
	mov ecx, [g_tmpQword + 4] ; hi
	mov edx, [g_tmpQword] ; lo
	mov [eax], ecx
	mov [ebx], edx

	leave
	ret

section .data
g_tmpQword dd 0
dd 0

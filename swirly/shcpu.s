[bits 32]

section .text

global shcpu_DO_MACL

; shcpu_DO_MACL(Sdword rn, Sdword rm, Dword *mach, Dword *macl)
; this function doesn't increment rm and rn and does not check the s bit
; the function that calls this should inc rm and rn and do the saturation if needed
shcpu_DO_MACL:
	push ebp
	mov ebp, esp

	fild dword [ebp + 8]
	fild dword [ebp + 12]
	fmulp st1

	mov eax, [ebp + 16] ; get mach
	mov eax, [eax]
	mov ebx, [ebp + 20] ; get macl
	mov ebx, [ebx]
	mov [g_tmpQword], ebx ; low dword
	mov [g_tmpQword + 4], eax ; high dword
	fild qword [g_tmpQword]
	faddp st1

	fistp qword [g_tmpQword]

	leave
	ret

section .data
g_tmpQword dd 0
dd 0
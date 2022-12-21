.586							; система команд (процессор Pentium)
.model flat, stdcall			; модель памяти, соглашение о вызовах
includelib kernel32.lib
includelib libucrt.lib
includelib StdLib.lib

ExitProcess PROTO: dword		; прототип функции для завершения процесса Windows

EXTRN strlens: proc
EXTRN print_int: proc
EXTRN print_str: proc
EXTRN atois: proc
EXTRN adate: proc
EXTRN atime: proc

.stack 4096

.const							; сегмент констант - литералы
	L0 sdword 2747
	L1 sdword 5
	L2 sdword 9
	L3 sdword 0
	L4 sdword -14
	L5 sdword 14
	L6 sdword 2147483647
	L7 sdword -2147483648
	L8 byte "", 0
	L9 sdword 20
	L10 sdword 1
	L11 sdword 4
.data							; сегмент данных - переменные и параметры
	numberh_inttypes sdword 0
	numberb_inttypes sdword 0
	numbero_inttypes sdword 0
	num2_main sdword 0
	num_main sdword 0
	data_main sdword 0
	time_main sdword 0
	num3_main sdword 0
	abc_main dword ?
	i_main sdword 0
	callf_main sdword 0
.code							; сегмент кода

;----------- inttypes ------------
inttypes PROC,	ccc_inttypes : sdword  
; --- сохранить регистры ---
push ebx
push edx
; ----------------------
push L0
pop numberh_inttypes

push numberh_inttypes
call print_int

push L1
pop numberb_inttypes

push numberb_inttypes
call print_int

push L2
pop numbero_inttypes

push numbero_inttypes
call print_int

mov edx, numbero_inttypes
cmp edx, numberb_inttypes

jg true0
jl false0
true0:
push numbero_inttypes
call print_int

jmp next0

false0:
push numberb_inttypes
call print_int

next0:

; --- восстановить регистры ---
pop edx
pop ebx
; -------------------------
mov eax, L3
ret
inttypes ENDP
;------------------------------


;----------- MAIN ------------
main PROC

push L4
pop num2_main

push L5
pop num_main

call adate
push eax
pop data_main

push data_main
call print_int

call atime
push eax
pop time_main

push time_main
call print_int

push L6
pop num_main

push L7
pop num3_main

mov abc_main, offset L8
push num3_main
call print_int

push abc_main
call print_str

push L3
pop i_main

cyclenext1:
mov edx, i_main
cmp edx, L9
jg cycle1
push num_main
call print_int

push i_main
push L10
pop ebx
pop eax
add eax, ebx
push eax
pop i_main

jmp cyclenext1
cycle1:

push L11
call inttypes
push eax
pop callf_main

push 0
call ExitProcess
main ENDP
end main
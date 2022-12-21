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
	L0 sdword 0
	L1 sdword 20
	L2 sdword 1
.data							; сегмент данных - переменные и параметры
	i_main sdword 0
.code							; сегмент кода

;----------- MAIN ------------
main PROC

push L0
pop i_main

cyclenext0:
mov edx, i_main
cmp edx, L1
jg cycle0
push i_main
call print_int

push i_main
push L2
pop ebx
pop eax
add eax, ebx
push eax
pop i_main

jmp cyclenext0
cycle0:

push 0
call ExitProcess
main ENDP
end main
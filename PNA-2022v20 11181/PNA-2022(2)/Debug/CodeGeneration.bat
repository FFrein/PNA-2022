call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
:: Абослютынй пусть к папке с вижлой. Там запускаем консоль вижлы
cd/d "C:\MyFile\Универ\3сем\курсач\PNA-2022 prdRls\PNA-2022(2)"
:: Путь к курсачу, тоже только абсолютом сделать можно
Call "..\Debug\PNA-2022(2).exe" -in:in2.txt -out:out.asm
:: Создание asm файла
ml /c /coff /Zi "out.asm"
:: создаём различные модули .. начало относительного позиционирования, заходим в папку с ссемблером и выбираем файл ассемблера
link /OPT:NOREF /DEBUG "out.obj" "StdLib.lib" /SUBSYSTEM:CONSOLE /NODEFAULTLIB:libcurt.lib
:: Собираем екзекут файл ассемблера (Exe)
call out.exe
pause
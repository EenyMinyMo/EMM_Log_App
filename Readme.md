Структура проекта:
Папка source (обязательная): файлы с исходниками программы и библиотеки.
Папка header (обязательная): папка с header файлами программы и библиотеки.
Папка test (в целом не является обязательной): пара тестов для логера и приложения.
Папка exe (необязательная): папка, куда будут компилироваться программа и библиотека.
Папка temp (необязательная): папка для разных временных файлов.
При желании необязательные папки могут быть удалены пользователем.

Код библиотеки находится в папке lib в папках header и source.
Код приложения находится в папке app в папках header и source.

make:
Команда "make subdirs" создает необязательные папки
Команда "make clean" удаляет временные файлы (в том числе может удалить тестовые логи)
Команда "make dyn_lib" компилирует динамическую библиотеку с логером в ./exe/lib.l
Команда "make app" компилирует тестовую программу в ./exe/app

How to use:
1. Убедиться в наличии и работе make, g++.
2. Выполнить "make subdirs", чтобы создать папки для необязательных файлов.
3. Выполнить "make app", проверить отсутствие ошибок.
4. Запустить программу "./exe/app <log_file_name> <log_level>", где <log_file_name> - имя файла, куда будут записывать логи, <log_level> номер уровня логирования.
5. Писать логи в консоль.
6. Для закрытия программы нужно в пустой строке консоли вставить символ конца файла (к примеру CTRL+Z) и нажать Enter.

Пример запуска приложения: "./exe/app log.log 0".

В случае, если в пути файла логирования есть несозданные папки или программа не сможет создать/перезаписать файл, программа вернет ошибку. Если в качестве файла логирования был указан существующий файл, он будет перезаписан.
В качестве уровня логирования нужно написать число из [0, 1, 2]. При неверном числе программа вернет ошибку.

Текст логов писать из символов базового набора ASCII (без кириллицы).

Уровни логирования:
0 - fine
1 - warning
2 - error
Если уровень логирования по умолчанию выше того, что указан при вводе лога - лог будет отброшен.

Написание лога:
При вводе лога любая непустая (не из пробелов) строка считается логом.
Пробелы перед значащими символами отбрасываются.
Для того, чтобы указать уровень лога, нужно перед сообщением лога указать конструкцию -ll=<log_level>.
Пример: "-ll=0 some message". При этом будет выставлен уровень info и текст лога "some message".
Указание уровня, отличного от кодов уровней логирования (приведено выше) вызовет ошибку логирования, данные отправлены не будут.

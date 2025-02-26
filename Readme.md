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

По поводу программы:
решается примерно так:
	ОС     |			программа				| дин. либа | 		ОС
консоль -> | producer -> task queue -> consumer | -> logger | -> файл с логами

Производитель и потребитель выполняются в отдельных потоках.
Между производителем и потребителем потокобезопасная (если я не напортачил) очередь.
Производитель через shared_pointer c флагом жизни потребителя отслеживает его состояние и прекращает работу в случае преждевременного завершния работы потребителя.
Минус такой архитектуры очевидно в том, что передать ошибку от потребителя к производителю сложно (обратное неверно - передать данные производителю просто - достаточно нужный таск отправить). Поэтому пришлось городить костыль с передачей shared_pointer с флагом.
К тому же при передаче потребителя и производителя потоку они копируются, поэтому мы теряем доступ к используемым объектам. shared_pointer с флагом жизни все еще позволяет осуществлять воздействие на объекты, к которым мы потеряли доступ. Не лучшее из возможных решений.

Очередь ограничена сверху заданным кол-вом элементов, чтобы при активном производителе (или группе таковых) не раздуть ее до огромных размеров.
У очереди есть блокирующий push (ждет свободного места в очереди, если его нет) и не блокирующий (соответственно не ждет).
Можно выбирать в зависимости от желаения.

В целом код можно было сделать проще, отказавшись от очереди (тогда обмен между производителем и потребителем будет проходить через метод), но тогда смысла от многопоточки теряется. Очередь в определенных пределах амортизирует разность скоростей производителя и потребителя (в общем случае, не с консолью :)) и выполняет синхронизацию.

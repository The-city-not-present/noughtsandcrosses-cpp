Программа, реализующая искусственный интеллект в игре "крестики-нолики на свободном поле". Попробуйте.

Программа состоит из двух частей. Серверная ( в директории /ai/ ) - консольная программа на с++. Принимает данные в формате
JSON в виде последовательности ходов по следующему образцу;
"[{\"x\":3,\"y\":5},{\"x\":3,\"y\":6},{\"x\":-1,\"y\":6},{\"x\":-1,\"y\":2},{\"x\":-1,\"y\":1},{\"x\":1,\"y\":7}]"
Может читать как первый параметр командной стороки, иначе читает со стандартного входа число байт, равное
переменной окружения Content-length. Иными словами, она принимает запрос POST. Это сделано для того, что
серверная часть запускается как cgi-script (у меня в apache).

Клиентская часть ( в директории /web-client/ ) - это простая html страница, работающая при помощи скриптов на языке Javascript. Она
отправляет запросы через XMLHttpRequest методом POST, общаясь с сервером в формате JSON.

Итого, чтобы посмотреть, как программа работает, надо
либо 1. поднять свой сервер с cgi,
либо 2. ну, вручную вбивать циферки в командной строке и смотреть, что программа отвечает. Это, конечно,
не вариант, так как игры не получится.

Алгоритм основан как на поиске в ширину (я это называю статической оценкой), так и на поиске в глубину.

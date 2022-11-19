#### Яндекс.Практикум
#### Проект "Интерпретатор языка Mython"

Это упрощенный аналог языка Python. Реализована поддержка некоторых примитивных типов, литералов, строк,
классов, методов, объектов, переменных. Арифметические операции: сложение, умножение, вычитание, деление.
Реализованы логические операции и условный оператор. В тексте программы допустимы однострочные комментарии.
Для вывода информации в стандартный поток вывода реализована функция **print**.
В результате парсинга текста программы формируется AST (абстрактное синтаксическое дерево) которое, по сути, 
представляет одну большую составную инструкцию (содержащую все остальные инструкции программы). Далее, 
интерпретатор пошагово выполняет все инструкции программы одну за другой.

#### Сборка
Сборка выполняется с помощью **cmake**. Сторонних зависимостей нет.
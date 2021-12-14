# Parallel algorithms implementations using OpenCilk

Для сборки необходимо добавить файлы `OpenCilk` и `Google Tests` в проект:
## Установка OpenCilk
* `git clone -b opencilk/v1.0 https://github.com/OpenCilk/infrastructure`
* `infrastructure/tools/get $(pwd)/opencilk`
* `infrastructure/tools/build $(pwd)/opencilk $(pwd)/build`
* `cd $(pwd)/build`
* `cmake -DCMAKE_INSTALL_PREFIX=`<путь до папки с проектом>`/parallel-qsort/cilk -P cmake_install.cmake`

## Установка Google Tests
* `git clone https://github.com/google/googletest.git `<путь до папки с проектом>`/parallel-qsort/tests/lib`

## Запуск
Компиляция проекта производится из директории с использованием `CMake`
* `cmake .`
* `make`

Запуск тестирования производится из папки `tests`
* `./src/tests/run_primitives_tests` для запуска тестов базовых инструментов распараллеливания
* `./src/tests/run_qsort_tests` для запуска тестов сортировки


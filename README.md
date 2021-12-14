# Parallel algorithms using OpenCilk
Для сборки необходимо добавить файлы `OpenCilk` и `Google Tests` в проект:

## Установка OpenCilk
* `git clone -b opencilk/v1.0 https://github.com/OpenCilk/infrastructure`
* `infrastructure/tools/get $(pwd)/opencilk`
* `infrastructure/tools/build $(pwd)/opencilk $(pwd)/build`
* `cd $(pwd)/build`
* `cmake -DCMAKE_INSTALL_PREFIX=<путь до папки с проектом>/parallel_algorithms-hw/cilk -P cmake_install.cmake`

## Установка Google Tests
* `git clone https://github.com/google/googletest.git <путь до папки с проектом>/parallel_qsort/tests/googletest`

## Запуск
Компиляция проекта производится из директории с использованием `CMake`
* `CC=cilk/bin/clang CXX=cilk/bin/clang++ cmake .`
* `make`

Запуск тестирования производится из папки `tests`
* `./parallel_qsort/tests/run_primitives_tests` для запуска тестов базовых инструментов распараллеливания
* `./parallel_qsort/tests/run_qsort_tests` для запуска тестов сортировки


# Parallel algorithms using OpenCilk
Для сборки необходимо добавить файлы `OpenCilk` и `Google Tests` в проект:

## Установка [OpenCilk](https://github.com/OpenCilk/)
* `git clone -b opencilk/v1.0 https://github.com/OpenCilk/infrastructure`
* `infrastructure/tools/get $(pwd)/opencilk`
* `infrastructure/tools/build $(pwd)/opencilk $(pwd)/build`
* `cd $(pwd)/build`
* `cmake -DCMAKE_INSTALL_PREFIX=<путь до папки с проектом>/parallel_algorithms-hw/cilk -P cmake_install.cmake`

## Установка [Google Tests](https://github.com/google/googletest)
* `git clone https://github.com/google/googletest.git <путь до папки с проектом>/parallel_qsort/tests/googletest`

## Установка [PCTL](https://github.com/deepsea-inria/pctl/blob/master/doc/pctl.md)
* `wget https://raw.githubusercontent.com/deepsea-inria/pctl/master/script/get.sh`
* `chmod +x ./get.sh`
* `./get.sh <путь до папки с проектом>/parallel_algorithms-hw`

## Запуск
Компиляция проекта производится из директории с использованием `CMake`
* `CC=cilk/bin/clang CXX=cilk/bin/clang++ cmake .`
* `make`

Запуск тестирования производится из папки `tests`
* `./parallel_qsort/tests/run_primitives_tests` для запуска тестов базовых инструментов распараллеливания
* `./parallel_qsort/tests/run_qsort_tests` для запуска тестов сортировки


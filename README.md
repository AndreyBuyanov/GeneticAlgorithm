GeneticAlgorithm
===========
![License](https://img.shields.io/badge/Code%20License-MIT-blue.svg)
---------------
Лабораторная работа №2 по дисциплине "Интеллектуальные системы".

Сборка и запуск
---------------
```bash
cmake -S . -B build && cmake --build build --config RelWithDebInfo
```

Запуск в докере
---------------
```bash
docker build -t intelligent-systems/genetic-algorithm .
docker run -it intelligent-systems/genetic-algorithm
```

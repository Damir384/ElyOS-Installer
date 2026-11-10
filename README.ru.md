# ElyOS Installer (Модульный установщик)

[![Arch Linux CI Build](https://github.com/Damir384/ElyOS-Installer/actions/workflows/ci.yml/badge.svg)](https://github.com/Damir384/ElyOS-Installer/actions/workflows/ci.yml)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Arch Linux Native](https://img.shields.io/badge/Target-Arch_Linux-blue?logo=arch-linux)](https://archlinux.org/)

[English](README.md) | [Русский](README.ru.md)

> **«Автономность — это не изоляция, а независимость.»**  
> *«Автоматизируй то, что хочешь. Контролируй то, что нужно.»*

Модульный и легковесный установщик на C++, предназначенный для установки стандартного **Arch Linux** с возможностью добавления надстройки **ElyOS**. Работает быстро, без тяжелых зависимостей (без Python и Electron), используя удобный терминальный интерфейс (TUI).

---

## Что такое ElyOS?

ElyOS — это **не изолированный форк** Arch Linux:
1. **Чистая база Arch:** Установщик развертывает официальную, неизмененную систему Arch Linux напрямую из стандартных репозиториев.
2. **Слой ElyOS:** Пользователь может по желанию установить дополнительные инструменты, готовые конфигурации и стили оформления семейства ElyOS.

---

## Возможности установщика

- **Быстрый и нетребовательный:** Запускается мгновенно и работает прямо в терминале без необходимости поднимать графический сервер.
- **Пошаговый мастер (FSM):** Архитектура конечного автомата контролирует каждый этап, не дает перескочить нужные шаги и позволяет спокойно вернуться назад.
- **Понятный выбор накопителей:** Сканирует физические диски через ``lsblk``, автоматически отсеивая служебные устройства памяти (zram, loop-устройства).
- **Надежное выполнение системных команд:** Запускает утилиты с принудительной локалью ``LANG=C``, исключая ошибки из-за системных языков.
- **Подробный журнал:** Записывает каждый шаг и вывод команд в файл ``elyos-installer.log`` для удобной отладки.

---

## Стек технологий

* **Язык:** C++17 / C++20
* **Система сборки:** CMake (>= 3.20) + Ninja
* **Интерфейс:** FTXUI (Terminal UI)
* **Парсинг данных:** nlohmann_json
* **Целевая система:** Arch Linux (UEFI/GPT, systemd, ``util-linux``)

---

## Сборка и запуск

### Зависимости (Arch Linux):
Выполните: ``sudo pacman -S base-devel cmake ninja git nlohmann-json``

### Сборка:
Выполните: ``cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel $(nproc)``

### Запуск:
Для работы с дисковыми накопителями требуются права суперпользователя:
- Обычный режим: ``sudo ./build/bin/ElyOS_Installer``
- Режим отладки: ``sudo ./build/bin/ElyOS_Installer --debug``

Журнал работы сохраняется в: ``elyos-installer.log``.

---

## Разработка

* **Архитектура, концепция и реализация:** ARDamir384
* **AI Co-Pilot & Парное программирование:** Проект разработан при участии Google Gemini (генерация заготовок кода, верификация POSIX-вызовов, архитектурный аудит и техническая документация).

## Лицензия

Исходный код распространяется под лицензией **Mozilla Public License 2.0 (MPL-2.0)**.  
Подробности см. в файле [LICENSE](LICENSE).

Copyright (c) 2026 ARDamir384. Licensed under the MPL-2.0.

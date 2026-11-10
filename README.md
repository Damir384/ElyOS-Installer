# ElyOS Installer

[![Arch Linux CI Build](https://github.com/Damir384/ElyOS-Installer/actions/workflows/ci.yml/badge.svg)](https://github.com/Damir384/ElyOS-Installer/actions/workflows/ci.yml)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Arch Linux Native](https://img.shields.io/badge/Target-Arch_Linux-blue?logo=arch-linux)](https://archlinux.org/)

[English](README.md) | [Русский](README.ru.md)

> **"Autonomy is not isolation, but independence."**  
> *"Automate what you want. Control what you need."*

A modular, lightweight C++ system installer designed to install standard **Arch Linux** with an optional **ElyOS layer**. Built without runtime bloat (no Python, no Electron) using a fast terminal interface (TUI).

---

## What is ElyOS?

ElyOS is **not an isolated fork** of Arch Linux:
1. **Clean Arch Base:** The installer deploys an official, unmodified Arch Linux system directly from upstream mirrors.
2. **Optional ElyOS Layer:** Users can optionally install additional tools, configurations, and desktop styles developed for the ElyOS project.

---

## What the Installer Does

- **Fast & Lightweight:** Starts instantly and runs directly in the terminal without requiring a graphical desktop environment.
- **Safe Step-by-Step Flow:** Built on a state machine that controls the installation stages, prevents accidental skips, and allows going back to previous steps.
- **Clear Disk Selection:** Inspects drives via ``lsblk`` JSON parsing, automatically filtering out system RAM disks (zram, loop devices).
- **Predictable System Calls:** Runs installation commands with forced ``LANG=C`` to prevent errors caused by different system languages.
- **Detailed Logging:** Automatically records all installation events and command outputs to ``elyos-installer.log``.

---

## Tech Stack

* **Language:** C++17 / C++20
* **Build System:** CMake (>= 3.20) + Ninja
* **Interface:** FTXUI (Terminal UI)
* **Data Serialization:** nlohmann_json
* **Target System:** Arch Linux (UEFI/GPT, systemd, ``util-linux``)

---

## Build and Execution

### Dependencies (Arch Linux):
Run: ``sudo pacman -S base-devel cmake ninja git nlohmann-json``

### Build:
Run: ``cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel $(nproc)``

### Run:
Root privileges are required to inspect and partition storage devices:
- Standard Interactive Mode: ``sudo ./build/bin/ElyOS_Installer``
- Debug Verbose Mode: ``sudo ./build/bin/ElyOS_Installer --debug``

Logs are written to: ``elyos-installer.log``.

---

## Development

* **Architecture, Design & Implementation:** ARDamir384
* **AI Co-Pilot & Pair Programming:** Developed in collaboration with Google Gemini (boilerplate generation, POSIX verification, architectural audits, and technical documentation).

## License

This project is licensed under the **Mozilla Public License 2.0 (MPL-2.0)**.  
See the [LICENSE](LICENSE) file for complete details.

Copyright (c) 2026 ARDamir384. Licensed under the MPL-2.0.

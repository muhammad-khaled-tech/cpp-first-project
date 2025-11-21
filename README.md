# Cross-Platform C++ TUI Menu System

## 📖 Description

This is a professional, cross-platform Text-based User Interface (TUI) application written in C++. It demonstrates advanced terminal control, a modular architecture based on **SOLID principles**, and a hardware abstraction layer to run natively on both **Linux** and **Windows** from a single C++ codebase.

The application starts with an animated loading screen, then launches a fully interactive menu that can be navigated using the keyboard (Up, Down, Home, End, Enter, Esc).

## ✨ Features

  * **Cross-Platform:** Natively compiles and runs on both Linux (using `termios`) and Windows (using `windows.h`).
  * **SOLID Architecture:** Code is decoupled into distinct "layers" (UI, Input, Terminal Control) following the **Single Responsibility Principle (SRP)**.
  * **Interactive TUI:**
      * Full keyboard navigation (Up, Down, Home, End).
      * Selection highlighting.
      * `Esc` and `q` keys for exiting.
  * **True Raw Mode:** Disables canonical mode (`ICANON`) and character echoing (`ECHO`) for full control over the terminal.
  * **Smart Input Handling:**
      * Solves the "Esc vs. Arrow Key" problem using a non-blocking, timed-read mechanism.
      * Handles different key codes from Linux (Escape sequences) and Windows (Scan codes).
  * **Clean Rendering:** Features a flicker-free menu and an animated loading bar by using smart cursor positioning (`CURSOR_HOME`) instead of `CLEAR_SCREEN` in the main loop.

-----

## 🛠️ How to Build

This project is divided into multiple `.cpp` files and must be compiled together.

### Prerequisites

  * **Linux:** A `g++` compiler (e.g., `build-essential`).
  * **Windows:** A `g++` compiler for Windows (we recommend **MinGW-w64**).

### Build with VS Code (Recommended)

This project is configured to build perfectly with VS Code's `tasks.json`.

1.  Make sure you have the C/C++ extension for VS Code.

2.  Create a folder named `.vscode` in your project's root.

3.  Inside `.vscode`, create a file named `tasks.json` and paste the following:

    ```json
    {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "Build Project",
                "type": "shell",
                "command": "g++",
                "args": [
                    "-fdiagnostics-color=always",
                    "-g",
                    "main.cpp",
                    "terminal.cpp",
                    "ui.cpp",
                    "input.cpp",
                    "-o",
                    "my_program"
                ],
                "group": {
                    "kind": "build",
                    "isDefault": true
                }
            }
        ]
    }
    ```

4.  Press **`Ctrl+Shift+B`** to build the project.

5.  Run the compiled program from your terminal:

      * On Linux: `./my_program`
      * On Windows: `.\my_program.exe`

### Manual Build Command

Open your terminal in the project folder and run:

```bash
# On Linux
g++ main.cpp terminal.cpp ui.cpp input.cpp -o my_program

# On Windows (with MinGW)
g++ main.cpp terminal.cpp ui.cpp input.cpp -o my_program.exe
```

-----

## 🏗️ Project Architecture (The "SOLID" Philosophy)

The core philosophy of this project is the **Single Responsibility Principle (SRP)** and **Dependency Inversion (DIP)**. The code is decoupled into "layers" (modules), where each layer has only one job and "hides" its complexity from the other layers.

### 1\. `main.cpp` (The Conductor 🧠)

  * **Responsibility:** Application Logic and State Management.
  * **Job:** Contains the main `while(true)` loop. It holds the "state" (e.g., `int selectedOption`). It acts as a "conductor" or "maestro," telling the other layers what to do.
  * **Hides:** It knows *nothing* about *how* to draw, *how* to read keys, or *how* to enable raw mode. It only speaks the "abstract" language from the `.hpp` files.

### 2\. `ui.hpp` / `ui.cpp` (The Artist 🎨)

  * **Responsibility:** All screen rendering and drawing.
  * **Job:** Knows how to draw the loading bar, the menu, and the action screens. It contains all `Ansi::` color codes and `move()` commands. It holds the `menuOptions` array, hiding the menu's content from `main`.
  * **Hides:** It knows *nothing* about keyboard input or `termios`.

### 3\. `terminal.hpp` / `terminal.cpp` (The Plumber 🔧)

  * **Responsibility:** Low-level, system-specific terminal hardware control.
  * **Job:** This is the core "plumbing." It's the only file that contains `#ifdef _WIN32`. It knows *how* to enable/disable raw mode on Linux (using `termios.h`) and on Windows (using `windows.h`). It also provides the `flushInputBuffer()` utility.
  * **Hides:** It hides the *entire* complexity of system calls (`tcsetattr`, `SetConsoleMode`) behind simple function names.

### 4\. `input.hpp` / `input.cpp` (The Translator 🗣️)

  * **Responsibility:** Low-level, system-specific input abstraction.
  * **Job:** This layer's job is to read raw, cryptic key codes from two different "languages" (Linux vs. Windows) and translate them into one universal "language" (our `enum KeyType`).
  * **Hides:** It hides the complexity of `read()`, `_getch()`, escape sequences, and scan codes. `main.cpp` just receives a simple `KEY_UP` or `KEY_HOME`.

### 5\. `ansi_terminal.hpp` (The Color Palette 🖌️)

  * **Responsibility:** A simple, static "dictionary" of ANSI escape codes.
  * **Job:** Provides clean, readable names for all colors and cursor commands.

This design means we can support a new operating system (like macOS) by **only** modifying `terminal.cpp` and `input.cpp`. The `main.cpp` and `ui.cpp` files would not need to change at all.

-----

## 💡 Core Concepts (The Deep Dive)

This is the "magic" that makes the project work.

### 1\. The Raw Mode "Plumbing" (`terminal.cpp`)

To read arrow keys, we must leave "Canonical Mode" (which waits for `Enter`) and enter "Non-Canonical (Raw) Mode".

  * **On Linux (`termios.h`):**

      * We call `tcgetattr()` to get the current settings.
      * We use bitwise "AND" (`&`) with an inverted "mask" (`~`) to surgically turn OFF the flags:
          * `ICANON`: Disables "line-buffering" (no longer waits for `Enter`).
          * `ECHO`: Disables "echoing" (keys pressed are not printed to the screen).
          * `ISIG`: Disables signals (so `Ctrl+C` doesn't quit the program).
      * We call `tcsetattr()` to apply these new settings.

  * **On Windows (`windows.h`):**

      * We get a "handle" (a remote control) to the console: `GetStdHandle()`.
      * We get the current settings: `GetConsoleMode()`.
      * We use the same bitwise `&= ~` logic to turn OFF the Windows-equivalent flags:
          * `ENABLE_LINE_INPUT` (the same as `ICANON`).
          * `ENABLE_ECHO_INPUT` (the same as `ECHO`).
      * We apply the settings: `SetConsoleMode()`.

### 2\. The Input "Translator" (`input.cpp`)

Reading keys in raw mode is different on each OS.

  * **On Linux (`read()`):**

      * We set `termios` to `VMIN = 0` and `VTIME = 1`. This is the secret. It tells `read()`: "Do not wait for a minimum number of characters. Wait only 0.1 seconds (1 decisecond) for a key. If nothing comes, return 0 (timeout)."
      * This is how we solve the **"Esc vs. Arrow Key"** puzzle:
        1.  `read()` gets `\033` (Esc).
        2.  We immediately call `read()` again.
        3.  If it returns `0` (timeout), the user *only* pressed `Esc`. We return `KEY_ESCAPE`.
        4.  If it returns `[` and then `A`, the user pressed the Up Arrow. We return `KEY_UP`.

  * **On Windows (`conio.h`):**

      * Windows uses a different "polling" philosophy with two functions:
      * `_kbhit()`: A *non-blocking* check. It instantly returns `true` if a key is waiting, `false` if not.
      * `_getch()`: A *blocking* read. It waits for and returns a key.
      * Our `readKey()` function *simulates* the Linux timeout logic:
        1.  It calls `_kbhit()`.
        2.  If `false`, we immediately return `KEY_UNKNOWN` (same as a Linux timeout).
        3.  If `true`, we call `_getch()` to get the key.

### 3\. The Key Code Mismatch (The `enum`)

This is *why* the `input` layer is so crucial. The "languages" are completely different. `main.cpp` can't understand them, so `input.cpp` translates them into our universal `enum KeyType`.

| Key | Linux Code (Raw Bytes) | Windows Code (Raw Bytes) | Universal `KeyType` |
| :--- | :--- | :--- | :--- |
| **Up Arrow** | `27`, `91`, `65` (`\033[A`) | `224`, `72` | `KEY_UP` |
| **Down Arrow** | `27`, `91`, `66` (`\033[B`) | `224`, `80` | `KEY_DOWN` |
| **Home** | `27`, `91`, `72` (`\033[H`) | `224`, `71` | `KEY_HOME` |
| **End** | `27`, `91`, `70` (`\033[F`) | `224`, `79` | `KEY_END` |
| **Enter** | `10` (`\n`) | `13` (`\r`) | `KEY_ENTER` |
| **Esc** | `27` (with read timeout) | `27` | `KEY_ESCAPE` |

### 4\. The "Brake" Problem (The `flushInputBuffer` fix)

  * **Problem:** When a user presses `Enter` to open a screen, their key-press might be "repeated" by the OS (Key Repeat) or (on Windows) send two characters (`\r\n`). This "leftover" character in the buffer is immediately read by our "wait" loop, causing the screen to close instantly.
  * **Solution (in `main.cpp`):**
    1.  **`flushInputBuffer();`**: After detecting `KEY_ENTER`, we first call this "pump" function. It uses `tcflush` (Linux) or `FlushConsoleInputBuffer` (Windows) to clear all "leftover" characters from the input buffer.
    2.  **`while(readKey() == KEY_UNKNOWN);`**: This is the "brake." Now that the buffer is clean, this loop will safely run (checking every 0.1s) until a *genuinely new* key is pressed by the user.# employee-system

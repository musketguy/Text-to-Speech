# C++ Text-to-Speech

A simple Windows C++ text-to-speech program using **Microsoft SAPI (Speech API)**.

The program allows the user to select a voice, enter or provide text, and have the computer speak it aloud. It also supports keyboard controls for starting and stopping speech.

## Features

* 🗣️ Text-to-speech using Microsoft SAPI
* 🎙️ Select between installed Windows voices
* ⚡ Asynchronous speech
* ⌨️ Keyboard-controlled interface
* 🛑 Stop speech while it is playing
* 🔄 Replay text without restarting the program
* 🚫 Invalid keyboard inputs are ignored
* 🪟 Built for Windows

## Requirements

* Windows
* C++
* Visual Studio
* Microsoft Speech API (SAPI)
* Windows SDK

SAPI is included with Windows, so no separate SAPI installation is normally required.

## Controls

| Key        | Action                  |
| ---------- | ----------------------- |
| `S`        | Speak the selected text |
| `B`        | Go back / stop speech   |
| Other keys | Ignored                 |

## How It Works

The program uses Microsoft's `ISpVoice` interface to control speech.

Speech is started asynchronously:

```cpp
voice->Speak(text.c_str(), SPF_ASYNC, nullptr);
```

Using `SPF_ASYNC` allows the program to continue processing keyboard input while the voice is speaking.

The console keyboard is monitored using:

```cpp
_kbhit()
```

and individual keys are retrieved with:

```cpp
_getch()
```

This allows the program to detect a key press without requiring the user to press Enter.

## Building

Open the project in Visual Studio and build it using the desired configuration.

For example:

```text
Build → Build Solution
```

The resulting executable will be placed in the project's build output directory.

## Project Structure

```text
CppTextToSpeech/
│
├── main.cpp
├── README.md
├── .gitignore
│
└── ...
```

## SAPI

This project uses the Windows Speech API.

The main interface used for speech synthesis is:

```cpp
ISpVoice
```

A typical speech call looks like:

```cpp
voice->Speak(
    text.c_str(),
    SPF_ASYNC,
    nullptr
);
```

To stop the current speech:

```cpp
voice->Speak(
    nullptr,
    SPF_PURGEBEFORESPEAK,
    nullptr
);
```

## Notes

This project is currently intended for **Windows** because it relies on Microsoft SAPI and Windows-specific console functions such as `_getch()` and `_kbhit()`.

## License

This project is open source. Feel free to modify and experiment with it.

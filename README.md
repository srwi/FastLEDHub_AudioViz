# FastLEDHub-AudioViz

![Build](https://github.com/stnkl/FastLEDHub_AudioViz/actions/workflows/build.yml/badge.svg)
[![MIT license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/stnkl/FastLEDHub_AudioViz/blob/master/LICENSE)

FastLEDHub-AudioViz is a command line tool that can be used to send spectrum data from a Windows audio device to an ESP32 or ESP8266 running [FastLEDHub](https://github.com/stnkl/FastLEDHub). The transmitted data consists of 16 bins corresponding to different frequency ranges in the audio spectrum. It can send data to multiple devices via a websocket connection. More information can be found in the [FastLEDHub](https://github.com/stnkl/FastLEDHub) documentation.

## Example usage

Transmit spectrum to a single device:

```console
$ FastLEDHubAudioViz 192.168.0.42
```

Transmit spectrum to multiple devices and visualize in the terminal window:

```console
$ FastLEDHubAudioViz 192.168.4.2 192.168.13.37 --visualize
```

## Help

```console
$ FastLEDHubAudioViz --help
FastLEDHub-AudioViz
Usage: FastLEDHubAudioViz [OPTIONS] devices...

Positionals:
  devices TEXT:IPV4 ... REQUIRED    List of target ip addresses

Options:
  -h,--help                         Print this help message and exit
  -v,--visualize                    Visualize spectrum in terminal
  -m,--microphone                   Use microphone to capture audio (default: system audio)
  -p,--period FLOAT:POSITIVE        Data transmission period (default: 10 [ms])
  -b,--beta FLOAT:FLOAT in [0 - 1]  Low pass filter beta coefficient (default: 0.25)
```

## Build

Currently only Windows x64 is supported:

### Requirements

- Visual Studio
- [VCPKG](https://vcpkg.io/en/getting-started.html)
- [CMake](https://cmake.org/) ≥ 3.20

### Building

1. Define the root directory of VCPKG as environmental variable `VCPKG_ROOT`.
2. Install required packages

      `vcpkg install cli11:x64-windows ixwebsocket:x64-windows ftxui:x64-windows`
3. Clone repository

      `git clone https://github.com/stnkl/FastLEDHub_AudioViz.git`
4. Generate build files

      `cmake FastLEDHub_AudioViz`
5. Build using Visual Studio
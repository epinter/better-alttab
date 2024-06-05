# ***Better AltTab for Skyrim SE***
[![C++20](https://img.shields.io/static/v1?label=standard&message=C%2B%2B20&color=blue&logo=c%2B%2B&&logoColor=white&style=flat)](https://en.cppreference.com/w/cpp/compiler_support)
[![Latest Release](https://img.shields.io/github/release/epinter/better-alttab.svg)](https://github.com/epinter/better-alttab/releases/latest)
[![Downloads](https://img.shields.io/github/downloads/epinter/better-alttab/total.svg)](https://github.com/epinter/better-alttab/releases/latest)
[![Release Date](https://img.shields.io/github/release-date/epinter/better-alttab.svg)](https://github.com/epinter/better-alttab/releases/latest)
[![License](https://img.shields.io/github/license/epinter/better-alttab.svg)](https://github.com/epinter/better-alttab/blob/main/LICENSE)
[![Site](https://img.shields.io/static/v1?label=site&message=NexusMods&color=blue)](https://www.nexusmods.com/skyrimspecialedition/mods/121342)

A simple mod to mute audio when game is running background, and show cursor when mouse is over the windowed game.

## ***Runtime requirements***

- [Skyrim Script Extender (SKSE)](https://skse.silverlock.org/)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
- [Latest VC++ Redist](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist)

## ***Build requirements***

- [CMake](https://cmake.org/)
- [vcpkg](https://vcpkg.io/en/)
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/)
- [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)

## ***Configuration***
The BetterAltTab.ini has the following options:

~~~
[General]
Debug = false
CursorVisibility = true
AudioToggle = true
~~~

- CursorVisibility: Shows the cursor when mouse is over game window in background (with borderless fullscreen, when alt+tab is pressed). Set to false to disable the feature. This feature is automatically disabled in fullscreen mode.
- AudioToggle: This feature stops the audio when game loses focus, and starts again when game gains focus. Set to false to disable the feature.
- Debug: logs debug messages

## ***Building***

In `Developer PowerShell for VS`, run:

~~~
git clone https://github.com/epinter/better-alttab.git
cd better-alttab
~~~

then

~~~
.\cmake\build.ps1
~~~

or

~~~
.\cmake\build.ps1 -buildPreset default
~~~

or

~~~
.\cmake\build.ps1 -buildPreset debug -configPreset default -toolset v143
~~~

Then get the .dll in build/bin/Release, or the .zip (ready to install using mod manager) in build.
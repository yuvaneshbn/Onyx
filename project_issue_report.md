# Onyx Project Issue Report

## Executive summary
This workspace is a Qt 6 voice / video / chat / file-transfer application with Windows-specific build assumptions. The current build evidence shows that the user-visible project has progressed past the original compatibility-model event constant failure, but the active Visual Studio/MSVC build is still prevented by a compiler environment and include-root mismatch.

The current strongest failure surface is not around the app logic. The compiler is executing a Qt/MSVC compile command line but failing to resolve standard headers and Windows SDK headers:

- `fatal error C1083: Cannot open include file: 'winsock2.h': No such file or directory`
- `fatal error C1083: Cannot open include file: 'type_traits': No such file or directory`
- `fatal error C1083: Cannot open include file: 'utility': No such file or directory`

Those errors line up with the active compile commands emitted under the MSVC toolchain.

## Build configuration facts
The build cache is mixed and inconsistent:

- MSVC CMake cache points to a `cl.exe` compiler and a Qt 6.11.1 MSVC package path.
- Another build tree still points to MinGW `c++.exe` / `gcc.exe` and lacks a coherent single-compiler contract.
- CMake is driven through Ninja, but the build is using the MSVC host compiler from the VS Build Tools installation.

This means the project’s active binary generation is reaching the compiler, but the compiler cannot see the necessary header roots.

## Source findings

1. Compatibility event support
   - The fallback code in [lib/compat/pj_compat.h](lib/compat/pj_compat.h) now supplies the missing `PJMEDIA_EVENT_FMT_CHANGED`-style compatibility surface for the LAN-only build mode.
   - This was a real source-level compatibility gap in the old build flow.

2. Crash-report formatting
   - [source/src/onyx.cpp](source/src/onyx.cpp) had a `QString::arg` pattern that was passed a DWORD / DWORD / DWORD / QString mixed argument list.
   - That was a real API misuse in the project and has been corrected to build through string conversion components.

3. Crash-dump Win32 API surface
   - The code still relies on a Win32 call surface and the runtime library curve (`GetVersion`, `MiniDumpWriteDump`, WinInet, and similar APIs) that belongs to the older Windows architecture.
   - These are accepted as project-level compile-time dependencies, not as evidence that the project is not Qt-based.

## Current confirmed build blocker
The latest verified evidence is a build break at include discovery rather than app-level logic.

The following are confirmed as reduced, but not yet solved, because evidence shows the live compiler cannot locate the proper include roots:

- `winsock2.h`
- `type_traits`
- `utility`

Those headers are not being discovered from the compiler front end. This points to a broken or incomplete Visual Studio Build Tools installation, conflicting local compiler environment activation, or a corrupted CMake cache that still reserves incompatible paths.

## Analyzer output inventory
The project already contains analysis artifacts:

- [unused_functions_only.txt](unused_functions_only.txt)
- [unused_report.txt](unused_report.txt)

They list functions such as `hid_open`, JSONCpp helpers, and several `global.cpp` wrappers as static-analysis “unused” items. The list should be treated as an advisory inventory rather than a binary correctness verdict.

## Recommended next repair order

1. Repair the Visual Studio/MSVC environment or reinstall the Windows SDK / Build Tools support.
2. Recreate the MSVC build directory from a single, explicit compiler selection.
3. Re-run the build only after the CRT/STL/SDK headers are consistent.
4. After build stability, do a selective dead-code cleanup over the analyzer output so true support code is not removed accidentally.

## Status
The app logic has at least one real source correction already made, but the project is not build-verified end-to-end because the active build is still blocked in the compiler environment.

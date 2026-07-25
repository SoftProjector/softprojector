# Building SoftProjector on Linux

These instructions cover building the Qt6 version of SoftProjector on Linux.
They were verified on Fedora 44 (KDE) with Qt 6.11.1, but the same steps apply
to any distribution with Qt 6 packages — only the package-manager commands
differ.

## Prerequisites

SoftProjector needs Qt 6 (core, gui, widgets, network, sql, qml/quick,
printsupport, and — importantly — multimedia and multimediawidgets), plus a
C++17 compiler and `qmake`.

### Fedora / RHEL

```sh
sudo dnf install gcc-c++ make \
    qt6-qtbase-devel \
    qt6-qtdeclarative-devel \
    qt6-qtmultimedia-devel \
    qt6-qttools-devel
```

`qt6-qtmultimedia-devel` is easy to miss and is required — without it `qmake`
fails with `Unknown module(s) in QT: multimedia multimediawidgets`.

### Debian / Ubuntu

```sh
sudo apt install build-essential \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-multimedia-dev \
    qt6-tools-dev
```

## Build

Build out-of-tree from the `src` directory:

```sh
mkdir -p build && cd build
qmake6 ../src/softProjector.pro
make -j"$(nproc)"
```

On some distributions the Qt 6 qmake is called `qmake` rather than `qmake6`.

The resulting binary is written to `src/unix_build/bin/SoftProjector`.

## Run

```sh
./src/unix_build/bin/SoftProjector
```

On first launch the application creates its SQLite database (`spData.sqlite`)
next to the binary.

## Media playback

Qt 6's multimedia stack uses FFmpeg rather than platform-specific codec
backends, which is what makes video and audio playback portable across Linux.
Hardware-accelerated decoding uses VA-API/VDPAU where available; missing
acceleration backends fall back to software decoding and are not fatal.

## Notes

- SoftProjector uses `qmake`, not CMake. The project file is
  [src/softProjector.pro](src/softProjector.pro).
- A copy of Qt's `QMediaPlaylist` (removed from Qt 6's multimedia module) is
  vendored under [3rdparty/](3rdparty/) and compiled into the build.

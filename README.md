# softProjector

Church media presentation software (songs, Bible, announcements, media, schedules).

## Building

Requirements: **Qt 6.4 or newer** (6.8 LTS recommended, and what CI uses), CMake ≥ 3.21,
a C++17 compiler. Qt modules: Core, Gui, Widgets, Network, Sql, Qml, Quick,
PrintSupport, Multimedia, MultimediaWidgets, plus LinguistTools (Qt Tools) for translations.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

On Windows use a "x64 Native Tools" prompt (or Qt Creator: open the top-level
`CMakeLists.txt`), then deploy the runtime:

```
windeployqt --release --qmldir src\qml build\src\SoftProjector.exe
```

The legacy qmake project (`src/softProjector.pro`) is kept in sync for IDE users.

### Checks without a Qt install

```bash
python tools/run_checks.py
```

Cross-checks `ui->` members against `.ui` files, signal/slot signatures,
declared-vs-defined methods, build lists, resources, XML and QML. CI runs this
before compiling.

### CI

`.github/workflows/windows-build.yml` runs the static checks, a Linux build
and a Windows (MSVC 2022) build. The Windows job uploads a ready-to-run,
windeployqt'd folder as the **SoftProjector-windows** artifact.

## Runtime notes

- **Data location:** `spData.sqlite` and `DarkTheme.ini` live in the per-user
  *local* app-data folder (`%LOCALAPPDATA%\SoftProjector\SoftProjector` on
  Windows). On first start, data from older versions (next to the exe, or in
  roaming AppData) is **copied** there automatically; the originals are left untouched.
- **Translations** are compiled into the executable. A `translations/` folder
  next to the exe containing `softpro_*.qm` overrides them (for translators).
- **Video:** Qt 6 Multimedia plays through its FFmpeg backend; `windeployqt`
  deploys it with the Multimedia module.
- **Help** opens https://softprojector.org/help/ in the browser. Authoring
  sources are under `docs/src/` (MkDocs); old offline HTML is archived in `docs/help-legacy/`.
- **Git LFS:** `.gitattributes` tracks PNG/ICO/QM binaries; run `git lfs install` on clone.

See `ANALYSIS.md` for the full list of bugs fixed and how they were verified,
and `CHANGELOG.md` for a summary.

License: GPLv3. See LICENSE.


## Runtime notes (Windows)

Qt 6 Multimedia uses an FFmpeg-based backend. The CI workflow runs
`windeployqt --multimedia` so the required plugins are bundled. If you deploy
manually, include the same flag or video playback may fail on machines without
system codecs.

App data (database, DarkTheme.ini) is stored under the platform app-data
location (`QStandardPaths::AppDataLocation`), not next to the executable.


## Documentation and assets

- **User help**: opens https://softprojector.org/help/ in the browser.
- **Authoring**: Markdown under `docs/src/`; build with MkDocs (`docs/mkdocs.yml`).
- **Legacy offline HTML**: archived in `docs/help-legacy/` (not required at runtime).
- **UI icons**: `src/icons/` (embedded via qrc — required for builds).
- **Git LFS**: `.gitattributes` tracks PNG/ICO/QM binaries; run `git lfs install` on clone.

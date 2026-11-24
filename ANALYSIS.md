# SoftProjector — source review and fix log

This file replaces the earlier modernization notes. Several items those notes
described as "fixed" did not compile or did not work at runtime; they are
corrected below. Every item lists the file it concerns.

## How this pass was verified (and its limits)

The review environment had **no Qt 6 SDK, no CMake and no network**, so the
code could not be compiled or run here. To compensate:

* `tools/check_ui.py` checks every `ui->member` against the `.ui` files.
* `tools/check_sigslot.py` checks string `SIGNAL()/SLOT()` connects,
  pointer-to-member connects (missing or overloaded members), and that declared
  methods/slots are defined and vice versa. Both checkers were mutation-tested
  (deliberately injected bugs were caught). On the original tree the signal/slot
  check reported exactly the three compile errors listed under B1; on this tree
  it reports 0 errors, 0 warnings.
* `tools/run_checks.py` runs both, plus build-list, resource, XML and QML checks.
  CI runs it before the compile jobs.
* The SQL, regex, hex and volume-rounding fixes were exercised with Python
  (`sqlite3` is the same engine the app uses); each test reproduced the old bug
  and passed with the new logic.
* All edited C++ files were tokenized to confirm balanced braces/parentheses.

**The first real compile is the CI workflow** (`.github/workflows/windows-build.yml`,
Linux + Windows jobs). Expect it to surface compiler warnings; if it surfaces
an error, it will be in a spot these static checks cannot model.

## A. Build breakers (the tree as uploaded could not build)

| # | Problem | Fix |
|---|---|---|
| A1 | `softprojector.cpp` connected `&ProjectorDisplayScreen::videoPositionChanged` / `videoDurationChanged` / `videoPlaybackStateChanged`, but each name was both a private slot `(int)` and a signal → overloaded name, template deduction fails (compile error ×3). | Slots renamed `onQmlPositionChanged` etc.; signals keep their names. |
| A2 | `3rdparty/sources/*.cpp` included `"../include/qmediaplaylist.h"`; the folder is `3rdparty/headers/`. | Vendored `QMediaPlaylist` removed entirely (see B1). |
| A3 | `qt_add_translations()` used without `find_package(Qt6 COMPONENTS LinguistTools)` → "Unknown CMake command". | LinguistTools added; minimum Qt 6.4 declared. |
| A4 | CI used Ninja on `windows-latest` without the MSVC developer environment → no `cl.exe` (or a stray MinGW). | `ilammy/msvc-dev-cmd`; Qt 6.8 / MSVC 2022; Linux compile job; static-check job. |
| A5 | CI copied `.qm` files to the exe folder, but the app looked in `translations/`; flag icons were never deployed → only English in the language menu. | `.qm` compiled from `.ts` at build time and embedded with the flags under `:/translations` (CMake and qmake). |

## B. Runtime bugs fixed

**Video / audio (`qml/DisplayArea.qml`, `projectordisplayscreen.*`, `softprojector.cpp`)**
1. `MediaPlayer` had no `videoOutput` and no `audioOutput` → Qt 6 showed no video and played no sound on the projector. Both are now wired; volume/mute go through `AudioOutput`.
2. `player.volume`, `player.muted`, `player.seek()` do not exist in Qt 6 (threw on use) → `audioOut.volume/muted`, `player.setPosition()`.
3. Looping used `QMediaPlaylist::Loop` (= 3, "play three times") and `CurrentItemOnce` (= 0, ignored by Qt 6 so the old count stuck) → `-1` (infinite) / `1` (once). This was the only use of the 1,300-line vendored playlist.
4. `setVideoMuted()` invoked `setVideoVolume(!muted)` → unmuting jumped to 100 %. Now invokes `setVideoMuted`.
5. `renderVideo()` reset volume to full, un-muting secondary screens that `showVideo()` had just muted → every screen played audio (echo). Volume is no longer touched by `renderVideo()`.
6. `playbackStopped()` was declared in QML but never emitted; now emitted at end of media only (not on every stop, which would blank the display when switching items).
7. `tranType`/`backType` were uninitialized; QML lookups are now null-checked and QML load errors are logged.

**Freeze / crash**
8. `MediaWidget::goLiveFromSchedule()` slept in a loop on the GUI thread until a flag set by a slot that needs that same thread → permanent hang. Now deferred until `LoadedMedia`.
9. `SoftProjector::setChapterList()` called `first()` on a possibly empty selection; `MediaWidget::getMedia()` called `at(-1)`.
10. Module-list/songbook XML loops never checked `atEnd()` → a truncated download hung the app. `xml.StartElement &&` (always true) → `isStartElement()`.
11. `downloadModList()` called `deleteLater()` on a null/stale reply on file-open errors; download speed divided by a possibly-zero elapsed time.
12. `Settings::loadSettings()` read `set.at(1)` for lines without `=` (out of range) → `QString::section()`.

**Data correctness**
13. Schedule save built `INSERT … VALUES('%1','%2')`; titles with an apostrophe failed and the item's content was attached to the *previous* row's id. Now bound parameters + `lastInsertId()`, inside a transaction, with an error message if the file can't be opened.
14. Removing all items from a saved schedule and saving deleted nothing (`toDelete` started `false`). Fixed; table names from the file are whitelisted.
15. Media library: paths reloaded with `QUrl("C:/…")` (parsed as scheme `c:`) → saved media unplayable on Windows after restart; INSERT/DELETE built from strings; DELETE by file name removed same-named files in other folders.
16. Splitter/window layout never restored: saved with `toHex()`, loaded with `toUtf8()` → now `fromHex()`.
17. `saveNewSettings()` used `INSERT OR REPLACE` on a table without a unique key → duplicated all rows when one was missing.
18. Songbook-name lookups (`editwidget.cpp`, `song.cpp`) and settings updates now use bound parameters.
19. Renamed module downloads lost their extension (Qt 6 regex port matched the already-stripped name); legacy `@$` separator regex had an unescaped `$`.

**Upgrade path (`main.cpp`)**
20. The previous pass moved the database from the exe folder to roaming AppData **without migrating it** → upgrading users saw an empty database. The DB now lives in *local* AppData (large file; SQLite WAL is unsafe on redirected/network roaming folders) and is copied — never moved — on first run from the roaming folder or the exe folder, together with `DarkTheme.ini`.

**UI**
21. Runtime language switch labelled the Media tab "Announcements (F8)" (indices off by one).
22. "Always on top" could not be turned off and `setWindowFlags()` wiped other flags → `setWindowFlag(…, on)`.
23. Volume slider: now also reacts to keyboard/wheel/groove clicks; `qRound` fixes a float truncation on two values (53, 59); the position slider no longer jumps while being dragged.
24. Closing with "Save" then cancelling "Save As" quit anyway and lost the schedule.
25. Qt's own translations (`qtbase_*.qm`) are loaded, so standard buttons/dialogs are translated; `QTranslator::load()` results are checked.

**Performance**: "all words" Bible/song searches recompiled a regex per verse (~31,000 per word); now compiled once per search.

## C. Removed
`3rdparty/` (vendored QMediaPlaylist), dead `displayscreen.{cpp,hpp,ui}` and
`mpsettings.ui` (not in any build), committed build output `src/Makefile` and
`src/.qmake.stash`.

## D. Corrections to the earlier notes
* The C++ standard is **17** (Qt 6's requirement). The notes said 17 while CMake said 20.
* "Qt 6 volume/mute via `QAudioOutput`" was applied to the operator preview only, not to the projector (QML) — see B1–B2.
* The `PlaybackState` pointer-to-member connects described as converted did not compile — A1.
* `QMediaPlaylist` was not needed at all — B3.

## F. Performance, memory and modernization pass

Verified the same way as above (static checks, tokenizer balance, and
Python/sqlite3 benchmarks); still not compiled in this environment.

**Memory leaks fixed**
| Where | Leak | Fix |
|---|---|---|
| `SongCounter` | `SongCounterModel` never deleted; a new `QSortFilterProxyModel` allocated on every `loadCounts()` | model parented to the dialog; proxy created once |
| `PrintPreviewDialog` | `QPrinter` never deleted; a heap `QPrintDialog` per print | printer deleted in destructor; dialog on the stack |
| `EditWidget::on_pushButtonPrint_clicked` | a whole `PrintPreviewDialog` (+ printer) allocated per click, freed only when the app closed | stack object |
| `SoftProjector` | About / Song counter / Print / Slide show editor dialogs were `new` + `delete` (leak on any early return, not exception-safe) | stack objects; slide show editor now parented (centred on the main window) |

Checked and **not** leaking (no change needed): the blur effect in
`ImageGenerator::applyEffectToImage` (owned by the stack `QGraphicsPixmapItem`),
the image provider (owned by the QML engine), the QQuickView (tracked by the
window container), widgets handed to layouts/tab widgets, and all `ui` objects.

**Memory use**
* Song backgrounds: `SongDatabase::getSongs()` decoded every song's background
  image at startup (~8 MB per 1080p image). Songs now keep the encoded bytes
  and decode on first use (`Song::decodeBackground()`, called from
  `SongsModel::getSong()`, `getSettings()` and `getStanza()`). Schedule files
  store the original bytes when not yet decoded (no lossy re-encode).
* Slide shows: every full-size slide was decoded when a show was opened
  (~3.7 MB per 1280 px slide). Slides are decoded on demand and only the current
  slide and its neighbours stay decoded, so memory is flat regardless of show
  length and the next/previous slide is still instant. All save paths store the
  original encoded bytes.

**Speed**
* Database indexes (`ensureIndexes()` in `main.cpp`, created once, idempotent)
  on `BibleVerse(verse_id, bible_id)`, `BibleVerse(bible_id)`,
  `BibleBooks(bible_id, id)`, `Songs(songbook_id, number)`,
  `Slides(ss_id, p_order)`. Every displayed verse used to be a full table scan,
  repeated per Bible version and per screen. Benchmark on a 94,050-row, 3-Bible
  database with the app's own query shape: **9.6 ms → 0.03 ms per verse click
  (~300×)**; `EXPLAIN QUERY PLAN` changes from `SCAN BibleVerse` to
  `SEARCH … USING INDEX`.
* `PRAGMA optimize` on exit keeps SQLite's planner statistics current.
* Song-number lookup no longer copies every `Song` per keystroke.

**Bugs found in this pass**
* Opening an empty slide show added one blank slide (`do/while` on an invalid record).
* Slide ordering dropped any slide after a gap in `p_order`; now `std::stable_sort`.
* `SongDatabase::getSongs()` crashed at startup (`at(-1)`) on a song whose songbook was deleted.
* `SpImageProvider::requestPixmap()` did not report the image size (required by the API).
* `applyEffectToImage()` returned a pixmap while a `QPainter` was still active on it.

**Modernization**
* All 27 non-QML `SIGNAL()/SLOT()` connects are now pointer-to-member connects
  (compile-time checked; `qOverload<>` for the overloaded `addToShcedule`).
  The 7 remaining string connects bind to signals declared in QML, which have
  no C++ member to point to.
* `tools/check_sigslot.py` now also rejects pointer-to-member connects to
  non-public members of another class (string connects never needed access).
* 31 `parent = 0` defaults → `nullptr`; `override` / `const&` on the image provider.

## G. Version 3.0 changes

**Version bump:** 2.2 → 3.0 in `softprojector.cpp`.

**Crash fix — Settings dialog with no Bibles installed.**
The backtrace: `updateSecondaryBibleMenu()` ← `loadBibleVersions()` ←
`SettingsDialog::loadSettings()` ← `SoftProjector::on_actionSettings_triggered()`.

Root cause: when no Bibles are in the database, every primary-Bible combo box is
empty and `currentIndex()` returns -1. Nine functions then called
`QList::removeAt(-1)`, which is undefined behaviour and segfaulted:

* `updateSecondaryBibleMenu()` / 2 / 3 / 4
* `updateTrinaryBibleMenu()` / 2 / 3 / 4
* `updateOperatorBibleMenu()`

Fix: `loadBibleVersions()` returns early when `bibles.isEmpty()` (no combo boxes
to set up). Every `removeAt()` call is also individually guarded with a bounds
check, so even if `loadBibleVersions()` is refactored later, no combination of
empty or mismatched lists can crash.

**About dialog:** David Demchenkov added to developers; title fixed from
"About softProjecor" to "About SoftProjector"; subtitle typo "open souce" →
"open source".

## E. Recommended next steps
1. Push and let CI compile. Fix any residual warnings with `-Werror` in a branch.
2. Convert the remaining string-built SQL (all remaining cases interpolate internal integer ids, so they are not exploitable, but bound parameters are cheaper to review).
3. Convert the 39 `foreach` loops to range-for when touching those files, then define `QT_NO_FOREACH`. Not done mechanically: `foreach` iterates a copy, so loops whose body modifies the container (directly or via a called method) would become undefined behaviour — this needs a compiler and tests.
7. Create display screens 2–4 (`ProjectorDisplayScreen`, one `QQuickView` + QML engine + media player each) only when those screens are configured; today all four are created at startup.
8. When a secondary screen uses the primary screen's settings and has the same size, reuse the rendered text image instead of rendering it again per screen.
4. Add Qt Test unit tests for `Settings`, schedule save/load round-trip and song parsing.
5. Consider `QStyleHints::colorScheme()` (Qt ≥ 6.5) instead of reading the registry for dark mode, which would also make it cross-platform.
6. Sign the Windows build (Authenticode) to avoid SmartScreen warnings.

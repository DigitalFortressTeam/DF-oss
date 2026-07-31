# DF-main

> Competition firmware and design files for the **Digital Fortress mini sumo robot** — search, attack, and push the opponent out of the ring.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B11-informational.svg)
![Platform](https://img.shields.io/badge/platform-Arduino%20Mega%202560-00979D.svg)
![Framework](https://img.shields.io/badge/framework-PlatformIO-FF2D20.svg)

---

## Overview

`DF-main` is the working repository for DigitalFortressTeam's robotics competition projects, with the **mini sumo robot** as the primary focus. It also holds an older PID line follower and LEGO EV3 experiments.

The sumo bot is an autonomous fighting robot designed for mini sumo competitions: a 3-minute match on a 77 cm black ring where the first robot to push the other out wins.

**How it works:** an IR remote control (Panasonic protocol) selects one of five attack strategies (or a cleaning mode) before the match. Once started, the robot searches for the opponent with IR proximity sensors, attacks, and recovers from the ring edge using ground sensors.

### Core architecture

The firmware is built around three cooperating state machines:

```
┌─────────────────────────────┐
│  controlstate  (main.cpp)   │  Waiting → ACTIVE → STOP (match lifecycle,
│  IR remote handling         │  strategy selection, LED feedback)
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│  mainState    (main.cpp)    │  Advanced strategy phases:
│  search / attack / grounded │  first search → tornado search →
│  state machine              │  direct attack → slow rotate → recovery
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│  motion layer (functions.h) │  move/rotate primitives, exponential
│  PWM smoothing + differential│  PWM smoothing (MotionServerRun),
│  drive (128-centered)        │  millis()-based timing
└─────────────────────────────┘
```

- Sensors are read every loop (`readsesnors()`); state transitions react on the same tick.
- Motor output uses a **128-centered differential-drive PWM scheme**: `128` = stop, `>128` = forward, `<128` = reverse.
- The `MotionServerRun()` filter smoothly ramps PWM targets to reduce mechanical shock.

## Key Features

- **Remote strategy selection** — Panasonic IR remote picks one of 5 strategies or cleaning mode; LED blink feedback confirms the choice.
- **Remote start/stop** — start the match and stop the robot without touching it.
- **Multi-phase attack state machine** — first search, tornado (spinning) search, direct attack, slow rotation, and ring-edge recovery all handled in one loop.
- **Opponent detection** — 4 IR proximity sensors (front-left, front-right, left, right) with `INPUT_PULLUP` (active-low) wiring.
- **Ring-edge (ground) detection** — 4 ground sensors detect the white ring border so the robot corrects itself back into the ring.
- **Smooth motion control** — exponential filter on PWM targets prevents jerky starts and direction changes.
- **Non-blocking timing** — `millis()`-based state timers (except a few remaining `delay()` calls; see Code Quality section).
- **Clean PlatformIO project** — dependency resolution via `lib_deps` (IRremote), single-command build and upload.

## Prerequisites & Tech Stack

| Component | Requirement |
|-----------|-------------|
| OS | Linux, macOS, or Windows |
| PlatformIO Core | ≥ 6.x (standalone CLI or VS Code extension) |
| Board | Arduino Mega 2560 / Mega 2560 Pro (ATmega2560) |
| Framework | Arduino (`platformio.ini`: `atmelavr` / `megaatmega2560`) |
| Libraries | `z3t0/IRremote@^4.4.2` (resolved automatically by PlatformIO) |
| Sensors | 4× IR proximity sensors, 4× ground (ring-edge) sensors |
| Actuators | 2× DC motors + dual motor driver (PWM + enable pins) |
| Remote | IR remote using the Panasonic protocol |

**Repository-wide (sibling projects):**

| Project | Stack |
|---------|-------|
| `Line_tracker/` | Arduino (`Line_tracker.ino`), PID controller |
| `ev3 and vids/` | LEGO EV3, reference strategy videos |

## Installation & Setup

### 1. Clone the repository

```bash
git clone https://github.com/DigitalFortressTeam/DF-main.git
cd "DF-main/Sumo/2024-2025/Sumolatest"
```

### 2. Build the firmware

```bash
pio run
```

### 3. Flash to the board

Connect the Mega 2560 Pro over USB, then:

```bash
pio run --target upload
```

If the board is not auto-detected (e.g. multiple serial devices), specify the port:

```bash
pio run --target upload --upload-port [your-port]
# Linux example: /dev/ttyUSB0
# Windows example: COM3
```

> **Linux note:** if upload fails with a permission error, add your user to the `dialout` group (`sudo usermod -aG dialout [your-username]`) and log out/in.

### 4. (Optional) IDE setup

Install the [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) extension in VS Code. The project already recommends it in `.vscode/extensions.json`.

## Usage Guide

### Match procedure

1. Power on the robot — the serial console prints `TURTLE LOADING UP` (9600 baud).
2. Point the IR remote at the receiver and press a **strategy key** (see table). The built-in LED blinks once per selection.
3. Press **Start** (0x87) — the match begins.
4. Press **Stop** (0x89) at any time — motors halt and the robot returns to `Waiting`.

### IR remote commands

| Command | Action |
|---------|--------|
| `0x10` | Strategy 1 |
| `0x11` | Strategy 2 |
| `0x12` | Strategy 3 |
| `0x13` | Strategy 4 |
| `0x14` | Strategy 5 |
| `0x81` | Cleaning mode |
| `0x87` | Start match (LED holds if no strategy selected) |
| `0x89` | Stop match / reset to waiting state |

### Serial monitor

```bash
pio device monitor -b 9600
```

Prints sensor status on every loop — useful for tuning sensor thresholds.

### Pin map (firmware defaults, `src/pins.h`)

| Pin | Function | Pin | Function |
|-----|----------|-----|----------|
| 7 | Motor L enable | A7 | IR sensor, left |
| 9 | Motor R enable | 16 | IR sensor, right |
| 11 | Motor L PWM | 25 | IR sensor, front-right |
| 12 | Motor R PWM | A1 | IR sensor, front-left |
| 13 | Buzzer | 15 | Ground sensor, back-left |
| 10 | Ground sensor, front-left | A15 | Ground sensor, back-right |
| 14 | Ground sensor, front-right | — | — |

## Project Structure

```
DF-main/
├── LICENSE                              # MIT license
├── README.md
├── Line_tracker/
│   └── 2024/
│       └── Line_tracker.ino             # PID line-follower firmware (2024)
├── Sumo/
│   └── 2024-2025/
│       ├── components.xlsx              # Component list / BOM
│       ├── Sumo strategies.xlsx         # Strategy design notes
│       ├── sumo datasheets/             # Datasheets: sensors, motors, driver
│       ├── Flowchart/                   # Design flowcharts (images)
│       ├── PCB/                         # Altium schematics, PCB layout, Gerbers
│       └── Sumolatest/                  # Active PlatformIO firmware
│           ├── platformio.ini           # Build config: atmelavr, megaatmega2560,
│           │                            #   Arduino framework, IRremote dep
│           ├── .gitignore               # PlatformIO/IDE artifacts
│           ├── .vscode/                 # Editor recommendations
│           └── src/
│               ├── main.cpp             # Entry point: setup(), loop(), control
│               │                        #   state machine + advanced strategy states
│               ├── strategies.h         # simple() / smart() strategy logic
│               ├── functions.h          # Motion primitives, PWM smoothing
│               │                        #   (MotionServerRun), helpers
│               └── pins.h               # Pin map, sensor read variables, state
│                                        #   constants, timer variables
└── ev3 and vids/
    ├── Digitalfortresscode.ev3          # LEGO EV3 program
    └── *.mp4                            # Reference videos: sumo attack strategies
```

## Contributing & Credits

### Contributing (DigitalFortressTeam)

1. Create a feature branch (`git checkout -b feature/[your-feature]`) and open a PR to `main`.
2. Read the **Code Quality** section below before touching `Sumolatest/src/` — it lists known problems to avoid.
3. Keep every strategy as a named state and document its transitions.
4. Verify with `pio run` before pushing; don't merge build-breaking changes.
5. Don't commit large binaries (videos, PCBs, spreadsheets) unless necessary.

### Credits

- **Team:** DigitalFortressTeam — firmware, hardware (PCB), and strategy design.
- **Libraries:** [z3t0/IRremote](https://github.com/z3t0/Arduino-IRremote) (IR reception), PlatformIO build system.
- **License:** MIT — see [LICENSE](LICENSE).

## Code Quality — Sumo (read this first)

**The current Sumo code style is very bad and hard to read.** It works, but nobody should be expected to understand, debug, or extend it in this state. Before touching `Sumolatest/src/`, fix the issues below.

### Specific problems found

| Problem | Where | Example |
|---------|-------|---------|
| Typos in names | `functions.h`, `main.cpp` | `readsesnors()` (should be `readSensors`), `registerpins()`, `stratstates` |
| No consistent naming convention | `pins.h` | `motorLEN`, `motorL_PWM`, `GROUND_BL`, `IR_FR_READ` mixed with `robot_state`, `run1time` — pick one scheme and stick to it |
| Conflicting duplicate constants | `pins.h:24` vs `pins.h:101` | Two different `STOP` (4 and 8) that mean different things in different state machines |
| State constants as `#define` instead of `enum` | `pins.h:60-79, 98-118` | `Advanced_FirstSEARCh1l`, `Cleaner`, `Strat1`, `Waiting`… plain macros with no type safety |
| Magic numbers everywhere | `main.cpp` | `252`, `170`, `180`, `128`, `0.06`, `20`, IR commands `0x10`–`0x87` with no explanation of what any of them mean |
| Massive copy-paste in the state machine | `main.cpp` | The same sensor-transition blocks (`GROUND_FR_READ`, `IR_FR_READ == 0`…) are repeated dozens of times; extract them into functions |
| Blocking `delay()` calls | `functions.h`, `main.cpp` | `delay(1000)`/`delay(2000)` inside `starter()` and the `STOP` case freeze the control loop for the whole match |
| Dead / commented-out code | `main.cpp:57, 70, 86, 124, 393-398, 448-453`, `functions.h:190-212` | Large commented-out blocks and unused functions (`simple()`, `smart()`, `launcher()`) left in place |
| Unused variables | `pins.h`, `functions.h` | `pwm_prev`, `current_pwm`, `smoothedpwm`, `fullspeed`, `accelerating`, `use_strat1..5`, `flapsstate`, `appr_from_*`… |
| Implementation code inside headers | `functions.h`, `strategies.h` | Non-`inline` function definitions in headers — every include re-defines them |
| Bare register tweaking | `main.cpp:6-12` | `configTimer1()` writes `TCCR1B` bits with no comment on what frequency is being set |
| Debug prints left in the hot loop | `main.cpp:35` | `Serial.println(...)` every loop iteration with no debug guard |
| No documentation | whole project | No comments explaining strategy, states, or the 128-centered differential-drive PWM convention |

### Recommended fixes

1. **Fix all typos** and rename identifiers to one convention (`snake_case` or `camelCase`, one style per kind of thing).
2. **Replace `#define` constants with `enum class`** (or at least `enum`) for every state machine, and give the state machines distinct, non-overlapping values.
3. **Collapse the repeated transition logic** in `main.cpp` into named helper functions (e.g. `checkOpponent()`, `checkGround()`).
4. **Name your magic numbers**: `constexpr int MAX_PWM = 252;`, `constexpr float SMOOTHING_RATIO = 0.06;`, `enum class IRCommand : uint16_t { ... };`.
5. **Remove `delay()` calls** from anything time-critical; use the `millis()` pattern that already exists.
6. **Delete dead code and unused variables.** That's what version control is for.
7. **Move function definitions into `.cpp` files** (or mark them `inline`); keep declarations in headers.
8. **Wrap `Serial` debug output** in `#ifdef DEBUG` / a `DBG()` macro.
9. **Document the state machine** — one short diagram or a block comment per state would help enormously.

The hardware and strategy design here are solid. The code just needs a readability pass to match.

## License

MIT — see [LICENSE](LICENSE).

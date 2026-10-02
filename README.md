# DigitalFortressTeam · Robotics

> Open-source competition firmware, hardware, and programs for **DigitalFortressTeam's** two robotics projects: **Sumo** (autonomous fighting robot) and **Line Tracking** (PID + EV3 line followers).

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Arduino%20Mega%202560%20%7C%20LEGO%20EV3-00979D.svg)]()
[![Framework](https://img.shields.io/badge/framework-PlatformIO%20%7C%20EV3%20Lab-FF2D20.svg)]()

---

## 🏆 Competition Achievements

| Category | Competition | Award / Result | Code / System |
|----------|-------------|----------------|---------------|
| **Sumo** | **ARC9** (Annual Robotics Competition) | 🥈 **2nd Place** | [`Sumo/2024-2025/Sumolatest/`](Sumo/2024-2025/Sumolatest/) |
| **Sumo** | **ARC10** (Annual Robotics Competition) | 🏅 **Best Programming Rank** | [`Sumo/2024-2025/Sumolatest/`](Sumo/2024-2025/Sumolatest/) |
| **Line Tracking** | **MC2** (Mahdi Coding Competition) | 🥉 **3rd Place** | [`ev3/Digitalfortresscode.ev3`](ev3/Digitalfortresscode.ev3) |

---

## Table of Contents

- [Sumo](#sumo)
  - [Overview](#sumo--overview)
  - [How It Works](#sumo--how-it-works)
  - [Core Architecture](#sumo--core-architecture)
  - [Key Features](#sumo--key-features)
  - [Tech Stack](#sumo--tech-stack)
  - [Installation & Setup](#sumo--installation--setup)
  - [Usage Guide](#sumo--usage-guide)
  - [Pin Map](#sumo--pin-map)
  - [Achievements](#sumo--achievements)
  - [Code Quality](#sumo--code-quality)
- [Line Tracking](#line-tracking)
  - [Overview](#line-tracking--overview)
  - [How PID Line Following Works](#line-tracking--how-pid-line-following-works)
  - [Sharp Turns (Full-Turn Sensors)](#line-tracking--sharp-turns-full-turn-sensors)
  - [Arduino PID Follower](#line-tracking--arduino-pid-follower)
  - [LEGO EV3 Follower](#line-tracking--lego-ev3-follower)
  - [Achievements](#line-tracking--achievements)

---

# Sumo

## Sumo · Overview

The **Sumo** project is an autonomous fighting robot built for **sumo robot competitions**. Two robots face off on a black circular ring; the first to push the other out wins. A match runs in timed rounds, and the robot must find, attack, and defend itself fully autonomously — no human input once the match starts.

The firmware lives in [`Sumo/2024-2025/Sumolatest/`](Sumo/2024-2025/Sumolatest/), a clean PlatformIO project targeting the Arduino Mega 2560.

## Sumo · How It Works

Before the match, an **IR remote control** (Panasonic protocol) selects one of five attack strategies (or a cleaning mode); the built-in LED blinks to confirm the choice. Once the operator presses **Start**, the robot runs the full match on its own:

1. **Search** — sweep the ring looking for the opponent using IR proximity sensors.
2. **Attack** — once an opponent is detected, drive directly at it and push.
3. **Recover** — if the robot reaches the white ring border (detected by ground sensors), it backs up and turns back into the ring.

The operator can press **Stop** at any time to halt the robot instantly.

### High-level flow

```
Power on → Waiting (select strategy via IR) → START pressed
   → Search (spin/sweep for opponent)
      → Opponent found? → Attack (drive forward, push)
      → Edge detected?  → Recover (back up, rotate, re-search)
   → STOP pressed → halt → back to Waiting
```

## Sumo · Core Architecture

The firmware is built around three cooperating state machines:

```
┌──────────────────────────────┐
│  controlState  (main.cpp)    │  WAITING → ACTIVE → STOP (match lifecycle,
│  IR remote handling          │  strategy selection, LED feedback)
└──────────────┬───────────────┘
               │
┌──────────────▼───────────────┐
│  mainState    (main.cpp)     │  Advanced strategy phases:
│  search / attack / grounded  │  first search → tornado search →
│  state machine               │  direct attack → slow rotate → recovery
└──────────────┬───────────────┘
               │
┌──────────────▼───────────────┐
│  motion layer (functions.h)  │  move/rotate primitives, exponential
│  PWM smoothing + differential│  PWM smoothing (updateMotors),
│  drive (128-centered)        │  millis()-based timing
└──────────────────────────────┘
```

- Sensors are read every loop (`readSensors()`); state transitions react on the same tick.
- Motor output uses a **128-centered differential-drive PWM scheme**: `128` = stop, `>128` = forward, `<128` = reverse.
- The `updateMotors()` filter smoothly ramps PWM targets to reduce mechanical shock.

## Sumo · Key Features

- **Remote strategy selection** — Panasonic IR remote picks one of 5 strategies or cleaning mode; LED blink feedback confirms the choice.
- **Remote start/stop** — start the match and stop the robot without touching it.
- **Multi-phase attack state machine** — first search, tornado (spinning) search, direct attack, slow rotation, and ring-edge recovery all handled in one loop.
- **Opponent detection** — 4 IR proximity sensors (front-left, front-right, left, right) with `INPUT_PULLUP` (active-low) wiring.
- **Ring-edge (ground) detection** — 4 ground sensors detect the white ring border so the robot corrects itself back into the ring.
- **Smooth motion control** — exponential filter on PWM targets prevents jerky starts and direction changes.
- **Non-blocking timing** — `millis()`-based state timers (except a few remaining `delay()` calls; see Code Quality section).
- **Clean PlatformIO project** — dependency resolution via `lib_deps` (IRremote), single-command build and upload.

## Sumo · Tech Stack

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

## Sumo · Installation & Setup

### 1. Clone the repository

```bash
git clone https://github.com/DigitalFortressTeam/DF-oss.git
cd "DF-oss/Sumo/2024-2025/Sumolatest"
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

## Sumo · Usage Guide

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

## Sumo · Pin Map

Firmware defaults from `src/pins.h`:

| Pin | Function | Pin | Function |
|-----|----------|-----|----------|
| 7 | Motor L enable | A7 | IR sensor, left |
| 9 | Motor R enable | 16 | IR sensor, right |
| 11 | Motor L PWM | 25 | IR sensor, front-right |
| 12 | Motor R PWM | A1 | IR sensor, front-left |
| 13 | Buzzer | 15 | Ground sensor, back-left |
| 10 | Ground sensor, front-left | A15 | Ground sensor, back-right |
| 14 | Ground sensor, front-right | — | — |

## Sumo · Achievements

The **Sumo** firmware in this repository was used in competition by DigitalFortressTeam:

| Competition | Category | Result |
|-------------|----------|--------|
| **ARC9** — Annual Robotics Competition | Sumo | 🥈 **2nd place** |
| **ARC10** — Annual Robotics Competition | Sumo | 🏅 **Best programming rank** |

---

# Line Tracking

## Line Tracking · Overview

The **Line Tracking** project covers two line-follower robots on different platforms that share one goal: follow a contrasting line (typically white-on-black or black-on-white) as fast and smoothly as possible.

- **Arduino PID follower** — a custom robot using analog photo-reflector sensors and a PID controller ([`Line_tracker/2024/Line_tracker.ino`](Line_tracker/2024/Line_tracker.ino)).
- **LEGO EV3 follower** — a line follower programmed in LEGO EV3 Lab ([`ev3/Digitalfortresscode.ev3`](ev3/Digitalfortresscode.ev3)).

## Line Tracking · How PID Line Following Works

The Arduino follower uses a **PID controller** to keep the robot centered on the line. Two close-range analog sensors (left and right) read how much light they reflect — when the robot drifts off the line, the difference between the two readings becomes the **error** signal that the PID loop corrects.

### The PID components

The controller combines three terms, each tuned by a constant:

| Term | Symbol | What it does | This robot's constant |
|------|--------|--------------|------------------------|
| **Proportional** | `Kp` | Reacts to the *current* error — the bigger the drift, the harder it corrects. | `0.7` |
| **Integral** | `Ki` | Accumulates *past* error over time — eliminates small steady-state offset. | `0.001` |
| **Derivative** | `Kd` | Reacts to the *rate of change* of error — dampens oscillation so corrections don't overshoot. | `14` |

Each loop iteration:

1. Read both sensors → `error = rightSensor − leftSensor`.
2. Update the running sum → `integral += error`.
3. Compute the change → `derivative = error − lastError`.
4. Combine → `pid = (Kp × error) + (Ki × integral) + (Kd × derivative)`.
5. Map the PID output onto a speed difference, then drive the two motors in opposite directions by that amount:
   - `rightMotor = baseSpeed + correction`
   - `leftMotor  = baseSpeed − correction`
6. Save `lastError = error` for the next iteration, then short `delay(5)` to pace the loop.

When the robot is perfectly centered, `error ≈ 0` and both motors run at `baseSpeed` (255) — full speed straight. Any drift creates a non-zero `pid`, which steers the robot back toward the line automatically. `Kd = 14` is relatively high, which makes the follower aggressive at cancelling oscillation so it can take curves fast without weaving.

## Line Tracking · Sharp Turns (Full-Turn Sensors)

PID alone follows gentle curves but struggles at 90°+ turns where the line suddenly leaves the sensor range. Two extra **wide (full-turn) sensors** placed further apart detect when the line has drifted far to one side and trigger a hard turn:

- **`Full_Right_Turn()`** — if the right wide sensor detects the line, the robot stops the right motor and drives only the left motor, spinning hard left until the close sensors pick the line up again.
- **`Full_Left_Turn()`** — the mirror: drive only the right motor to spin hard right until the close sensors reacquire the line.

`delay(50)` gives the turn a small initial kick before the `while` loop holds the turn until the two close sensors are back over the line, at which point normal PID resumes.

## Line Tracking · Arduino PID Follower

| Component | Value |
|-----------|-------|
| Board | Arduino (analog + PWM pins) |
| Close sensors | `A1` (right), `A2` (left) — analog photo-reflectors |
| Full-turn sensors | pin `6` (right), pin `7` (left) — digital |
| Motors | pin `4` (right), pin `5` (left) — PWM via `analogWrite` |
| Base speed | 255 (max) |
| PID constants | `Kp = 0.7`, `Ki = 0.001`, `Kd = 14` |
| Loop pace | `delay(5)` per PID cycle |

Source: [`Line_tracker/2024/Line_tracker.ino`](Line_tracker/2024/Line_tracker.ino)

## Line Tracking · LEGO EV3 Follower

A line-following program written in LEGO's EV3 visual programming environment. It uses the EV3's color/light sensor and built-in motor controllers to track a line using EV3's own logic blocks rather than a hand-tuned PID loop. It served as the team's entry in the **Mahdi Coding Competition (MC2)**.

Source: [`ev3/Digitalfortresscode.ev3`](ev3/Digitalfortresscode.ev3)

## Line Tracking · Achievements

The **line tracking** programs in this repository were used in competition by DigitalFortressTeam:

| Program | Competition | Category | Result |
|---------|-------------|----------|--------|
| EV3 follower (`ev3/`) | **MC2** — Mahdi Coding Competition | Line tracking | 🥉 **3rd place** |

---

## Project Structure

```
DF-oss/
├── LICENSE                              # MIT license
├── README.md
├── Line_tracker/
│   └── 2024/
│       └── Line_tracker.ino            # Arduino PID line-follower firmware
├── ev3/
│   └── Digitalfortresscode.ev3         # LEGO EV3 line-following program
└── Sumo/
    └── 2024-2025/
        ├── components.xlsx              # Component list / BOM
        ├── Sumo strategies.xlsx         # Strategy design notes
        ├── sumo datasheets/             # Datasheets: sensors, motors, driver
        ├── Flowchart/                   # Design flowcharts (images)
        ├── PCB/                         # Altium schematics, PCB layout, Gerbers
        └── Sumolatest/                  # Active PlatformIO firmware
            ├── platformio.ini           # Build config: atmelavr, megaatmega2560,
            │                            #   Arduino framework, IRremote dep
            ├── .gitignore               # PlatformIO/IDE artifacts
            ├── .vscode/                 # Editor recommendations
            └── src/
                ├── main.cpp             # Entry point: setup(), loop(), control
                │                        #   state machine + advanced strategy states
                ├── strategies.h         # runSimpleStrategy() / runSmartStrategy() logic
                ├── functions.h          # Motion primitives, PWM smoothing
                │                        #   (updateMotors), helpers
                ├── pins.h               # Pin map, sensor read variables
                └── state.h              # State enums, strategy selection, timers,
                                        #   flags
```

## Contributing & Credits

### Contributing (DigitalFortressTeam)

1. Create a feature branch (`git checkout -b feature/[your-feature]`) and open a PR to `main`.
2. Read the **Code Quality** section above before touching `Sumolatest/src/` — it lists known problems to avoid.
3. Keep every strategy as a named state and document its transitions.
4. Verify with `pio run` before pushing; don't merge build-breaking changes.
5. Don't commit large binaries (videos, PCBs, spreadsheets) unless necessary.

### Credits

- **Team:** DigitalFortressTeam — firmware, hardware (PCB), and strategy design.
- **Libraries:** [z3t0/IRremote](https://github.com/z3t0/Arduino-IRremote) (IR reception), PlatformIO build system.
- **License:** MIT — see [LICENSE](LICENSE).

## Sumo · Code Quality (read this first)

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

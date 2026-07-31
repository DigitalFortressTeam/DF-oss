# DigitalFortressTeam / Main Work boys

Repository for the Digital Fortress robotics team's competition work: line tracking, mini sumo, and EV3 experiments.

## Repository structure

| Path | Contents |
|------|----------|
| `Line_tracker/` | Arduino PID line-follower firmware (`Line_tracker.ino`). |
| `Sumo/` | Mini sumo robot: source code, PCB design, datasheets, flowcharts, strategy notes. |
| `ev3 and vids/` | LEGO EV3 program plus videos of sumo strategies. |

## Line tracker

Arduino PID line follower for 2024. Sensor-based PID controller (Kp/Ki/Kd), differential motor outputs. Works standalone as a single `.ino`.

## Sumo

Mini sumo robot for the 2024-2025 season, built around an Arduino Mega 2650 Pro, developed in PlatformIO.

- **Firmware:** `Sumo/2024-2025/Sumolatest/` — PlatformIO project (`src/`, `platformio.ini`).
- **Control:** Panasonic-format IR remote picks a strategy; the robot then plays a search/attack state machine on the ring.
- **Hardware:** PCB Gerbers/schematics in `Sumo/2024-2025/PCB/`, component list and datasheets in `Sumo/2024-2025/`.
- **Design docs:** flowcharts and strategy spreadsheets under `Sumo/2024-2025/`.

> **Note on code quality:** The Sumo firmware currently has serious readability and maintainability problems. Read the section below before working on it.

## Code style — Sumo (read this first)

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

## Contributing

- Follow the recommended fixes above: readable names, no magic numbers, no dead code, no blocking delays.
- Keep every strategy as a named state and document transitions.
- Test firmware builds with `pio run` before committing.

## License

MIT — see [LICENSE](LICENSE).

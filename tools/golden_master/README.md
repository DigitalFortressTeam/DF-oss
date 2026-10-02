# Golden-master behaviour check

Proves that a refactor did not change what the robots do, without needing the hardware.

The Sumo firmware and the line tracker sketch are compiled on a PC against a mock
Arduino (`arduino_mock/`). The mock feeds them random but repeatable sensor values and
IR remote commands, and records every hardware call (pin reads and writes, `delay`,
`millis`, serial prints) plus the state machine values into a hash. If the code still
behaves exactly the same, every hash still matches `golden_hashes.txt`.

The Sumo hashes come from the original, pre-refactor code. The line tracker hashes were
re-recorded after its bug fixes (an intended behaviour change).

## Usage

```bash
tools/golden_master/run.sh            # check (needs only g++)
tools/golden_master/run.sh --update   # re-record after an INTENDED behaviour change
```

## What is exercised

| Program | Mode | What runs |
|---------|------|-----------|
| Sumo | `main` | `setup()` + 40,000 `loop()` iterations with random remote commands and sensors |
| Sumo | `simple` / `smart` | the older strategies, started from random robot states |
| Sumo | `prim` | motion primitives, motor filter, launcher, countdown and reset with random arguments |
| Line tracker | — | `setup()` + 100,000 `loop()` iterations with random sensor values |

Each mode runs 50 seeds. Sensor change rates vary per seed, so busy fights, calm phases
and timeouts are all reached.

## Limits

- It checks behaviour against the mock, not real timing or the real motor driver.
- The mock's `millis()` advances on every call, so adding or removing a `millis()` call
  shows up as a change even when it would not matter on the robot.
- `digitalRead()` only returns `HIGH` or `LOW` (as on the real board).

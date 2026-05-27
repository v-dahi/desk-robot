# Build journal

What I did, what broke, what fixed it. Chronological. New entry whenever I work on the project.

---

## 2026-05 — Single eye animation on ILI9341, hit screen tearing

Tried CircuitPython first on Adafruit Feather RP2040 + 2.4" TFT FeatherWing. Got static eye drawing working. Implemented `look_at()` with linear interpolation, then smoothstep easing.

**Problem:** noticeable screen tearing during motion, especially when moving left. Slow updates, "wave-like" jitter.

**What didn't work:**
- Shrinking iris radius (slight improvement, not enough)
- Removing highlights (no change)
- `auto_refresh` toggling (no change)
- Bumping SPI baudrate to 24MHz (no change)
- Switching to C++ on the Feather + Adafruit_GFX (different artifacts — white strips during motion)

**What I learned:** ILI9341 + small MCU has fundamental animation limits without DMA. Spent too long trying to optimize code when the issue was hardware-level.

---

## 2026-05 — Switched to ESP32 + GC9A01

New hardware: ESP32-WROOM, two GC9A01 round LCDs. Set up PlatformIO project with TFT_eSPI library, config baked into `platformio.ini` build flags.

Got single-eye animation working. Smooth, no tearing on stationary frames. But during motion, half the iris was sometimes missing — the screen captured mid-refresh in photos.

**The bug:** 16-bit sprite color depth was hitting the ESP32 heap ceiling. Sprite worked sometimes, failed quietly other times, animations were partial.

**The fix:** Changed `eye.setColorDepth(16)` to `eye.setColorDepth(8)`. Halved memory footprint (115KB → 57KB). Animation became smooth and reliable.

**What I learned:** Print `ESP.getFreeHeap()` and `ESP.getMaxAllocHeap()` when something feels off. Embedded systems aren't infinite. More precision isn't always better.

---

## 2026-05 — Two eyes on one ESP32

Shared SPI bus (MOSI, MISO, SCK, DC, RST) between both displays. Each display gets its own CS pin (GPIO 5 and GPIO 15). Custom `selectEye()` / `selectBoth()` helpers manually drive CS pins before drawing.

Both eyes now sync to the same animation. Independent control (left and right looking different directions) is the next step.

**What I learned:** SPI is designed for this — multiple devices share data lines, only chip select differentiates them. GPIO 15 is a "strapping pin" on ESP32 but fine as a regular output after boot.

---

## TEMPLATE for new entries

## YYYY-MM — short title

What I did:

What broke / what I tried:

What fixed it:

What I learned:

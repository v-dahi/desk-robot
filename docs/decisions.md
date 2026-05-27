# Decision log

Major architectural choices and why we made them. New entry whenever a meaningful decision happens.

Format: date, decision, why, what we rejected, implications.

---

## 2026-05 — Raspberry Pi 5 as the main compute

**Decision:** Pi 5 (8GB) as the brain.
**Why:** Headroom for local Whisper STT + LLM + audio processing. Plenty of community support for Pi-specific hardware (audio HATs, GPIO).
**Rejected:** Pi Zero 2W (too slow for STT), Jetson Nano (overkill, hot, expensive).

---

## 2026-05 — ESP32-WROOM as the eye driver

**Decision:** Dedicated MCU for the eyes, separate from the Pi.
**Why:** Eye animations need to stay smooth even when the Pi is busy with speech recognition or LLM calls. Industry-standard pattern for animatronics. Also, hands-on MCU experience is a personal learning goal.
**Rejected:**
- Pi-direct display drive (works but couples eye timing to Pi load — risky when Pi is busy)
- Feather RP2040 (smaller community for this use case, fewer working examples)

---

## 2026-05 — Switched from ILI9341 to GC9A01 displays

**Decision:** Round 1.28" GC9A01 LCDs instead of rectangular 2.4" ILI9341 TFTs.
**Why:**
- Round shape IS the eye — no need for 3D-printed eye holes to crop a rectangle
- GC9A01 chip is meaningfully faster for animation
- Better library support for animatronic eye projects (TFT_eSPI works well)
- Thinner profile behind the face (~10mm vs ~20mm with the FeatherWing stack)
**Rejected:** ILI9341 (older, slower, hit a wall with screen tearing during motion that we couldn't fix with code alone).
**Implications:** Eye hole in body design is ~33mm circle. Depth budget behind face is ~10-15mm. Boyfriend updated face CAD plan.

---

## 2026-05 — C++ on ESP32, Python on Pi

**Decision:** Two languages, one per device.
**Why:**
- C++ is faster than CircuitPython for tight animation loops. Verified empirically — earlier CircuitPython attempts were laggy compared to C++ on the same hardware.
- Python is where the Pi-side ecosystem is strongest (Whisper, openWakeWord, Anthropic SDK).
- Devices communicate over serial — language doesn't matter on the wire.
**Rejected:** CircuitPython on the MCU (too slow), C++ on the Pi (loses the Python ecosystem).

---

## 2026-05 — TFT_eSPI library with 8-bit sprite color depth

**Decision:** Use TFT_eSPI's sprite-based rendering at 8-bit color depth.
**Why:**
- 16-bit color depth caused memory pressure on ESP32-WROOM (115KB sprite + library overhead pushed close to heap limits)
- Animation was buggy at 16-bit; switching to 8-bit fixed it
- Two-color animation (black iris on white background) doesn't need 16-bit
**Rejected:**
- Adafruit_GFX with immediate-mode drawing (slower, lots of overdraw)
- 16-bit sprites (memory pressure)
- 1-bit sprites (TFT_eSPI's 1-bit mode is less mature)

---

## TEMPLATE for new entries

## YYYY-MM-DD — short title

**Decision:**
**Why:**
**Rejected:**
**Implications:**

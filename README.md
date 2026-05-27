# 🌳 Totoro

A desk companion robot inspired by *My Neighbor Totoro*. Built from scratch.

## The plan

- **Raspberry Pi 5** — the brain. Voice AI, emotions, behavior logic.
- **ESP32-WROOM** — the eye driver. Drives two round LCDs, animates expressions, listens to commands from the Pi.
- **2× GC9A01 round displays** — the eyes themselves (240×240, SPI).
- **Articulated ears, tail, and arms** — coming later.
- **Iconic Totoro sounds** — yawns, snores, the acorn dance tune.

The Pi will tell the ESP32 things like "be curious" or "look left." The ESP32 handles the actual pixel pushing so eye animations stay smooth even when the Pi is busy with speech recognition.

## Where this is right now

- [x] Single eye drawing and blinking on ESP32 + GC9A01
- [x] Smooth `look_at()` motion with smoothstep easing
- [x] Two displays driven from one ESP32 (shared SPI bus, separate CS pins)
- [ ] Independent eye control (left vs right looking differently)
- [ ] Pi ↔ ESP32 command protocol over serial
- [ ] Pi-side brain (voice, LLM, sounds)
- [ ] Articulated ears and tail
- [ ] Mechanical body
- [ ] Custom PCB

## Repo layout

```
firmware/eyes/    ESP32 code (PlatformIO project, C++)
brain/            Raspberry Pi code (Python, coming soon)
hardware/         Wiring diagrams and CAD (coming soon)
docs/             Notes and decision log
```

## Hardware

- Raspberry Pi 5 (8GB)
- ESP32-WROOM-32 DevKit
- 2× GC9A01 1.28" round LCDs (240×240, SPI)
- Breadboard, jumper wires, USB cables

## Wiring (ESP32 → GC9A01s)

Both screens share most pins. Only the CS pin is unique per screen.

| Signal | ESP32 GPIO | Notes |
|---|---|---|
| VCC | 3.3V | both screens |
| GND | GND | both screens |
| SCL | 18 | shared SPI clock |
| SDA | 23 | shared SPI data |
| DC | 2 | shared |
| RST | 4 | shared |
| CS (left eye) | 5 | unique |
| CS (right eye) | 15 | unique |

> ⚠️ GC9A01 is a 3.3V part. Do not connect VCC to 5V.

## Building the firmware

1. Open `firmware/eyes/` in VS Code with PlatformIO extension installed
2. Click Upload — PlatformIO handles compile + flash
3. Open the Serial Monitor at 115200 baud to see debug output

The TFT_eSPI display config is baked into `platformio.ini` as build flags, so no need to edit library files manually.

## Things I learned the hard way

- Screen tearing on small SPI displays is real, and clever partial-erase code can make it worse, not better
- `setColorDepth(8)` instead of 16 on TFT_eSPI sprites — fixed an animation problem nothing else did. Memory pressure, not pixel-pushing speed
- GC9A01 is meaningfully faster for animation than ILI9341, not just "different resolution"
- Start with one eye before adding the second, even if you have both displays in hand
- Print the free heap and largest contiguous block when something feels off. `ESP.getFreeHeap()` and `ESP.getMaxAllocHeap()` told me more in one print statement than hours of theorizing

## A note on the inspiration

This is a personal, non-commercial fan project inspired by *My Neighbor Totoro* (Studio Ghibli, 1988). No original Studio Ghibli artwork, audio, or assets are included in this repository — only my own code and original implementations. The character of Totoro and all related intellectual property belong to Studio Ghibli.

If I ever record original Totoro-style sounds for this project, they're sound-alikes created from scratch, not extracted from the film.

## Following along

I'm posting updates as I go. If you're building something similar, I'd love to compare notes.

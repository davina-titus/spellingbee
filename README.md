# 🐝 Spelling Bee Game & Solver

A terminal-based C implementation of the [NYT Spelling Bee](https://www.nytimes.com/puzzles/spelling-bee) game, extended with automatic solving capabilities.

---

## Overview

The game centers around a **hive** — a set of unique letters. Players must find words that:
- Use **only** letters from the hive
- **Always** include the designated required letter
- Are **at least 4 characters** long

The program supports both **interactive play** and **automatic solving**, with two solver strategies: brute-force and optimised recursive search.

---

## Features

- **Manual or random hive generation** — enter your own letters or generate a random hive of a chosen size
- **Interactive play mode** — guess words, receive real-time feedback, and track your score
- **Pangram detection** — words using every hive letter are marked with `*`; perfect pangrams (exactly hive-length) with `***`
- **Brute-force solver** — scans the entire dictionary for valid words
- **Optimised solver** — recursive binary search using prefix pruning for faster results
- **Bingo detection** — checks whether the solution set contains a word starting with every hive letter
- **Frequency table** — breakdown of valid words by starting letter and word length
- **Custom dictionary support** — load any word list via command-line flag

---

## Getting Started

### Prerequisites

- GCC compiler
- `make`
- A dictionary file (default: `dictionary.txt` in the working directory)

### Build

```bash
make build
```

This compiles `main.c` into `spellingBee.exe`.

### Run

```bash
make run          # Run the game normally
make run_simp     # Run with sample dictionary
make run_play     # Run in play mode with random 7-letter hive
make run_sample   # Run with piped sample input
```

Or run directly:

```bash
./spellingBee.exe              # manual hive entry
./spellingBee.exe -p -r 7      # play mode, random 7-letter hive
./spellingBee.exe -d mywords.txt  # custom dictionary
```

> ⚠️ Make sure `dictionary.txt` exists in the same folder before running.

---

## Options

| Flag | Description |
|------|-------------|
| `-r <n>` | Random mode — generate a hive of `n` unique letters (2–12) |
| `-d <file>` | Use a custom dictionary file |
| `-p` | Enable interactive play mode |
| `-o` | Use the optimised solver instead of brute-force |
| `-s <seed>` | Set a random seed for reproducible hive generation |

---

## Scoring

| Word Type | Points |
|-----------|--------|
| 4-letter word | 1 pt |
| 5+ letter word | 1 pt per letter |
| Pangram bonus | +hive size pts |

Pangrams (`*`) use every letter in the hive at least once. Perfect pangrams (`***`) use every letter exactly once.

---

## Project Structure

```
.
├── main.c          # All game logic
├── makefile        # Build & run targets
└── dictionary.txt  # Default word list (not included — bring your own)
```

---

## Memory Management

All dynamic allocations are tracked and freed. Check for memory leaks with:

```bash
make valgrind
```

---

## Author

**Davina Titus**

🐝 Spelling Bee Game & Solver
A terminal-based C implementation of the NYT Spelling Bee game, extended with automatic solving capabilities. Built as part of CS 211 – Programming Practicum at the University of Illinois Chicago.

Overview
The game centers around a hive — a set of unique letters. Players must find words that:

Use only letters from the hive
Always include the designated required letter
Are at least 4 characters long

The program supports both interactive play and automatic solving, with two solver strategies: brute-force and optimised recursive search.

Features

Manual or random hive generation — enter your own letters or generate a random hive of a chosen size
Interactive play mode — guess words, receive real-time feedback, and track your score
Pangram detection — words using every hive letter are marked with *; perfect pangrams (exactly hive-length) with ***
Brute-force solver — scans the entire dictionary for valid words
Optimised solver — recursive binary search using prefix pruning for faster results
Bingo detection — checks whether the solution set contains a word starting with every hive letter
Frequency table — breakdown of valid words by starting letter and word length
Custom dictionary support — load any word list via command-line flag


Getting Started
Prerequisites

GCC compiler
make
A dictionary file (default: dictionary.txt in the working directory)

Build
bashmake build
This compiles main.c into spellingBee.exe.

Usage
./spellingBee.exe [options]
Options
FlagDescription-r <n>Random mode — generate a hive of n unique letters (2–12)-d <file>Use a custom dictionary file-pEnable interactive play mode-oUse the optimised solver instead of brute-force-s <seed>Set a random seed for reproducible hive generation
Examples
bash# Run with default dictionary (manual hive entry)
./spellingBee.exe

# Random 7-letter hive with play mode
./spellingBee.exe -p -r 7

# Use a custom dictionary
./spellingBee.exe -d newsamp.txt

# Optimised solver with random 6-letter hive
./spellingBee.exe -r 6 -o
Or use the provided make shortcuts:
bashmake run          # Run the game normally
make run_simp     # Run with sample dictionary
make run_play     # Run in play mode with random 7-letter hive
make run_sample   # Run with piped sample input

Scoring
Word TypePoints4-letter word1 pt5+ letter word1 pt per letterPangram bonus+hive size pts
Pangrams (*) use every letter in the hive at least once. Perfect pangrams (***) use every letter exactly once.

Project Structure
.
├── main.c          # All game logic
├── makefile        # Build & run targets
└── dictionary.txt  # Default word list (not included — bring your own)
Key components in main.c
ComponentDescriptionWordList structDynamic array of strings with auto-resizingbuildDictionary()Loads words from file, filtering by minimum lengthbuildHive()Constructs a sorted, deduplicated hive from inputisValidWord()Validates a word against hive rulesisPangram()Checks if a word uses all hive lettersbruteForceSolve()Linear scan solverfindAllMatches()Recursive optimised solver with binary searchprintList()Displays word list with scores and pangram markers

Memory Management
All dynamic allocations are tracked and freed. The project can be checked for memory leaks with:
bashmake valgrind

Author
Davina Titus — CS 211, University of Illinois Chicago

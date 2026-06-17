# c-coursework

C coursework from FIT CTU Prague — data structures and algorithms in plain C.

## Contents

| File | What it demonstrates |
|------|----------------------|
| `linked_list_platoon.c` | Singly-linked list merge (interleaving) and split with manual memory management |
| `roman_numerals.c` | String scanning and in-place token replacement (Arabic → Roman numerals, range 1–3999) |
| `stock_trading.c` | Dynamic array with realloc growth, O(n²) max-profit and max-loss search over a price series |

## Building

Each file is self-contained and compiles with a single command.

```bash
gcc -std=c11 -Wall -o out <filename>.c && ./out
```

`stock_trading.c` is interactive — see the in-file comment for the command format.

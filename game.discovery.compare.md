# Zelda Discovery Comparison

| Measure | Current Heuristics | Zelda Oracle |
|---|---:|---:|
| Standalone emitted | 28 | 137 |
| `extra_label` emitted | 131 | 101 |
| Correct function matches | 24 | 137 |
| Correct label matches | 99 | 101 |
| Correct total semantic matches | 123 | 238 |
| Dangerous extra standalones | 4 | 0 |
| Extra labels not in oracle | 32 | 0 |
| Missed real standalone functions | 113 | 0 |
| Missed real labels | 2 | 0 |

Short takeaway:
- Current proposal emits 28 standalone functions and 131 labels.
- It matches 24 Zelda standalone roots and 99 Zelda labels (123 total semantic matches).
- It still has 4 generated-only standalones and 32 generated-only labels.

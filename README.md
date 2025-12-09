# Żmij

A double-to-string conversion algorithm based on Schubfach

Improvements:
* Faster logarithm approximations
* Fewer branches
* Faster division and modulo
* More efficient significand and exponent output

## Usage

```c++
#include "zmij.h"
#include <stdio.h>

int main() {
  char buf[zmij::buffer_size];
  zmij::dtoa(6.62607015e-34, buf);
  puts(buf);
}
```

## Performance

More than 2x faster than Ryu and 50% faster than [Schubfach](https://github.com/vitaut/schubfach)
on dtoa-benchmark.

| Function       | Time (ns) | Speedup  |
|----------------|----------:|---------:|
| ostringstream  | 883.690   | 1.00x    |
| sprintf        | 736.937   | 1.20x    |
| doubleconv     | 83.762    | 10.55x   |
| to_chars       | 43.272    | 20.42x   |
| ryu            | 37.201    | 23.75x   |
| schubfach      | 24.747    | 35.71x   |
| fmt            | 22.284    | 39.66x   |
| dragonbox      | 20.689    | 42.71x   |
| zmij           | 13.581    | 65.07x   |
| null           | 0.930     | 950.20x  |

<img width="762" height="335" alt="image" src="https://github.com/user-attachments/assets/c700dedd-b6ff-4fb6-ab46-02c2996bb9ae" />

<img width="879" height="666" alt="image" src="https://github.com/user-attachments/assets/54bf2057-258f-43aa-bcc5-bddd3e85cab6" />



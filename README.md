# C keyboard layout analyzer

Tiny little analyzer written in C, using my own vector and string implementations. Leaks some memory
at this point, but valgrind is telling me it's about 3000 bytes total. I'm willing to accept that
loss at this point.

## Usage

First, build the program with `make br`. Then, you can use it as follows:

```bash
Usage: ./build/c-analyzer [-h] [-l layout]
Options:\n
  -h           Display this help message
  -l path      Shows information about a layout
  -r           Ranks all layouts in './layouts', sorted by sfb
```

Note that you might need to drop the `./` or add `.exe` to the file path if you're on windows.

There is also some commented out code that loads in new data from `/corpora`, and enumerates all
layouts from `./layouts` if you wanna mess aruond with that.

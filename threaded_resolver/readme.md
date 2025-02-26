## Multi-threaded DNS Resolver

### File overview
- `input/`: directory of text files containing host names that will be used as input to your program
- `array.c`: where your thread-safe shared array implementation should live
- `array.h`: header file for your shared array implementation
- `libutil.a`: library file that contains the `dnslookup()` function you can use
- `Makefile`: GNU Makefile used to compile your code
  - `make`: compiles your code into an executable named `multi-lookup`
  - `make clean`: removes compiled binary and object files
  - `make submit`: bundles your code in a text file for submission
- `multi-lookup.c`: where your thread-safe DNS resolver implementation should live
- `multi-lookup.h`: header file for your DNS resolver implementation
- `util.h`: header file containing the `dnslookup()` function prototype

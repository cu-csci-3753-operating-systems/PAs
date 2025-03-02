## Least Recently Used Paging

### File overview
- `Makefile`: GNU Makefile used to compile your code
  - `make`: compiles your code and the basic implemtnation into executables named `test-basic` and `test-lru`
  - `make clean`: removes compiled binaries, object files, and CSV files
  - `make submit`: bundles your code in a text file for submission
- `pager-basic.c`: basic paging algorithm implemtnation
- `pager-lru.c`: where your LRU paging implemtnation should live
- `programs.c`: programs used by the paging simulator
- [`see.R`](../common/see.R): R-script to visually display running/blocking process activity
  - Use `./test-* -csv` to generate the necessary CSV files
- `simulator.c`: paging simulator source code (for reference only)
- `simulator.h`: paging simulator header file

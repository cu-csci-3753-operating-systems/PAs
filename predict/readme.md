## Least Recently Used Paging

### File overview
- `Makefile`: GNU Makefile used to compile your code
  - `make`: compiles your code into an executable named `test-predict`
  - `make clean`: removes compiled binary, object files, and CSV files
  - `make submit`: bundles your code in a text file for submission
- `pager-predict.c`: where your predictive paging implemtnation should live
- `programs.o`: compiled object file containing programs used by the paging simulator
- [`see.R`](../common/see.R): R-script to visually display running/blocking process activity
  - Use `./test-predict -csv` to generate the necessary CSV files
- `simulator.c`: paging simulator source code (for reference only)
- `simulator.h`: paging simulator header file

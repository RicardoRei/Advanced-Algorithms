TESTS:

- output_test*.in files are just for checking if outputs are correct.

- P<nº>_*.in files tests the size of the pattern in high repetitive sequences. The <nº> represents the size of the
             pattern we want to find.

- Rtest<size>_*.in files tests the nr of compares (and time) for files with highly repetitive sequences and patterns.
             The <size> represents the size of T and we assume the size of P is always 10 because in a random sequence
             pattern of size 10 is not very common. We want to see how regularity influences comparations and time.

- test<size>_*.in files tests the nr of compares (and time) for files with completly random sequences and patterns.
              The <size> represents the size of T and we assume the size of P is always 10 because in a random sequence
              pattern of size 10 is not very common.
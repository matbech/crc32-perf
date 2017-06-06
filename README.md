# crc32-perf
Compares the performance of various crc32 implementations.

- zlib's crc32 (with optimizations)
- Intel's crc32 implementation with PCLMULQDQ
- Splice by 4
- Splice by 8
- Splice by 16

## Performance results
On modern CPUs (Intel Westmere, AMD Bulldozer) the PCLMULQDQ implementation offers the best performance for buffers >= 512 bytes.
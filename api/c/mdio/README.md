# Introduction

The purpose of the files within this directory is to provide an optional file-format interface for MDIO instead of RSF.

This is WIP and likely lacks many necessary functions still.

# Methodolgy

My goal is to provide a 1-1 overload of the `rsf.h` functions and provide compile time macros to switch between the traditional RSF files and MDIO file output. 

This is less desirable as it would break existing flows depending on the compilation flags used. For that reason I believe there may be some functionality built in that would handle the file format conditionality at runtime instead of compile time.


We will leverage the `extern c` to enable the C++ MDIO library to be properly linked.

# TODO

There is still a lot to do, and I don't remember exactly where I left off. This probably won't compile with the MDIO interface enabled.

I believe that running `scons --mdio=1` in the `src/api/c` directory will be sufficient for checking compile and linking testing.
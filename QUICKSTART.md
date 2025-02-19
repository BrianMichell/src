# Introduction
The purpose of this quickstart guide is to facilitate the MDIO hackathon.

By following this quickstart guide, the user will clone, build, and install a custom fork of Madagascar that will have MDIO available as an optional dependency in the C++ API.

MDIO is only tested to be available in the user modules directory and may require additional work to be available throught the inclusion of traditional `rsf.hh` and `cub.hh` for a more native experience.

# Table of Contents
- [Helpful links](#helpful-links)
- [Getting started](#getting-started)
- [Developing your own user module with MDIO](#developing-your-own-user-module-with-mdio)
  - [SConstruct](#sconstruct)
    - [Required imports for SConstruct file](#required-imports-for-sconstruct-file)
    - [A list of our C++ programs](#a-list-of-our-c-programs)
    - [Environment setup](#environment-setup)
    - [Environment configuration](#environment-configuration)
    - [Make it all installable](#make-it-all-installable)
    - [Self documentation](#self-documentation)
  - [klipp.cc](#klippcc)
    - [Self-documentation](#self-documentation-1)
    - [Includes](#includes)
    - [Main function](#main-function)
      - [Hello MDIO](#hello-mdio)
      - [Clip example](#clip-example)

# Helpful links
- [MDIO V1.0 dataset model](https://mdio-python.readthedocs.io/en/v1/data_models/version_1.html#)

- [MDIO C++ API documentation](https://tgsai.github.io/mdio-cpp/html/annotated.html)

- [MDIO C++ Examples](https://github.com/TGSAI/mdio-cpp/tree/main/examples)
  - [Creating a dataset](https://github.com/TGSAI/mdio-cpp/tree/main/examples/hello_mdio/src)
  - [Basic usage](https://github.com/TGSAI/mdio-cpp/tree/main/examples/dataset_example/src)
  - [Accessing AWS data](https://github.com/TGSAI/mdio-cpp/tree/main/examples/real_data_example) 

# Getting started
```bash
cd ~
git clone --branch hackathon https://github.com/BrianMichell/src.git
cd src

# There may be some warnings regarding line terminations. These can safely be ignored.
./configure --prefix=~/install

# The install process will generate a lot of output. Use the commented out version to redirect it to a log.
# scons -j32 install MDIO=1 > inst.log 2>&1
scons -j32 install MDIO=1

# Running the installed binary should print out some information from the MDIO open dataset.
~/install/bin/sfklipp
```

# Developing your own user module with MDIO

## SConstruct

### Required imports for SConstruct file
```python
import os, sys, string, glob
sys.path.append('../../framework')
sys.path.append('../../api/c++')
from SCons.Script import Dir
api_cpp_dir = Dir('#/api/c++').abspath
if api_cpp_dir not in sys.path:
    sys.path.insert(0, api_cpp_dir)
import bldutil
import setup_mdio  # Import the module directly
```

### A list of our C++ programs
```python
progs = '''
klipp
'''
```

### Environment setup
```python
try:  # distributed version
    Import('env root bindir pkgdir')
    env = env.Clone()
except: # local version
    env = bldutil.Debug()
    root = None
    SConscript('../../api/c++/SConstruct')
```

### Environment configuration
```python
env.Append(CPPPATH=['../../include'],
           LIBPATH=['../../lib'],
           LIBS=['rsf++', 'rsf'])

auxFlags = [
    "-w",
    "-DMAX_NUM_SLICES=32",
    "-DNO_BLAS",
    "-DFS_HAS_RPC=False"
]
for flag in auxFlags:
    env.Append(CXXFLAGS=f" {flag}")

if int(env.get('MDIO', 0)):
    print("MDIO support enabled. Configuring MDIO dependency via API module.")
    setup_mdio.setup_mdio(env, root)
    # print("###########After setting up MDIO###########")
    # print(f"CPPPATH: {env.get('CPPPATH')}")
    # print(f"LIBPATH: {env.get('LIBPATH')}")
    # print(f"LINKFLAGS: {env.get('LINKFLAGS')}")
else:
    print("WARNING: MDIO support is disabled. Compiling without MDIO support.")
    env.Append(CCFLAGS=['-DNO_MDIO'])
```

### Make it all installable
```python
mains = Split(progs)
for prog in mains:
    sources = [prog + '.cc']
    prog_target = env.Program(prog, sources)
    if root:
        env.Install(bindir, prog_target)
```

### Self documentation
```python
if root:
    user = os.path.basename(os.getcwd())
    main_doc = 'sf%s.py' % user
    
    docs = [env.Doc(prog, prog + '.cc', lang='c++') for prog in Split(progs)]
    env.Depends(docs, '#/framework/rsf/doc.py')
    doc = env.RSF_Docmerge(main_doc, docs)
    env.Install(pkgdir, doc)
```

## klipp.cc
This is the example program that is in the `src/user/brianmichell` directory.

If attempting to re-create this example, a new unique name is recommended. 
Be sure to update the name in the [SConstruct progs](#a-list-of-our-c-programs) section.

### Self-documentation

```c++
/* A Hello World example of opening an MDIO file.
 * 
 * https://ahay.org/wiki/Guide_to_madagascar_API#C++_interface
 * Clip the data.
 * This program reads an RSF input and for each trace clips sample values
 * that exceed the given threshold (and similarly clips values below
 * the negative threshold). The threshold is passed via the "clip" parameter.
 */
```

### Includes

```C++
#include <valarray>
#include <rsf.hh>

#ifdef NO_MDIO
#error "Madagascar API not built with MDIO support; disable MDIO dependent code or rebuild API with MDIO"
#else
#include <mdio/mdio.h>
#endif
```

### Main function

#### Hello MDIO

```c++
    #ifndef NO_MDIO
    // BEGIN HELLO WORLD MDIO
    std::string path = "s3://tgs-opendata-poseidon/full_stack_agc.mdio";

    mdio::Future<mdio::Dataset> dsRes = mdio::Dataset::Open(path, mdio::constants::kOpen);
    if (!dsRes.status().ok()) {
        std::cerr << "Failed to open dataset: " << dsRes.status() << std::endl;
        return 1;
    }

    mdio::Dataset ds = dsRes.value();
    std::cout << ds << std::endl;
    // END HELLO WORLD MDIO
    #endif
```

#### Clip example

```c++
    sf_init(argc, argv); // Initialize RSF

    iRSF par(0), in; // Input parameter and file
    oRSF out;        // Output file

    int n1, n2;      // Trace length and number of traces
    float clip;
    
    in.get("n1", n1);
    n2 = in.size(1);

    par.get("clip", clip); // Parameter from the command line

    std::valarray<float> trace(n1);

    for (int i2 = 0; i2 < n2; i2++) { // Loop over traces
        in >> trace; // Read a trace

        for (int i1 = 0; i1 < n1; i1++) { // Loop over samples
            if (trace[i1] > clip)
                trace[i1] = clip;
            else if (trace[i1] < -clip)
                trace[i1] = -clip;
        }

        out << trace; // Write the clipped trace
    }

    return 0;
```
import os
import glob
from SCons.Script import GetOption, Exit, COMMAND_LINE_TARGETS

def setup_mdio(env, root=None):
    """
    Clones/installs MDIO if needed and appends the appropriate MDIO
    include paths, lib paths, compile flags, and linker flags.
    
    The optional 'root' parameter can be passed from the calling SConscript.
    """
    mdio_dir = os.path.abspath(os.path.join(root, 'mdio_installation')) if root else os.path.abspath('mdio_installation')
    is_cleaning = ('clean' in COMMAND_LINE_TARGETS or 
                   'distclean' in COMMAND_LINE_TARGETS or 
                   GetOption('clean'))
    if not is_cleaning:
        if not os.path.exists(mdio_dir):
            print("MDIO dependency not installed. Cloning and installing...")
            os.makedirs(mdio_dir, exist_ok=True)
            install_path = os.path.join(mdio_dir, 'inst')
            print(f"Installing to {install_path}")
            command_str = (
                f'git clone --branch drivers_enhancement https://github.com/BrianMichell/mdio-cpp-installer.git {mdio_dir} '
                f'&& cd {mdio_dir} '
                f'&& chmod +x install.sh '
                f'&& ./install.sh {install_path} --curl'
            )
            print("Executing MDIO installation command:", command_str)
            if env.Execute(command_str) != 0:
                print("Error: MDIO installation failed!")
                Exit(1)
    # Configure MDIO include and library paths and flags
    install_include = os.path.join(mdio_dir, 'inst', 'include')
    install_lib = os.path.join(mdio_dir, 'inst', 'lib')
    install_drivers = os.path.join(install_lib, 'drivers')
    env.Append(CPPPATH=[install_include])
    env.Append(LIBPATH=[install_lib, install_drivers])
    env.Append(CCFLAGS=['-DHAVE_MDIO'])
    # Set the MDIO linker flags with the necessary order and options
    LDFLAGS_STRING = (
        f"-Wl,-rpath,{install_lib},-rpath,{install_drivers},"
        f"--whole-archive,-L{install_lib},-L{install_drivers},"
        f"-lnlohmann_json_schema_validator,-ltensorstore_driver_zarr_bzip2_compressor,"
        f"-ltensorstore_driver_zarr_driver,-ltensorstore_driver_zarr_spec,"
        f"-ltensorstore_driver_zarr_zlib_compressor,-ltensorstore_driver_zarr_zstd_compressor,"
        f"-ltensorstore_driver_zarr_blosc_compressor,-ltensorstore_kvstore_gcs_http,"
        f"-ltensorstore_kvstore_gcs_gcs_resource,-ltensorstore_kvstore_gcs_validate,"
        f"-ltensorstore_kvstore_gcs_http_gcs_resource,-ltensorstore_driver_json,"
        f"-ltensorstore_internal_cache_cache_pool_resource,-ltensorstore_internal_data_copy_concurrency_resource,"
        f"-ltensorstore_kvstore_file,-ltensorstore_internal_file_io_concurrency_resource,"
        f"-ltensorstore_internal_cache_kvs_backed_chunk_cache,-labsl,-lblosc,-ltensorstore,"
        f"-lre2,-lriegeli,-lcurl,-lopenssl,--no-whole-archive,-lpthread,-lm"
    )
    env.Append(LINKFLAGS=LDFLAGS_STRING.split())
    auxFlags = [
        "-w",
        "-DMAX_NUM_SLICES=32",
        "-DNO_BLAS",
        "-DFS_HAS_RPC=False"
    ]
    for flag in auxFlags:
        env.Append(CXXFLAGS=f" {flag}")
    DIRS = [d for d in glob.glob(os.path.join(mdio_dir, "inst", "include", "*"))
            if os.path.isdir(d) and "gtest-src" not in d]
    DIRS.append(os.path.join(mdio_dir, "inst", "include", "nlohmann_json-src", "include"))
    DIRS.append(os.path.join(mdio_dir, "inst", "include", "half-src", "include"))
    env.Append(CPPPATH=DIRS)
    print("MDIO dependency successfully configured.")
    return mdio_dir 
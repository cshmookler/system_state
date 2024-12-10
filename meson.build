# Declare project information
project(
    'system_state',
    'cpp',
    version : files('VERSION'),
    default_options : [ 'cpp_std=c++17' ],
)

# Project root and build directories
root_dir = meson.project_source_root()
build_dir = meson.project_build_root()
include_dir = root_dir / 'system_state'
src_dir = root_dir / 'src'
tests_dir = root_dir / 'tests'

# Insert the project version into the version header file
conf_data = configuration_data()
conf_data.set('version', meson.project_version())
configure_file(
    configuration : conf_data,
    input : 'system_state' / 'version.hpp.in',
    output : 'version.hpp',
)

lib_system_state_headers = files(
    build_dir / 'version.hpp',
    include_dir / 'core.hpp',
    include_dir / 'error.hpp',
)
lib_system_state = library(
    'system_state',
    files(
        src_dir / 'version.cpp',
        src_dir / 'error.cpp',
        src_dir / 'util.cpp',
        src_dir / 'system.cpp',
        src_dir / 'block.cpp',
        src_dir / 'cpu_usage.cpp',
        src_dir / 'backlight.cpp',
        src_dir / 'battery.cpp',
        src_dir / 'network_interface.cpp',
    ),
    version : meson.project_version(),
    install : true,
)
install_headers(lib_system_state_headers, subdir : 'system_state')

dep_gtest_main = dependency(
    'gtest_main',
    required : false,
    method : 'auto',
)

tests = [
    'version',
    'system',
    'block',
    'cpu_usage',
    'backlight',
    'battery',
    'network_interface',
]

if dep_gtest_main.found()
    foreach test_name : tests
        test_exec = executable(
            test_name,
            files(
                tests_dir / (test_name + '.test.cpp'),
                src_dir / (test_name + '.cpp'),
                src_dir / 'error.cpp',
                src_dir / 'util.cpp',
            ),
            dependencies : dep_gtest_main,
        )
        test(test_name, test_exec)
    endforeach
else
    warning('Skipping tests due to missing dependencies')
endif

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
    include_dir / 'core.hpp'
)
lib_system_state = library(
    'system_state',
    files(
        src_dir / 'version.cpp',
        src_dir / 'system.cpp',
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

if dep_gtest_main.found()
    test_version = executable(
        'version',
        files(
            src_dir / 'version.test.cpp',
            src_dir / 'version.cpp',
        ),
        dependencies : dep_gtest_main,
    )
    test('version', test_version)
else
    warning('Skipping tests due to missing dependencies')
endif

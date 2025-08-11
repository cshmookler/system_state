# Declare project information
project(
    'system_state',
    ['cpp', 'c'],
    version : files('VERSION'),
    default_options : [ 'cpp_std=c++17', 'c_std=c17' ],
)

# Project root and build directories
root_dir = meson.project_source_root()
build_dir = meson.project_build_root()
include_dir = root_dir / 'system_state'
src_dir = root_dir / 'src'
src_c_dir = src_dir / 'c'
tests_dir = root_dir / 'tests'
examples_dir = root_dir / 'examples'
examples_c_dir = examples_dir / 'c'

# Insert the project version into the version header file
conf_data = configuration_data()
conf_data.set('version', meson.project_version())
configure_file(
    configuration : conf_data,
    input : 'system_state' / 'version.h.in',
    output : 'version.h',
)

dep_alsa_main = dependency(
    'alsa',
    required : true,
    method : 'auto',
)

lib_system_state_headers = files(
    build_dir / 'version.h',
    include_dir / 'system_state.hpp',
    include_dir / 'system_state.h',
)
lib_system_state = library(
    'system_state',
    files(
        src_dir / 'version.cpp',
        src_dir / 'util.cpp',
        src_dir / 'strerror.cpp',
        src_dir / 'user.cpp',
        src_dir / 'system.cpp',
        src_dir / 'block.cpp',
        src_dir / 'cpu_usage.cpp',
        src_dir / 'thermal.cpp',
        src_dir / 'backlight.cpp',
        src_dir / 'battery.cpp',
        src_dir / 'network_interface.cpp',
        src_dir / 'sound.cpp',
        src_dir / 'kernel.cpp',
        src_c_dir / 'string_c.cpp',
        src_c_dir / 'backlight_c.cpp',
        src_c_dir / 'battery_c.cpp',
        src_c_dir / 'sound_c.cpp',
    ),
    version : meson.project_version(),
    dependencies : [dep_alsa_main],
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
    'user',
    'system',
    'block',
    'cpu_usage',
    'thermal',
    'backlight',
    'battery',
    'network_interface',
    'sound',
    'kernel',
]

if dep_gtest_main.found()
    foreach test_name : tests
        test_exec = executable(
            'test_' + test_name,
            files(
                tests_dir / (test_name + '.test.cpp'),
            ),
            dependencies : dep_gtest_main,
            link_with : lib_system_state,
        )
        test(test_name, test_exec)
    endforeach
else
    warning('Skipping tests due to missing dependencies')
endif

examples = [
    'version',
    'user',
    'system',
    'block',
    'cpu_usage',
    'thermal',
    'backlight',
    'battery',
    'network_interface',
    'sound',
    'kernel',
]

foreach example_name : examples
    executable(
        'example_' + example_name,
        files(
            examples_dir / (example_name + '.cpp'),
        ),
        link_with : lib_system_state,
    )
endforeach

examples_c = [
    'version',
    'backlight',
    'battery',
    'sound',
]

foreach example_name : examples_c
    executable(
        'example_' + example_name + '_c',
        files(
            examples_c_dir / (example_name + '.c'),
        ),
        link_with : lib_system_state,
    )
endforeach

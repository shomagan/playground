import optparse
import logging
import os
import fnmatch

__description__ = '''add UT templates to the current folder
 exmpl. python3 add_ut_template.py -p . -n EthGXBSPApp
 folders to be created: test, test/stubs, test/unit, test/unit/mocks
 files to be created:
    build-linux.sh
    prepare-linux.sh
    test/CmakeLists.txt
 what else should be done:
    1. Application CmakeLists.txt has to be adopted to use UT.
        if(UNIT_TESTS)
        enable_testing()
        add_subdirectory(test)
        else()
        ...
        endif(UNIT_TESTS)
    2. Refer to another UT's to complite test/CmakeList.txt.
    3. Add all stubs and mock files what is needed to test.
 '''
CURRENT_FOLDER = os.path.dirname(os.path.abspath(__file__))

dir_path = os.path.dirname(os.path.realpath(__file__))


def main():
    parser = optparse.OptionParser(usage="python3 add_ut_template.py",
                                   description=__description__)

    group = optparse.OptionGroup(parser, "")
    group.add_option("-p", "--path", dest="path", action="store",
                     help="active directory", default=CURRENT_FOLDER)
    group.add_option("-n", "--name", dest="name", action="store",
                     help="name will be used in cmake list app name",
                     default="EthGXBSPApp")
    group.add_option("-l", "--logging_level", dest="loglevel", action="store",
                     help="DEBUG, INFO, WARNING, ERROR, CRITICAL",
                     default="INFO")
    parser.add_option_group(group)
    (options, args) = parser.parse_args()
    logging.basicConfig(level=options.loglevel.upper())
    logging.info(__description__)

    if not os.path.exists(options.path):
        logging.error("path not exist")
        exit()

    create_ut_folders()
    executables_src_list = []
    executables_ut_list = []
    headers_directories = []
    for root, dirnames, filenames in os.walk(options.path):
        print(root)
        for filename in fnmatch.filter(filenames, '*.cpp'):
            generate_ut_cpp_template("test/unit", filename)
            executables_src_list.append(root + '/' + filename)
            executables_ut_list.append('UT_' + filename)
        for filename in fnmatch.filter(filenames, '*.hpp'):
            if root not in headers_directories:
                headers_directories.append(root)
    generate_cmake_test("test")
    generate_cmake_unit("test/unit", options.name, headers_directories,
                        executables_src_list, executables_ut_list)
    generate_prepare_sh(".")
    generate_build_sh(".", options.name)


def generate_ut_cpp_template(path, filename):
    '''for each app cpp file, found in the dir'''
    ut_filename = 'UT_' + filename
    if not os.path.isfile(path+'/' + ut_filename):
        ut_file = open(path+'/' + ut_filename, 'w', encoding="utf-8")
        ut_file.write(f"""
#ifndef {ut_filename.replace(".","_").upper()}
#define {ut_filename.replace(".","_").upper()} 1

/*===========================================================================*/
/**
 * @file {ut_filename}
 *
 * @brief Unit tests implementation for {filename}
 *
 * @author
 *
 *------------------------------------------------------------------------------
 *
 * Copyright 2024 Aptiv. All rights reserved.
 * Information Contained Herein Is Proprietary and Confidential
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   None.
 *
 * @section ABBR ABBREVIATIONS:
 *   None.
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - None.
 *
 *   - Requirements Document(s):
 *     - None.
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - SW REF 264.15D "Delphi C Coding Standards"
 *     - SW REF 264.01E "Delphi C++ Coding Standards"
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 */

/*===========================================================================*\\
 * System Header Files
\*===========================================================================*/

/*===========================================================================*\\
 * Project Header Files
\*===========================================================================*/
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class {filename}Test : public Test
{{
public:
   /*starts before every TEST_F with the same class name*/
   void SetUp() override
   {{
   }}

   /*starts after every TEST_F with the same class name*/
   void TearDown() override
   {{
   }}
}};
TEST_F({filename}Test, Common)
{{
}}

#endif /*{ut_filename.replace(".","_").upper()}*/
""")
        ut_file.close()


def create_ut_folders():
    '''creates folders only if not exist'''
    if not os.path.exists("test"):
        os.makedirs("test")
    if not os.path.exists("test/stubs"):
        os.makedirs("test/stubs")
    if not os.path.exists("test/unit"):
        os.makedirs("test/unit")
    if not os.path.exists("test/unit/mocks"):
        os.makedirs("test/unit/mocks")


def generate_cmake_test(path):
    cmake_filename = "CMakeLists.txt"
    if not os.path.isfile(path+'/' + cmake_filename):
        cmake_file = open(path+'/' + cmake_filename, 'w', encoding="utf-8")
        cmake_file.write("""
# ---------------------------------------------------------------------------
# Copyright © 2024 Aptiv. All rights reserved.
# Information Contained Herein Is Proprietary and Confidential
# ---------------------------------------------------------------------------

add_subdirectory(unit)

""")
        cmake_file.close()


def generate_cmake_unit(path, app_name, headers_directories,
                        executables_src_list, executables_ut_list):
    cmake_filename = "CMakeLists.txt"
    if not os.path.isfile(path+'/' + cmake_filename):
        ut_sources = "".join('   '+str(ut) + '\n' for ut in executables_ut_list)
        app_sources = "".join('   ../../' + str(src) + '\n' for src in executables_src_list)
        headers = "".join('   ../../' + str(h) + '\n' for h in headers_directories)

        cmake_file = open(path+'/' + cmake_filename, 'w', encoding="utf-8")
        cmake_file.write(f"""

# ---------------------------------------------------------------------------
# Copyright © 2024 Aptiv. All rights reserved.
# Information Contained Herein Is Proprietary and Confidential
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
add_definitions(-DICB_UNIT_TEST)

include(CTest)

set(UT_SOURCES
{ut_sources}
{app_sources}
)

add_executable(UT_{app_name} ${{UT_SOURCES}})

target_compile_features(UT_{app_name}
    PUBLIC cxx_std_20
)

target_include_directories(UT_{app_name}
PUBLIC
    .
    ../stubs
    mocks
{headers}
)

target_link_libraries(UT_{app_name}
    gtest_main
    gmock_main
)


add_test(NAME UT_{app_name}
    COMMAND ./UT_{app_name}
)

""")
        cmake_file.close()


def generate_prepare_sh(path):
    build_filename = "prepare-linux.sh"
    if not os.path.isfile(path+'/' + build_filename):
        build_file = open(path+'/' + build_filename, 'w', encoding="utf-8")
        qnx_apps_name = 'qnx_apps'
        qnx_apps_pos = CURRENT_FOLDER.find(qnx_apps_name)
        if qnx_apps_pos > 0:
            build_dir_pos = qnx_apps_pos + len(qnx_apps_name)
            path_to_build = CURRENT_FOLDER[:build_dir_pos]
            build_file.write(f"""
#!/bin/bash -xe
HERE=$(dirname $(realpath $0))

pushd {path_to_build}

if ! test -d build_linux
then
  mkdir build_linux
fi

cd build_linux
cmake \\
   -DCMAKE_C_FLAGS="-DLINUX -DUNIT_TEST -fprofile-arcs -ftest-coverage -g -O0" \\
   -DCMAKE_CXX_FLAGS="-DLINUX -DUNIT_TEST -fprofile-arcs -ftest-coverage -g -O0" \\
   -DCMAKE_BUILD_TYPE=Debug \\
   -DLIB_PPS= \\
   -DPROJ_NAME=Linux_UT \\
   -DXSAL_TYPE=XSAL_APP_LOCAL \\
   -DUNIT_TESTS=ON \\
   $@ \\
   ..

popd
""")
        build_file.close()


def generate_build_sh(path, name):
    build_filename = "build-linux.sh"
    if not os.path.isfile(path+'/' + build_filename):
        build_file = open(path+'/' + build_filename, 'w', encoding="utf-8")
        qnx_apps_name = 'qnx_apps'
        qnx_apps_pos = CURRENT_FOLDER.find(qnx_apps_name)
        if qnx_apps_pos > 0:
            build_dir = 'build_linux/'
            build_dir_pos = qnx_apps_pos + len(qnx_apps_name) + 1
            path_in_build_to_app = CURRENT_FOLDER[:build_dir_pos] + build_dir + CURRENT_FOLDER[build_dir_pos:]
            path_to_build = CURRENT_FOLDER[:build_dir_pos] + build_dir
            build_file.write(f"""
#!/bin/bash -x
#./build-linux.sh -c

HERE=$(dirname $(realpath $0))

set -e

CMAKE_ARGS=""
RUN_COVERAGE=0
RET=0

while getopts "cdt" o; do
    case "${{o}}" in
        c)
            RUN_COVERAGE=1
            ;;
        t)
            ;;
        *)
            ;;
    esac
done

${{HERE}}/prepare-linux.sh ${{CMAKE_ARGS}}
pushd {path_to_build}

make -j$(nproc) \\
    UT_{name}
timeout -k 5 120 \\
    ctest -VV -R UT_{name}
popd

COVERAGE_PATH="${{HERE}}/coverage"
if ! test -d $COVERAGE_PATH
then
    mkdir $COVERAGE_PATH
fi

if [ ${{RUN_COVERAGE}} -eq 1 ]
then
    gcovr -r {path_in_build_to_app} -f '' '-e=.*test*.' '-e=.*usr*.' --html --html-details -o ${{COVERAGE_PATH}}/gcovr_report.html
fi
""")
        build_file.close()


if __name__ == "__main__":
    'add ut templates in the path'
    main()

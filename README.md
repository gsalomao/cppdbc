# cppdbc
> Database Connector for C++

[![Build Status](https://travis-ci.org/gsalomao/cppdbc.svg?branch=develop)](https://travis-ci.org/gsalomao/cppdbc)
[![codecov](https://codecov.io/gh/gsalomao/cppdbc/branch/develop/graph/badge.svg)](https://codecov.io/gh/gsalomao/cppdbc)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

The cppdbc allows any C++ application to interact with databases.

## Installation
Use CMake to build and install the cppdbc.

### Dependencies

- CMake 3.13+
- G++

### Configuration
The first step is configure the project using CMake.
```bash
cmake -B build .
```

### Build
After the project has been configured, the next step is to build the library.
```bash
cmake --build build
```

### Installation
After the library has been build, the last step is to install it.
```bash
cmake --build build --target install
```

## Tests
The project is shipped with two groups of tests: unit and integration tests.

### Dependencies

- Google Test
- Google Mock

To run the unit and integration tests, the tests must be enabled during the configuration.
of the project.
```bash
cmake -B build -D ENABLE_TESTS=YES .
```

Once the tests have been enabled, the tests can be built.
```bash
cmake --build build --target build_tests
```

After the tests has been built, each group of test can run individually or all at once.
```bash
cmake --build build --target test_unit
cmake --build build --target test_integration
cmake --build build --target test
```

After the tests have been run, the test reports are generated by default in the path 
build/test-results. 

The test results paths can be changed during the configuration of the project.
```bash
cmake -B build -D TEST_REPORT_PATH=<path> .
```

The name of the test reports can also be changed.
```bash
cmake -B build -D UNIT_TEST_REPORT_FILE=<file> -D INTEGRATION_TEST_REPORT_FILE=<file> .
```

To build the test coverage report, the test coverage has to be enabled during the configuration
of the project.
```bash
cmake -B build -D ENABLE_COVERAGE=YES .
```

Once the test coverage has been enabled, and the unit test has been run, the test
coverage report can be built.
```bash
cmake --build build --target coverage
```

The test coverage report is generated under the test results path (by default build/test-results).

## Static analysis
The project uses static analysis to prevent bugs and improve the code quality.

### Dependencies

- cppcheck

Run the static analysis tool, whenever you would like to check the code.
```bash
cmake --build build --target static
```

## Documentation
The project has all classes and functions documented to help the users to understand them.

### Dependencies

- Doxygen

The documentation can be build automatically.
```bash
cmake --build build --target docs
```

The documentation is built in the path build/docs.

## Contributing
Pull requests are welcome. 

For major changes, please open an issue first to discuss what you would like to change.
Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)

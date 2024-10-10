
# Style

Simple integrated code verification tool

---


## Flags

| Flag                           | Description                                          |
| -------------------------------|------------------------------------------------------|
| `-h, --help`                    | show this help message and exit                      |
| `-r, --recursive`               | Search files recursively in curr dir and all subdirs |
| `-f [FILES ...], --files [FILES ...]` | Specify the filenames to analyze (space-separated)   |

By default, `style` checks all files in the current directory.

## Rules

You can change the behavior of style by changing these files:

- [сlang-tidy](../codestyle_checker/.clang-tidy) - verification rules
- [Config](../codestyle_checker/config.json) - file search configuration

## Addition

- Style uses the `CODESTYLE_CONFIG_DIR_PATH` environment variable - the path to the codestyle_checker directory of the form `*/codestyle_checker`

- You can specify the project's build directory if you use Cmake with the flag `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`


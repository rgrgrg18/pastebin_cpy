#!/usr/bin/env python3

import os
import json
import argparse
import time

import linters

def load_config():
    config_dir_path = os.getenv('CODESTYLE_CONFIG_DIR_PATH')
    build_dir = os.getenv('CODESTYLE_BUILD_DIR')
    with open(config_dir_path + '/config.json', 'r') as config_file:
        config = json.load(config_file)


    clang_tidy_config_path = config_dir_path + '/.clang-tidy'
    extensions = config.get('extensions', [])

    return clang_tidy_config_path, extensions, build_dir


def run():
    parser = argparse.ArgumentParser(description='Run clang-tidy on source files.')
    parser.add_argument('-r', '--recursive', action='store_true',
                        help='Search files recursively in all directories.')
    parser.add_argument('-f', '--files', nargs='*',
                        help='Specify the filenames to analyze (space-separated).')
    args = parser.parse_args()

    clang_tidy_config_path, extensions, build_dir = load_config()

    if args.files:
        files = args.files
    elif args.recursive:
        files = linters.find_files_recursively(extensions)
    else:
        files = linters.find_files_in_current_directory(extensions)

    start_time = time.time()

    if files:
        print(f"Found {len(files)} files to analyze.")
        linters.run_clang_tidy_on_files(files, clang_tidy_config_path, build_dir)
    else:
        print("No files found for analysis.")

    end_time = time.time()
    execution_time = end_time - start_time
    print(f"Execution time: {execution_time:.2f} seconds")

if __name__ == "__main__":
    run()

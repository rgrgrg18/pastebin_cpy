#!/usr/bin/env python3

import os
import json
import argparse
import linters

def load_config():
    config_dir_path = os.getenv('CODESTYLE_CONFIG_DIR_PATH')
    with open(config_dir_path + '/config.json', 'r') as config_file:
        config = json.load(config_file)


    clang_tidy_config_path = config_dir_path + '/.clang-tidy'
    extensions = config.get('extensions', [])

    return clang_tidy_config_path, extensions


def run():
    parser = argparse.ArgumentParser(description='Run clang-tidy on source files.')
    parser.add_argument('-r', '--recursive', action='store_true',
                        help='Search files recursively in all directories.')
    parser.add_argument('-f', '--files', nargs='*',
                        help='Specify the filenames to analyze (space-separated).')
    args = parser.parse_args()

    clang_tidy_config_path, extensions = load_config()

    if args.files:
        files = args.files
    elif args.recursive:
        files = linters.find_files_recursively(extensions)
    else:
        files = linters.find_files_in_current_directory(extensions)

    if files:
        print(f"Found {len(files)} files to analyze.")
        linters.run_clang_tidy_on_files(files, clang_tidy_config_path)
    else:
        print("No files found for analysis.")

if __name__ == "__main__":
    run()

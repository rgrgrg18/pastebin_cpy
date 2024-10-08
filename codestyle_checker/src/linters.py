import os
import subprocess
import glob

def find_files_recursively(extensions):
    files = []
    for ext in extensions:
        files.extend(glob.glob(f'**/*.{ext}', recursive=True))
    return files

def find_files_in_current_directory(extensions):
    files = []
    for ext in extensions:
        files.extend(glob.glob(f'*.{ext}'))
    return files

def run_clang_tidy_on_files(files, clang_tidy_config_path):
    for file in files:
        print(f"Analyzing file: {file}")

        command = ['clang-tidy', file, '--config-file=' + clang_tidy_config_path, '--quiet', '--', '-std=c++17']

        try:
            result = subprocess.run(command, check=True, text=True)
            print(result.stdout)
        except subprocess.CalledProcessError as e:
            print(f"Error while analyzing {file}:\n{e.stderr}")

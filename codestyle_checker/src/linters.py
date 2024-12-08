import os
import subprocess
import glob
import re

def find_files_recursively(extensions):
    files = []
    extensions = set(extensions)

    for root, dirs, filenames in os.walk('.'):
        dirs[:] = [d for d in dirs if 'test' not in d and 'build' not in d]

        for filename in filenames:
            ext = filename.split('.')[-1]
            if ext in extensions:
                files.append(os.path.join(root, filename))

    return files

def find_files_in_current_directory(extensions):
    files = []
    for ext in extensions:
        files.extend(glob.glob(f'*.{ext}'))
    return files

def run_clang_tidy_on_files(files, clang_tidy_config_path, build_dir):
    for file in files:
        print(f"\033[95mAnalyzing file:\033[0m {file}")

        command = [
            'clang-tidy',
            '--config-file=' + clang_tidy_config_path,
            '--quiet',
            '--system-headers=false',
        ]

        if len(build_dir) != 0:
            command.append('-p=' + build_dir)

        command.append(file)

        try:
            result = subprocess.run(command, check=True, text=True)
        except subprocess.CalledProcessError as e:
            continue

        print("\033[92mSuccess\033[0m")
#!/usr/bin/env python

import os
import argparse
import json
import os.path as path

# Absolute path where this script resides in
project_dir = path.dirname(path.realpath(__file__))

# Arguments parser
parser = argparse.ArgumentParser(description='Merge compile_commands.json in the project')
parser.add_argument('-a', '--arch', required=True, help='work with <ARCH>')
args = parser.parse_args()

# https://stackoverflow.com/a/57422761
def merge_json_files(files):
    result = list()

    for file in files:
        with open(file, 'r') as infile:
            result.extend(json.load(infile))

    with open('compile_commands.json', 'w') as outfile:
        json.dump(result, outfile)

def main():
    compile_commands = [f'{project_dir}/build/{args.arch}/compile_commands.json', ]

    (dirpath, dirnames, _) = next(os.walk(f'{project_dir}/third_party/build/{args.arch}'))

    for party in dirnames:
        compile_commands.append(f'{dirpath}/{party}/compile_commands.json')

    merge_json_files(compile_commands)

if __name__ == '__main__':
    main()

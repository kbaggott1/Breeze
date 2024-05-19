import os
import sys

EXPOSE_STR = "//!expose"

def clean_line(line: str) -> str:
    line = line.replace(EXPOSE_STR, '')
    line = line.replace(" {", ';')
    return line

def build_header(lines: list[str], file_name: str) -> list[str]:
    headers = []
    stack = []
    
    header_name = file_name.upper()
    header_name = header_name.replace('.', '_')

    for line in lines:
        if len(stack) > 0:
            if "{" in line:
                stack.append("{")
            elif "}" in line:
                stack.pop()
            headers.append(line + ('\n' if len(stack) == 0 else ''))

        if EXPOSE_STR in line:
            if "enum" in line or "union" in line or "struct" in line:
                headers.append(line.replace(EXPOSE_STR, ''))
                stack.append("{")
            else:
                headers.append(clean_line(line) + "\n")

    if len(headers) > 0:
        headers.insert(0, f"#ifndef {header_name}\n")
        headers.insert(1, f'#define {header_name}\n')
        headers.append("#endif\n")

    return headers

def main():
    source_files = {}
    header_files = {}

    if len(sys.argv) != 3:
        print(f"Usage: python {sys.argv[0]} path_to_source path_to_dest")
        print(f"Example: python {sys.argv[0]} ./source ./headers")
        return

    source_dir = os.path.relpath(sys.argv[1])
    destination_dir = os.path.relpath(sys.argv[2])
    
    for file in os.listdir(source_dir):
        if ".c" in file:
            source_files[file.replace(".c", ".h")] = os.getcwd() + "/source/" + file # TODO use source_dir instead of hard coding
    
    for file_name, file_path in source_files.items():
        with open(file_path) as file:
            lines = file.readlines()
            header = build_header(lines, file_name)
            if len(header) > 0:
                header_files[file_name] = header

        # TODO clean and write headers dir


if __name__ == "__main__":
    main()

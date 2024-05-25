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

    # TODO Ignore comments in structs unions and enums
    
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

def get_headers(source_files: dict) -> dict:
    header_files = {}
    for file_name, file_path in source_files.items():
        file_name = file_name.replace(".c", ".h")
        with open(file_path) as file:
            lines = file.readlines()
            header = build_header(lines, file_name)
            if len(header) > 0:
                header_files[file_name] = header
    
    return header_files

def write_headers_to_dest(destintation_path: str, header_files: dict):
    for file_name, header_content in header_files.items():
        with open(destintation_path + "/" + file_name, "w") as file:
            file.writelines(header_content)

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
            source_files[file] = os.getcwd() + "/" + source_dir + "/" + file
    
    header_files = get_headers(source_files)
    
    for file in os.listdir(destination_dir):
        if ".h" in file:
            os.remove(destination_dir+"/"+file)
    
    write_headers_to_dest(destination_dir, header_files)


if __name__ == "__main__":
    main()

import os

def main():
    source_files = []
    for file in os.listdir(os.getcwd() + "/source"):
        if ".c" in file:
            source_files.append(os.getcwd() + "/source/" + file)
    
    for file_path in source_files:
        with open(file_path) as file:
            lines = file.readlines()

        for line in lines:
            print(line)

if __name__ == "__main__":
    main()
# Scan directory and make comparison one by one
# Image file names shall be ordered regarding to time of the taken picture

import os
import sys

class style():
    BLACK = '\033[30m'
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    MAGENTA = '\033[35m'
    CYAN = '\033[36m'
    WHITE = '\033[37m'
    UNDERLINE = '\033[4m'
    RESET = '\033[0m'

if len(sys.argv) != 4:
    print(style.RED + "Expected count of paths parameters is 3.")
    print(style.YELLOW + "EXAMPLE: python3 direcotry_scan_and_compare.py directory/ conf.csv output/")
    quit()

iteration = 0
base = "empty"
for file in os.listdir(sys.argv[1]):
    print(file)
    if file.endswith(".bmp") or file.endswith(".BMP"):        
        iteration = iteration + 1

        print(iteration)
        if iteration >= 2:
            cmd = '../build/./aquamarine ' + sys.argv[1] + base + ' ' + sys.argv[1] + file + ' ' + sys.argv[2] + ' ' + sys.argv[3] + 'cmp' + str(iteration) + '.bmp'
            print(cmd)
            os.system(cmd)

        base = file

print(style.GREEN + "Directory: " + sys.argv[1] + "scan and compare complete.")    
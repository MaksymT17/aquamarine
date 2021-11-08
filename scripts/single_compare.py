import os
import sys

# expected input paths parameters are: base_img to_compare_img conf output
cmd = '../build/./aquamarine ' + sys.argv[1] + ' ' + sys.argv[2] + ' ' + sys.argv[3] + ' ' + sys.argv[4]
os.system(cmd)

import os, sys

cmd = '../build/./aquamarine '+sys.argv[1] + ' '+sys.argv[2] + ' ' + sys.argv[3] + ' ' + sys.argv[4]
os.system(cmd) # returns the exit status

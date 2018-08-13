# read a single character without pressing enter

import sys
import getch

print("Yes or no: ", end="")
sys.stdout.flush()
ans = getch.getche()
print("")

if ans.lower() == "y":
    print("yes")
elif ans.lower() == "n":
    print("no")
else:
    print("What??")

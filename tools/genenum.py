import re
import sys

if __name__ == "__main__":
    with open("/usr/include/linux/input.h") as f:
        for line in f:
            match = re.match(r"^#define " + sys.argv[2] + "_(.*)\t+((?:0x[0-9a-f]+)|(?:\d+))", line)
            if match:
                print "%s = (%s, %s)" % (match.group(1).strip(), sys.argv[1], match.group(2).strip())

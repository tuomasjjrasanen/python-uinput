#!/bin/sh

set -eu

python genenum.py 0x01 KEY > ../src/key.py
python genenum.py 0x01 BTN > ../src/btn.py
python genenum.py 0x02 REL > ../src/rel.py
python genenum.py 0x03 ABS > ../src/abs.py

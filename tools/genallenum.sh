#!/bin/sh

set -eu

python genenum.py 0x01 KEY > ../src/ev.py
python genenum.py 0x01 BTN >> ../src/ev.py
python genenum.py 0x02 REL >> ../src/ev.py
python genenum.py 0x03 ABS >> ../src/ev.py

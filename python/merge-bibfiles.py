"""
Merge two BibTeX files.
Removes duplicates identified by their bib key.
"""

import sys
from pathlib import Path

from bibtexparser.bparser import BibTexParser
from bibtexparser.bwriter import BibTexWriter
import bibtexparser

assert len(sys.argv) == 4

if Path(sys.argv[3]).exists():
    print("Output file exists")
    sys.exit(1)

# read input
with open(sys.argv[1], "r") as f:
    a = bibtexparser.load(f, BibTexParser(common_strings=True))

with open(sys.argv[2], "r") as f:
    b = bibtexparser.load(f, BibTexParser(common_strings=True))


# merge data bases
merged = a
merged.comments.extend(b.comments)
merged.preambles.extend(b.preambles)
merged.strings.update(b.strings)

known = set(merged.entries_dict.keys())
for key, entry in b.entries_dict.items():
    if key not in known:
        known.add(key)
        merged.entries.append(entry)

# write to file
writer = BibTexWriter()
writer.indent = "  "
writer.add_trailing_comma = True
writer.display_order = ["author", "title"]

with open(sys.argv[3], "w") as f:
    f.write(writer.write(merged))

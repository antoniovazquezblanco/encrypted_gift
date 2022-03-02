#!/bin/bash

input="$1"
output="$2"

echo "$output will decompress to $input"

echo -ne 'outfile = "' > gen.py
basename $input | tr -d '\n' >> gen.py
echo -ne '"\n\n' >> gen.py

echo -ne 'payload = "' >> gen.py
base64 $input -w 0 >> gen.py
echo -ne '"\n\n' >> gen.py

cat gen.py > final.py
cat main.py >> final.py

# pip install python-obfuscator
~/.local/bin/pyobfuscate -i final.py -r True

echo "#!/usr/bin/env python3" | cat - final.py > runme.py

tar -czvf $output runme.py
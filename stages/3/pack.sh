#!/bin/bash

input="$1"
output="$2"

echo "#!/usr/bin/env python3" > gen.py
echo -ne 'outfile = "' >> gen.py
basename $output | tr -d '\n' >> gen.py
echo -ne '"\n\n' >> gen.py

echo -ne 'payload = "' >> gen.py
base64 $input | tr -d '\n' >> gen.py
echo -ne '"\n\n' >> gen.py

cat gen.py > final.py
cat main.py >> final.py

# pip install python-obfuscator
~/.local/bin/pyobfuscate -i final.py -r True

cp final.py $output

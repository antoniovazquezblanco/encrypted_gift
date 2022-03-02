#!/bin/bash

input="$1"
output="$2"

echo "$output will decompress to $input"

echo -ne '$outfile = "' > content.ps1
basename $input | tr -d '\n' >> content.ps1
echo -ne '"\n\n' >> content.ps1

echo -ne '$payload = "' >> content.ps1
base64 $input -w 0  >> content.ps1
echo -ne '"\n\n' >> content.ps1

cat content.ps1 > final.ps1
cat main.ps1 >> final.ps1

python PyFuscation/PyFuscation.py -f -v -p --ps ./final.ps1
cp $(date +"%m%d%Y")*/*.ps1 best_prog_lang.ps1
rm -rf $(date +"%m%d%Y")*
tar -czvf $output best_prog_lang.ps1

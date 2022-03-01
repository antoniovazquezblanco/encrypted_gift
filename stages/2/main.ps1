$output = [System.Convert]::FromBase64String($payload)
[io.file]::WriteAllBytes($outfile,$output)
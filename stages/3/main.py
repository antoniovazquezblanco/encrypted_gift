
import sys
import base64

print("Let the gaaaaames begin!")
 
if not len(sys.argv) == 2:
    print("Syntax: script.py <Password>")
    exit()
    
password = ""
intPass = "83,117,112,101,114,83,101,99,117,114,101,80,97,115,115,119,111,114,100,87,104,105,99,104,67,97,110,116,66,101,67,114,97,99,107,101,100,87,105,116,104,111,117,116,66,108,97,99,107,109,97,103,105,99"

intPass = intPass.split(",")

for c in intPass:
    password = password + chr(int(c))

if sys.argv[1] == password:
    print("Wow, that's a great beginning!")
    with open(outfile, 'wb') as file_to_save:
        decoded_image_data = base64.decodebytes(payload.encode('utf-8'))
        file_to_save.write(decoded_image_data)
else:
    print("Nope!")
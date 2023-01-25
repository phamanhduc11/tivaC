import sys
import os

def bit_mask(high, low):
    if (low > high):
        high += low
        low = high - low
        high -= low
    if ((high == low) and (high == 0)):
        return 1
    if ((high > 31) or (high == 0) or (low > 31)):
        return 0x10101010
    return (((1 << (high +1)) - 1) - ((1 << low) -1))

def output(inputFile, cmpFormat):
    start   = cmpFormat[0]
    end     = cmpFormat[1]
    found   = 0
    for line in inputFile:
        if start in line:
            found |= 1
        if end in line:
            found |= 2
    if (found == 0 or found == 1 or found == 2):
        return False
    inputFile.seek(0)
    for line in inputFile:
        if start in line:
            found = 1
            continue
        if end in line:
            found = 2
            return True
        if (found == 1):
            if("_REG" in line): 
                print()
                print(line.replace("\n",""))
            if("#define" in line):
                splitStr = line.split(',')
                high = splitStr[0][splitStr[0].find("(bitmask("):].replace('(bitmask(', '')
                low = splitStr[1].replace("))","")
                concatStr = '0x%08X' % bit_mask(int(high),int(low))
                concatStr = splitStr[0][:splitStr[0].find("(bitmask(")] + concatStr
                print(concatStr)

    return True






headerPath = os.path.join(os.getcwd(), "../src/INC")
inputFileName = "sys.h"
inputFile = open(os.path.join(headerPath, inputFileName), 'r')
if (not inputFile.readable()):
    print("Openfile: error")
    inputFile.close()
    exit()

cmpFormat = "// Registers Mask //"
cmpEndFormat = "// End Registers Mask //"
fmt = [cmpFormat, cmpEndFormat]


if (output(inputFile, fmt)):
    print("Found")
else:
    print("Not Found!")

# file = open()
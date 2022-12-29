import sys
import os

attributes = [
    "Offset",
    "Name",
    "Type",
    "Reset"
]

typeRegClass = "PHY_Register"
typeRegInfo = "regBitInfo"
pre_concate_1 = "GPTM_"

def output(filestream, attribute):
    for line in filestream:
        splits = line.split()
        if (attribute == "Offset"):
            print(splits[0])
        elif (attribute == "Name"):
            print(splits[1])
        elif (attribute == "Type"):
            print(splits[2])
        elif (attribute == "Reset"):
            print(splits[3])
        else:
            return False
            print("Error attribute input")
    return True

def cpp_define_reg_prepare(filestream,outputstream):
    outputstream.write("Offset data for c file\n")
    for line in filestream:
        splits = line.split()
        outputstream.write("#define %s%s_OFF %s\n" % (pre_concate_1, splits[1], splits[0]))
    outputstream.write("\n")

def cpp_definePHYRegister_prepare(filestream, outputstream):
    outputstream.write("cpp_definePHYRegister_prepare\n")
    for line in filestream:
        splits = line.split()
        outputstream.write("static %s %s;\n" % (typeRegClass, splits[1]))
    outputstream.write("\n")

def cpp_regBitInfo_prepare(filestream, outputstream):
    outputstream.write("regBitInfo data preare\n")
    for line in filestream:
        splits = line.split()
        outputstream.write("%s %s_Info[] {\n" % (typeRegInfo, splits[1]))
        outputstream.write("    {\"NULL\", 32, 32}\n")
        outputstream.write("};\n\n")
    outputstream.write("\n")

def cpp_bitList_prepare(filestream, outputstream):
    outputstream.write("cpp_bitList_prepare\n")
    for line in filestream:
        splits = line.split()
        outputstream.write("PHY_Reg_Bit %s_bitList[sizeof(%s_Info)/sizeof(%s_Info[0]) - 1];\n" % (splits[1], splits[1], splits[1]))


    outputstream.write("\n")

def cpp_moduleContructor_prepare(filestream, outputstream):
    outputstream.write("cpp_moduleContructor_prepare\n")
    for line in filestream:
        splits = line.split()
        outputstream.write("%s.update(\"%s\", this->baseModuleAddr, %s%s_OFF, %s_bitList, %s_Info);\n" % (splits[1], splits[1], pre_concate_1, splits[1], splits[1], splits[1]))
    outputstream.write("\n")

# Use for printing correspond data in Register Map
readFileName = "timer.registers"
curFilePath = os.path.dirname(os.path.abspath(__file__))
filePath = os.path.join(curFilePath, "../registers_info")
inputFile = open(os.path.join(filePath, readFileName), 'r')

outputFilePath = os.path.join(curFilePath, "output/test")
outputFile = open(outputFilePath, "w")
if (not inputFile.readable()):
    print("Openfile: error")
    inputFile.close()
    exit()

cpp_define_reg_prepare(inputFile, outputFile)
inputFile.seek(0)
cpp_definePHYRegister_prepare(inputFile, outputFile)
inputFile.seek(0)
cpp_regBitInfo_prepare(inputFile, outputFile)
inputFile.seek(0)
cpp_bitList_prepare(inputFile, outputFile)
inputFile.seek(0)
cpp_moduleContructor_prepare(inputFile, outputFile)
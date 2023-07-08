#!/usr/bin/python3

import sys;

booloaderData = open("bootloader.bin", "rb").read()
partitionData = open("partition-table.bin", "rb").read()
networkData = open("network_adapter.bin", "rb").read()

# 0x0 bootloader.bin 0x8000 partition-table.bin 0x10000 network_adapter.bin


# calculate the output binary size, app offset 
outputSize = 0x10000 + len(networkData)
if (outputSize % 1024):
	outputSize += 1024 - (outputSize % 1024)

# allocate and init to 0xff
outputData = bytearray(b'\xff') * outputSize

# copy data: bootloader, partitions, app
for i in range(0, len(booloaderData)):
	outputData[0x0000 + i] = booloaderData[i]

for i in range(0, len(partitionData)):
	outputData[0x8000 + i] = partitionData[i]

for i in range(0, len(networkData)):
	outputData[0x10000 + i] = networkData[i]


outputFilename = "ESP32-C3.bin"
if (len(sys.argv) > 1):
	outputFilename = sys.argv[1]

# write out
with open(outputFilename,"w+b") as f:
	f.seek(0)
	f.write(outputData)

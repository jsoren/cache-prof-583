import subprocess as sp
import re
import math
import sys

if (len(sys.argv) != 3):
	print("Error: incorrect command line arguments")
	print("Usage: python " + sys.argv[0] + " input_file output_file")
	sys.exit()

# the filepath to the cache simulator. Change as needed
sim_location = '~/proj583/cpu-cache-simulator/cpu-cache-simulator/simulator.py'

# the arguments to the cache simulator. Change as needed. Currently using cache size 2^13 = 8192, block size 2^6 = 64, LRU replacement, and write-back.
# These arguments are based on size of these values in eecs583 machines
cache_size = 13 # 2^13
block_size = 6 # 2^6
sim_args = str(cache_size) + ' ' + str(block_size) + ' 0 LRU WB'

# The commands that will be fed to the cache simulator
command = ''

# The list of all memory accesses
memaccesses = []
# The corresponding list of types of access.
# 'l' for load and 's' for store
memtypes = []

# We need to use simulated memory addresses, but they need to be the same distance from each other
# This is the minimal and maximal address of any memory operation
# we'll shift all memory accesses to start at 0
minAddr = -1
maxAddr = -1

# Get file to read from and file to write from
infile = sys.argv[1]
outfile = sys.argv[2]

# This loop checks for the lowest and highest memory addresses
with open(infile, 'r') as f:	
	for line in f.readlines():
		# we only care about lines that look like:
		#  store # to #
		# or:
		#  load # from #
		words = line.split()
		if (len(words) == 4):
			if words[0] == 'store' or words[0] == 'load':
				addr = int(words[3], 0)
				if minAddr == -1 or addr < minAddr:
					minAddr = addr
				if addr > maxAddr:
					maxAddr = addr

# the block size is 64, so reduce to a multiple of 64
minAddr -= minAddr % 64

print("Address range:", maxAddr - minAddr)

# Rescale each memory address to fit in a range
# where 0 is the minimum address
with open(infile, 'r') as f:	
	for line in f.readlines():
		# we only care about lines that look like:
		#  store # to #
		# or:
		#  load # from #
		words = line.split()
		if (len(words) == 4):
			if words[0] == 'store':
				memaccesses.append(words[1])
				memtypes.append('s')

				idx = int(words[3], 0) - minAddr

				# For simulation purposes, we don't care about the actual memory contents,
				# so we just write 0 each time
				command += 'write ' + str(idx) + ' 0\nstats\n'

			if words[0] == 'load':
				memaccesses.append(words[1])
				memtypes.append('l')

				idx = int(words[3], 0) - minAddr

				command += 'read ' + str(idx) + '\nstats\n'

# This command will end the simulator without error
command += 'quit'

# We want to start the simulator with just enough memory for all of our accesses
mem = int(math.ceil(math.log(maxAddr - minAddr, 2)))

with open('command.txt', 'w') as c:
	c.write(command)

print('Simulating results...')
# Feed all commands into the simulator
result = sp.check_output('cat command.txt | python3 ' + sim_location + ' ' + str(mem) + ' ' + sim_args, shell=True)
result = result.decode('utf-8')

# Extract all stat output from the result
pattern = 'Hits: [0-9]+ \\| Misses: [0-9]+\\n'
matches = re.findall(pattern, result)

# Stats should have been printed exactly once for each memory access
if len(matches) != len(memaccesses):
	print("Error parsing results")
	sys.exit()

# Keep track of the total number of hits and misses so far to determine if each new access was a hit or a miss
# stored as [hits, misses]
hitmiss = [0, 0]

# Keep track of the number of hits and misses for each memory access
# stored as {memory_access: [hits, misses]}
mem_hitmiss = {}
for acc in memaccesses:
	mem_hitmiss[acc] = [0, 0]

# Keep track of the number of hits and misses for loads and stores separately
type_hitmiss = {}
type_hitmiss['l'] = [0, 0]
type_hitmiss['s'] = [0, 0]

for i, match in enumerate(matches):

	# each match looks like:
	#  Hits: # | Misses: #
	filtered = match.split()
	hit = int(filtered[1])
	miss = int(filtered[4])
	
	memaccess = memaccesses[i]
	memtype = memtypes[i]

	if hit > hitmiss[0]:
		hitmiss[0] = hit
		mem_hitmiss[memaccess][0] += 1
		type_hitmiss[memtype][0] += 1

	if miss > hitmiss[1]:
		hitmiss[1] = miss
		mem_hitmiss[memaccess][1] += 1
		type_hitmiss[memtype][1] += 1

# Print total hits and misses
# Also print hits and misses for loads and stores
print("Total hits:", hitmiss[0])
print("Total misses:", hitmiss[1])
print("Hit ratio:", hitmiss[0] / (hitmiss[0] + hitmiss[1]), "\n")

print("Load hits:", type_hitmiss['l'][0])
print("Load misses:", type_hitmiss['l'][1])
print("Hit ratio:", type_hitmiss['l'][0] / (type_hitmiss['l'][0] + type_hitmiss['l'][1]), "\n")

print("Store hits:", type_hitmiss['s'][0])
print("Store misses:", type_hitmiss['s'][1])
print("Hit ratio:", type_hitmiss['s'][0] / (type_hitmiss['s'][0] + type_hitmiss['s'][1]), "\n")

# print profile as:
#  memory_access hits misses
# for all memory acceses
with open(outfile,'w') as hm:
	#print("Memory_access_ID Hits Misses")
	hm.write("Memory_access_ID Hits Misses\n")
	for k in mem_hitmiss.keys():
		#print(k, mem_hitmiss[k][0], mem_hitmiss[k][1])
		hm.write(str(k) + " "  + str(mem_hitmiss[k][0]) + " "  + str(mem_hitmiss[k][1]) + "\n")

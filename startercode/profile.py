import subprocess as sp
import re
import math

# the filepath to the cache simulator. Change as needed
sim_location = '~/proj583/cpu-cache-simulator/cpu-cache-simulator/simulator.py'

# the arguments to the cache simulator. Change as needed. Currently using cache size 2^4, block size 2^0, LRU replacement, and write-back
sim_args = '4 0 0 LRU WB'

# The commands that will be fed to the cache simulator
command = ''

# The list of all memory accesses
memaccesses = []

# each memory address is reassigned to a simpler address for simulation purposes
# This dictionary maps true addresses to simulated ones
addresses = {}
addrIdx = 0

with open('prof.txt', 'r') as f:	
	for line in f.readlines():
		# we only care about lines that look like:
		#  store # to #
		# or:
		#  load # from #
		words = line.split()
		if (len(words) == 4):
			if words[0] == 'store':
				memaccesses.append(words[1])

				idx = 0
				if (words[3] in addresses.keys()):
					idx = addresses[words[3]]
				else:
					idx = addrIdx
					addresses[words[3]] = idx
					addrIdx += 1

				# we just write 0 to the memory address. We don't care about writing new data.
				command += 'write ' + str(idx) + ' 0\nstats\n'

			if words[0] == 'load':
				memaccesses.append(words[1])

				idx = 0
				if (words[3] in addresses.keys()):
					idx = addresses[words[3]]
				else:
					idx = addrIdx
					addresses[words[3]] = idx
					addrIdx += 1

				command += 'read ' + str(idx) + '\nstats\n'

# This command will end the simulator without error
command += 'quit'

# We want to start the simulator with just enough memory for all of our accesses
mem = int(math.ceil(math.log(addrIdx, 2)))

# Feed all commands into the simulator
result = sp.check_output('echo -e "' + command + '" | python3 ' + sim_location + ' ' + str(mem) + ' ' + sim_args, shell=True)
result = result.decode('utf-8')

# Extract all stat output from the result
pattern = 'Hits: [0-9]+ \\| Misses: [0-9]+\\n'
matches = re.findall(pattern, result)

# Stats should have been printed exactly once for each memory access
if len(matches) != len(memaccesses):
	print("Error parsing results")

# Keep track of the total number of hits and misses so far to determine if each new access was a hit or a miss
# stored as [hits, misses]
hitmiss = [0, 0]

# Keep track of the number of hits and misses for each memory access
# stored as {memory_access: [hits, misses]}
mem_hitmiss = {}
for acc in memaccesses:
	mem_hitmiss[acc] = [0, 0]

for i, match in enumerate(matches):

	# each match looks like:
	#  Hits: # | Misses: #
	filtered = match.split()
	hit = int(filtered[1])
	miss = int(filtered[4])
	
	memaccess = memaccesses[i]

	if hit > hitmiss[0]:
		hitmiss[0] = hit
		mem_hitmiss[memaccess][0] += 1

	if miss > hitmiss[1]:
		hitmiss[1] = miss
		mem_hitmiss[memaccess][1] += 1

# print profile as:
#  memory_access hits misses
# for all memory acceses
print("Memory_access_ID Hits Misses")
for k in mem_hitmiss.keys():
	print(k, mem_hitmiss[k][0], mem_hitmiss[k][1])

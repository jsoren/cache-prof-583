import sys

if (len(sys.argv) != 3):
	print("Error: incorrect command line arguments")
	print("Usage: python " + sys.argv[0] + " input_file output_file")
	sys.exit()

infile = sys.argv[1]
outfile = sys.argv[2]

prof = []
with open(infile, 'r') as p:
	prof = p.readlines()

loads = []
stores = []
for line in prof:
	words = line.split()
	if (len(words) > 0):
		if (words[0] == 'load'):
			loads.append(line)
		if (words[0] == 'store'):
			stores.append(line)

instr = loads + stores

with open(outfile, 'w') as n:
	n.writelines(instr)
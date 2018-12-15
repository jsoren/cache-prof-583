# Rearrange script.
# Tries to move loads to be close to stores

import sys

def moveload(prof, index):
	moveable = True
	i = 10
	while moveable and i > 0:
		if index == 0:
			moveable = False
			break
		
		line = prof[index]
		prevline = prof[index-1]

		words = line.split()
		prevwords = prevline.split()

		if len(words) != 4 or len(prevwords) != 4:
			moveable = False
			break

		if (words[3] == prevwords[3]):
			moveable = False
			break

		prof[index - 1] = line
		prof[index] = prevline

		index -= 1
		i -= 1
	return

if (len(sys.argv) != 3):
	print("Error: incorrect command line arguments")
	print("Usage: python " + sys.argv[0] + " input_file output_file")
	sys.exit()

infile = sys.argv[1]
outfile = sys.argv[2]

prof = []
with open(infile, 'r') as p:
	prof = p.readlines()


for i, line in enumerate(prof):
	words = line.split()
	if (len(words) == 4):
		if (words[0] == 'load'):
			moveload(prof, i)


with open(outfile, 'w') as n:
	n.writelines(prof)
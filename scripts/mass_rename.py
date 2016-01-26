import sys
import os

def main(path):
	i = 0
	print (path)
	tree = os.walk(path)
	for (path, dirs, file) in tree:
		for filename in file:
			print (filename)
			# TODO: place rename logic here
		 	#os.rename(os.path.join(path, filename), os.path.join(path,  newnames[i]+".flv"))
			
			i+=1
	print('{0} files renamed'.format(i))





###################################
if __name__ == "__main__":
    try:
        path = sys.argv[1]
    except:
        print("Usage: python mass_rename.py <path>")
        sys.exit(1)
    main(path)

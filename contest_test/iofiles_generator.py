import random

class generation_settings():
	''' Contains settings for all generators,
	which uses only nessesary of them
	'''
	__init__(self):
		self.start = 0
		self.end = 100
		self.step = 1
		self.alphabetical = False
		self.distribution = None
		

class random_sequence():
	''' Basic class for different random sequences generation '''
	pass

class plain_random_sequence(random_sequence):
	''' Class for plain random sequence generation '''
	def __init__(self):
		self.start = 0
		self.end = 10
		self.step = 1

	def generate(self):
		''' Produce function for plain rendom sequence
		Return plain array
		'''
		rnd_array = []
		gen = self.start
		for i in range(1, size):
			gen = random.randrange(self.start, self.end, self.step)
			rnd_array.append(gen)
		return rnd_array

class monotone_sequence(random_sequence):
	def __init__(self, acsend = True):
		self.start = 0
		self.end = 100
		self.step = 1

	def generate(self):
		''' Produce function for monotone (ascending or descending )
		random sequence.
		Return plain array with given parameters
		'''
		rnd_array = []
		gen = self.start
		for i in range(1, size):
			# todo linear distribution
			gen = random.randrange(gen, self.end, self.step)
			rnd_array.append(gen)
		write_line(rnd_array)

class normal_distribution_sequence(random_sequence):
	# todo : does it as same as plain?
	pass

class sequence_with_symbols(random_sequence):
	''' Random sequence contains not only numbers, 
	but some additional symbols, as from the given list, 
	so as from all alphabetical symbols'''
        def __init__(self, additional_symbols):
		pass

class unit_tester_generator():
	def __init__(self, logic_func, line_produce_functions, ):
		''' Constructor
		param lines - how much lines to generate
		param logic_function - function for special logic how much 
		sequences to generate and what dependences do they have
		param line_produce_functions - function object arrays - one per logic condition
		'''
		self.symbols = ('+', '-', '*')
		self.numbers = True
		self.string = False
		#self.program_name = program_name
		self.thefile = open("input.txt", 'w+')

	def __del__(self):
		self.thefile.close()

	def write_line(self, thelist):
		for item in thelist:
  			self.thefile.write("%s " % item)
	
	def write_file(self, array_list):
		# todo check type of array_list as a list of lists
		for line in thelist:
			self.write_line(line)

if __name__ == "__main__":
	generator = unit_tester_generator()

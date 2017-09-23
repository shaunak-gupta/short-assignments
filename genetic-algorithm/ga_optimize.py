import numpy as np
import sys
import random
import matplotlib.pyplot as plt
from operator import itemgetter

if len(sys.argv) > 4:
    print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " <NUM_ITERATIONS> <REPETETIONS> <POPULATION_SIZE>\n"
    quit()

def get_string_val(string):
	ones = np.where(np.array(string) == 1)[0]
	values = [2**(4-x) for x in ones]
	return sum(values)

def get_function_val(input):
	if input < 2*np.pi:
		return np.sin(input) + 6
	if input < 4*np.pi:
		return 2*np.sin(input) + 6
	if input < 6*np.pi:
		return 3*np.sin(input) + 6
	if input < 8*np.pi:
		return 4*np.sin(input) + 6
	if input < 10*np.pi:
		return 5*np.sin(input) + 6
	if input < 32:
		return np.sin(input) + 6

def get_fitness(string):
	x = get_string_val(string)
	return get_function_val(x)

def get_random_string(bit_length):
	r_val = random.randint(0, 2**bit_length - 1)
	string = []
	for i in range(bit_length):
		string.append(r_val & 1)
		r_val = r_val >> 1
	return string

def get_random_population(population_size, bit_length):
	population = []
	for i in range(population_size):
		string = get_random_string(bit_length)
		fitness = get_fitness(string)
		population.append((fitness, string))
	return population

def get_stochastic_element(population):
	r_val = np.random.random()
	fitness_vals = [x[0] for x in population]
	fitness_sum = sum(fitness_vals)
	fitness_vals = [x*1.0/fitness_sum for x in fitness_vals]
	val = 0
	j = -1
	while val < r_val:
		j += 1
		val += fitness_vals[j]
	return population[j]

def select_population(current_population):
	new_population = []
	for i in range(len(current_population)):
		new_population.append(get_stochastic_element(current_population))
	return new_population


def perform_crossover(parent1, parent2):
	child1 = parent1[:]
	child2 = parent2[:]
	crossover_point = np.random.randint(0, len(parent1))
	child1[0:crossover_point] = parent2[0:crossover_point]
	child2[0:crossover_point] = parent1[0:crossover_point]
	return child1, child2

def crossover_selection(current_population, probability_crossover):
	i=0
	while i < len(current_population):
		parent1 = current_population[i]
		parent2 = current_population[i+1]
		if np.random.random() < probability_crossover:
			child1, child2 = perform_crossover(parent1[1], parent2[1])
			child1_fitness = get_fitness(child1)
			child2_fitness = get_fitness(child2)
			current_population[i] = (child1_fitness, child1) 
			current_population[i+1] = (child2_fitness, child2)
		i += 2
	return current_population

def get_probability_mutation(current_iteration, max_iteration):
	if current_iteration < max_iteration/3:
		return 0.49 - (0.49-1.0/bit_length)*current_iteration*3/max_iteration
	if current_iteration < 2*max_iteration/3:
		current_iteration -= max_iteration/3
		return 1.0/bit_length + (0.49-1.0/bit_length)*current_iteration*3/max_iteration
	current_iteration -= 2*max_iteration/3
	return 0.49 - (0.49-1.0/bit_length)*current_iteration*3/max_iteration


def population_mutation(population, probability_mutation):
	new_population = []
	for i in range(len(population)):
		chromosome = []
		for j in range(len(population[i][1])):
			if np.random.random() < probability_mutation:
				chromosome.append(1-(population[i][1][j]))
			else:
				chromosome.append(population[i][1][j])
		new_population.append((get_fitness(chromosome), chromosome))
	return new_population

def print_population(population):
	for i in population:
		print_chromosome(i)
		
def print_chromosome(chromosome):
	print chromosome[1], ": ", chromosome[0]
	print "Value: ", get_string_val(chromosome[1])


def genetic_algorithm(init_population, num_generations):
	current_generation = 0
	current_population = init_population
	best = []
	mean = []
	worst = []
	prob_mutation_list = []

	while current_generation<num_generations:
		best.append(max(current_population, key=itemgetter(0))[0])
		best_chromosome = max(current_population, key=itemgetter(0))
		worst.append(min(current_population, key=itemgetter(0))[0])
		mean.append(np.mean([x[0] for x in current_population]))

		next_population = select_population(current_population)
		next_population = crossover_selection(next_population, probability_crossover)

		probability_mutation = get_probability_mutation(current_generation, num_generations)
		prob_mutation_list.append(probability_mutation)
		next_population = population_mutation(next_population, probability_mutation)

		if max(next_population, key=itemgetter(0))[0] < best_chromosome[0]:
			replaced = random.randint(0, len(next_population)-1)
			next_population[replaced] = best_chromosome

		current_population = next_population
		current_generation += 1

	return max(current_population, key=itemgetter(0)), best, worst, mean, prob_mutation_list

num_generations = int(sys.argv[1]) if len(sys.argv) > 1 else 300
num_restarts = int(sys.argv[2]) if len(sys.argv) > 2 else 10
population_size = int(sys.argv[3]) if len(sys.argv) > 3 else 10

probability_crossover = 0.85
bit_length = 15

best_solution = (0,[])
for i in range(num_restarts):
	population = get_random_population(population_size, bit_length)
	for j in range(10):
		solution, best_fitness, worst_fitness, mean_fitness, prob_mutation_list = genetic_algorithm(population, num_generations)
		print "Population :", i+1, ", Run: ", j+1, " -> ", (get_string_val(solution[1]), solution[0])
		if solution[0] > best_solution[0]:
			best_solution = solution

	plt.plot(best_fitness)
	plt.pause(1)


print "Best solution string is ", best_solution[1]
print "Corresponding solution is", get_string_val(best_solution[1])
print "Fitness (Function value) is ", best_solution[0]

plt.figure(1)
plt.subplot(411)
plt.plot(best_fitness)

plt.subplot(412)
plt.plot(worst_fitness)

plt.subplot(413)
plt.plot(mean_fitness)


plt.subplot(414)
plt.plot(prob_mutation_list)

plt.show()
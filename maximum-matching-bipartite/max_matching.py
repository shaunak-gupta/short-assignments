import numpy as np
from cylp.cy import CyClpSimplex
from cylp.py.modeling.CyLPModel import CyLPArray
import sys

def main():
	if len(sys.argv) != 2:
		print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " <INPUT_FILE>\n"
		quit()
	infile = sys.argv[1]
	with open(infile) as f:
		m, n = [int(x) for x in next(f).split()]		# m = number of points in X, n = number of points in Y
		e = int(next(f))								# e = number of edges
		incidence_matrix = np.zeros((m,n))
		edge_matrix = np.zeros((m+n,e))
		weights = []
		i = 1
		for line in f:
			a,b,weight = [int(x) for x in line.split()]
			if a>m or b>n:
				print "Invalid vertex id. Aborting!"
				quit()
			incidence_matrix[a-1][b-1] = weight
			edge_matrix[a-1][i-1] = 1
			edge_matrix[m+b-1][i-1] = 1
			weights.append(weight)
			i += 1
		print "Adjacency Matrix:"
		print incidence_matrix
		print "Edge Matrix:"
		print edge_matrix
		print "Weights:"
		print weights
		s = CyClpSimplex()

		# Add variables
		x = s.addVariable('x', e)

		# Create coefficients and bounds
		A = np.matrix(edge_matrix[0:m])			# vertices corresponding to the set X (Left)
		B = np.matrix(edge_matrix[m:])			# vertices corresponding to the set Y (Right)
		a = CyLPArray(np.ones(m))
		b = CyLPArray(np.ones(n))

		# Add constraints
		s += A * x <= a					# all vertices in set X must be perfectly matched
		s += B * x <= b					# matching should be valid
		s += 0 <= x <= 1

		# Set the objective function
		s.objective = CyLPArray(np.array(weights) * -1) * x

		# Solve using primal Simplex
		s.primal()
		print s.primalVariableSolution['x']
		for i in range(len(s.primalVariableSolution['x'])):
			if s.primalVariableSolution['x'][i] > 1e-3:
				print "Edge ", 1+i, ": Weight ", weights[i]


if __name__ == "__main__":
	main()
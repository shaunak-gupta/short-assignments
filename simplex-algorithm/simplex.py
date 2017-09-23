import numpy as np
import sys

def get_initial_tableau(m, n, objective, matrix):
	slack_variables = m 			# number of slack variables = number of constraints
	if np.all(matrix[:,-1]>=0):
		tableau = np.zeros((m+1,slack_variables+n+1))
		tableau[0] = np.concatenate([np.zeros(1), objective*-1, np.zeros(slack_variables)])
		tableau[1:,0] = matrix[:,-1]
		tableau[1:,1:n+1] = matrix[:,0:-1]
		tableau[1:,1+n:] = np.identity(slack_variables)
		basis = range(n+1,n+slack_variables+1)
		print "Initial basic feasible solution"
		return tableau, basis
	else:
		# generate auxilliary lp to find an initial basic feasible solution
		tableau = np.zeros((m+1,slack_variables+n+2))			# extra auxilliary variable
		tableau[0] = np.concatenate([np.zeros(1+n+slack_variables), [1]])
		tableau[1:,0] = matrix[:,-1]
		tableau[1:,1:n+1] = matrix[:,0:-1]
		tableau[1:,1+n:-1] = np.identity(slack_variables)
		tableau[1:,-1] = np.ones(m)*-1
		basis = range(n+1,n+slack_variables+1)

		entering_variable = slack_variables+n+1
		leaving_varible = 1+np.argmin(matrix[:,-1])
		print "Generating auxilliary LP"
		print_tableau(tableau, basis)
		print "Initial Pivot for a basic feasible solution"
		tableau, basis = pivot(tableau, entering_variable, leaving_varible, basis)
		print_tableau(tableau, basis)
		tableau, basis, flag = perform_simplex(tableau, basis)
		if (tableau[0][0] != 0):
			return None, None
		tableau = tableau[:,0:-1]			# drop the last column for auxilliary variable
		# update the original objective function in tableau
		tableau[0] = np.concatenate([np.zeros(1), objective*-1, np.zeros(slack_variables)])
		for i in range(len(basis)):
			tableau, basis = pivot(tableau, basis[i], i+1, basis)
		print "Initial basic feasible solution for the original problem"
		return tableau, basis


def get_entering_variable(tableau):
	obj = tableau[0,1:]
	if np.all(obj>=0):
		return 0
	return 1+np.argmax(obj<0)

def get_leaving_variable(tableau, entering_variable):
	matrix = tableau[1:]
	coefficients = matrix[:,entering_variable]
	values = matrix[:,0]
	minimum = float("inf")
	index = 0
	for i in range(len(coefficients)):
		if coefficients[i] > 0:
			bound = values[i]/coefficients[i]
			if bound >= 0 and bound < minimum:
				minimum = bound
				index = i
	if minimum == float("inf"):
		return 0
	return 1+index
	#bounds = np.divide(matrix[:,0], coefficients)
	#if (np.all(bounds<=0)):
	#	return 0
	#m = min(i for i in bounds if i>0)
	#return 1+np.argmax(bounds==m)

def pivot(tableau, entering_variable, leaving_varible, basis):
	pivot_element = tableau[leaving_varible][entering_variable]
	tableau[leaving_varible] = tableau[leaving_varible]/pivot_element
	for i in range(tableau.shape[0]):
		if i != leaving_varible:
			tableau[i] = tableau[i] - tableau[leaving_varible]*tableau[i][entering_variable]
	basis[leaving_varible-1] = entering_variable
	return tableau, basis

def print_tableau(tableau, basis):
	print "Tableau:"
	print tableau
	print "Basis:"
	print basis

def perform_simplex(tableau, basis):
	flag = True
	while True:
		print_tableau(tableau, basis)
		entering_variable = get_entering_variable(tableau)
		if (entering_variable == 0):
			break
		leaving_varible = get_leaving_variable(tableau, entering_variable)
		if (leaving_varible == 0):
			flag = False
			break
		print "\n\nx", entering_variable, " enters, x", basis[leaving_varible-1], " leaves the basis"
		tableau, basis = pivot(tableau, entering_variable, leaving_varible, basis)
	return tableau, basis, flag

def main():
	if len(sys.argv) != 2:
		print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " <INPUT_FILE>\n"
		quit()
	infile = sys.argv[1]
	with open(infile) as f:
		m, n = [int(x) for x in next(f).split()]		# m = number of constraints, n = number of variables
		objective = np.array([float(x) for x in next(f).split()])	# read the coefficients of variables in objective function
		if len(objective) != n:
			print "Not all variables' coefficients in the objective function provided. Aborting!\n"
			quit()
		matrix = np.array([[float(x) for x in line.split()] for line in f])
	
	if len(matrix.shape) != 2 or matrix.shape[0] != m or matrix.shape[1] != n+1:
		print "Size of the constraint matrix is not correct. Aborting!\n"
		quit()

	tableau, basis = get_initial_tableau(m,n,objective,matrix)
	if (basis == None):
		print "The problem is infeasible. Aborting!"
		quit()
	tableau, basis, flag = perform_simplex(tableau, basis)
	if (flag):
		print "Optimal value of ", tableau[0][0], " found. Solution: "
		for i in range(len(basis)):
			print "x", basis[i], "= ", tableau[i+1][0]
	else:
		print "Problem is unbounded."

if __name__ == "__main__":
	main()
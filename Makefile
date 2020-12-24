all: pg

pg:
	gcc random_number.c -pg -o ran.o -lm
	gcc -pg -c timing.c
	gcc -pg -c Event.c
	gcc -pg -c OptimizationSplay.c
	gcc -pg -c memory.c
	gcc markov_hold_model.c -pg -o mar.o -lm
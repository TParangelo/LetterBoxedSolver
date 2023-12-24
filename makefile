all: submission

# Specify the object files that the target depends on
# Also specify the object files needed to create the executable
submission: letterBoxed.o 
	g++ letterBoxed.o -o submission

letterBoxed.o: letterBoxed.cpp
	g++ -c letterBoxed.cpp

clean:
	rm *.o	
	rm submission



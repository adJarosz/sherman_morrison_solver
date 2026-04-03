
CXX = g++
CXXFLAGS = -Wall -O2

GSL_LIBS = -lgsl -lgslcblas -lm

SRC = num2.cpp
EXEC = num2

all: $(EXEC)

$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC) $(GSL_LIBS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC)
DIR = $(notdir $(CURDIR))
tar: clean
	(cd ..; tar -cvzf $(DIR).tar.gz $(DIR))

CC = g++
# CFLAGS = --std=c++17 -Wall -Werror -pedantic -g
CFLAGS = --std=c++17 -Wall -Werror -pedantic -g -I./boost/include
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
# TEST_LIBS = -lboost_unit_test_framework
TEST_LIBS = -L./boost/lib -lboost_unit_test_framework
DEPS = Sokoban.hpp
OBJECTS = Sokoban.o
PROGRAM = Sokoban
TEST = test

.PHONY: all clean lint

all: $(PROGRAM) $(TEST) Sokoban.a

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(TEST): test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(TEST_LIBS)

Sokoban.a: $(OBJECTS)
	ar rcs Sokoban.a $(OBJECTS)

clean:
	rm *.o $(PROGRAM) Sokoban.a

lint:
	cpplint *.cpp *.hpp

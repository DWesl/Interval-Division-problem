CXX = g++
CXXFLAGS = -g -Wall

all: interval_tests

interval_tests: TimeInterval.o interval_tests.o
	$(CXX) $(CXXFLAGS) -o interval_tests TimeInterval.o interval_tests.o

TimeInterval.o: TimeInterval.cpp TimeInterval.h
	$(CXX) $(CXXFLAGS) -c TimeInterval.cpp

interval_tests.o: interval_tests.cpp TimeInterval.h
	$(CXX) $(CXXFLAGS) -c interval_tests.cpp

clean:
	rm -f interval_tests.o TimeInterval.o interval_tests
	rm -f time_interval_divmod time_interval_divmod.o

check: all
	./interval_tests

time_interval_divmod: time_interval_divmod.o TimeInterval.o
	$(CXX) $(CXXFLAGS) -o time_interval_divmod time_interval_divmod.o TimeInterval.o

time_interval_divmod.o: time_interval_divmod.cc TimeInterval.h
	$(CXX) $(CXXFLAGS) -c time_interval_divmod.cc

timings: time_interval_divmod
	./time_interval_divmod

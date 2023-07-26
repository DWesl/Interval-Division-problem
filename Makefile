CXX = g++
CXXFLAGS = -g -Wall

all: interval_tests interval_divmod_tests

interval_divmod_tests: TimeInterval.o TimeIntervalDivMod.o TestTimeIntervalDivMod.o
	$(CXX) $(CXXFLAGS) -o interval_divmod_tests TestTimeIntervalDivMod.o TimeIntervalDivMod.o TimeInterval.o

TestTimeIntervalDivMod.o: TestTimeIntervalDivMod.cc TimeIntervalDivMod.hh
	$(CXX) $(CXXFLAGS) -c TestTimeIntervalDivMod.cc

TimeIntervalDivMod.o: TimeIntervalDivMod.cc TimeIntervalDivMod.hh
	$(CXX) $(CXXFLAGS) -c TimeIntervalDivMod.cc

interval_tests: TimeInterval.o interval_tests.o
	$(CXX) $(CXXFLAGS) -o interval_tests TimeInterval.o interval_tests.o

TimeInterval.o: TimeInterval.cpp TimeInterval.h
	$(CXX) $(CXXFLAGS) -c TimeInterval.cpp

interval_tests.o: interval_tests.cpp TimeInterval.h
	$(CXX) $(CXXFLAGS) -c interval_tests.cpp

clean:
	rm -f interval_tests.o TimeInterval.o interval_tests
	rm -f TestTimeIntervalDivMod.o TimeIntervalDivMod.o interval_divmod_tests

check: all
	./interval_tests
	./interval_divmod_tests

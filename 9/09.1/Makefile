#N.B.: per vedere i tab ho dovuto andare in
#file, preferences, settings, editor, e mettere
#e togliere il check su detect indentation
CXX = g++
CXXFLAGS = -Wall -O3 --std=c++11
LDFLAGS = -larmadillo
OBJS = main.o lib.o random.o popolazione.o
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.cpp lib.h random.h popolazione.h
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)

lib.o: lib.cpp lib.h random.h popolazione.h
	$(CXX) -c lib.cpp -o lib.o $(CXXFLAGS)

random.o: random.cpp random.h
	$(CXX) -c random.cpp -o random.o $(CXXFLAGS)

popolazione.o: popolazione.cpp popolazione.h random.h lib.h
	$(CXX) -c popolazione.cpp -o popolazione.o $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
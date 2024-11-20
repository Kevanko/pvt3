CXX = mpicxx
CXXFLAGS = -O3 -Wall
LDFLAGS = -lstdc++

TARGET = sgemv
SRC = sgemv.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

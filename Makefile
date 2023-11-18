CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDLIBS = -I/usr/include/graphviz -lgvc -lcgraph -lcdt -lsfml-graphics -lsfml-window -lsfml-system
SOURCES = metro.cpp card_operations.cpp graph_operations.cpp
EXECUTABLE = metro

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(EXECUTABLE)
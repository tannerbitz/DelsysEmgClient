BASE_DIR = ../../../../..
CXX = g++
CFLAGS = -std=c++11 -pthread
INCLUDE = -I/usr/local/include
LFLAGS = -L/usr/local/lib/ -lboost_system


TARGET = tcp_client_emg

$(TARGET): $(TARGET).o
	$(CXX) -o $@ $(TARGET).o $(CFLAGS) $(LFLAGS)

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDE)

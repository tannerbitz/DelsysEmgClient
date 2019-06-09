BOOST_DIR = /usr/lib/boost_1_70_0
CXX = g++
CFLAGS = -std=c++11 -pthread
INCLUDE = -I/usr/local/include $(BOOST_DIR)
LFLAGS = -L/usr/local/lib/ -lboost_system


TARGET = tcp_client_emg

$(TARGET): $(TARGET).o
	$(CXX) -o $@ $^ $(CFLAGS) $(LFLAGS)

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm *.o \
	   $(TARGET)

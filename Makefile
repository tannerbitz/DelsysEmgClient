BOOST_DIR = /home/tannerbitz/Documents/cpp/boost_1_68_0
CXX = g++
CFLAGS = -std=c++11 -pthread
INCLUDE = -I/usr/local/include $(BOOST_DIR)
LFLAGS = -L/usr/local/lib/ -lboost_system -lpthread -lboost_thread  -lboost_chrono


OBJ_DIR =	obj
SOURCES = 	TrignoClientTest.cpp \
		 	TrignoEmgClient.cpp
TARGET 	= 	trigno_client_test

OBJ 	=	$(SOURCES:%.cpp=$(OBJ_DIR)/%.o)


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CFLAGS) $(LFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDE)


clean:
	rm obj/*.o \
	   $(TARGET)

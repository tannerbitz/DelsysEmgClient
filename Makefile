BOOST_DIR = /home/tannerbitz/Documents/cpp/boost_1_68_0 	# Tanner's Laptop
# BOOST_DIR = /usr/include/boost_1_70_0						# KUKA Computer
CXX = g++
CXXFLAGS = -std=c++11 -pthread
INCLUDE = -I/usr/local/include $(BOOST_DIR)
LDFLAGS = -L/usr/local/lib/ -lboost_system -lpthread -lboost_thread  -lboost_chrono -lboost_timer

# ---------------- HDF ----------------------
HDF_INSTALL = /usr/local/hdf5
#HDF_LIB 	= $(HDF_INSTALL)/lib64			# KUKA Computer
HDF_LIB 	= $(HDF_INSTALL)/lib			# Tanner's laptop
HDF_INCLUDE = -I$(HDF_INSTALL)/include
HDF_LIBS 	= -L$(HDF_LIB)	\
			  -lhdf5_cpp	\
			  -lhdf5 		\
			  -lz			\
			  -lm 			\
			  -ldl			\
			  #-lsz

# ------------------------------------------
INCLUDE += $(HDF_INCLUDE)
LDFLAGS += $(HDF_LIBS)

OBJ_DIR =	obj
SOURCES = 	TrignoClientTest.cpp \
		 	TrignoEmgClient.cpp
TARGET 	= 	trigno_client_test

OBJ 	=	$(SOURCES:%.cpp=$(OBJ_DIR)/%.o)


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCLUDE)


clean:
	rm obj/*.o \
	   $(TARGET)

CXX = g++

# ---------------- Boost ----------------------
BOOST_DIR = /home/tannerbitz/Documents/cpp/boost_1_68_0 	# Tanner's Laptop
# BOOST_DIR = /usr/include/boost_1_70_0						# KUKA Computer
CXXFLAGS 	+= 	-std=c++11 -pthread
BOOST_INC 	= 	-I/usr/local/include $(BOOST_DIR)
BOOST_LIB 	= 	-L/usr/local/lib/ 	\
				-lboost_system 		\
				-lpthread 			\
				-lboost_thread 		\
				-lboost_chrono

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
			  -ldl

# ------------------------------------------
INCLUDE += $(HDF_INCLUDE) $(BOOST_INC)
LDFLAGS += $(HDF_LIBS) $(BOOST_LIB)

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

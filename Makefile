CXX = g++

INCLUDE_DIR=include
SRC_DIR=src

CXXFLAGS = -I$(INCLUDE_DIR)

all: 
	mkdir client
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/main.cpp -o client/client `pkg-config --cflags --libs gtk+-3.0 gdk-pixbuf-2.0`
	chmod +x client/client
	cp $(SRC_DIR)/client-default-config.json client/config.json
	cp $(SRC_DIR)/images/user.png client/user.png
clean:
	rm -rf client
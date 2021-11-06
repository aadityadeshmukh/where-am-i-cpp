CC		= g++
TARGET		= main
SRC		= main.cpp  GLWifiPoints.cpp

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)


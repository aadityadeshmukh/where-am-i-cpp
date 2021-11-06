CC		= g++
TARGET		= main
SRC		= main.cpp  GLWifiPoints.cpp cJSON.c
LDLIBS	= -lwlanapi

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(LDLIBS)


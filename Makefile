CC		= g++
TARGET		= main
SRC		= main.cpp  GLWifiPoints.cpp GLRequest.cpp cJSON.c
LDLIBS	= -lwlanapi -lcurl

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(LDLIBS)


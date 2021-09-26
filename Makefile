CC=gcc
BUILD_DIR=build
SOURCE_DIR=src
TARGET=$(BUILD_DIR)/simulation
OBJS=$(BUILD_DIR)/types.o $(BUILD_DIR)/input_reader.o $(BUILD_DIR)/main.o
 
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(BUILD_DIR)/types.o : $(SOURCE_DIR)/types.c
	$(CC) -c -o $@ $^

$(BUILD_DIR)/input_reader.o : $(SOURCE_DIR)/input_reader.c
	$(CC) -c -o $@ $^

$(BUILD_DIR)/main.o : $(SOURCE_DIR)/main.c
	$(CC) -c -o $@ $^

clean:
	rm $(OBJECT) $(TARGET)
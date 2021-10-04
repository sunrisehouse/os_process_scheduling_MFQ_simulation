CC=gcc
BUILD_DIR=build
SOURCE_DIR=src
TARGET=$(BUILD_DIR)/simulation
OBJECTS=$(BUILD_DIR)/types.o $(BUILD_DIR)/list.o $(BUILD_DIR)/input_reader.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/scheduler.o $(BUILD_DIR)/main.o
 
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

$(BUILD_DIR)/types.o : $(SOURCE_DIR)/types.c
	$(CC) -c -o $@ $^

$(BUILD_DIR)/list.o : $(SOURCE_DIR)/list.c
	$(CC) -c -o $@ $^

$(BUILD_DIR)/input_reader.o : $(SOURCE_DIR)/input_reader/input_reader.c
	$(CC) -c -o $@ $^

$(BUILD_DIR)/queue.o : $(SOURCE_DIR)/queue.c
	$(CC) -c -o $@ $^

$(BUILD_DIR)/scheduler.o : $(SOURCE_DIR)/scheduler/scheduler.c
	$(CC) -c -o $@ $^

$(BUILD_DIR)/main.o : $(SOURCE_DIR)/main.c
	$(CC) -c -o $@ $^

clean:
	rm $(OBJECTS) $(TARGET)
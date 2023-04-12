
CC := clang
LD := clang
CFLAGS := -std=c99 -g -Wall

.PHONY: clean

BUILD_DIR := output
INCLUDES := `pkg-config --cflags gstreamer-1.0`
LIBS := `pkg-config --libs gstreamer-1.0`

VPATH := output \
		./src \
		./inc 

all: checkdirs output/main

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/main: output/main.o 
	$(CC) $(filter %.o,$^) -o $@ $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDES) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/main 
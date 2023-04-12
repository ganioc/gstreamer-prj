
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

output/main: main.o 
	$(CC) $(filter %.o,$^) -o $@ $(LIBS) $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDES) $(CPPFLAGS) -c $< 

clean:
	rm -f *.o 
	rm -f output/main 
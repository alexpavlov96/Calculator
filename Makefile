BUILD_DIR = ${CURDIR}/build
BIN_DIR = ${CURDIR}
SRC_DIR = ${CURDIR}/src

all: 
	gcc $(BUILD_DIR)/calculate.o $(BUILD_DIR)/errorhandler.o $(BUILD_DIR)/main.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/qust.o $(BUILD_DIR)/scanner.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/variable.o -lm -o $(BIN_DIR)/calc
	
$(BUILD_DIR)/calculate.o: $(SRC_DIR)/calculate.c
	gcc -c $(SRC_DIR)/calculate.c -o $(BUILD_DIR)/calculate.o
	
$(BUILD_DIR)/errorhandler.o: $(SRC_DIR)/errorhandler.c
	gcc -c $(SRC_DIR)/errorhandler.c -o $(BUILD_DIR)/errorhandler.o
	
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	gcc -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o	
	
$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser.c
	gcc -c $(SRC_DIR)/parser.c -o $(BUILD_DIR)/parser.o	
	
$(BUILD_DIR)/qust.o: $(SRC_DIR)/qust.c
	gcc -c $(SRC_DIR)/qust.c -o $(BUILD_DIR)/qust.o
	
$(BUILD_DIR)/scanner.o: $(SRC_DIR)/scanner.c
	gcc -c $(SRC_DIR)/scanner.c -o $(BUILD_DIR)/scanner.o	
	
$(BUILD_DIR)/utils.o: $(SRC_DIR)/utils.c
	gcc -c $(SRC_DIR)/utils.c -o $(BUILD_DIR)/utils.o	
	
$(BUILD_DIR)/variable.o: $(SRC_DIR)/variable.c
	gcc -c $(SRC_DIR)/variable.c -o $(BUILD_DIR)/variable.o
	
clean:
ifeq ($(OS),Windows_NT)
	cd $(BUILD_DIR) && del *.o
else
	cd $(BUILD_DIR) && rm *.o
endif

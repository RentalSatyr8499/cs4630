CC = gcc
CFLAGS = -Os -w

TARGET_DIR = ./targets
INFECTED_DIR = ./infected-targets
EXPLOITS_DIR = ./exploits

INFECTORS = infect1 infect2 infect3a

all: $(INFECTORS)

infect1: $(EXPLOITS_DIR)/infect1.c
	$(CC) $(CFLAGS) -o $@ $<

infect2: $(EXPLOITS_DIR)/infect2.c
	$(CC) $(CFLAGS) -o $@ $<

infect3a: $(EXPLOITS_DIR)/infect3a.c
	$(CC) $(CFLAGS) -o $@ $<

infect: all
	@mkdir -p $(INFECTED_DIR)

	cp $(TARGET_DIR)/target1.exe $(INFECTED_DIR)/
	cp $(TARGET_DIR)/target2.exe $(INFECTED_DIR)/
	cp $(TARGET_DIR)/target3a.exe $(INFECTED_DIR)/

	./infect1 $(INFECTED_DIR)/target1.exe
	./infect2 $(INFECTED_DIR)/target2.exe
	./infect3a $(INFECTED_DIR)/target3a.exe


clean:
	rm -f $(INFECTORS)
	rm -rf $(INFECTED_DIR)

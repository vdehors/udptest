CC?=gcc
HOST=host
SUBSCRIBER=subscriber
COMMON=com.c timer.c clock.c ftrace.c

all: $(HOST) $(SUBSCRIBER)

$(HOST): $(HOST).c $(COMMON)
	$(CC) -o $(HOST) $(HOST).c $(COMMON) -lrt

$(SUBSCRIBER): $(SUBSCRIBER).c $(COMMON)
	$(CC) -o $(SUBSCRIBER) $(SUBSCRIBER).c $(COMMON) -lrt

clean:
	rm -rf $(HOST) $(SUBSCRIBER)


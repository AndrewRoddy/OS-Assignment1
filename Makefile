COMPILER = g++
COMPILERFLAGS = -std=c++11 -pthread

TARGETS = producer consumer

# SETUP
all: $(TARGETS)

producer: producer.o
	$(COMPILER) $(COMPILERFLAGS) -o producer producer.o -lrt
consumer: consumer.o
	$(COMPILER) $(COMPILERFLAGS) -o consumer consumer.o -lrt

producer.o: producer.cpp
	$(COMPILER) $(COMPILERFLAGS) -c producer.cpp
consumer.o: consumer.cpp
	$(COMPILER) $(COMPILERFLAGS) -c consumer.cpp

# Commands that I will run
run: producer consumer
	./producer& ./consumer&

clean:
	rm -f *.o $(TARGETS)
	rm -f /dev/shm/shared_buffer
	pkill consumer
	pkill producer

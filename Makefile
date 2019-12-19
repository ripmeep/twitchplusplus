CC=g++
WSRC=example_watch.cpp
CSRC=example_chat.cpp
HEADER=twitch.hpp
FLAGS=-o
WOUT=watch
COUT=chat

all: watch chat

watch:
	$(CC) $(WSRC) -H $(HEADER) $(FLAGS) $(WOUT)
chat:
	$(CC) $(CSRC) -H $(HEADER) $(FLAGS) $(COUT)

clean:
	rm -r $(WOUT)
	rm -r $(COUT)

EXEC = get_network
OBJS = get_network.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o  $@ $(OBJS)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o


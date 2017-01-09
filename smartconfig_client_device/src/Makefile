EXEC = test_client
OBJS = test_client.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o


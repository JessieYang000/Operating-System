CC = gcc
CFLAGS = -Wall -g
SRCS = main.c file1.c file2.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# Rule to generate object files
$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to generate dependency files
%.d: %.c
	$(CC) -M -MP -MF $@ -MT $(@:.d=.o) $<

# Include generated dependency files
-include $(DEPS)

# Final target to create the executable
my_program: $(OBJS)
	$(CC) $(OBJS) -o my_program

%.out: %.c
	gcc -o $@ $<

%.out: %.c
	gcc -o $@ $<

TARGET = cas

CC = gcc

CFLAGS = -Wall -g

LIBS = -lm

SRCS = main.c lexer.c ast.c parser.c math_engine.c plotter.c solver.c taylor.c

OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

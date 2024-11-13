# Definir el compilador y las banderas
CC = gcc
CFLAGS = -Wall -g

# Definir los nombres de los ejecutables
EXEC = main

# Definir las fuentes y los objetos
SRC = main.c server.c client.c common.c
OBJ = $(SRC:.c=.o)

# Librerías a enlazar
LIBS = -lenet

# Regla por defecto (compilar el ejecutable principal)
all: $(EXEC)

# Regla para enlazar los objetos y crear el ejecutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

# Regla para compilar los archivos fuente en objetos
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJ) $(EXEC)

MKDIR   := mkdir
RMDIR   := rm -rf
CC      := g++

CURL_PATH = ~/Documents/workspace/src/kaios/rsc/curl 
LIB	:= ./lib
BIN     := ./bin
OBJ     := $(BIN)/obj
INCLUDE := ./include
SRC     := ./src
JASON_INCLUDE = ./usr

SRCS    := $(wildcard $(SRC)/*.c)
OBJS    := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
EXE     := $(BIN)/rtls_impl

LDLIBS  := -lcurl
LDFLAGS	:= -L$(LIB)
CFLAGS = -B $(CURL_PATH) -I$(JASON_INCLUDE) -I$(INCLUDE)

.PHONY: all run clean

all: $(EXE)

$(EXE): $(OBJS) | $(BIN)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ) $(BIN):
	$(MKDIR) -p $@

run: $(EXE)
	./$<

clean:
	$(RMDIR) $(OBJ) $(BIN)

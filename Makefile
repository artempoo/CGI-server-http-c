CC = gcc
CFLAGS = -Wall -Wextra 
TARGET = app 
SRC = main.c  
BINDIR = app/cgi-bin

.PHONY: all clean get post

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(BINDIR)/$(TARGET) $(SRC)

get: $(BINDIR)/$(TARGET)
	REQUEST_METHOD=GET \
	QUERY_STRING="id=5&lang=ru" \
	DOCUMENT_ROOT=./app \
	SCRIPT_NAME="/" \
	./$(BINDIR)/$(TARGET)

404: $(TARGET)
	REQUEST_METHOD=GET \
	QUERY_STRING="id=5&lang=ru" \
	DOCUMENT_ROOT=./app \
	SCRIPT_NAME="/index" \
	./$(BINDIR)/$(TARGET)

post: $(BINDIR)/$(TARGET)
	@printf "title=test" | \
	REQUEST_METHOD=POST \
	CONTENT_TYPE="application/x-www-form-urlencoded" \
	CONTENT_LENGTH=10 \
	SCRIPT_NAME="/news" \
	./$(BINDIR)/$(TARGET)

clean:
	rm -f $(BINDIR)/$(TARGET)

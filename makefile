CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Iinclude

SRC = src/TextEditorBackend.cpp src/main.cpp src/BSTHelpers.cpp
TEST_SRC = src/TextEditorBackend.cpp src/BSTHelpers.cpp test/TextEditorTest.cpp

OBJ = $(SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

EXEC = textEditor
TEST_EXEC = textEditorTest

all: $(EXEC)
	 @echo "Running $(EXEC)..."
	 @./$(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_EXEC): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC) $(TEST_EXEC)

test: $(TEST_EXEC)
	@echo "Running tests..."
	./$(TEST_EXEC)

.PHONY: all clean test

CXX = g++
      CXXFLAGS = -std=c++17 -Wall -Wextra -O2

                      SRC = main.cpp BmpHeader.cpp BmpImage.cpp
                            OBJ = $(SRC:.cpp=.o)
                                  TARGET = program


                                          TEST_SRC = test.cpp BmpHeader.cpp BmpImage.cpp
                                                  TEST_BIN = test

                                                          all:
                                                          $(TARGET)

                                                          $(TARGET): $(OBJ)
                                                              $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

                                                          %.o:
                                                              %.cpp
                                                              $(CXX) $(CXXFLAGS) -c $<

                                                          run:
                                                              all
                                                              ./$(TARGET)


                                                          test:
                                                              $(TEST_SRC)
                                                              $(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(TEST_SRC) -lgtest -lgtest_main -pthread
                                                              ./$(TEST_BIN)

                                                          clean:
                                                              rm -f $(OBJ) $(TARGET) $(TEST_BIN)

                                                          .PHONY:
                                                              all run clean test


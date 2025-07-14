CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

SRCS = main.cpp corretor.cpp cliente.cpp imovel.cpp funcoes_auxiliares.cpp
OBJS = $(SRCS:.cpp=.o)

imobiliaria: $(OBJS)
	$(CXX) $(CXXFLAGS) -o imobiliaria $(OBJS)

clean:
	rm -f imobiliaria $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
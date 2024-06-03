CXX := g++  #Compilador
CXXFLAGS := -Wall -std=c++17  #(warnings, C++ standard)

SRC := main.cpp transacoes.cpp consulta.cpp #arquivos de entrada

OUT := main.out #Arquivo de saída

all:	#Compila tudo
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

 
clean: 	#Remove arquivos executáveis 			
	rm -f $(OUT)	


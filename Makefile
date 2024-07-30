CXX := g++  #Compilador
CXXFLAGS := -Wall -std=c++17  #(warnings, C++ standard)

SRC := "src/main.cpp" "src/consulta.cpp" "src/manipulacao.cpp"  #arquivos de entrada

OUT := main.out #Arquivo de saída

OTHERS:= LOG.txt auxiliar-consulta.txt bin/consolidadas*

all:	#Compila tudo
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

 
clean: 	#Remove arquivos executáveis 			
	rm -f  $(OUT) $(OTHERS)


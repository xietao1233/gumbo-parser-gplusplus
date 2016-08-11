VPATH = ./src
objects = main.o HtmlParser.o attribute.o char_ref.o error.o parser.o string_buffer.o string_piece.o\
	tag.o tokenizer.o utf8.o util.o vector.o
cc = g++
compileoption = -g -Wall -std=c++11

testparser : $(objects)
	$(cc) -o testparser $(objects)

main.o :
	$(cc) -c main.cpp $(compileoption)

.PHONY : clean
clean : 
	rm testparser $(objects)

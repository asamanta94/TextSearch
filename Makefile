CC=g++
OUT=search

fs: Search.cpp FileSearch.cpp
	$(CC) -o $(OUT) Search.cpp FileSearch.cpp

clean:
	rm -rf $(OUT)

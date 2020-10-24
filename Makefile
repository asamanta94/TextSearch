CC=g++
OUT=search

rk: rabin_karp.cpp
	$(CC) -o $(OUT) rabin_karp.cpp

clean:
	rm -rf $(OUT)

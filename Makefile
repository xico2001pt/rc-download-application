all: download

$(shell mkdir -p build)

download:
	gcc source/* -Wall -o build/download

clean:
	rm -rf ./build
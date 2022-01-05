all: download

$(shell mkdir -p build)

download:
	gcc source/* -o build/download

clean:
	rm -rf ./build
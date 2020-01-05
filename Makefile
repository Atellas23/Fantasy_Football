m = update
name = main

all:
	@echo "doing nothing"

fetch:
	@git pull

update:
	@git add --all
	@git commit -m $(m)
	@git push
	@echo "done!"

compile:
	g++ -Wall -O3 -D_GLIBCXX_DEBUG $(name).cc -o $(name).out

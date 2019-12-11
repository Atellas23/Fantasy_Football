all:
	@echo "doing nothing"

fetch:
	@git pull

update:
	@git add --all
	@git commit -m 'coses'
	@git push

compile:
	g++ -Wall -D_GLIBCXX_DEBUG $(name).cc -o $(name).out

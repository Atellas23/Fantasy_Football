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
	g++ -Wall -D_GLIBCXX_DEBUG $(name).cc -o $(name).out

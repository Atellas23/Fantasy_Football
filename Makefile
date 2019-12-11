all:
	@echo "updating"

fetch:
	@git pull

update:
	@git add --all
	@git commit -m 'coses'
	@git push

.PHONY: shell

shell:
	@echo "creating the shell..."
	gcc -Wall main.c kfsh.c source/gdir.c source/genv.c source/prt.c source/mkf.c source/rmf.c source/ldir.c source/sdir.c source/cdir.c source/pfl.c source/mfl.c source/rfl.c source/finf.c -o shell -g

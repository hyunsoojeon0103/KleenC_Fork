.PHONY: all
all: kleenc.native funcs.o

kleenc.native: scanner.mll parser.mly ast.ml semant.ml sast.ml codegen.ml kleenc.ml test.kc
	ocamlbuild -pkgs llvm kleenc.native
#	./kleenc.native -l test.kc > test.out
.PHONY: clean
clean:
	ocamlbuild -clean
	rm -rf ocamlllvm test.out *.o *.s *.ll *.out *.exe

funcs: funcs.c
	cc -o funcs -DBUILD_TEST funcs.c

.PHONY: all
all: kleenc.native realloc.o

kleenc.native: scanner.mll parser.mly ast.ml semant.ml sast.ml codegen.ml kleenc.ml test.kc
	ocamlbuild -pkgs llvm kleenc.native
	./kleenc.native -l test.kc > test.out

.PHONY: clean
clean:
	ocamlbuild -clean
	rm -rf ocamlllvm test.out *.o

realloc: realloc.c
	cc -o realloc -DBUILD_TEST realloc.c

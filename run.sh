Run() 
{
    echo $* 1>&2
    eval $* || echo "$* failed" || exit 1 
}

#./kleenc.native ./test.kc > ./test.ll



#	R E A D M E
# this path may differ depending on the system
# check it by running "which llc"
LLC="/usr/bin/llc"





#$LLC -relocation-model=pic ./test.ll > ./test.s
Run "./kleenc.native" "./test.kc" ">" "./test.ll" &&
Run "$LLC" "-relocation-model=pic" "./test.ll" ">" "./test.s" &&
Run "cc" "-o" "./test.exe" "./test.s" "funcs.o" &&
Run "./test.exe"  > "./test.out" #&& // still working on lli test.out
#lli test.out
echo ================PROGRAM OUTPUT====================
./test.exe


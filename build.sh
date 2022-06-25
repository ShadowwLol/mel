DEFINES=""
DDEFINES="" #"-D_MEL_DEBUG"

INCLUDE=./mel/inc

case "$1" in
"--debug"|"-d")
printf "[+] Debug mode\n"
CFLAGS="-O -g -std=c99 $DDEFINES -Wextra -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -Wshadow -Wformat=2 -Wformat-truncation -Wformat-overflow -fno-common -fstack-usage"
;;
*)
printf "[+] Release mode\n"
CFLAGS="-static-libgcc -std=c99 $DEFINES -O3 -Wextra "
;;
esac

case "$2" in
"win32")
EXE="./dist/win32/mel-win32.exe"
printf "[#] Windows\n"
CC=i686-w64-mingw32-gcc
LFLAGS="./.win32/libglfw3dll.a ./.win32/libopengl32.a ./.win32/libmingw32.a ./.win32/libgdi32.a ./.win32/libuser32.a ./.win32/libkernel32.a ./.win32/libwinpthread.a ./.win32/libwinpthread.dll.a ./.win32/libm.a"
;;
"win64")
EXE="./dist/win64/mel-win64.exe"
printf "[#] Windows\n"
CC=x86_64-w64-mingw32-gcc
LFLAGS="libglfw3dll.a libopengl32.a libmingw32.a libgdi32.a libuser32.a libkernel32.a libwinpthread.a libwinpthread.dll.a libm.a"
;;
"all")
./$0 - win32
./$0 - win64
./$0 - linux
;;
*)
EXE="./dist/linux/mel-linux.x86_64"
printf "[+] GNU/Linux\n"
CC=gcc
LFLAGS="-l:libglfw3.a -ldl -lm -lGL -lvulkan -pthread -lrt -lX11 -lxcb -lXau -lXdmcp "
;;
esac

if [ ! -d "dist" ]; then
  mkdir "dist" 
  mkdir "dist/win32" 
  cp ".rsrc" -r "dist/win32/resources"
  cp ./.win32/* -r "dist/win32/"
  mkdir "dist/win64" 
  cp ./.rsrc -r "dist/win64/resources"
  mkdir "dist/linux" 
  cp ".rsrc" -r "dist/linux/resources"
fi

[ ! -d ".bsh" ] && mkdir ".bsh"

cd .bsh

cfiles=$(find ../mel/ | grep "\.c$")

printf "[+] Compiling:\n"

for cfile in $cfiles; do

#printf "\t* [$cfile]\n"
compiled=${cfile%.*}.o
compiled=$(echo $compiled | sed 's|.*/||')

folder=$(echo $cfile | sed 's|\(.*\)/.*|\1|')
[ ! -d "$folder" ] && mkdir -p "$folder" && printf "[#] MAKING FOLDER: $folder\n"

if [ -s $compiled ]; then
	#file exists and is not empty!
	csource=${compiled%.*}.c
	STATUS="$(cmp --silent $csource $cfile; echo $?)"
	if [ $STATUS -ne 0 ]; then
		# files differ
		printf "$CC -c $cfile $CFLAGS -o $compiled\n"
		$CC -c $cfile -I$INCLUDE $CFLAGS -o $compiled
		cp $cfile -r .
	fi
else
	printf "$CC -c $cfile $CFLAGS -o $compiled\n"
	$CC -c $cfile -I$INCLUDE $CFLAGS -o $compiled
	cp $cfile -r .
fi

done

cd ..

printf "$CC $(find .bsh/ | grep "\.o$") -I$INCLUDE $LFLAGS -o $EXE\n"
$CC $(find .bsh/ | grep "\.o$") -I$INCLUDE $LFLAGS -o "$EXE"

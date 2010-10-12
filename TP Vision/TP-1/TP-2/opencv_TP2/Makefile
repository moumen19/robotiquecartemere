
#Directories
INDCIR = include

CC = gcc
LDFLAGS = `pkg-config opencv --libs`
CFLAGS = -I$(INCDIR) `pkg-config opencv --cflags` -g

#EXEC=webcam
EXEC=lecture_image

all : ${EXEC}  

${EXEC} : ${EXEC}.o
	${CC} ${LDFLAGS} -o ${EXEC} ${EXEC}.o


${EXEC}.o : ${EXEC}.c
	${CC} ${CFLAGS} -o ${EXEC}.o -c ${EXEC}.c

clean :
	rm -rf *.o ${EXEC}


#\
IN:=-I \
IN+=include \
IN+=-Wall \
IN+=-g \
bin/main:obj/main.o obj/admin.o obj/user.o obj/control.o obj/issueLink.o obj/notary.o obj/menuView.o\
	gcc obj/main.o obj/admin.o obj/user.o obj/control.o obj/issueLink.o obj/notary.o obj/menuView.o -o bin/main $(IN)\
obj/main.o:src/main.c \
	gcc src/main.c -o obj/main.o -c $(IN)\
obj/admin.o:src/admin.c \
	gcc src/admin.c -o obj/admin.o -c $(IN)\
obj/user.o:src/user.c \
	gcc src/user.c -o obj/user.o -c $(IN)\
obj/menuView.o:src/menuView.c \
	gcc src/menuView.c -o obj/menuView.o -c $(IN)\
obj/control.o:src/control.c \
	gcc src/control.c -o obj/control.o -c $(IN)\
obj/issueLink.o:src/issueLink.c \
	gcc src/issueLink.c -o obj/issueLink.o -c $(IN)\
obj/notary.o:src/notary.c \
	gcc src/notary.c -o obj/notary.o -c $(IN)\

IN:=-I 
IN+=include 
IN+=-Wall 
IN+=-g 
bin/main:obj/main.o obj/admin.o obj/user.o obj/control.o obj/issueLink.o obj/notary.o obj/menuView.o
	gcc -o $@ $^ $(IN)
obj/%.o:src/%.c
	gcc -o $@ $^ -c $(IN)

.PHONY:clean
clean:
	-rm obj/*.o

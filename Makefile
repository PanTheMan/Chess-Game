CXX = g++
CXXFLAGS = -Wall -MMD -g -L/opt/X11/lib -I/opt/X11/include
EXEC = pp9k
OBJECTS = main.o window.o graphicdisplay.o game.o gamecontroller.o textdisplay.o view.o human.o player.o position.o computer.o computer1.o computer2.o computer3.o computer4.o computer5.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
					 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
						rm ${OBJECTS} ${EXEC} ${DEPENDS}


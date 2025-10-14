##########################################################################
# Configuración de SystemC
SYSTEMC_HOME = /home/luisvaca/systemc-3.0.1/install
INCLUDE_DIR  = $(SYSTEMC_HOME)/include
LIB_DIR      = $(SYSTEMC_HOME)/lib-linux64 

# Compilador y flags
CXX         = g++
CXXFLAGS    = -std=c++17 -g -Wall -Wno-deprecated -pthread
INCLUDES    = -I$(INCLUDE_DIR) -I.  # Incluye el directorio actual para tus headers

# Flags de enlace
LDFLAGS     = -L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR)  # Asegura que el runtime encuentre libsystemc
LDLIBS      = -lsystemc -lm

# Archivos del proyecto (ajusta los nombres)
SRCS        =GCD_tb.cpp  # Incluye todos los .cpp
OBJS        = $(SRCS:.cpp=.o)
TARGET      = GCD_test  # Nombre del ejecutable

##########################################################################

all: $(TARGET)

# Regla de enlace
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@ $(LDLIBS)

# Regla de compilación para cada .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Ejecutar y limpiar
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.vcd *.bak *~

.PHONY: all run clean
##########################################################################
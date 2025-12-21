OUTPUT := main
SRCDIR := src
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
CXX ?= g++

EXE :=
RM := rm -f
ifeq ($(OS),Windows_NT)
  EXE := .exe
  RM := cmd /C del /Q /F
endif

.PHONY: all clean
all:
	$(CXX) $(SOURCES) -o $(OUTPUT)$(EXE)

clean:
	-$(RM) "$(OUTPUT)$(EXE)"
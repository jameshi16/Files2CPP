CXX = g++
CXXFLAGS = -std=c++14 --static -static-libstdc++
CXXFLAGS_D = -std=c++14 -g -Wall
LINKS := $(file < links.txt)
LIBRARIES := $(file < libraries.txt)
LINKS_D := $(file < links_d.txt)
LIBRARIES_D := $(file < libraries_d.txt)
CPPFILES := $(wildcard *.cpp) $(wildcard */*.cpp)
OBJFILES := $(addprefix obj/, $(CPPFILES:.cpp=.o))

FINAL_LINKS :=
FINAL_LIBRARIES :=
FINAL_FLAGS :=

$(shell mkdir -p out/ obj/)

.PHONY: clean
.SILENT: clean

main:
	@echo "Re-run make with an argument: debug or release."
	@echo "Target $@ ran."

debug: FINAL_LINKS := $(LINKS_D)
debug: FINAL_LIBRARIES := $(LIBRARIES_D)
debug: FINAL_FLAGS := $(CXXFLAGS_D)
debug: out/files2cpp.exe

release: FINAL_LINKS := $(LINKS)
release: FINAL_LIBRARIES := $(LIBRARIES)
release: FINAL_FLAGS := $(CXXFLAGS)
release: out/files2cpp.exe

out/files2cpp.exe: $(OBJFILES)
	$(CXX) $(FINAL_FLAGS) -o $@ $^ $(FINAL_LIBRARIES)

obj/%.o: %.cpp
	$(CXX) $(FINAL_FLAGS) -o $@ -c $< $(FINAL_LINKS)

clean:
	$(RM) -r out/ obj/

CXX = g++
CXXFLAGS = -std=c++17 -g -IAstroPartyWindows/ALLIncludes
LDFLAGS = -lGLEW -lglfw -lpthread -ldl

SRC := $(wildcard AstroPartyWindows/AstroPartyWindows/*.cpp) \
       $(wildcard AstroPartyWindows/AstroPartyWindows/*.c) \
       $(wildcard AstroPartyWindows/AstroPartyWindows/Classes/*.cpp) \
       $(wildcard AstroPartyWindows/AstroPartyWindows/Classes/Entities/*.cpp) \
       $(wildcard AstroPartyWindows/AstroPartyWindows/SubClasses/*.cpp)

OBJ := $(patsubst %.cpp,build/%.o,$(patsubst %.c,build/%.o,$(SRC)))

OUT = AstroPartyWindows/AstroPartyWindows/CosmicDuels.elf

# --- Default rule ---
all: $(OUT)

# --- Linking rule ---
$(OUT): $(OBJ)
	@echo "Linking $@"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# --- Compilation rule ---
# build/%.o will match build/AstroPartyWindows/.... file structure
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Cleanup ---
clean:
	rm -rf build
	rm -f $(OUT)

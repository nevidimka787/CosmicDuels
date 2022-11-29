OUT = AstroPartyWindows/AstroPartyWindows.elf
FLAGS = \
	-std=c++17

SRCS = \
	AstroPartyWindows/gald.c \
	AstroPartyWindows/Main.cpp \
	AstroPartyWindows/stb_image.cpp \
	AstroPartyWindows/Classes/Buffer.cpp \
	AstroPartyWindows/Classes/Camera.cpp \
	AstroPartyWindows/Classes/Color.cpp \
	AstroPartyWindows/Classes/Line.cpp \
	AstroPartyWindows/Classes/Logs.cpp \
	AstroPartyWindows/Classes/Map.cpp \
	AstroPartyWindows/Classes/Mat.cpp \
	AstroPartyWindows/Classes/Menu.cpp \
	AstroPartyWindows/Classes/Shader.cpp \
	AstroPartyWindows/Classes/Texture.cpp \
	AstroPartyWindows/Classes/Entities/AggressiveEntity.cpp \
	AstroPartyWindows/Classes/Entities/AnigAreaGenerator.cpp \
	AstroPartyWindows/Classes/Entities/Asteroid.cpp \
	AstroPartyWindows/Classes/Entities/Bomb.cpp \
	AstroPartyWindows/Classes/Entities/Bonus.cpp \
	AstroPartyWindows/Classes/Entities/Bullet.cpp \
	AstroPartyWindows/Classes/Entities/ControledEntity.cpp \
	AstroPartyWindows/Classes/Entities/DecelerationArea.cpp \
	AstroPartyWindows/Classes/Entities/DynamicEntity.cpp \
	AstroPartyWindows/Classes/Entities/DynamicParticle.cpp \
	AstroPartyWindows/Classes/Entities/Entity.cpp \
	AstroPartyWindows/Classes/Entities/GravGen.cpp \
	AstroPartyWindows/Classes/Entities/KillerEntity.cpp \
	AstroPartyWindows/Classes/Entities/Knife.cpp \
	AstroPartyWindows/Classes/Entities/Laser.cpp \
	AstroPartyWindows/Classes/Entities/MegaLaser.cpp \
	AstroPartyWindows/Classes/Entities/Particle.cpp \
	AstroPartyWindows/Classes/Entities/Pilot.cpp \
	AstroPartyWindows/Classes/Entities/Portal.cpp \
	AstroPartyWindows/Classes/Entities/Ship.cpp \
	AstroPartyWindows/Classes/Entities/StaticEntity.cpp \
	AstroPartyWindows/Classes/Entities/SupportEntity.cpp \
	AstroPartyWindows/Classes/Entities/Turel.cpp \
	AstroPartyWindows/SubClasses/Game.cpp \
	AstroPartyWindows/SubClasses/GameMap.cpp \
	AstroPartyWindows/SubClasses/GameMemory.cpp \
	AstroPartyWindows/SubClasses/GamePhysic.cpp \
	AstroPartyWindows/SubClasses/Linker.cpp \
	AstroPartyWindows/SubClasses/MenuFunctions.cpp \
	AstroPartyWindows/SubClasses/OpenGLRealisation.cpp

all: build complete

build: $(SRCS)
	$(CXX) $(FLAGS) -o $(OUT) $(SRCS) -ldl -lm -lGL -lglfw -lepoxy -lstdc++ -pthread

complete:
	echo --COMPLETE--
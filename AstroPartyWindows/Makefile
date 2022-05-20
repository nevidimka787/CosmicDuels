OUT = AstroPartyWindows.elf

SRCS = \
	AstroPartyWindows/Classes/Buffer.cpp \
	AstroPartyWindows/Classes/Camera.cpp \
	AstroPartyWindows/Classes/Color.cpp \
	AstroPartyWindows/Classes/GameEngine.cpp \
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
	AstroPartyWindows/Classes/Entities/Turel.cpp

all: build run

build: $(SRCS)
	$(CXX) -o $(OUT) $(SRCS)

run:
	./$(OUT)
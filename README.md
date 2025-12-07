# Cosmic Duels
The idea of this game was got from the game “Astro Party”.
But this one has better physic, more maps and bonuses.

# Instalation
Unzip CosmicDuelsEXE.zip to empty folder.
Actual file: https://cloud.mail.ru/public/Yv3r/vKq1YNcM7

# Technique information
## Windows
Use Visual Studio 2022 for compiling and debugging the project.
The release version of the game starts up without a console.
## Linux
Use VSCode for compiling and debugging the project.
Game use 3 threads.
Physic calculation thread.
Input output thread.
Draw thread.

# Controls
Game uses next keys for controlling.
Q - rotate ship 0
A - ship 0 shoots or pilot 0 goes
C - rotate ship 1
V - ship 1 shoots or pilot 1 goes
LEFT - rotate ship 2
DOWN - ship 2 shoots or pilot 2 goes
L - rotate ship 3
O - ship 3 shoots or pilot 3 goes

Use rotate button 2 times to douge.


# Game info.
This is battle royal. There are from 2 to 4 player try to kill each other using different strategies. The game also has flexible parameter setting, that can fully change strategy for each match.
One match consists several rounds. There isn't an up limit for a count of the rounds for one match. If winner not defined the new round begins.

Team mode is available now. Multiplayer mode not available yet.

# Match making system.
There are up to 4 players can play this game at one time. Each player choice team. There are 2 or more teams must exist and any count of players can be in one team.

# Score system.
The main battle is in match that divided to round.
Round complete after only 1 or less teams alive. The alive team must consist at least one alive player.

Each player can be killed in some cases by another players in other cases by map and neutral objects. Score system takes into consideration these aspects. If a player kill another player then one point of score add to his team. But if a player is killed by something other, his team lose one point of score.

After round complete each team add or lose points. Queue of adding and losing is similar as queue of events that cause this.
If a team moves to left side of score table then it loses and skips all next rounds until the match will be ended. If a team moves to the right side of the score table it plays the next round with teams who also moves to the right side of score table only. If no other teams move to the right side of the score table the the team wins the match and the math is ended.

# Game rules.
The game rules located int the `Options` menu.
1) Random spawn. If it is active then players spawn in random corners else players spawn in the defined by team-select-button corner.
2) Random spawn direction. If it is active then players spawn with random directions else they set targeted the center of the map.
3) Spawn with bonus. If it active then players will have random bonus in inventories after spawn, else players will be spawned without bonuses.
4) Can spawn with different bonuses. If it is active then players can spawn with the different starting bonus for each player, else all players will have similar bonus after spawn.
5) Triple bonuses. If active then count of any obtained bonus will be set to 3 (max count for one type for each bonus).
6) Spawn with triple. Ship will have buff `Triple shoot` after spawn.
7) Spawn with shield. Ship will have buff `Personal shield` after spawn.
8) Knifes can destroy bullets. If it is active then knifes destroy bullet after collision with one and after that immediately destroyed, else bullets fly trough knifes.
9) Need kill pilot. If it is active then pilots can respawn after some time and a team is alive if at least one pilot or shit exists, else pilots will not respawn and a team is alive if at least one shit exists and pilots not counting.
10) Friendly sheep can restore. If it is active then pilots immediately be respawned after touch to an ally ship.
11) Friendly fire. Ally ships can destroy each other by bullets. If it is disable ally ships still can destroy each other by other mechanics.
12) Auto balance. If it is active then loser player is obtained personal shield.

# Pull menus.
Spawn rules of objects can be setted in "Spawn objects pull menu".
Bonuses available in match can be selected in "Bonus pull menu".
Maps available in match can be selected in Map pull menu".

# Objects
1) `Ship`. It constantly flies forward. It can rotate in one direction. It can use its abilities. Controlled by a player.
2) `Pilot`. It spawns after ship was destroyed. It can flys forward and rotate in one direction. After some time it respawns to ship (if match rules allow to do it).
3) `Map`. It contains some elements. Each element can change its size, scale, and angle. Each element can be `unbreakable`/`destroyable`, can `kill`/`doesn't kill` players and immediately detonate bombs, can `destroy`/`doesn't destroy` other objects and activate bombs. Each characteristic has its part or color. So maps with different properties has different colors.
4) `Bonus`. Can be obtained by any ship. After that it will be in the ship's inventory. Bonus constantly rotated and doesn't collision with any other object include map. Bonuses will be described in the next character.
5) `Asteroid`. A round object that can or can't keep some bonuses. Asteroid with one-time-use bonus has red shards. Asteroid with long-time-usable bonus has blue shards. Asteroid can have red and blue shards at the same time. There are 3 size of asteroids. Big asteroid is divided to 4 medium ones. Medium asteroid is divided to 2 small ones. Small asteroids drop bonuses after have been destroyed.
6) `Turret`. The object can shoot rotate and is destructible. Its behavior doesn't define and is defined by the map.
7) `Bullet`. The object destroys the first object that collides with one.
8) `Laser`. The object destroys all objects until it meets unbreakable map's element. Can mirrors from the sides if has such property.
9) `Knife`. The object destroys all objects that collides with one.
10) `Bomb`. The object has 3 phases. In the first phase it is passive and wait enemy player. If an enemy player flies too close, then the bomb activates and changes to the phase 2. It stays in this phase some time. After that it detonates and crates an `annihilation area`.
11) `Annihilation area`. The object destroys all objects that collides with one. Nothing can stop it. Two collision bullets is most common way to create annihilation areas.
13) `Desceleration area`. The area increases resistance to any object which is moving inside it.
14) `Acceleration area`. The area increases resistance to any object which is moving inside it.
15) Gravity generator. It generates gravity or antigravity around it. The force of gravity is defined as `F = G_COEF / r^2`

# Bonus system
There are 4 bonuses, 3 buffs and 1 round rule changer in the game. Each ship can have up to 3 bonuses of one type and all types at one time (there are 12 bonuses in total). Bonuses can create combinations. 16 combinations are available in total.

# Abilities
1) `Basic shoot`. If ship doesn't have any bonus and buff in its inventory then it creates bullet on front of it.
2) `Loop`. Ship shoots little bullets around it.
3) `Laser`. Ship shoot laser on front of it. Laser can't change direction after spawn. But it can change position.
4) `Bomb`. Ship drops bomb behind it. Bomb is ally for the ship.
5) `Knife`. Ship generates two knifes. The relativity to the ship positions of the knifes is constant. Each knife can destroy 3 map elements or asteroids. If knife destroys pilot or bullet it is destroyed immediately.
6) `Reflectable laser (laser + loop)`. It is a laser that can reflects from the walls of the map.
7) `Half-loop of bombs ability (loop + bomb)`. Ship drops some bombs in half-cyrcle behind it. The bombs have started velocity.
8) `Knife circle (loop + knife)`. Ship generates some knifes around which form circle. Each laser can destroy only one object.
9) `Bomb delivery (laser + bomb)`. Ship shoots some activated bombs on front of it.
10) `Long knife (laser + knife)`. Ship generates long knife on front of it. The knife can destroy 5 asteroids and map elements or 1 pilot and turret.
11) `Moving annihilation area (bomb + knife)`. Ship generates annihilation area. The annihilation area has ship's velocity. It is the most questionable ability because it kills more frequently ally players than enemies.
12) `Asteroid overpower (laser + bomb + loop)`. Ship creates laser that splits to more ones in case of shooting an `asteroid`.
13) `Wedge of annihilation areas (laser + bomb + knife)`. Ship generates a wedge of annihilation areas on front of it.
14) `Loop of annihilation areas (loop + bomb + knife)`. Ship generates a loop of annihilation areas around it.
15) `Hedgehog (loop + knife + laser)`. Ship generates some long knife from its center. Each knife can destroy 2 asteroids and map element or 1 pilot or turret.
16) `Annihilation area gun (loop + knife + laser + bomb)`. Ship generates 2 guns which shoot annihilation areas. The guns has no reloading time and the annihilation areas can fly thought the walls. So it is the match-ender bonus.

# Buffs
1) `Triple shoot`. The buff changes ship's gun to gun that shoots wedge of 3 bullets. Each shoot cost 3 bullets for the ship's magazine.
2) `Increase magazine size`. It is used immediately. After using one the ship will have one more place for bullet in its magazine. Maximum amount of places is 12.
3) `Personal shield`. The ship is got a personal shield. Shield can save the ship from one bullet but will be destroyed after that. Destroying the shield get the `unbreakable status` for a short time to the the ship.

# Round rule changer.
1) `Reverse`. If the changer is obtained by any ship the directions of the rotations for all ships and pilots will be reversed. Also this game rule can impact a behavior of some other objects on the map.

# Destroy and respawn.
After being destroyed any ship becomes a pilot. After some time pilot will become to ship (If gamerules set allow it). Newly respawned ship is got the `unbreakable status` for a short time. Some game rules can change respawn process.

# Maps
Each map's element has 3 properties.
1) Breakable or unbreakable.
2) Destroying or not destroying ships and pilots.
3) Destroying or not destroying not controlled object on the map.

Each property changes color of the element so player can identifier its properties more easy.
Each property can be combined with each other so there are 8 different possible combinations.

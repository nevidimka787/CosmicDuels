# Astro Duels
The idea of this game was got from the game “Astro Party”.
But this one had worse arcade physic, low count of maps and game rules to change.
So, I wrote new engine on C++ by myself using OpenGL as graphic API.


# Technique information
For starting project use Visual Studio 2022.
"Release" version of the game startup without console.
Game use 3 threads.
Physic calculation thread.
Input output thread.
Draw thread.
Now game ported to Linux, but work 100 time slower than on Windows.

# Controles
Game use next keys to control.
q - rotate ship 0
a - ship 0 shoots or pilot 0 goes
c - rotate ship 1
v - ship 1 shoots or pilot 1 goes
LEFT - rotate ship 2
DOWN - ship 2 shoots or pilot 2 goes
l - rotate ship 3
o - ship 3 shoots or pilot 3 goes

Use rotate button 2 times to douge.


# Game info.
This is battle royal. There are from 2 to 4 player try kill each other using different strategies. Also game has flexible parameter setting, that can fully change priority strategy in each match.
Team mode is available. Multiplayer mode will be added soon.

# Match making system.
There are up to 4 players can play this game at one time. Each player choice team. There are 2 or more teams must exist but any count of players can be in one team.

# Score system.
The main battle is in math that divided to round.
Round complete after 1 or less players alive.
Each player can be killed in some cases by another players in other cases by map, neutral objects ect. Score system takes into consideration these aspects. If a player kill another player then one point of score add to him. But if a player is killed by something other, this player lose one point of score.
After round complete each player add or lose points. Queue of adding and losing is similar as queue of events that cause this.
If player move to left side of score table then he loses and skips all next round until the match will be ended. If player move to right side of score table he play next round only with players who move to right side of score table too. If no one else move to right side of score table the the player win the match and the math is ended.
For team play mode rules are similar, but score get not only one player but all team.

# Game rules.
There are numerous rules in the game.
1) Random spawn. If it is active then players spawn in random corners else players spawn in defined by team-select-button corner.
2) Random spawn direction. If it is active then players spawn with random directions else they set target center of map.
3) Spawn with bonus. If it active then players will have random bonus in inventories after spawn else players will be spawned without bonuses.
4) Can spawn with different bonuses. If it is active then players spawn with different bonuses else all players will have similar bonus after spawn.
5) Triple bonuses. If active then count of any obtained bonus will be set to 3 (max count of bonuses of one type).
6) Spawn with triple. Ship will have buff "Triple soot" after spawn.
7) Spawn with shield. Ship will have buff "Personal shield" after spawn.
8) Knifes can destroy bullets. If it is active then knifes destroy bullet after collision with one and after that immediately destroyed else bullets fly trough knifes.
9) Need keel pilot. If it is active then pilots can respawn after some time and round will be ended after 1 ore less ship or pilot alive else pilots will not respawn and round will be ended after 1 or less ships exist.
10) Friendly sheep can restore. If it is active then pilots immediately respawn after touch with ally ship.
11) Friendly fire. Ally ships can destroy each other by bullets. If it is disable ally ships anyway can destroy each other by other mechanics.
12) Auto balance. If it is active then loser player obtain personal shield.

# Pull menus.
Spawn rules of objects can be setted in "Spawn objects pull menu".
Bonuses available in match can be selected in "Bonus pull menu".
Maps available in match can be selected in Map pull menu".

# Objects
There are numerous types of objects in the game.
1) Ship. It constantly fly forward. It can rotate in one direction. It can use its abilities.
2) Pilot. It spawn after ship was destroy. It can fly forward and rotate in one direction. After some time it respawn to ship (if match rules allow to do it).
3) Map. It contain some elements. Each element can change its size, scale, and angle. Each element can be unbreakable/destroyable, kill/doesn't kill players and immediately detonate bombs, destroy/doesn't destroy other objects and activate bombs. Each characteristic has its part or color. So maps with different properties has different colors.
4) Bonus. Can be obtain by any ship. After that it will be in the ship's inventory. Bonus constantly rotated and doesn't collision with any other object include map. Bonuses will be descripted in a next character.
5) Asteroid. Cyrcle object that can or can't keep some bonus or bonuses. Asteroid with one-time-use bonus has red shards. Asteroid with long-time-usable bonus has blue shards. Asteroid can have red and blue shards in one time. There are 3 size of asteroids. Big asteroid is divided to 4 medium asteroids. Medium asteroid is divided to 2 small asteroids. Small asteroid spawns after destroying keeping bonus.
6) Turret. The object can shoot rotate and is destroyed. Its behavior doesn't define.
7) Bullet. The object destroy a first object with which it collisions.
8) Laser. The object destroy all objects until it meets unbreakable map's element.
9) Knife. The object destroy all objects which it touch.
10) Bomb. The object has 3 phases. In the first phase it is passive and wait enemy player. If enemy player fly too close then the bomb activate and go to phase 2. I this phase it is been some defined time. After that it detonate and crate after it annihilation area.
11) Annihilation area. The object destroy all objects which it touch. Nothing can stop it. Two collision bullets is most common way to create annihilation areas.
12) Mega laser. The object has 2 conditions. In the first condition it do nothing. In second condition it destroy everything. Time from condition to condition is defined by map's rules.
13) Desseleration area. The area increase resistance to any object which move inside it.
14) Gravity generator. It generates gravity or antigravity around it.

# Bonus system
There are 4 bonuses, 3 buffs and 1 round rule changer in the game. Each ship can have up to 3 bonuses of one type and all types at one time (it is 12 bonuses). Bonuses can create combinations. 16 combinations are available include no-ability-combination. Using ability the larges combination is geted from ship's inventory and is used. 
# Abilityes
1) Basic shoot. If ship doesn't have any bonus and buff in its inventory then it create bullet on front of it.
2) Loop. Ship spawn little bullets around it.
3) Laser. Ship spawn laser on front of it. Laser can't change direction after spawn. But it can change position.
4) Bomb. Ship spawn bomb behind it. Bomb is ally for the ship.
5) Knife. Ship spawn two knifes. Knife's position relativity of the ship is constant. Each knife can destroy 3 map elements or asteroid. If knife destroy pilot or bullet it is destroyed immediately.
6) Loop creating (laser + loop). It is a laser that spawn bullets around each asteroid with bonus that it destroy.
7) Half-loop of bombs ability (loop + bomb). Ship spawn some bombs in half-cyrcle behind it. The bombs have started velocity.
8) Knife circle (loop + knife). Ship create around it some knifes which form circle. Each laser can destroy only one object.
9) Bomb delivery (laser + bomb). Ship shoots some activated bombs on front of it.
10) Long knife (laser + knife). Ship spawn long knife on front of it. The knife can destroy 5 asteroids and map elements or 1 pilot and turret.
11) Moving annihilation area (bomb + knife). Ship spawn annihilation area. The annihilation area has ship's velocity. It is the most questionable ability because it kill more ally players than enemies.
12) Asteroid overpower (laser + bomb + loop). Ship create laser that spawn annihilation area around any asteroid which it touch.
13) Wedge of annihilation areas (laser + bomb + knife). Ship create wedge of annihilation areas on front of it.
14) Loop of annihilation areas (loop + bomb + knife). Ship create loop of annihilation areas around it.
15) Hedgehog (loop + knife + laser). Ship create some long knife from its center. Each knife can destroy 2 asteroids and map element or 1 pilot or turret.
16) Annihilation area gun (loop + knife + laser). Ship spawn 2 guns which shoot annihilation areas. The guns has no reloading time.
# Buffs
1) Triple shoot. After using the buff change ship's gun to gun that shoots wedge of 3 bullets. Each shoot cost 3 bullets in magazine.
2) Increase magazine size. It is used immediately. After using the ship will have one more place for bullet in its magazine. Maximum amount of places is 12.
3) Personal shield. After using the ship get personal shield. Shield can save ship from one bullet and is destroyed after that. After the shield is destroyed the ship get unbreakable status one some time.
# Round rule changer.
1) Reverse. If the changer is obtained by any ship then rotation direction of all ships and pilots will be changed. Also this game rule can give impact to behavior of some other objects on map.

# Destroy and respawn.
After destroy any ship becomes a pilot. After some time pilot will become to ship. After becoming a ship, the ship is unbreakable some time. Some game rules can change respawn process.

# Maps
Each map's element has 3 properties.
1) Breakable or unbreakable.
2) Destroying or not destroying ships and pilots.
3) Destroying or not destroying not controlled object on the map.

Each property changes color of the element so player can identifier its properties easily.
Each property can be combined with each other so there are 8 different possible combinations.


# Project MineStorm
<hr />

**Authors : Omaya Lise , Bryan Bachelet**

For ISART Digital

<hr /><br />

## **Project description :**

Our goal was to recode the MineStorm, originally published in 1982 on the Vectrex. We recreated the game from scratch, excluding the sprites. To achieve our objective, we wrote our own math library and collision system before coding the game. 

To launch game : 
<br />Use the command "make && ./minestorm" in the terminal.

<br /><hr /><br />

## **Game's features :**
<ul>
<li>The player can shoot, move and teleport randomly on the screen.
<li>The player gets an invincibility bonus for a short time after being hit.
<li>Four types of enemies, with different patterns, exist in three sizes.
<li>All the enemies must be destroyed to reach next level.
<li>Each kill gives points that are added to the score at the screen's bottom. 
<li>The levels are randomly generated with more enemies each time. 
<li>Up to two players can play in cooperation (friendly fire is activated).
<li>Collider and direction of players, enemies and bullets can be seen by pressing C.</ul>

##  **`Design choice :`**  
`We voluntarily made the player's hitbox smaller than its sprite. Its collider is located at the spaceship's head so its back can be touched by enemies without making the player's life drop. We chose this to make the game easier and more playable for the player.`

##  **Features to add:**
<ul>
<li>Add mothership pattern
<li>Animation for the enemies and player death
<li>Music and sound
<li>Points scored appearing next to the ennemy destroyed
</ul><br />
<hr /><br />

## **Architecture**
<ul>
<li>assets</li><ul>
    <li>logo.png</li>
    <li>minestorm_background.png</li>
    <li>minestorm_foreground.png</li>
    <li>minestorm_sprite_atlas_mine_storm.png</li>
    </ul>
<li>src</li><ul>
      <li>collider.c</li>
      <li>collider.h</li>
      <li>ennemies.c</li>
      <li>ennemies.h</li>
      <li>game.c</li>
      <li>game.h</li>
      <li>mathLibrary.c</li>
      <li>mathLibrary.h</li>
      <li>menu.c</li>
      <li>menu.h</li>
      <li>player.c</li>
      <li>player.h</li>
      <li>projectile.c</li>
      <li>projectile.h</li>
      <li>sprite.h</li>
      <li>system.h</li>
</ul>
<li>third_party</li><ul>
      <li>include</li><ul>
      <li>raylib.h</li></ul>
      <li>libs-x86_64-linux-gnu</li>
      <li>libs-x86_64-pc-cygwin</li>
      </ul>
<li>.gitignore</li>
<li>Makefile</li>
<li>README.md</li>
</ul><br />

Code editor : Visual Studio Code
<br />
System : Linux


# Summary:
This project is a portoflio that showcases gameplay, UI and simple AI mechanics codded within UE engine. The ThirdPersonTemplate has been used as a baseline to start the project.

It is a stripped down version of a shooter where there are enemies and moving targets which you are able to destroy by shooting at them.
There is also some simple AI codded without the UE AI toolset, some simple UI and some specific mechanics like a platform which modifies the shooting speed.

As compilmentary documentation, there is a <a href="Documentation/html/index.html">ClassDocumentation.html</a> that you can open with your browser.
This documentation has been generated with <a href="https://www.doxygen.nl">Doxygen</a>.

# Player
The player is divided between the MShooterCharacter and the MShooterController. These classes extended the Third Person
Character and Controller functionality.

The player also has the ability to shoot projectiles. This shooting ability is handled by the MShooterShootingComponent
which enables any actor to shoot projectiles. If this component detects that the owner is the player,
it introduces a modifier that spawns the projectile according to the rotation of the camera, which helps
aim at the desired target. The shooting mechanic shoots automatically while you keep pressing the shoot key accordig
to a "shooting speed" variable. This component also allows for an "aim" mechanic. It locks the camera to a specific
enemy. This aim maechanic only can be canceled when you stop shooting or when there is no enemies left, since
it auto targets the nearest enemy upon killing the current target. You can also toggle aim again when shooting to
target the nearest enemy not taking into account the current target.

# Enemies
Enemies hold a series of components that make add functionality to them.

### Life Component:
This component makes this class killable. It adds a health attribute and a world spawnable widget represting a health bar.
This component handles taking damage as well as controlling the healthbar, so buy just calling the function TakeDamage within the 
enemy base class it will handle the whole process, including killing the enemy if it were necessary.

### Shooting Component:
Although shooting is not currently enabled within the AI for enemies, the enemies have the capability of shooting
when calling the Shoot function within this component. In their case, it would use the enemy mesh rotation
to determine the spawn location and rotation of the projectile. The aim functionality will also work, but
it is currently not set up to detect the player. It would suffice to pass the player to this component as a target within the enemy base class.

### AI Component:
This component handles the AI for the enemy, although it can be place on any actor (it does not need to be a pawn).
It has a simple state machine in form of an enum that currently handles Patroling, Rest and Chasing the player.
Attacking has been set as an option but it's not currently implemented.
<ul>
  <li>Patrol: the actor patrols within a specific patrol zone.</li>
  <li>Rest: every 5 seconds the actor rests for 3 seconds and then goes back to the previous state.</li>
  <li>Chase: chases the Player as soon as he enteres the patrol zone, and stops chasing as soon as the Player leaves the zone.</li>
</ul>

# Sooting Platform:
This platform allows to modify in real time the Player's speed while he is standing over the platform.

# Moving Target:
This target works similar to the enemies, but it does not have any AI. This actor holds a simple up and
down animation based on a public attribute that can be changed within the editor. This animation accounts for clipping, essentially it will make sure that it never goes below the floor accounting for the mesh size.


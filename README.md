# COMP2401 Final: Threads and Simulations

Thread based “Phasmaphobia” simulation game

# Program Authors  

Adam Esanhaji  
Anas Ismaili

# Description of the program:

This program is a simulation of "Phasmaphobia", a game where 4 hunters must collaborate to find a ghost and try to determine its type, based on the evidence the ghost leaves. This program uses threads to have all the hunters and ghost running/making decisions simultaneously. The ghost wanders around the house, randomly leaving evidence of different types, where each type can only be picked up by its corresponding device. The hunters must work together, since they can only carry a single device each. This means they must share any helpful data they collect with their device with the other hunters. If a hunter manages to find at least 3 pieces of evidence with different types and ghostly values, they win the game and leave the building. If a hunter can't find enough evidence, they will either end up getting too bored and leave, thinking theres no ghost, or will have too many ghostly encounters to handle and get too scared to continue, forcing them to leave. Similarly, the ghost can get bored and leave if it goes too long without any encounters with hunters. A hunter or ghost leaving means that their thread will be closed, and once all threads close the program will print the results of the hunt. This will show the reason each hunter left the building, and if any hunters found the ghost, its speculated type will be displayed.

# Insructions:  

- Compilation: In the terminal, navigate to the /SystemsFinal directory, then into the /src directory. Afterwards, type "make" without the quotation marks. This will create all the necessary object files then link them together into the simulation.exe executable.
- Launching and operating instructions: While in the /src directory where the compiled "simulation" file is saved along with the others, type "./simulation" into the terminal to execute the program, again without any quotation marks. Outputs will be printed to and inputs will be read from the terminal.
- Cleaning: While in the /src directory, run the "make clean" command to remove the previously generated object files and executable.

# List of files: 

- defs.h
- ghost.c
- room.c
- building.c
- evidence.c
- hunter.c
- main.c
- makefile
- README.md

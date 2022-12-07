# COMP2401 Final

COMP2401B Final Project: Threads and Simulations

# Program Authors  

Adam Esanhaji (student id: 101244122)  
Anas Ismaili (student id: 101196484)  

# Description of the program:

This program is a simulation of "Phasmaphobia", a game where 4 hunters must collaborate to find a ghost and try to determine its type, based on the evidence the ghost leaves. This program uses threads to have all the hunters and ghost running/making decisions simultaneously. The ghost wanders around the house, randomly leaving evidence of different types, where each type can only be picked up by its corresponding device. The hunters must work together, since they can only carry a single device each. This means they must share any helpful data they collect with their device with the other hunters. If a hunter manages to find at least 3 pieces of evidence with different types and significant values, they win the game and leave the building. If a hunter can't find enough evidence, they will either end up getting too bored and leave, thinking theres no ghost, or will have too many ghostly encounters to handle and get too scared to continue, forcing them to leave. Similarly, the ghost can get bored and leave if it goes too long without any encounters with hunters. A hunter or ghost leaving means that their thread will be closed, and once all threads close the program will print the results of the hunt. This will show the reason each hunter left the building, and if any hunters found the ghost, its speculated type will be displayed.

# Insructions:  

- Compilation: In the terminal, navigate to the /SystemsFinal directory, then into the /src directory. Afterwards, type "make" without the quotation marks. This will create all the necessary object files then link them together into the simulation.exe executable.
- Launching and operating instructions: While in the /src directory where the compiled "simulation" file is saved along with the others, type "./simulation" into the terminal to execute the program, again without any quotation marks. Outputs will be printed to and inputs will be read from the terminal.
- Cleaning: While in the /src directory, run the "make clean" command to remove the previously generated object files and executable.

# Design decisions

- Decided to have the hunters leave the van when they first spawn in, since the van won't have any evidence when first starting.  
- Made hunters make moves based on a fixed probability, i.e. they have a higher chance of making certain actions compared to others, depending on the circumstance.  
- Created a BOREDOM_RATE constant, to make it easier to change the speed at which the threads get bored.  
- Changed the BOREDOM_MAX constant to 100 so that it would work better with various BOREDOM_RATE values, like 1, 2, 5, 10, etc.  
- Increased the FEAR_RATE constant to 5, just to make hunters leave quicker (we found that encountering the ghost 100 times could take a while).  
- Reduced the MAX_STR constant to 21 since 64 made our print formatting take a lot of space horizontally.  

# Bonus marks

- Used github to allow for collaborative coding and version control. (Commit history screenshot included in submission)  
- Used print formatting to make the simulation data clear and easy to read.  
- Implemented a building shared evidence collection that all hunters pointed their evidence to, allow for easy memory freeing and avoiding double freeing. 

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

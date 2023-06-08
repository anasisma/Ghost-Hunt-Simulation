# Systems Programming Project: Threads and Simulations

Agent-based ghost hunt simulation made using low-level systems programming concepts in C.

# Program Authors  

Adam Esanhaji  
Anas Ismaili

# Description of the program:

This program is a simulation where 4 hunters must collaborate to find a ghost and try to determine its type, based on the evidence the ghost leaves. This program uses threads to have all the hunters and ghost running/making decisions simultaneously. The ghost wanders around the house, randomly leaving evidence of different types, where each type can only be picked up by its corresponding device. The hunters must work together, since they can only carry a single device each. This means they must share any helpful data they collect with their device with the other hunters. If a hunter manages to find at least 3 pieces of evidence with different types and ghostly values, they win the game and leave the building. If a hunter can't find enough evidence, they will either end up getting too bored and leave, thinking theres no ghost, or will have too many ghostly encounters to handle and get too scared to continue, forcing them to leave. Similarly, the ghost can get bored and leave if it goes too long without any encounters with hunters. A hunter or ghost leaving means that their thread will be closed, and once all threads close the program will print the results of the hunt. This will show the reason each hunter left the building, and if any hunters found the ghost, its speculated type will be displayed.

# Insructions:  

- Compilation: In a terminal, navigate into the /src directory then run the "make" command to compile source code into an executable
- Launching: Run the simulation executable and watch every agent's real-time actions being printed to the standard output.
- Cleaning: While in the /src directory, run the "make clean" command to remove the generated object files and executable.

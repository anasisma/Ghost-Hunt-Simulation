# COMP2401 Final

Created by Adam Esanhaji (student id: 101244122) and Anas Ismaili (student id: 101196484)  
COMP2401B Final Project: Threads and Simulations

# Due dates:

November 28: First Check-Up  
December 5: Second Check-Up  
December 7: Project Deadline

# Description of the program:

This program is a simulation of "Phasmaphobia", a game where 4 hunters must collaborate to find a ghost and try to determine its type, based on the evidence the ghost leaves. This program uses threads to have all the hunters and ghost running/making decisions simultaneously. The ghost wanders around the house, randomly leaving evidence of different types, where each type can only be picked up by its corresponding device. The hunters must work together, since they can only carry a single device each. This means they must share any helpful data they collect with their device with the other hunters. If a hunter manages to find at least 3 pieces of evidence with different types and significant values, they win the game and leave the building. If a hunter can't find enough evidence, they will either end up getting too bored and leave, thinking theres no ghost, or will have too many ghostly encounters to handle and get too scared to continue, forcing them to leave. Similarly, the ghost can get bored and leave if it goes too long without any encounters with hunters. A hunter or ghost leaving means that their thread will be closed, and once all threads close the program will print the results of the hunt. This will show the reason each hunter left the building, and if any hunters found the ghost, its speculated type will be displayed.

hunters leave van before doing anything else, but can come back later  
boredrom rate = 2
boredom max = 100
fear rate = 5
hunter names up to 20 characters

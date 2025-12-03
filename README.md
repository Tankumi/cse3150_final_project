# cse3150_final_project
 

 To use, run these two commands:

 c++ src/*.cpp -Iinclude -std=c++17

 ./a.out /workspaces/cse3150_final_project/CAIDAASGraphCollector_2025.10.16.txt /workspaces/cse3150_final_project/anns.csv /workspaces/cse3150_final_project/rov_asns.csv


 Make sure to replace the file paths and names to whatever it is on your system.


This program runs in literally like 3 seconds on my system.
Some of the design decisions to make my program quick were adding references when possible. I split everything into a bunch of classes, some of which inherit other classes. For example, ASGraph is made out of ASNodes that are in a unordered_map. I used unordered_maps because they have O(1) access time. The prefix is stored in a specific class, so it doesn't have to be stored as a string, but instead the values are put into an array.
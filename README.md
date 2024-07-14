# Pizza Delivery Robot

## Development of a Pizza Delivery Robot

**Authors:**  
Artem Paliesika, Mhd Bashar Al Fattal  
**Course:**  
Studiengang Informatik  
**Department:**  
Fachbereich Informatik und Medien  
**Date:**  
03.01.2024  
**Project Supervisors:**  
Dipl.-Inform. Ingo Boersch, Prof. Dr. Emanuel Kitzelmann  

## Introduction

The goal of this project is to create a self-driving pizza delivery robot. The robot utilizes its sensors to recognize intersections, analyze the map and obstacles, and determine the shortest path to successfully deliver pizzas to destination addresses.

## Functionality

The robot's journey begins when it recognizes a light on the table. As soon as the light turns on, the code on the machine board is executed. The robot is programmed to always follow a line and execute instructions at intersections. The map of the table is represented as a string, where each character denotes an intersection. For example: `{F..x..}`, where the dot represents a drivable intersection, and `x` indicates an invalid intersection that cannot be used to create a path to the destination address `F`. The routing algorithm of the robot consists of two main procedures: Planner and Interpreter.

### Planner

The robot performs the Breadth-First Search (BFS) algorithm, analyzing characters in the string to determine the shortest path to the delivery address. Each intersection is an element in the string with its position representing its ID. The algorithm starts by inserting all neighboring intersections of the start position into the queue and saving their parent intersections in an array. It then dequeues and analyzes the elements one by one. If the dequeued element is the destination address `F`, the search stops, and the path is built based on the parent array. If the element is not the destination, its neighbors are inserted into the queue, and the search continues. If no path to `F` is found and there are no elements left in the queue, the algorithm stops and returns `1`, indicating that the route was not found.

### Interpreter

If the search is successful and the path to the destination is built, the robot still needs instructions on where to drive and which turns to take. The interpreter is responsible for this. It analyzes each element and its follower in the path array. Based on the direction the robot is facing and the elements of the array, it creates a new character array called `instructions`, which can contain the following elements: `R` (right turn), `L` (left turn), and `F` (forward movement). After the delivery, the robot needs instructions to return to the starting position. This is achieved by reversing the instructions array and swapping all left turns with right turns and vice versa.

## Hardware

The robot is constructed using LEGO bricks and various sensors that enable it to drive and make successful deliveries. The key components of the robot are:
- Machine board
- Two engines for the wheels
- Two infrared sensors for distinguishing black and white colors
- One light sensor
- One pressure sensor
- Cargo bay with an opening gate
- Acoustic alert system
- Framework to hold all parts together

[KI_Poster.pdf](https://github.com/user-attachments/files/16216465/KI_Poster.pdf)

<img src="docs\media\photos\transparent_logo_sized.png" width="400" alt="logo">

An interactive, laser-shooting game bootlegged off Rock'Em Sock'Em Robots

### Creators:

[Andrew Carr](https://andrewpatcarr.github.io/projects/)

[Kai De La Cruz](https://sites.google.com/view/kaicustoms/home?authuser=0) 


## Documentation:

### [Zap'Em Blast'Em Robots Documentation](https://andrewpatcarr.github.io/zap_em__blast_em/)

A more thorough look at our project.

## File Organization:

All of the materials necessary to replicate this project are provided.

/hardware has the mechanical and electrical designs. This includes CAD files, our PCB design, a bill of materials, and our MCU pinout.

/software has all of the code, written in c, to run our project

## Game Play:

[![Watch the video](https://img.youtube.com/vi/SelIsVGXWzg/0.jpg)](https://www.youtube.com/watch?v=SelIsVGXWzg)


## Project Overview:

Zap'Em Blast'Em Robots is an interactive game inspired by Rock'Em Sock'Em Robots, developed as part of the Mechanical Control System Design course at Cal Poly, San Luis Obispo. Designed by graduate students in the Mechanical Engineering program, the project showcases a fully integrated system combining mechanical, electrical, and software components.

Over the course of a 10-week quarter, our team designed a complete mechanical design, fully integrated electronics, and task-based software design.

Our mechanical design focuses on game-specific mechanics, aesthetics and manufacturability. We had multiple actuators and sensors that needed to be implemented in tight spaces while minimizes structural size.

![mech design](docs\media\photos\assemblyCAD.png)

Our electrical design focused on integration and support for our significant amount of game features. It was our first time designing a PCB so lots of time was spent learning design techniques and iterating based on feedback from our instructor **Charlie Refvem**. 

![pcb](docs\media\photos\MCU_Design_v32.png)

Our software design was designed a task-based system. Each task uses a finite state machine implemented with "pseudo-object-oriented" C structs. This architecture makes our code base scalable and organized.

** add photo


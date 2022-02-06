# Playing with Physics and Projective Geometry

This is a small old (2014) project I did during a graphic computation course. It is written with spanish vocabulary, I will translate it to English presently.

It is coded in C++ and makes use of Allegro 5.0.10.

Here I play with spherical particles under gravity and their elastic interactions among them and with the floor.

The 3D representation of the scene on the 2D Allegro canvas is calculated using projective geometry principles.

All the code is in a single file and during the execution the interaction is done selecting the console and using the keyboard.

The program makes use of OpenMP to parallelize the calculation of the particles' behavior.



![Optional Text](C:\Users\adrig\source\repos\PhysicsAndProjectiveGeometry\Pictures\bolas_2.png)

## Changing the observer

The observer and the light source are in the same starting spot in space (0,-10000,-25000). 

### Observation controls



- W- positive Z

- S- negative Z

- Q- negative Y

- E- positive Y

- A- negative X

- D- positive  X

### Particles properties controls



- M- Adds 50 particles.

- N- Deletes 50 particles.

- L- IncreasesK elastic constant in 0.1.

- J- Decreases K elastic constant in 0.1.

- B- ON/OFF light reflection on the particles.

- O- End execution.





 

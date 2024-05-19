
# An approach to the world of graphics programming.

**Computer graphics** is a fascinating field of computer science. Who would have thought that, with just a couple of functions and basic physics principles, we could create fairly accurate **simulations of real-world natural systems**? I'm by no means an expert in this field, but I would like to become one in the future.  
This is my first attempt at graphics programming using **OpenGL**. I hope you find this article useful or interesting, at least. **Enjoy!** [Source code](https://github.com/salvaKaraka/Particle_simulation).

![Particles](https://giphy.com/embed/Szu98O1hHYdhirAxIc)

## What is this project about?

My goal was learning **OpenGL** basics. I used **C++** to create a particle simulation, which has **three main parts**:
1. **Entry point:** It is the main function, which contains an *"infinite"* loop to execute each step of the simulation.
2. **Particles:** Each particle is an instance of the same **particle object**. They have velocity, acceleration, position, color, mass, amorg others.
3. **Collision handler:** Its function is **detecting collisions** between particles and with the environment and **modifying the particle's position, velocity, and acceleration** accordingly.

Additionally, I added **gravity acceleration** to make the particles fall to the ground and a **collision damping coeficient** so the system loses kinetic force with each impact. If the collisions where elastic *(meaning the system does not lose kinetic force with collisions)*, this coeficient would be set to 1.


## Libraries that I used:
* **GLFW:** *It is a library that lets us create and modify OpenGL windows and recieve inputs from the periphericals.*
* **Glad:** *As most OpenGL functions' locations are not known at compile-time, they need to be queried at run-time. Glad solves this issue by managing these function pointers.*

## Development process:
* **Initialization:**  
I started creating the main window using **GLFW**, then I initialized **Glad**. *I'm not going to go into much detail since it's not the focus of this project, but if you want to know how I did it, you can check the [Source code](https://github.com/salvaKaraka/Particle_simulation).*

* **Particle Object:**  
It has the following attributes:

```cpp
    std::vector<float> position;
    std::vector<float> nextPosition;
    float mass;
    float radius;
    std::vector<float> velocity;
    std::vector<float> acceleration;
    std::vector<float> color;
```
* **The graphics pipeline**:  
OpenGL uses a resterization pipeline to go from an array of vertices specified within the program to an image on the screen:

![graphics_pipeline](https://graphicscompendium.com/intro/figures/graphics-pipeline.png)
   1. **Vertex Shader:** *Takes the vertex information and transforms it if necessary (for example, to apply perspective).*
   2. **Shape Assembly:** *Takes the resulting positions from applying the vertex shader and connects them according to a primitive *(point, line or polygon)*. In this case, *triangles*.*
   3. **Rasterization:** *The shapes generated previously are translated into pixels that can be displayed on the screen.*
   4. **Fragment Shader:** *Computes the expeected color of each pixel of the primitive.*
   5. **Test and Blending:** *In case of having overlapping objects, their colors are "blended" depending on their opacity and other factors.*

* **Particle rendering:**  
 Now that we understand how OpenGL works, we can delve into the details of how I managed to **show the particles on the screen**. In this case, I used **triangular primitives** to render my circular particles. To achieve this, I **divided the circle into segments**, similar to slicing a pizza, **placing vertices along its perimeter and drawing triangles between them and the central point**, the **more vertices** we add, the **better resolution** the circle has: 

![circle_drawing](https://i.stack.imgur.com/NXQbJ.png)

To do so, I first had to **calculate the position of each vertex and store it into an array**:

```cpp
    std::vector<float> vertices;
    for (int i = 0; i < steps; ++i) { //steps is the ammount of vertices that we will calculate
        float x = pos[0] + radius * cos(angle * i); //pos is the position of the center of the circle
        float y = pos[1] + radius * sin(angle * i);
        vertices.push_back(x);
        vertices.push_back(y);
    }
```
Then, I created and configured the **Vertex Buffer Object (VBO)** to store the vertex data and the **Vertex Array Object (VAO)** to organize and configure that data for processing and rendering.  
Buffers are useful because they can be used to **send large amounts of data to the GPU**, which makes the transfers faster. They also allow for more efficient memory usage since **the data resides directly in the GPU's memory**, where it's needed for rendering.
*I'm not displaying the code here because it's pretty standard, but if you're interested, you can check the [Source code](https://github.com/salvaKaraka/Particle_simulation).*

* **Collision handler:**  
It's job is to detect collisions and respond according laws of physics, ensuring that the particles interact realistically with each other and with the boundaries of the simulation environment.
The Collision Handler consists of two main functions: **handleBorderCollisions** and **handleParticleCollisions**:
   1. **Border Collisions**: The handleBorderCollisions function detects collisions between the particles and the boundaries of the simulation area. If a particle collides with a border, its velocity in that direction is inverted and a damping factor is applied to simulate the loss of energy in the collision.

        ```cpp
            void CollisionHandler::handleBorderCollisions(Particle& p) {
                std::vector<float> velocity = p.getVelocity();
                std::vector<float> position = p.getPosition();
            
                if (abs(p.getXPosition()) + p.getRadius() > width/2) {
                    // Reverse velocity and apply damping
                    position[0] = (width/2 - p.getRadius()) * sign(position[0]);
                    velocity[0] *= -1 * borderCollisionDamping;
                }
                
                if (abs(p.getYPosition()) + p.getRadius() > height/2) {
                    // Reverse velocity and apply damping
                    position[1] = (height/2 - p.getRadius()) * sign(position[1]);
                    velocity[1] *= -1 * borderCollisionDamping;
                }
            
                p.setPosition(position);
                p.setVelocity(velocity);
            }
        ```
   2. **Particle Collisions:**  If the distance between two particles is less than the sum of their radii, it means that they collided.  

        ```cpp
        bool CollisionHandler::collide(Particle p1, Particle p2) {
            return (calculateDistance(p1, p2) <= p1.getRadius() + p2.getRadius());
        }
        ```
    
    The **handleParticleCollisions** function detects collisions between individual particles. If two particles collide, the new velocity of each particle is calculated using the laws of conservation of momentum and energy. Additionally, a damping factor is applied to model the energy loss in the collision.
```cpp
            void CollisionHandler::handleParticleCollisions(Particle& p1, Particle& p2) {

                // variable declarations
                // ...

                if (&p1 != &p2) { //It avoids comparing a particle with itself
                    if (collide(p1, p2)) { //if there is a collision
                        
                        //distance between centers
                        std::vector<float> diff_x(2);
                        diff_x[0] = xp1 - xp2;
                        diff_x[1] = yp1 - yp2;
                    
                        float dot_product = (xv1 - xv2) * diff_x[0] + (yv1 - yv2) * diff_x[1];
            
                        float norm_squared = diff_x[0] * diff_x[0] + diff_x[1] * diff_x[1];
            
                        //Formula to calculate the new velocity of the particle
                        float new_xv1 = (xv1 - (2 * m2 / (m1 + m2)) * dot_product / norm_squared * diff_x[0])* particleCollisionDamping;
                        float new_yv1 = (yv1 - (2 * m2 / (m1 + m2)) * dot_product / norm_squared * diff_x[1]) * particleCollisionDamping;
                        
                        //Update particle's velocity
                        p1.setVelocity({ new_xv1, new_yv1 });
                        p1.setPosition({
                            xp1 - (p2.getPosition()[0] - p1.getPosition()[0])/10,
                            yp1 - (p2.getPosition()[1] - p1.getPosition()[1])/10
                        });
                    }
                }
            }
```
   * **Position Update:** The **updatePositions** function is responsible for updating the positions of the particles at each simulation step. First, it updates the properties of each particle, such as velocity and position, according to external forces like gravity. Then, using the previously mentioned functions, it checks for collisions with the boundaries and between the particles themselves, adjusting positions and velocities accordingly.
```cpp
        void CollisionHandler::updatePositions() {
            for (Particle& p : particles) {
        
                // Update particle properties
                p.setXVelocity(p.getXVelocity() + p.getXAcceleration() * dt);
                p.setYVelocity(p.getYVelocity() + p.getYAcceleration() * dt - gravity * dt);
        
                p.setXPosition(p.getXPosition() + p.getXVelocity() * dt);
                p.setYPosition(p.getYPosition() + p.getYVelocity() * dt);
        
                // Check border collisions
                handleBorderCollisions(p);
        
                // Check particle collisions
                for (Particle& p2 : particles) {
                    handleParticleCollisions(p, p2);
                }
        
                p.draw();
            }}           
```

   * **Other functions:** In addition to the main functions, the Collision Handler includes auxiliary functions to calculate distances, dot products, and normalize vectors. These functions are used in the computation of collisions between particles. you can check them out **[Here](https://github.com/salvaKaraka/Particle_simulation).**  


* **Entry Point *(main function)*:** This is where all the parts come together. After the initialization is done, the main function executes the main loop, where the simulation occurs.

```cpp
        int main() {
            // GLFW initialization
            // ...
        
            // GLFW Window creation
            // ...
        
            // Glad initialization
            // ...
        
            // Particle array creation
            std::vector<Particle> particles;
            // ...
        
            // Collision Handler initialization
            CollisionHandler handler(particles, width, height, deltaTime, gravity);
        
            // Main loop, rendering and update
            while (!glfwWindowShouldClose(window)) //while the window is open
            {
                // User imput
                processInput(window, handler, particlesRestore);
        
                // Particle position update
                handler.updatePositions();
        
                // OpenGL functions to manage buffers and events
                // ...
            }
        
            // GLFW Termination
            // ...
        }
```

In this code snippet, a main loop is created that runs until a window close request is detected. During each iteration of the loop, user input is processed, the scene is rendered, particle positions are updated through the Collision Handler, and GLFW events are managed.

## Things to improve:
Even though the program works fine, there are many things that could get better, such as the collision detection and handling algorithm. The current algorithm has a cost of O(n^2), which is far from optimal. 
Additionally, the simulation currently runs on a single core of the processor.  
Here are some possible solutions to these problems:
1. **Improved collision detection:**  
    
   * **Spatial Partitioning:** Collisions can be calculated using a fixed grid to separate the environment into cells. Then, collisions would only be checked if two particles are in the same cell or in contiguous ones.

2. **Parallel processing:**
   * **Multithreading:**  The workload can be divided across multiple CPU cores, drastically enhancing performance.
   * **Compute Shaders:** We can use the GPU for general-purpose computing. It is designed to execute complex calculations at a much faster rate compared to traditional CPU processing. The parallel computing capabilities of compute shaders are useful to significantly improve the performance of the simulation.

## Conclusions:
I had a great time exploring the fundamental concepts of physics and computer graphics. Despite having accomplished my goal of creating a simple particle simulation, now that I've learned so much, I feel the next step is applying this knowledge to improve performance, introduce interactive elements for users, enhance the visual representation of particles, or even create more complex simulations. Perhaps demonstrating a Gaussian distribution or refining the particle object for a fluid simulation. Stay tuned—I'll keep working on it!

## Relevant links:
* **[Source code](https://github.com/salvaKaraka/Particle_simulation)**: Github repository with the project files.
* **[Victor Gordan](https://www.youtube.com/@VictorGordan)**: Great graphics programming YouTube channel.
* **[LearnOpenGL.com](https://learnopengl.com/Getting-started/Creating-a-window)**: The perfect E-book to start learning  OpenGL.
* **["Writing a Physics Engine from scratch - collision detection optimization"](www.youtube.com/watch?v=9IULfQH7E90&t)**: A video I watched to learn about particle collisions.
* **[Video: "Coding Adventure: Simulating Fluids"](https://www.youtube.com/watch?v=rSKMYc1CQHE)**: Another video with a nice explanation of how particle simulations work.

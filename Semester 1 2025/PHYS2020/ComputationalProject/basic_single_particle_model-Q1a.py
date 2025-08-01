#Basic simulation of trajectories of a single particle in a two-dimensional box.
#Saves position of particle as a function of time, and a movie showing the trajectories.
#Written by Warwick Bowen, February 2021 [translated to Python by Carla Verdi in 2024]. Can be used as a basis for the 
# computational project of PHYS2020.

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

T_i = 300    #Temperature in Kelvin
k = 1.38e-23 #Boltzmann constant in J/K
m = 1e-26    #Mass of a particle in kg
L = 1 	     #Length of cube side in meters
N = 1	     #Number of particles (only 1 particle here)

#Particle position: randomly locating particle
#Note: np.random.rand() pulls a random number from uniform distribution in range 0 to 1
#Note 2: np.random.rand() can be used to create a vector or matrix of random numbers using the syntax np.random.rand(m,n) 
# (where m,n are the size of the matrix). You could use this to generate initial positions for many particles in a vector

x_i = np.random.rand()
y_i = np.random.rand()

#Particle velocity: selecting initial particle velocity components
vx_i = 400  #initial x velocity in m/s
vy_i = 200  #initial y velocity in m/s

Tau = L/min(vx_i,vy_i)  #time for particle to traverse box. Note: when you start pulling velocities from a thermal distribution, 
#you may instead want to use the average thermal velocity here
tmax = Tau*20  #tot simulation time: choosing a sim. time for which the particle collides many times with the walls
tres = Tau/500  #time step: choosing a time resolution small compared to the time between collisions with walls
N_time = round(tmax/tres)  #number of time steps in simulation

#Plotting the initial position of the particle
fig = plt.figure(1,figsize=(5,5))
ax = plt.axes(xlim=(0,L), ylim=(0,L))
particle = ax.plot(x_i, y_i, '.', markersize=15)[0]  #for creating the movie later

x = np.zeros((N,N_time))  #Initializing positions in loop. Here, we store the positions sequentially in time in a matrix, with 
#one dimension being the number of particles and the other being time. Of course here we have only 1 particle, but it will be 
# easy for you to extend this to simulate multiple particles. Note that initialising the variables like this, rather than increasing the size 
# at each step in the loop, makes the computation faster.
y = np.zeros((N,N_time))
x[:,0] = x_i  #initial position x
y[:,0] = y_i  #initial position y

vx = np.zeros(N)  #Initialising velocities in loop. No need to store the velocities for each time step (but you can)
vy = np.zeros(N)
vx[:] = vx_i  #initial velocity x
vy[:] = vy_i  #initial velocity y

T = []  #Initialising temperature of ensemble (note, without any potential energy this should not change during the simulation. 
#You could use this as one check that the simulation is working properly)

#As we are interested in keeping track of the impulse on the right wall over
#time, we initialise lists for Impulse and Time
J_x = []
Time = []
#Additionally, we employ a variable J_total to keep track of the
#total impulse on the right wall
J_total = 0

for ring in range(1, N_time):  #Using a "for" loop to loop through time, propagating the trajectory of the particle. If you store the positions and 
    #times of your multiple particles in a matrix, this same for loop should work for you.
    x[:,ring] = x[:,ring-1] + vx*tres  #propagating the position: i.e., position 2 is equal to position 1 plus velocity times time step
    y[:,ring] = y[:,ring-1] + vy*tres 

    #Checking if particle has moved outside the box and reversing the relevant velocity component if it has. 
    #Here, x[ring,:]>L retrieves the indices of x values at the time "ring" for which the particle is outside of the box. 
    #Since there is only one particle here, it is somewhat redundant to find the indices. An alternative and equivalent code 
    #would be: if x[ring]>L; vx=-vx; end
    #and similar for the other three walls. We use the indices because it should be easier for you to extend this to 
    #multiple particles - applied to a vector of particle positions it will simultaneously identify each particle 
    #that is outside the bounds of the box
    #Checks if the particle is hitting right wall
    ind = x[:,ring] > L
    vx[ind] = -vx[ind]
    
    #Add the momentum change of each particle in
    #contact with the wall to the total wall impulse
    J_total += 2*m*np.sum(abs(vx[ind]))

    #Append the impulse and time of current itteration to respective lists
    J_x.append(J_total)
    Time.append(ring*tres)



    ind = x[:,ring] < 0
    vx[ind] = -vx[ind]

    ind = y[:,ring] > L
    vy[ind] = -vy[ind]

    ind = y[:,ring] < 0
    vy[ind] = -vy[ind]

    
#Adding labels to the plot
ax.set_xlabel('x position of particle in box (m)')
ax.set_ylabel('y position of particle in box (m)')
ax.set_title('Particle in a box')

step_anim = 50  #only plot every 50 steps to make it faster
xn = x[:,::step_anim]  #select data every 50 steps
yn = y[:,::step_anim]

def animate(frame):
    particle.set_data(xn[:,frame], yn[:,frame])
    return particle

#Plotting the particle trajectory
nframes = round(N_time/step_anim)
anim = animation.FuncAnimation(fig, animate, frames=nframes, repeat=False, interval=1)
#Saving the movie as a gif
anim.save('particle_trajectory_py.gif', writer='pillow', fps=10)

#A figure is made in order to plot the Impulse against Time for 
#the single particle simulation
plt.figure()
plt.plot(Time, J_x) #Plots the data as poinst
#Title axis and apply grid
plt.xlabel('Time (s)')
plt.ylabel('Impulse (N*s)')
plt.title('Impulse on right wall over time for Single Particle')
plt.grid(True)
plt.tight_layout()
#Show the graph
plt.show()

#The force applied on the wall:
#Force is given by total impulse over total runtime
average_force = J_total / tmax
print(f"The average force was {average_force:.3e}N")
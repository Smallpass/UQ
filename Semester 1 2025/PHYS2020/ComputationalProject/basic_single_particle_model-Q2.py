#Basic simulation of trajectories of a single particle in a two-dimensional box.
#Saves position of particle as a function of time, and a movie showing the trajectories.
#Written by Warwick Bowen, February 2021 [translated to Python by Carla Verdi in 2024]. Can be used as a basis for the computational project of PHYS2020.

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

T_i = 0.5 #Temperature in Kelvin
k = 1.38e-23 #Boltzmann constant in J/K
m = 6.65*10**(-27)    #Mass of helium in kg
L = 1*10**(-8) 	     #Length of cube side in meters
N = 16   #Number of particles (only 1 particle here)
#Epsilon and sigma values for helium
epsilon = 1.5*10**(-23)
sigma = 0.264*10**(-9)

#Particle position: randomly locating particle
#Note: np.random.rand() pulls a random number from uniform distribution in range 0 to 1
#Note 2: np.random.rand() can be used to create a vector or matrix of random numbers using the syntax np.random.rand(m,n) (where m,n are the size of the matrix). You could use this to generate initial positions for many particles in a vector

x_i = np.random.rand()
y_i = np.random.rand()

vx = np.zeros(N)  #Initialising velocities in loop. No need to store the velocities for each time step (but you can)
vy = np.zeros(N)

#To initialise the velocities of all N particles, we 
#draw values randomly from a normal distibution with
#standard deviation given by the equipartition for
#thermal equilibrium
Vstd = np.sqrt(k * T_i / m)
#Create lists for horizontal and vertical velocities
#of N particles
vx = np.random.randn(N) * Vstd 
vy = np.random.randn(N) * Vstd


tres = 0.5*10**(-13)  #time step: choosing a time resolution small compared to the time between collisions with walls
N_time = 5000  #number of time steps in simulation

#Plotting the initial position of the particle
fig = plt.figure(1,figsize=(5,5))
axis = plt.axes(xlim=(0,L), ylim=(0,L))
particle = axis.plot(x_i, y_i, '.', markersize=15)[0]  #for creating the movie later

x = np.zeros((N,N_time))  #Initializing positions in loop. Here, we store the positions sequentially in time in a matrix, with one dimension being the number of particles and the other being time. Of course here we have only 1 particle, but it will be easy for you to extend this to simulate multiple particles. Note that initialising the variables like this, rather than increasing the size at each step in the loop, makes the computation faster.
y = np.zeros((N,N_time))

#We create a for loop to even space each of the particles initial locations
n_side = int(np.ceil(np.sqrt(N))) #Number of particels per side
num_particles = 0
spacing = L / n_side               # Distance between particles
for i in range(n_side):
    for j in range(n_side):
        if num_particles < N:
            x[num_particles, 0] = (i + 0.5) * spacing
            y[num_particles, 0] = (j + 0.5) * spacing
            num_particles += 1
        else:
            break

ax = np.zeros(N)
ay = np.zeros(N)

T = np.zeros(N_time)  #Initialising temperature of ensemble (note, without any potential energy this should not change during the simulation. You could use this as one check that the simulation is working properly)
T[0] = T_i
Time = []
P = []

#We define a function acceleration() which returns a list [ax, ay],
#where ax and ay are lists containing the x and y accelerations for each particle
def acceleration(x, y, N):
    ax = np.zeros(N)
    ay = np.zeros(N)
    #Itterate for each particle
    for i in range(0, N):
        #Itterate again to find interaction between particle i and the rest of them
        for j in range(i+1, N): 
            x_distance = x[i]-x[j]
            y_distance = y[i] - y[j]
            r = np.sqrt(x_distance**2 +y_distance**2)

            #Consider that force is the derivative of V with respect to r
            F = 24*epsilon*(2*(sigma**12/r**13)-sigma**6/r**7)
            F_x = F*(x_distance/r)
            F_y = F*(y_distance/r)

            #Adjusting acceleration values by calculated interaction
            ax[i] += F_x/m
            ax[j] -= F_x/m
            ay[i] += F_y/m
            ay[j] -= F_y/m
    return ax,ay

for ring in range(1, N_time):  #Using a "for" loop to loop through time, propagating the trajectory of the particle. If you store the positions and times of your multiple particles in a matrix, this same for loop should work for you.
    ax, ay = acceleration(x[:, ring-1], y[:, ring-1], N)
    J = 0
    # 2. Update positions using current velocity and acceleration
    x[:, ring] = x[:, ring-1] + vx * tres + 0.5 * ax * tres**2
    y[:, ring] = y[:, ring-1] + vy * tres + 0.5 * ay * tres**2

    # 3. Half-step velocity update
    vx = vx + 0.5 * ax * tres
    vy = vy + 0.5 * ay * tres

    # 4. Recompute acceleration at new position
    ax_new, ay_new = acceleration(x[:, ring], y[:, ring], N)

    # 5. Complete the second half-step of velocity update
    vx = (vx + 0.5 * ax_new * tres)*0.999
    vy = (vy + 0.5 * ay_new * tres)*0.999


    #Checking if particle has moved outside the box and reversing the relevant velocity component if it has. 
    #Here, x[ring,:]>L retrieves the indices of x values at the time "ring" for which the particle is outside of the box. 
    #Since there is only one particle here, it is somewhat redundant to find the indices. An alternative and equivalent code 
    #would be: if x[ring]>L; vx=-vx; end
    #and similar for the other three walls. We use the indices because it should be easier for you to extend this to 
    #multiple particles - applied to a vector of particle positions it will simultaneously identify each particle 
    #that is outside the bounds of the box
    ind = x[:,ring] > L
    vx[ind] = -vx[ind]
    J += 2*m*np.sum(abs(vx[ind]))

    ind = x[:,ring] < 0
    vx[ind] = -vx[ind]


    ind = y[:,ring] > L
    vy[ind] = -vy[ind]


    ind = y[:,ring] < 0
    vy[ind] = -vy[ind]


    T[ring] = 0.5 * m * np.mean(vx**2 + vy**2) / k
    KE = 0.5 * m * np.sum(vx**2 + vy**2)
    Time.append(ring * tres)
    pressure = J/(tres * 4*L)
    P.append(pressure)

    print(f"Step {ring}, KE = {KE:.2e}, T = {T[ring]:.2f}, P = {pressure}")

#Adding labels to the plot
axis.set_xlabel('x position of particle in box (m)')
axis.set_ylabel('y position of particle in box (m)')
axis.set_title('Particle in a box')

step_anim = 50  #only plot every 50 steps to make it faster
xn = x[:,::step_anim]  #select data every 50 steps
yn = y[:,::step_anim]

T = np.array(T)
P = np.array(P)

#Reshape into 5 rows and 1000 columns
#P_reshaped = P[:5000].reshape(100, 50)  # Ensure only first 5000 values if extra

# Compute average of each group
#P_means = P_reshaped.mean(axis=1)

#print(P_means)

Time = np.array(Time)
nonzero_indices = np.nonzero(P)[0]  # or: np.where(P != 0)[0]

# Use these indices to extract corresponding values
T = T[nonzero_indices]
P = P[nonzero_indices]
Time = Time[nonzero_indices]
info_text = axis.text(0.02, 0.95, '', transform=axis.transAxes, verticalalignment='top')
step_anim = 50
ds = pd.DataFrame({
    'Time': Time,
    'Temperature': T,
    'Pressure': P
})
ds.to_excel('dummy.xlsx', sheet_name='TempTrial')

def animate(frame):
    particle.set_data(xn[:,frame], yn[:,frame])
    return particle

#Plotting the particle trajectory
nframes = round(N_time/step_anim)
anim = animation.FuncAnimation(fig, animate, frames=nframes, repeat=False, interval=1)
#Saving the movie as a gif
anim.save('particle_trajectory_py.gif', writer='pillow', fps=10)
plt.show()

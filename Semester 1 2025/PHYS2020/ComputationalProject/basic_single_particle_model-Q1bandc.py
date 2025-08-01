#Basic simulation of trajectories of a single particle in a two-dimensional box.
#Saves position of particle as a function of time, and a movie showing the trajectories.
#Written by Warwick Bowen, February 2021 [translated to Python by Carla Verdi in 2024]. Can be used as a basis for the computational project of PHYS2020.

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

T_i = 300    #Temperature in Kelvin
k = 1.38e-23 #Boltzmann constant in J/K
m = 1e-26    #Mass of a particle in kg
N = 49    #Number of particles (only 1 particle here)

#We also notice that the definition of Tau must be altered
#to accound for this velocity change, we will take the
#averate thermal velocity given by the rms
average_v = np.sqrt(2 * k * T_i / m)


#Particle position: randomly locating particle
#Note: np.random.rand() pulls a random number from uniform distribution in range 0 to 1
#Note 2: np.random.rand() can be used to create a vector or matrix of random numbers using the syntax np.random.rand(m,n) (where m,n are the size of the matrix). You could use this to generate initial positions for many particles in a vector
def simulation(L):
    #Multiply initial positions by L to ensure initial position is inside the box
    x_i = L * np.random.rand()
    y_i = L * np.random.rand()

    vx = np.zeros(N)  
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

    Tau = L/average_v  #time for particle to traverse box. Note: when you start pulling velocities from a thermal distribution, you may instead want to use the average thermal velocity here
    tmax = Tau*20  #tot simulation time: choosing a sim. time for which the particle collides many times with the walls
    tres = Tau/500  #time step: choosing a time resolution small compared to the time between collisions with walls
    N_time = round(tmax/tres)  #number of time steps in simulation
    
    x = np.zeros((N,N_time))  #Initializing positions in loop. Here, we store the positions sequentially in time in a matrix, with one dimension being the number of particles and the other being time. Of course here we have only 1 particle, but it will be easy for you to extend this to simulate multiple particles. Note that initialising the variables like this, rather than increasing the size at each step in the loop, makes the computation faster.
    y = np.zeros((N,N_time))
    x[:,0] = x_i  #initial position x
    y[:,0] = y_i  #initial position y

    T = []  #Initialising temperature of ensemble (note, without any potential energy this should not change during the simulation. You could use this as one check that the simulation is working properly)

    #Additionally, we employ a variable J_total to keep track of the
    #total impulse on the right wall
    J_total = 0

    for ring in range(1, N_time):  #Using a "for" loop to loop through time, propagating the trajectory of the particle. If you store the positions and times of your multiple particles in a matrix, this same for loop should work for you.
        x[:,ring] = x[:,ring-1] + vx*tres  #propagating the position: i.e., position 2 is equal to position 1 plus velocity times time step
        y[:,ring] = y[:,ring-1] + vy*tres 

        #Checking if particle has moved outside the box and reversing the relevant velocity component if it has. 
        #Here, x[ring,:]>L retrieves the indices of x values at the time "ring" for which the particle is outside of the box. 
        #Since there is only one particle here, it is somewhat redundant to find the indices. An alternative and equivalent code 
        #would be: if x[ring]>L; vx=-vx; end
        #and similar for the other three walls. We use the indices because it should be easier for you to extend this to 
        #multiple particles - applied to a vector of particle positions it will simultaneously identify each particle 
        #that is outside the bounds of the box
        ind = x[:,ring] > L
        vx[ind] = -vx[ind]

        J_total += 2*m*np.sum(abs(vx[ind])) #Add the momentum change of each particle in
                                            #contact with the wall to the total wall impulse

        ind = x[:,ring] < 0
        vx[ind] = -vx[ind]

        ind = y[:,ring] > L
        vy[ind] = -vy[ind]

        ind = y[:,ring] < 0
        vy[ind] = -vy[ind]
    return J_total


Pressure_sim = []
Volume_sim = []
Pressure_uncert = []
L = [0.0001, 0.0002, 0.0003] #Box side lengths to trial
num_trials = 4
for j in L:    
    tmax = (j/average_v) * 20 
    results = []

    i = 0
    print(f"{j}:{i}") #Prints in order to see program is running
    while i < num_trials:
        #Calculate the pressure for one trial (P = <J_x>/(L*tmax))
        results.append((simulation(j) / (j * tmax)))
        i += 1
    #Append mean pressure and volume to lists
    P_mean = np.mean(results)
    Pressure_sim.append(P_mean)
    Volume_sim.append(j*j)

    #Define uncertainty
    P_uncert = 0
    #Itterate through results of simulations to find 
    #standard deviation
    for i in range(0, num_trials):
        P_uncert += (results[i] - P_mean)**2
    #Append standard deviation to list
    Pressure_uncert.append(np.sqrt(P_uncert / num_trials))

plt.figure()
V = np.linspace(min(Volume_sim), max(Volume_sim), 500)
NA = 6.022*(10**23)  # Avogadro's number
n_moles = N / NA
P_ideal = (n_moles * 8.3141 * T_i) / V
plt.plot(V, P_ideal, label='Ideal Gas Law', color='orange')
plt.errorbar(Volume_sim, Pressure_sim, yerr=Pressure_uncert, fmt='o', capsize=5, capthick=1, elinewidth=1.2, label='Simulated data')
#Title axis and apply grid
plt.xlabel('Volume (m^2)')
plt.ylabel('Pressure (Pa)')
plt.title('PV diagram for particles in a box')
plt.grid(True)
plt.tight_layout()
#Show the graph
plt.show()

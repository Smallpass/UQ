% Define parameters
omega = 1; % Arbitrary unit
hbar = 1;
m = 1;
l_ho = sqrt(hbar / (m * omega));

% Define x and dimensionless variables
x = linspace(-5*l_ho, 5*l_ho, 1000);
xi = x / l_ho;

% Define wave functions
psi0 = (1/sqrt(pi)) * exp(-0.5 * xi.^2);
psi1 = sqrt(2) * xi .* (1/sqrt(pi)) .* exp(-0.5 * xi.^2); % Element-wise multiplication

% Time points
taus = [0, pi/4, pi/2, 3*pi/4, pi];

% Plot probability densities
figure;
hold on;
for tau = taus
    % Calculate time-dependent wave function
    psi0_t = psi0 .* exp(-1i * 0.5 * tau);
    psi1_t = psi1 .* exp(-1i * 1.5 * tau);
    
    % Total wave function
    Psi_t = sqrt(2) * psi0_t + psi1_t;
    
    % Probability density
    prob_density = abs(Psi_t).^2;
    
    % Plot
    plot(xi, prob_density, 'DisplayName', sprintf('\\tau = %.2f', tau));
end
hold off;
xlabel('ξ');
ylabel('Probability Density');
title('Probability Density at Different Tau');
legend;
grid on;


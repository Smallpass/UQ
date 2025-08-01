% Define the grid
x = linspace(-2, 2, 20); % Fewer points for quiver clarity
y = linspace(-2, 2, 20);
[X, Y] = meshgrid(x, y);

% Define the function
F = (X.^2 + Y.^2) .* exp(1 - X.^2 - Y.^2);

% Compute the gradient
[Fx, Fy] = gradient(F, x, y);

% Plot the contour
figure;
contour(X, Y, F, 20, 'LineWidth', 1.5);
colorbar;
hold on;

% Plot the gradient field (quiver plot)
quiver(X, Y, Fx, Fy, 'k', 'LineWidth', 1, 'AutoScaleFactor', 1.2);

% Labels and title
xlabel('x');
ylabel('y');
title('Contour Plot with Gradient Field');
grid on;
hold off;

% Script to display Linear regression plot
% Written by Samuel Allpass
% Version 1
% 4th August 2024

% Open the file
fid = fopen('data.dat', 'rt');
if fid == -1
    error('Cannot open file: data.dat');
end

% Read the first three lines with numeric data
dataLines = textscan(fid, '%f%f%f', 15, 'Delimiter', ',');
fclose(fid);

% Extract x, y, and uncertainty
x = dataLines{1};    % First column
y = dataLines{2};    % Second column
uncerty = dataLines{3};  % Third column

% Read the remaining lines for titles and coefficients
fid = fopen('data.dat', 'rt');
allLines = textscan(fid, '%s', 'Delimiter', '\n');
fclose(fid);

xtitle = allLines{1}{end-3};  % Title for x-axis
ytitle = allLines{1}{end-2};  % Title for y-axis
m = str2double(allLines{1}{end-1});  % Slope
c = str2double(allLines{1}{end});      % Intercept

% Display values
disp('x values:');
disp(x);
disp('y values:');
disp(y);
disp('uncerty values:');
disp(uncerty);

% Calculate the fit line
y_fit = m * x + c;

% Plot the results
figure;
errorbar(x, y, uncerty, 'o'); % Keep the circle markers for error bars
hold on;
plot(x, y_fit, '-r'); % Plot the line of best fit in red
xlabel(xtitle);
ylabel(ytitle);
legend('Data Points', 'Line of Best Fit');
grid on;
hold off;

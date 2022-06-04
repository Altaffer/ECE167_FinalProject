clc; close all; clear all;

%input modifiers
filename = 'compensated_data_1.csv'; % insert name of file to plot here

%% Sort data from measurements

% collects the data
data=csvread(filename, 2);
dtheta = data(:,1);     % in degrees
step_time = data(:,2);    % in seconds

fig1 = figure(1);
plot(step_time, "x");
title("compensating leg time-of-step");
xlabel("step");
ylabel("time (seconds)");

fig2 = figure(2);
plot(dtheta, "o");
title("compensating leg knee rotation");
xlabel("time (seconds)");
ylabel("degrees");

disp(mean(step_time))
disp(std(step_time))
disp(mean(dtheta))
disp(std(dtheta))

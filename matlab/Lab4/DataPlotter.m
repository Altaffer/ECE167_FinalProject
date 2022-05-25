clc; close all; clear all;

%input modifiers
filename = 'RollData_just_acc.csv'; % insert name of file to plot here

%% Sort data from measurements

% collects the data
data=csvread(filename, 2);
yaw = data(:,1);
pitch = data(:,2);    % in degrees
roll = data(:,3);
time = data(:,4); % in seconds

fig1 = figure(1);
plot(time, yaw);
title("yaw vs time just accelerometer");
xlabel("time (seconds)");
ylabel("degrees");

fig2 = figure(2);
plot(time, pitch);
title("pitch vs time just accelerometer");
xlabel("time (seconds)");
ylabel("degrees");

fig3 = figure(3);
plot(time, roll);
title("roll vs time just accelerometer");
xlabel("time (seconds)");
ylabel("degrees");

a = [5215.1 41349.7 47441.9];
b = a./norm(a);
disp(b)

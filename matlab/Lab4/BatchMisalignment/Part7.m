
clc; close all; clear all;

%% retrieve info from file
filename = 'tumble data.xlsb.csv'; % insert name of file to plot here

%% Sort data from measurements

% collects the data
data=csvread(filename, 2);
accelXmeas = data(:,1);
accelYmeas = data(:,2);
accelZmeas = data(:,3);
magXmeas = data(:,4);
magYmeas = data(:,5);
magZmeas = data(:,6);

%Accelerometer
Pb = readmatrix('tumble data.xlsb.csv','Range','A1:C415');

%Magnetometer
Sb = readmatrix('tumble data.xlsb.csv','Range','D1:F415');

pi = [];
pi(:,1) = 0;
pi(:,2) = 0;
pi(:,3) = 1;

si = [];
si(:,1) = 0.0826;
si(:,2) = 0.6548;
si(:,3) = 0.7513;


%Inertial unit vector for accelerometer = [0,0,1]
%Inertial unit vector for magnetometer = [0.0826, 0.6548, 0.7513]

AlignPrimarySecondary(Pb,Sb,pi,si,eye(3));

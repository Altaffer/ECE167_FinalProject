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
%Pb = readmatrix('tumble data.xlsb.csv','Range','A1:C415');
accels = [accelXmeas accelYmeas accelZmeas];
Pb = accels';


%Magnetometer
mags = [magXmeas magYmeas magZmeas];
Sb = mags';

pi = [0; 0; 1];
si = [0.0826; 0.6548; 0.7513];


%Inertial unit vector for accelerometer = [0,0,1]
%Inertial unit vector for magnetometer = [0.0826, 0.6548, 0.7513]

AlignPrimarySecondary(Pb,Sb,pi,si,eye(3));

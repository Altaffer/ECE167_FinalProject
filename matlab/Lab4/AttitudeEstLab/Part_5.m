clc; clear all; close all;

%% part 5

% set frequency and noise flag
freq = 50; % Hz
noiseFlag = 1;

% collect simulated data
[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(1/freq,noiseFlag);

% find the gyro biases
p_bias = 0; q_bias = 0; r_bias = 0;
for i = 1:1:50 % first second
    p_bias = p_bias + wGyro(i,1);
    q_bias = q_bias + wGyro(i,2);
    r_bias = r_bias + wGyro(i,3);
end
% p_bias = p_bias/50; q_bias = q_bias/50; r_bias = r_bias/50; % without
                                                              % integral gain
p_bias = 0; q_bias = 0; r_bias = 0;   % with integral gain
                                                              
% the bias vectors
accelInertial=[0;0;1];
He = [22825;5393;41690.2];     % Earth's magnetic field in mT (NED)
magInertial=He/norm(He);
disp(magInertial)

% integrate DCM and collect data to plot
DCMplus = zeros(3);
Bplus = [0;0;0];
Bminus = [0;0;0];
IntegratedEul = zeros(size(Eul));
DCMstart = [1 0 0; 0 1 0; 0 0 1];
DCMminus = DCMstart;

for i = 1:1:3330
    IntegratedEul(i,:) = DCM2Euler(DCMminus);
    gyros = (1/((1/250)*(2^15-1)) * 0.0174533) .* [(wGyro(i,1) - p_bias); (wGyro(i,2) - q_bias); (wGyro(i,3) - r_bias)]; 
    mags = [Mag(i,1); Mag(i,2); Mag(i,3)];
    accels = [Acc(i,1); Acc(i,2); Acc(i,3)];
    [DCMplus, Bplus] = IntegrateClosedLoop(DCMminus, Bminus, gyros, mags, accels, magInertial, accelInertial, 1/freq);
    DCMminus = DCMplus;
    Bminus=Bplus;
end

% plot data
yawError = figure(1);
plot(Eul(:,1) - IntegratedEul(:,1),'x');
title('yaw error between closed loop integration and real yaw');
xlabel("iteration");
ylabel("degrees error");
disp(mean(Eul(:,1) - IntegratedEul(:,1)))
disp(std(Eul(:,1) - IntegratedEul(:,1)))

pitchError = figure(2);
plot(Eul(:,2) - IntegratedEul(:,2),'x');
title('pitch error between closed loop integration and real pitch');
xlabel("iteration");
ylabel("degrees error");
disp(mean(Eul(:,2) - IntegratedEul(:,2)))
disp(std(Eul(:,2) - IntegratedEul(:,2)))

rollError = figure(3);
plot(Eul(:,3) - IntegratedEul(:,3),'x');
title('roll error between closed loop integration and real roll');
xlabel("iteration");
ylabel("degrees error");
disp(mean(Eul(:,3) - IntegratedEul(:,3)))
disp(std(Eul(:,3) - IntegratedEul(:,3)))




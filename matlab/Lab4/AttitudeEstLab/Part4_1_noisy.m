clc; clear all; close all;

%% part 4.1

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
p_bias = p_bias/50; q_bias = q_bias/50; r_bias = r_bias/50;

% extract DCM from initial euler angles and create DCM
yaw = Eul(1,1); pitch = Eul(1,2); roll = Eul(1,3);
DCMstart = Euler2DCM(yaw, pitch, roll);

% integrate DCM and collect data to plot
DCMplus = zeros(3);
IntegratedEul = zeros(size(Eul));
DCMstart = [1 0 0; 0 1 0; 0 0 1]; % leave uncommented to test identity DCM
DCMminus = DCMstart;

for i = 1:1:3330
    IntegratedEul(i,:) = DCM2Euler(DCMminus);
    DCMplus = IntegrateOpenLoop(DCMminus, (wGyro(i,:) - [p_bias q_bias r_bias]) .* 1/((1/250)*(2^15-1)) .* 0.0174533, 1/freq, 1);
    DCMminus = DCMplus;
end

% plot data
yawError = figure(1);
plot(Eul(:,1) - IntegratedEul(:,1),'x');
title('yaw error between matrix exponential integration and real yaw');
xlabel("iteration");
ylabel("degrees error");
disp(mean(Eul(:,1) - IntegratedEul(:,1)))
disp(std(Eul(:,1) - IntegratedEul(:,1)))

pitchError = figure(2);
plot(Eul(:,2) - IntegratedEul(:,2),'x');
title('pitch error between matrix exponential integration and real pitch');
xlabel("iteration");
ylabel("degrees error");
disp(mean(Eul(:,2) - IntegratedEul(:,2)))
disp(std(Eul(:,2) - IntegratedEul(:,2)))

rollError = figure(3);
plot(Eul(:,3) - IntegratedEul(:,3),'x');
title('roll error between matrix exponential integration and real roll');
xlabel("iteration");
ylabel("degrees error");
disp(mean(Eul(:,3) - IntegratedEul(:,3)))
disp(std(Eul(:,3) - IntegratedEul(:,3)))




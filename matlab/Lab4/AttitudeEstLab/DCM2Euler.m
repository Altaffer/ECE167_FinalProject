function Euler = DCM2Euler(DCM)
% function takes a 3x3 dcm matrix and returns euler values
% angles in degrees

if DCM(1,3) - 1 > 0
    DCM(1,3) = 1;
elseif DCM(1,3) + 1 < 0
    DCM(1,3) = -1;
end

yaw = atan2(DCM(1,2), DCM(1,1)) * 57.2958;
pitch = -asin(DCM(1,3)) * 57.2958;
roll = atan2(DCM(2,3), DCM(3,3)) * 57.2958;

Euler = [yaw pitch roll];
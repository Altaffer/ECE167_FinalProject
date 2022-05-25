#ifndef Part3_Rexp
#define	Part3_Rexp

/**
 * Conducts the math for the Rexp function for the forward integration
 * @param w[3], the omega matrix a 3x1.
 * @param dT, the time step of the function in s
 * @param R_exp[3][3], The output of the function, the Rexp matrix
 * @return None
 * @author Luca Altaffer*/
void Rexp(double w[3],int dT,double R_exp[3][3]);

/**
 * Conducts the math for the forward integration
 * @param w[3], the omega matrix a 3x1.
 * @param dT, the time step of the function in s
 * @param R_exp[3][3], the Rexp matrix
 * @param newR[3][3], the updated matrix
 * @return None
 * @author Luca Altaffer*/
void Integrate_R(double w[3],int dT, double R_exp[3][3], double prevMatrix[3][3], double newR[3][3]);

#endif
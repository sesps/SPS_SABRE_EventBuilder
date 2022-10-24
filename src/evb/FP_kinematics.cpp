/*

  Functions for the calculation of the kinematic shift of the FP
  for the SESPS @ FSU.


>>>  Delta_Z(int...) returns the shift of the FP in the z-direction in
     cm. A negative (<0) delta-z is defined as a shift towards the
     magnet.

     Arguments: Delta_Z(int ZT, int AT, int ZP, int AP, int ZE, int AE,
	                double EP, double angle, double B),
        where Z,A are atomic number and mass number for each particle,
        EP is the KE of the projectile (i.e. beam energy in MeV),
        angle is the spectrograph angle (in degrees),
        B is the field in Gauss.

>>>  Wire_Dist() returns the distance (in cm) between the wires for
     the calculation of relative weights between FP1 and FP2.

  //format: T(P,E)R
  //   i.e., T = target,
  //         P = projectile,
  //         E = ejectile,
  //         R = residual;
  //expects angle in degs, B in G, masses and KE in MeV

  KGH -- Jul19

  Small modifications for use with the MassLookup class GWM -- Jan 2021

*/

#include <cmath>
#include "MassLookup.h"
#include "FP_kinematics.h"

namespace EventBuilder {

	//requires (Z,A) for T, P, and E, as well as energy of P,
	// spectrograph angle of interest, and field value
	double Delta_Z(int ZT, int AT, int ZP, int AP, int ZE, int AE,
		       double EP, double angle, double B) 
	{
	
		/* CONSTANTS */
		const double UTOMEV = 931.4940954; //MeV per u;
		const double MEVTOJ = 1.60218E-13; //J per MeV
		const double RESTMASS_ELECTRON = 0.000548579909; //amu
		const double UNIT_CHARGE = 1.602E-19; //Coulombs
		const double C = 2.9979E8; //m/s
	
		/* SESPS-SPECIFIC */
		const double DISP = 1.96; //dispersion (x/rho)
		const double MAG = 0.39; //magnification in x
		const double DEGTORAD = M_PI/180.;
	
		int ZR = ZT + ZP - ZE, AR = AT + AP - AE;
		double EE=0; //ejectile energy
	
		double MT=0, MP=0, ME=0, MR=0; //masses (MeV)
	
		B /= 10000; //convert to tesla
		angle *= DEGTORAD;
	
		MT = MASS.FindMass(ZT, AT);
		MP = MASS.FindMass(ZP, AP);
		ME = MASS.FindMass(ZE, AE);
		MR = MASS.FindMass(ZR, AR);
		
		if (MT*MP*ME*MR == 0) 
		{
			EVB_WARN("Illegal mass at FP_kinematics::Delta_Z! Returning offset of 0.");
			return 0;
		}
	
		double Q = MT + MP - ME - MR; //Q-value
		
		//kinematics a la Iliadis p.590
		double term1 = sqrt(MP*ME*EP)/(ME + MR)*cos(angle);
		double term2 = (EP*(MR - MP) + MR*Q)/(ME + MR);
	
		EE = term1 + sqrt(term1*term1 + term2);
		EE *= EE;
	
		//momentum
		double PE = sqrt(EE*(EE+2*ME));
	
		//calculate rho from B a la B*rho = (proj. momentum)/(proj. charge)
		double rho = (PE*MEVTOJ)/(ZE*UNIT_CHARGE*C*B)*100; //in cm
	
		double K;
	
		K  = sqrt(MP*ME*EP/EE);
		K *= sin(angle);
	
		double denom = ME + MR - sqrt(MP*ME*EP/EE)*cos(angle);
	
		K /= denom;
		return -1*rho*DISP*MAG*K; //delta-Z in cm
	
	}
	
	double Wire_Dist() {return 4.28625;} //cm

}

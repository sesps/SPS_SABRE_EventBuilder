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
	double DeltaZ(int ZT, int AT, int ZP, int AP, int ZE, int AE,
		       double EP, double angle, double B, double nudge) 
	{
	
		/* CONSTANTS */
		static constexpr double s_speedOfLight = 2.9979E8; //m/s
		static constexpr double s_qbrho2p = s_speedOfLight * 1.0e-9; //Converts QBrho -> p (kG cm -> MeV)
	
		/* SESPS-SPECIFIC */
		static constexpr double s_dispersion = 1.96; //dispersion (x/rho)
		static constexpr double s_magnification = 0.39; //magnification in x
		static constexpr double s_deg2rad = M_PI/180.;
		static constexpr double s_centralTrajAngle = 45.0 * s_deg2rad; //Central trajectory angle to detector plane
	
		int ZR = ZT + ZP - ZE;
		int AR = AT + AP - AE;
		double ejectileEnergy = 0.0; //ejectile energy
	
		angle *= s_deg2rad;
	
		//reactant masses
		auto masses = MassLookup::GetInstance();
		double mt = masses.FindMass(ZT, AT);
		double mp = masses.FindMass(ZP, AP);
		double me = masses.FindMass(ZE, AE);
		double mr = masses.FindMass(ZR, AR);
		
		if (
			MassLookup::IsInvalidMass(mt) ||
			MassLookup::IsInvalidMass(mp) ||
			MassLookup::IsInvalidMass(me) ||
			MassLookup::IsInvalidMass(mr)
		) 
		{
			EVB_WARN("Illegal mass at FP_kinematics::Delta_Z! Returning offset of 0.");
			return 0;
		}
	
		double Q = mt + mp - me - mr; //Q-value
		
		//kinematics a la Iliadis p.590
		double term1 = std::sqrt(mp*me*EP)/(me + mr)*std::cos(angle);
		double term2 = (EP*(mr - mp) + mr*Q)/(me + mr);
	
		ejectileEnergy = term1 + std::sqrt(term1*term1 + term2);
		ejectileEnergy *= ejectileEnergy;
	
		//momentum
		double ejectileP = std::sqrt(ejectileEnergy * (ejectileEnergy + 2.0*me));
		//calculate rho from B a la B*rho = (proj. momentum)/(proj. charge)
		double rho = ejectileP / (ZE * B * s_qbrho2p ); //in cm
		//kinematic factor K
		double K = (std::sqrt(mp*me*EP / ejectileEnergy) * std::sin(angle)) /
				   (me + mr - std::sqrt(mp*me*EP / ejectileEnergy) * std::cos(angle));

		return -1.0*rho*s_dispersion*s_magnification*K * std::cos(s_centralTrajAngle) + nudge; //delta-Z in cm
	
	}
	
}

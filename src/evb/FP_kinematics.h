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

*/

#ifndef FP_KINEMATICS
#define FP_KINEMATICS

//requires (Z,A) for T, P, and E, as well as energy of P,
// spectrograph angle of interest, and field value
double Delta_Z(int ZT, int AT, int ZP, int AP, int ZE, int AE,
	       double EP, double angle, double B); 

double Wire_Dist();

#endif

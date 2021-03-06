#include "pch.h"
#include <iostream>
#include "sfbmd.h"

int main()
{
	BendingMoments beam;
	beam.initbeam();
	beam.PointLoadSetup();
	beam.UDLSetup();
	beam.TDLSetup();
	beam.SupportSetup();

	if (beam.getTDLnumber() != 0) {
		beam.TDLSupports();
		beam.computeTDLShear();
		beam.computeTDLMoments();
	}


	if (beam.getPointLoadNumber() != 0) {
		beam.PointLoadSupports();
		beam.PointLoadShearSetup();
		beam.ComputePointShear();
		beam.ComputePointLoadMoments();
	}

	if (beam.getUDLNumber() != 0) {
		beam.UDLSupports();
		beam.computeUDLShear();
		beam.computeUDLMoments();
	}

	if (beam.getUDLNumber() != 0 || beam.getPointLoadNumber() != 0 || beam.getTDLnumber() != 0) {
		beam.ParseShearData();
		beam.ParseMomentsData();
	}
	system("pause");
}


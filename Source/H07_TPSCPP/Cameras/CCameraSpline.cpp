#include "CCameraSpline.h"
#include "Global.h"
#include "Components/SplineComponent.h"

ACCameraSpline::ACCameraSpline()
{
	CHelpers::CreateSceneComponent(this, &Spline, "Spline");

	Spline->ScaleVisualizationWidth = 30.f;
	Spline->bShouldVisualizeScale = true;
}


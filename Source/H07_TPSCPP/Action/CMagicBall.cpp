#include "CMagicBall.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACMagicBall::ACMagicBall()
{
	CHelpers::CreateSceneComponent(this, &Sphere, "Sphere");
	CHelpers::CreateSceneComponent(this, &BallParticle, "BallParticle", Sphere);
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");

	InitialLifeSpan = 3.f;

	Projectile->InitialSpeed = 2000.f;
	Projectile->MaxSpeed = 4000.f;
	Projectile->ProjectileGravityScale = 0.f;
}

void ACMagicBall::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACMagicBall::BeginOverlap);
}

void ACMagicBall::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());

	//Play Impact Particle
	if (!!ImpactParticle)
	{
		FTransform transform = ImpactTransform;
		transform.AddToTranslation(GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, transform);
	}

	//Take Damage Delegate
	if (OnMagicBallBeginOverlap.IsBound())
		OnMagicBallBeginOverlap.Broadcast(SweepResult);

	Destroy();
}


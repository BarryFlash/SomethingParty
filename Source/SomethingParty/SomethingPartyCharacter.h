// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <TileActor.h>
#include <Runtime/Engine/Classes/Components/TimelineComponent.h>
#include <Components/WidgetComponent.h>
#include "SomethingPartyCharacter.generated.h"


UCLASS(Blueprintable)
class ASomethingPartyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASomethingPartyCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool isMoving();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float getTileWalkSpeed();

	void CreateMoveSpline(ATileActor* SplineStartTile, int amount);
	void Move();

	ATileActor* CurrentTile;

	

protected:
	class USplineComponent* MovementSpline;
	
	UPROPERTY(Replicated)
	bool moving;
	FTimeline MovementTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UHealthActorComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float TileWalkSpeed;

	virtual void BeginPlay() override;
	UFUNCTION()
	void MoveAlongSpline(float Value);
	UFUNCTION()
	void OnEndReached();
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	
	
};


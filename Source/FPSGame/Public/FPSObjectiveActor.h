// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class USphereComponent;

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp; //静态网格组件

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;  //球体组件，可用于接纳物体碰撞信息，物体碰撞时用到该组件

	//公开给蓝图，仅编辑默认
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PickupFX; //粒子效果对象

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffect();  //生成粒子效果并播放

public:	
	

	//当组件重叠时通知Actor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	
};

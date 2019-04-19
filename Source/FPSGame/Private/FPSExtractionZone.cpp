// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	//创建组件
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //碰撞发出询问
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);//设置碰撞为响应所有通道，ECR_Ignore忽略
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);//设置碰撞为响应指定通道，ECC_Pawn表示仅选择人形体进行设置
	OverlapComp->SetBoxExtent(FVector(200.0f));//设置合适的响应范围，设置盒体范围是一个向量,xyz都是200
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false); //游戏里显示物体边框线条

	//绑定Actor重叠触发事件
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	//创建贴图组件
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);
	DecalComp->SetupAttachment(RootComponent);
}



void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	//转换对象
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr)
	{
		return;
	}

	if (MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn, true);
		}
	}
	else
	{ 
		//如果未携带，则播放声音
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}


	UE_LOG(LogTemp, Log, TEXT("Overlapped with  extraction zone !"));

		
}




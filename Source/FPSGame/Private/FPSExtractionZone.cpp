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
	//�������
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //��ײ����ѯ��
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);//������ײΪ��Ӧ����ͨ����ECR_Ignore����
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);//������ײΪ��Ӧָ��ͨ����ECC_Pawn��ʾ��ѡ���������������
	OverlapComp->SetBoxExtent(FVector(200.0f));//���ú��ʵ���Ӧ��Χ�����ú��巶Χ��һ������,xyz����200
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false); //��Ϸ����ʾ����߿�����

	//��Actor�ص������¼�
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	//������ͼ���
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);
	DecalComp->SetupAttachment(RootComponent);
}



void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	//ת������
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
		//���δЯ�����򲥷�����
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}


	UE_LOG(LogTemp, Log, TEXT("Overlapped with  extraction zone !"));

		
}




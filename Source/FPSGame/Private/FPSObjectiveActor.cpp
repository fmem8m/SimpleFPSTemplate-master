// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "FPSCharacter.h"

/************************************************************************/
/* ��ɫ����ߵ�Ŀ������ǰ��ʰ������Ȼ��Ŀ���������� */
/************************************************************************/
// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{

	//ʵ���������CreateDefaultSubobject ����Ĭ���Ӷ��� TEXT�������ƣ�
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //���þ�̬���������ײ��
	RootComponent = MeshComp;   //����Ϊ�����

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp")); //�����������
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  //����ѯ����Ч��,�磺׷�ٹ켣�߻��ص��¼���
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore); //����ײ����Ϊ��Ӧ����ͨ������ѡ��ECR_lgnore����
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);//����һ��ͨ��������ײ����Pawn�������壩ͨ�������������������ͨ��������Ϊ��Ӧͨ��������һ���������������Ŀ������ʱ���㲻�����裬���ǻ�ֱ�Ӵ�������
	SphereComp->SetupAttachment(MeshComp);  //���ø����

	
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffect();
	
}

//�ú�������Ŀ�������ڷ����ص�ʱ��������Ч��
void AFPSObjectiveActor::PlayEffect()
{
	//��ԭ���������ӷ�����
	//this������֪���Ǹ����糡����ע����һ����Ч��
	//PickupFX ������ģ�壬ע����Ч��Դ
	//GetActorLocation ��ȡActor����λ��
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());

}


//��дActor�����еĺ���������������ص�ʱ֪ͨActor
void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//������Ч
	PlayEffect();

	//��OtherActorǿ��ת��ΪFPS��ɫ���� 
	//��������������Actor�������ɫ���ͷ����ص�
	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
	
	//��������Ƿ�Ϊ��
	if (MyCharacter)  
	{
		//���ǽ���Ŀ�����岼��ֵ��Ϊtrue
		MyCharacter->bIsCarryingObjective = true;

		//���Ե����Իٺ�������Ŀ��������Ի�
		//��Ϊ���ǲ�ϣ�������������������ʰ��Ŀ�����壬��������֮���������ɫ������Ŀ������ʱ������Ϊfalse 
		Destroy();
	}

}


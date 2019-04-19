// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "FPSCharacter.h"

/************************************************************************/
/* 角色玩家走到目标物体前，拾起物体然后目标物体销毁 */
/************************************************************************/
// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{

	//实例化组件（CreateDefaultSubobject 创建默认子对象 TEXT设置名称）
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //禁用静态网格组件碰撞体
	RootComponent = MeshComp;   //设置为根组件

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp")); //创建球体组件
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  //仅查询物理效果,如：追踪轨迹线或重叠事件等
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore); //将碰撞设置为响应所有通道，并选择ECR_lgnore忽略
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);//设置一个通道启用碰撞，即Pawn（人形体）通道，它是我们玩家人物通道，设置为响应通道，这样一来，当发射物击中目标物体时，便不会受阻，而是会直接穿过对象
	SphereComp->SetupAttachment(MeshComp);  //设置父组件

	
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffect();
	
}

//该函数是让目标物体在发生重叠时生成粒子效果
void AFPSObjectiveActor::PlayEffect()
{
	//在原处生成粒子发射器
	//this是引擎知道那个世界场景中注册这一粒子效果
	//PickupFX 发射器模板，注入特效资源
	//GetActorLocation 获取Actor自身位置
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());

}


//重写Actor基类中的函数，当组件发生重叠时通知Actor
void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//播放特效
	PlayEffect();

	//将OtherActor强制转换为FPS角色类型 
	//现在任意数量的Actor可以与角色类型发生重叠
	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
	
	//检查这项是否为空
	if (MyCharacter)  
	{
		//我们将此目标物体布尔值设为true
		MyCharacter->bIsCarryingObjective = true;

		//可以调用自毁函数，即目标物体的自毁
		//因为我们不希望让其他的玩家人物又拾起目标物体，当除我们之外的其他角色，经过目标物体时，条件为false 
		Destroy();
	}

}


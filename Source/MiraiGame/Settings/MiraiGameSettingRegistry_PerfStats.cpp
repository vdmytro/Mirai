// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomSettings/MiraiSettingValueDiscrete_PerfStat.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"
#include "GameSettingCollection.h"
#include "MiraiGameSettingRegistry.h"
#include "Performance/MiraiPerformanceStatTypes.h"

class UMiraiLocalPlayer;

#define LOCTEXT_NAMESPACE "Mirai"

//////////////////////////////////////////////////////////////////////

void UMiraiGameSettingRegistry::AddPerformanceStatPage(UGameSettingCollection* PerfStatsOuterCategory, UMiraiLocalPlayer* InLocalPlayer)
{
	//----------------------------------------------------------------------------------
	{
		static_assert((int32)EMiraiDisplayablePerformanceStat::Count == 15, "Consider updating this function to deal with new performance stats");

		UGameSettingCollectionPage* StatsPage = NewObject<UGameSettingCollectionPage>();
		StatsPage->SetDevName(TEXT("PerfStatsPage"));
		StatsPage->SetDisplayName(LOCTEXT("PerfStatsPage_Name", "Performance Stats"));
		StatsPage->SetDescriptionRichText(LOCTEXT("PerfStatsPage_Description", "Configure the display of performance statistics."));
		StatsPage->SetNavigationText(LOCTEXT("PerfStatsPage_Navigation", "Edit"));

		StatsPage->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

		PerfStatsOuterCategory->AddSetting(StatsPage);

		// Performance stats
		////////////////////////////////////////////////////////////////////////////////////
		{
			UGameSettingCollection* StatCategory_Performance = NewObject<UGameSettingCollection>();
			StatCategory_Performance->SetDevName(TEXT("StatCategory_Performance"));
			StatCategory_Performance->SetDisplayName(LOCTEXT("StatCategory_Performance_Name", "Performance"));
			StatsPage->AddSetting(StatCategory_Performance);

			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::ClientFPS);
				Setting->SetDisplayName(LOCTEXT("PerfStat_ClientFPS", "Client FPS"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_ClientFPS", "Client frame rate (higher is better)"));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::ServerFPS);
				Setting->SetDisplayName(LOCTEXT("PerfStat_ServerFPS", "Server FPS"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_ServerFPS", "Server frame rate"));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::FrameTime);
				Setting->SetDisplayName(LOCTEXT("PerfStat_FrameTime", "Frame Time"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_FrameTime", "The total frame time."));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::IdleTime);
				Setting->SetDisplayName(LOCTEXT("PerfStat_IdleTime", "Idle Time"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_IdleTime", "The amount of time spent waiting idle for frame pacing."));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::FrameTime_GameThread);
				Setting->SetDisplayName(LOCTEXT("PerfStat_FrameTime_GameThread", "CPU Game Time"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_FrameTime_GameThread", "The amount of time spent on the main game thread."));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::FrameTime_RenderThread);
				Setting->SetDisplayName(LOCTEXT("PerfStat_FrameTime_RenderThread", "CPU Render Time"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_FrameTime_RenderThread", "The amount of time spent on the rendering thread."));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::FrameTime_RHIThread);
				Setting->SetDisplayName(LOCTEXT("PerfStat_FrameTime_RHIThread", "CPU RHI Time"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_FrameTime_RHIThread", "The amount of time spent on the Render Hardware Interface thread."));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::FrameTime_GPU);
				Setting->SetDisplayName(LOCTEXT("PerfStat_FrameTime_GPU", "GPU Render Time"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_FrameTime_GPU", "The amount of time spent on the GPU."));
				StatCategory_Performance->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
		}

		// Network stats
		////////////////////////////////////////////////////////////////////////////////////
		{
			UGameSettingCollection* StatCategory_Network = NewObject<UGameSettingCollection>();
			StatCategory_Network->SetDevName(TEXT("StatCategory_Network"));
			StatCategory_Network->SetDisplayName(LOCTEXT("StatCategory_Network_Name", "Network"));
			StatsPage->AddSetting(StatCategory_Network);

			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::Ping);
				Setting->SetDisplayName(LOCTEXT("PerfStat_Ping", "Ping"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_Ping", "The roundtrip latency of your connection to the server."));
				StatCategory_Network->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::PacketLoss_Incoming);
				Setting->SetDisplayName(LOCTEXT("PerfStat_PacketLoss_Incoming", "Incoming Packet Loss"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_PacketLoss_Incoming", "The percentage of incoming packets lost."));
				StatCategory_Network->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::PacketLoss_Outgoing);
				Setting->SetDisplayName(LOCTEXT("PerfStat_PacketLoss_Outgoing", "Outgoing Packet Loss"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_PacketLoss_Outgoing", "The percentage of outgoing packets lost."));
				StatCategory_Network->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::PacketRate_Incoming);
				Setting->SetDisplayName(LOCTEXT("PerfStat_PacketRate_Incoming", "Incoming Packet Rate"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_PacketRate_Incoming", "Rate of incoming packets (per second)"));
				StatCategory_Network->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::PacketRate_Outgoing);
				Setting->SetDisplayName(LOCTEXT("PerfStat_PacketRate_Outgoing", "Outgoing Packet Rate"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_PacketRate_Outgoing", "Rate of outgoing packets (per second)"));
				StatCategory_Network->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::PacketSize_Incoming);
				Setting->SetDisplayName(LOCTEXT("PerfStat_PacketSize_Incoming", "Incoming Packet Size"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_PacketSize_Incoming", "The average size (in bytes) of packets recieved in the last second."));
				StatCategory_Network->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
			{
				UMiraiSettingValueDiscrete_PerfStat* Setting = NewObject<UMiraiSettingValueDiscrete_PerfStat>();
				Setting->SetStat(EMiraiDisplayablePerformanceStat::PacketSize_Outgoing);
				Setting->SetDisplayName(LOCTEXT("PerfStat_PacketSize_Outgoing", "Outgoing Packet Size"));
				Setting->SetDescriptionRichText(LOCTEXT("PerfStatDescription_PacketSize_Outgoing", "The average size (in bytes) of packets sent in the last second."));
				StatCategory_Network->AddSetting(Setting);
			}
			//----------------------------------------------------------------------------------
		}
	}
}

#undef LOCTEXT_NAMESPACE

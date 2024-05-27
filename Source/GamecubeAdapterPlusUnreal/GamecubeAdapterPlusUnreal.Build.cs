// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System;

namespace UnrealBuildTool.Rules
{
	public class GamecubeAdapterPlusUnreal : ModuleRules
	{
		public GamecubeAdapterPlusUnreal(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

            if (Target.Platform == UnrealTargetPlatform.Win64) // || Target.Platform == UnrealTargetPlatform.Linux || Target.Platform == UnrealTargetPlatform.LinuxAArch64)
            {
                PublicAdditionalLibraries.Add("$(ModuleDir)/ThirdParty/libusb-cygwin-x64/bin/libusb-1.0.lib");

                PublicDelayLoadDLLs.Add("cygusb-1.0.dll");
                RuntimeDependencies.Add("$(ModuleDir)/ThirdParty/libusb-cygwin-x64/bin/cygusb-1.0.dll");
                PublicDelayLoadDLLs.Add("libusb-1.0.dll");
                RuntimeDependencies.Add("$(ModuleDir)/ThirdParty/libusb-cygwin-x64/bin/libusb-1.0.dll");
            }
            else
            {
                //Console.WriteLine("Error: Platform not supported");
            }


            PublicIncludePaths.AddRange(
                new[] 
				{
					"$(ModuleDir)/ThirdParty/gca-plus",
					"$(ModuleDir)/ThirdParty/libusb-cygwin-x64"
				}
            );

            PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
				    "CoreUObject",
				    "Engine",
					"Projects",
                    "InputCore",
					"SlateCore",
					"Slate"
				}
			);

            PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"InputDevice",
                    "DeveloperSettings",
				}
			);
		}
	}
}

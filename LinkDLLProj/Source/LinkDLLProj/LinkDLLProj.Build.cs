// Fill out your copyright notice in the Description page of Project Settings.
using System.IO;
using UnrealBuildTool;

public class LinkDLLProj : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }

    }

	public LinkDLLProj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "RenderCore", "ShaderCore" });

        LoadOpenCV(Target);
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}

    public bool LoadOpenCV(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        //Create OpenCV path
        string OpenCVPath = Path.Combine(ThirdPartyPath, "OpenCV");

        //Get library path
        string LibPath = "";
        bool isdebug = Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            LibPath = Path.Combine(OpenCVPath, "Libraries", "Win64");
            isLibrarySupported = true;
        }
        else
        {
            string Err = string.Format("{0} dedicated server is made to depend on {1}. We want to avoid this, please correct module dependencies.",
                                        Target.Platform.ToString(), this.ToString());
            System.Console.WriteLine(Err);
        }

        if (isLibrarySupported)
        {
            //add include path
            PublicIncludePaths.AddRange(new string[] { Path.Combine(OpenCVPath, "Includes") });

            //Add Library Path
            PublicLibraryPaths.Add(LibPath);

            //Add static libraries
            PublicAdditionalLibraries.Add("opencv_core400.lib");
            PublicAdditionalLibraries.Add("opencv_face400.lib");
            PublicAdditionalLibraries.Add("opencv_videoio400.lib");
            PublicAdditionalLibraries.Add("opencv_video400.lib");
            PublicAdditionalLibraries.Add("opencv_photo400.lib");
            PublicAdditionalLibraries.Add("opencv_objdetect400.lib");
            PublicAdditionalLibraries.Add("opencv_highgui400.lib");
            PublicAdditionalLibraries.Add("opencv_imgproc400.lib");
            PublicAdditionalLibraries.Add("opencv_xobjdetect400.lib");

            //Add dynamic Libraries
            PublicDelayLoadDLLs.Add("opencv_core400.dll");
            PublicDelayLoadDLLs.Add("opencv_face400.dll");
            PublicDelayLoadDLLs.Add("opencv_videoio400.dll");
            PublicDelayLoadDLLs.Add("opencv_video400.dll");
            PublicDelayLoadDLLs.Add("opencv_photo400.dll");
            PublicDelayLoadDLLs.Add("opencv_objdetect400.dll");
            PublicDelayLoadDLLs.Add("opencv_imgproc400.dll");
            PublicDelayLoadDLLs.Add("opencv_xobjdetect400.dll");

        }
        PublicDefinitions.Add(string.Format("WITH_OPENCV_BINDING={0}", isLibrarySupported ? 1 : 0));
        return isLibrarySupported;
    }

}

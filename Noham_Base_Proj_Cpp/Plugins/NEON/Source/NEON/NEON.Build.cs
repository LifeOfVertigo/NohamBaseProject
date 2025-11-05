/* 
* Copyright (C) 2024 Michael Saller - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

using UnrealBuildTool;
using System.IO;
using System;
using System.Diagnostics;
using EpicGames.Core;

public class NEON : ModuleRules
{

	public NEON(ReadOnlyTargetRules Target) : base(Target)
	{
		this.PublicDefinitions.Add("USING_CEF_SHARED=1");
		this.PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bUseUnity = false;

		// paths
		string pluginRoot = Path.GetFullPath(Path.Combine(this.ModuleDirectory, "../../"));
		// absolute path to binaries directory
		string binariesDir = Path.Combine(pluginRoot, "Binaries", "Win64");
		string cefRoot = Path.Combine(pluginRoot, "Source", "ThirdParty", "CEF");
		string cefLib = Path.Combine(cefRoot, "lib");

		AddDirectoryAsRuntimeDependencyRecursively(
				cefLib,
				binariesDir
		);

		string libCef = Path.Combine(cefLib, "libcef.lib");
		string libCefWrapper = Path.Combine(cefLib, "libcef_dll_wrapper.lib");

		this.PublicIncludePaths.AddRange(
				new string[] {
								Path.Combine(Target.RelativeEnginePath, "Source", "ThirdParty", "Windows", "D3DX12", "Include"),
								Path.Combine(pluginRoot, "Source", "NEON", "Public"),
								cefRoot
				}
		);

		// Add paths to system libraries
		PublicAdditionalLibraries.Add(libCef);
		PublicAdditionalLibraries.Add(libCefWrapper);

		PublicDelayLoadDLLs.Add("libcef.dll");
		PublicDependencyModuleNames.AddRange(new string[] {
						"DX11",
						"DX12"
				});

		this.PublicDependencyModuleNames.AddRange(
				new string[]
				{
								"Core",
								"CoreUObject",
								"Engine",
								"HTTP",
								"Renderer",
								"Json",
								"JsonUtilities",
								"UMG",
								"Slate",
								"SlateCore",
								"InputCore",
								"RenderCore",
								"RHI",
								"Projects",
								"D3D11RHI",
								"D3D12RHI"
				}
		);

		this.PrivateIncludePaths.Add(Path.Combine(pluginRoot, "Source", "NEON", "Private"));
	}

	void AddDirectoryAsRuntimeDependencyRecursively(string sourceDir, string targetDir, string relativePath = "")
	{
		// Ensure the target directory exists
		if (!Directory.Exists(Path.Combine(targetDir, relativePath)))
		{
			// System.Console.WriteLine($"Creating directory: {Path.Combine(targetDir, relativePath)}");
			Directory.CreateDirectory(Path.Combine(targetDir, relativePath));
		}

		// Copy all files in the current directory
		foreach (string filePath in Directory.GetFiles(sourceDir))
		{
			string fileName = Path.GetFileName(filePath);
			string targetFilePath = Path.Combine(targetDir, relativePath, fileName);

			// the neon cef sub exe is not being copied properly, so we copy it manually
			if (fileName.EndsWith(".exe"))
			{
				System.Console.WriteLine($"Copying {filePath} to {targetFilePath}");
				File.Copy(filePath, targetFilePath, true);
			}

			// Register the file as a runtime dependency
			System.Console.WriteLine($"Adding runtime dependency {filePath} -> {targetFilePath}");
			RuntimeDependencies.Add(targetFilePath, filePath);
		}

		// Recursively copy subdirectories
		foreach (string subDirPath in Directory.GetDirectories(sourceDir))
		{
			string subDirName = Path.GetFileName(subDirPath);
			string subDirRelativePath = Path.Combine(relativePath, subDirName);
			AddDirectoryAsRuntimeDependencyRecursively(subDirPath, targetDir, subDirRelativePath);
		}
	}
}
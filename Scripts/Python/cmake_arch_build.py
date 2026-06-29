"""
CMake Functional Decomposition Architecture Builder
====================================================
Run with Understand's Python API:
    upython cmake_arch_build.py -db /path/to/cmake.und

Or from within Understand via Tools > Run Script.

The script creates (or replaces) a top-level architecture named
"CMake Functional Decomposition" with the subsystems below.
"""

import understand
import sys
import os
import re
import subprocess
import tempfile
import html

# ---------------------------------------------------------------------------
# Architecture definition
# Each entry: (arch_name, parent_name_or_None, [filename_patterns])
# Patterns are matched against the file's short name (no path).
# Use None as the pattern list for container nodes with no direct files.
# ---------------------------------------------------------------------------

ARCH = [
    # ── Top level ──────────────────────────────────────────────────────────
    ("CMake Functional Decomposition", None, None),

    # 1. Bootstrap & Entry Points
    ("Bootstrap & Entry Points", "CMake Functional Decomposition", [
        "cmake_main.cxx", "cmakemain.cxx", "ccmake.cxx", "cmake-gui.cxx",
        "cpack.cxx", "ctest.cxx", "CMakeGUIExec.cxx",
        "cmcmd.cxx", "cmcmd.h",
        "cmCommandLineArgument.h",
        "cmCommands.cxx", "cmCommands.h",
        "cmSubcommandTable.cxx", "cmSubcommandTable.h",
    ]),

    # 2. Core Configuration
    ("Core Configuration", "CMake Functional Decomposition", None),
    ("CMake Object & State", "Core Configuration", [
        "cmake.cxx", "cmake.h",
        "cmState.cxx", "cmState.h",
        "cmStateDirectory.cxx", "cmStateDirectory.h",
        "cmStateSnapshot.cxx", "cmStateSnapshot.h",
        "cmStatePrivate.h", "cmStateTypes.h",
    ]),
    ("Cache", "Core Configuration", [
        "cmCacheManager.cxx", "cmCacheManager.h",
        "cmCacheDocumentationTable.cxx", "cmCacheDocumentationTable.h",
        "cmCachePatternTable.cxx", "cmCachePatternTable.h",
    ]),
    ("Makefile & Directory Model", "Core Configuration", [
        "cmMakefile.cxx", "cmMakefile.h",
    ]),
    ("Policies", "Core Configuration", [
        "cmPolicies.cxx", "cmPolicies.h",
    ]),
    ("Presets", "Core Configuration", [
        "cmCMakePresetsGraph.cxx", "cmCMakePresetsGraph.h",
        "cmCMakePresetsGraphInternal.h",
        "cmCMakePresetsGraphReadJSON.cxx",
        "cmCMakePresetsGraphReadJSONBuildPresets.cxx",
        "cmCMakePresetsGraphReadJSONConfigurePresets.cxx",
        "cmCMakePresetsGraphReadJSONPackagePresets.cxx",
        "cmCMakePresetsGraphReadJSONTestPresets.cxx",
        "cmCMakePresetsGraphReadJSONWorkflowPresets.cxx",
        "cmCMakePresetsGraphResolve.cxx",
        "cmCMakePresetsErrors.cxx", "cmCMakePresetsErrors.h",
        "cmCMakePresetsArgs.h",
    ]),

    # 3. Scripting Language Engine
    ("Scripting Language Engine", "CMake Functional Decomposition", None),
    ("Lexer/Parser", "Scripting Language Engine", [
        "cmListFileLexer.c", "cmListFileLexer.h", "cmListFileLexer.in.l",
        "cmListFileCache.cxx", "cmListFileCache.h",
        "cmExprLexer.cxx", "cmExprLexer.h", "cmExprLexer.in.l",
        "cmExprParser.cxx", "cmExprParser.y", "cmExprParserTokens.h",
        "cmFortranLexer.cxx", "cmFortranLexer.h", "cmFortranLexer.in.l",
        "cmFortranParser.cxx", "cmFortranParser.y", "cmFortranParserTokens.h",
        "cmDependsJavaLexer.cxx", "cmDependsJavaLexer.h", "cmDependsJavaLexer.in.l",
        "cmDependsJavaParser.cxx", "cmDependsJavaParser.y", "cmDependsJavaParserTokens.h",
        "cmGccDepfileLexer.cxx", "cmGccDepfileLexer.h", "cmGccDepfileLexer.in.l",
        "cmCTestResourceGroupsLexer.cxx", "cmCTestResourceGroupsLexer.h",
    ]),
    ("Control Flow", "Scripting Language Engine", [
        "cmIfCommand.cxx", "cmIfCommand.h",
        "cmConditionEvaluator.cxx", "cmConditionEvaluator.h",
        "cmWhileCommand.cxx", "cmWhileCommand.h",
        "cmForEachCommand.cxx", "cmForEachCommand.h",
        "cmBreakCommand.cxx", "cmBreakCommand.h",
        "cmContinueCommand.cxx", "cmContinueCommand.h",
        "cmReturnCommand.cxx", "cmReturnCommand.h",
        "cmBlockCommand.cxx", "cmBlockCommand.h",
    ]),
    ("Functions & Macros", "Scripting Language Engine", [
        "cmFunctionCommand.cxx", "cmFunctionCommand.h",
        "cmFunctionBlocker.cxx", "cmFunctionBlocker.h",
        "cmMacroCommand.cxx", "cmMacroCommand.h",
    ]),
    ("Generator Expressions", "Scripting Language Engine", [
        "cmGeneratorExpression.cxx", "cmGeneratorExpression.h",
        "cmGeneratorExpressionDAGChecker.cxx", "cmGeneratorExpressionDAGChecker.h",
        "cmGeneratorExpressionEvaluationFile.cxx", "cmGeneratorExpressionEvaluationFile.h",
        "cmGeneratorExpressionEvaluator.cxx", "cmGeneratorExpressionEvaluator.h",
        "cmGeneratorExpressionLexer.cxx", "cmGeneratorExpressionLexer.h",
        "cmGeneratorExpressionNode.cxx", "cmGeneratorExpressionNode.h",
        "cmGeneratorExpressionParser.cxx", "cmGeneratorExpressionParser.h",
    ]),

    # 4. Built-in Commands
    ("Built-in Commands", "CMake Functional Decomposition", None),
    ("Target Commands", "Built-in Commands", [
        "cmAddExecutableCommand.cxx", "cmAddExecutableCommand.h",
        "cmAddLibraryCommand.cxx", "cmAddLibraryCommand.h",
        "cmAddCustomCommandCommand.cxx", "cmAddCustomCommandCommand.h",
        "cmAddCustomTargetCommand.cxx", "cmAddCustomTargetCommand.h",
        "cmTargetCompileDefinitionsCommand.cxx", "cmTargetCompileDefinitionsCommand.h",
        "cmTargetCompileFeaturesCommand.cxx", "cmTargetCompileFeaturesCommand.h",
        "cmTargetCompileOptionsCommand.cxx", "cmTargetCompileOptionsCommand.h",
        "cmTargetIncludeDirectoriesCommand.cxx", "cmTargetIncludeDirectoriesCommand.h",
        "cmTargetLinkDirectoriesCommand.cxx", "cmTargetLinkDirectoriesCommand.h",
        "cmTargetLinkLibrariesCommand.cxx", "cmTargetLinkLibrariesCommand.h",
        "cmTargetLinkOptionsCommand.cxx", "cmTargetLinkOptionsCommand.h",
        "cmTargetPrecompileHeadersCommand.cxx", "cmTargetPrecompileHeadersCommand.h",
        "cmTargetSourcesCommand.cxx", "cmTargetSourcesCommand.h",
        "cmTargetPropCommandBase.cxx", "cmTargetPropCommandBase.h",
        "cmAddCompileDefinitionsCommand.cxx", "cmAddCompileDefinitionsCommand.h",
        "cmAddCompileOptionsCommand.cxx", "cmAddCompileOptionsCommand.h",
        "cmAddLinkOptionsCommand.cxx", "cmAddLinkOptionsCommand.h",
        "cmAddDependenciesCommand.cxx", "cmAddDependenciesCommand.h",
        "cmLinkLibrariesCommand.cxx", "cmLinkLibrariesCommand.h",
        "cmLinkDirectoriesCommand.cxx", "cmLinkDirectoriesCommand.h",
        "cmSetTargetPropertiesCommand.cxx", "cmSetTargetPropertiesCommand.h",
        "cmGetTargetPropertyCommand.cxx", "cmGetTargetPropertyCommand.h",
        "cmSourceGroupCommand.cxx", "cmSourceGroupCommand.h",
    ]),
    ("Project & Directory Commands", "Built-in Commands", [
        "cmProjectCommand.cxx", "cmProjectCommand.h",
        "cmAddSubDirectoryCommand.cxx", "cmAddSubDirectoryCommand.h",
        "cmSubdirCommand.cxx", "cmSubdirCommand.h",
        "cmIncludeCommand.cxx", "cmIncludeCommand.h",
        "cmIncludeDirectoryCommand.cxx", "cmIncludeDirectoryCommand.h",
        "cmIncludeGuardCommand.cxx", "cmIncludeGuardCommand.h",
        "cmIncludeRegularExpressionCommand.cxx", "cmIncludeRegularExpressionCommand.h",
        "cmEnableLanguageCommand.cxx", "cmEnableLanguageCommand.h",
        "cmEnableTestingCommand.cxx", "cmEnableTestingCommand.h",
        "cmOptionCommand.cxx", "cmOptionCommand.h",
        "cmSetCommand.cxx", "cmSetCommand.h",
        "cmUnsetCommand.cxx", "cmUnsetCommand.h",
        "cmAddDefinitionsCommand.cxx", "cmAddDefinitionsCommand.h",
        "cmRemoveDefinitionsCommand.cxx", "cmRemoveDefinitionsCommand.h",
        "cmSetDirectoryPropertiesCommand.cxx", "cmSetDirectoryPropertiesCommand.h",
        "cmGetDirectoryPropertyCommand.cxx", "cmGetDirectoryPropertyCommand.h",
        "cmMarkAsAdvancedCommand.cxx", "cmMarkAsAdvancedCommand.h",
        "cmCMakePolicyCommand.cxx", "cmCMakePolicyCommand.h",
    ]),
    ("File & String Commands", "Built-in Commands", [
        "cmFileCommand.cxx", "cmFileCommand.h",
        "cmStringCommand.cxx", "cmStringCommand.h",
        "cmListCommand.cxx", "cmListCommand.h",
        "cmMathCommand.cxx", "cmMathCommand.h",
        "cmMessageCommand.cxx", "cmMessageCommand.h",
        "cmConfigureFileCommand.cxx", "cmConfigureFileCommand.h",
        "cmWriteFileCommand.cxx", "cmWriteFileCommand.h",
        "cmGetFilenameComponentCommand.cxx", "cmGetFilenameComponentCommand.h",
        "cmSeparateArgumentsCommand.cxx", "cmSeparateArgumentsCommand.h",
        "cmMakeDirectoryCommand.cxx", "cmMakeDirectoryCommand.h",
        "cmRemoveCommand.cxx", "cmRemoveCommand.h",
        "cmCMakePathCommand.cxx", "cmCMakePathCommand.h",
        "cmCMakeHostSystemInformationCommand.cxx", "cmCMakeHostSystemInformationCommand.h",
        "cmSiteNameCommand.cxx", "cmSiteNameCommand.h",
    ]),
    ("Property Commands", "Built-in Commands", [
        "cmDefinePropertyCommand.cxx", "cmDefinePropertyCommand.h",
        "cmGetCMakePropertyCommand.cxx", "cmGetCMakePropertyCommand.h",
        "cmGetPropertyCommand.cxx", "cmGetPropertyCommand.h",
        "cmSetPropertyCommand.cxx", "cmSetPropertyCommand.h",
        "cmGetSourceFilePropertyCommand.cxx", "cmGetSourceFilePropertyCommand.h",
        "cmSetSourceFilesPropertiesCommand.cxx", "cmSetSourceFilesPropertiesCommand.h",
        "cmGetTestPropertyCommand.cxx", "cmGetTestPropertyCommand.h",
        "cmSetTestsPropertiesCommand.cxx", "cmSetTestsPropertiesCommand.h",
        "cmVariableWatchCommand.cxx", "cmVariableWatchCommand.h",
        "cmLoadCacheCommand.cxx", "cmLoadCacheCommand.h",
    ]),
    ("Process & Build Commands", "Built-in Commands", [
        "cmExecuteProcessCommand.cxx", "cmExecuteProcessCommand.h",
        "cmExecProgramCommand.cxx", "cmExecProgramCommand.h",
        "cmBuildCommand.cxx", "cmBuildCommand.h",
        "cmTryCompileCommand.cxx", "cmTryCompileCommand.h",
        "cmTryRunCommand.cxx", "cmTryRunCommand.h",
        "cmCoreTryCompile.cxx", "cmCoreTryCompile.h",
        "cmAddTestCommand.cxx", "cmAddTestCommand.h",
        "cmDiscoverTestsCommand.cxx", "cmDiscoverTestsCommand.h",
        "cmParseArgumentsCommand.cxx", "cmParseArgumentsCommand.h",
        "cmCMakeLanguageCommand.cxx", "cmCMakeLanguageCommand.h",
        "cmCMakeDiagnosticCommand.cxx", "cmCMakeDiagnosticCommand.h",
        "cmAuxSourceDirectoryCommand.cxx", "cmAuxSourceDirectoryCommand.h",
    ]),

    # 5. Target & Dependency Model
    ("Target & Dependency Model", "CMake Functional Decomposition", None),
    ("Target Model", "Target & Dependency Model", [
        "cmTarget.cxx", "cmTarget.h",
        "cmTargetDepend.h",
        "cmTargetExport.h",
        "cmTargetPropertyComputer.cxx", "cmTargetPropertyComputer.h",
        "cmTargetPropertyEntry.cxx", "cmTargetPropertyEntry.h",
        "cmCustomCommand.cxx", "cmCustomCommand.h",
        "cmCustomCommandGenerator.cxx", "cmCustomCommandGenerator.h",
        "cmCustomCommandLines.cxx", "cmCustomCommandLines.h",
        "cmCustomCommandTypes.h",
        "cmSourceFile.cxx", "cmSourceFile.h",
        "cmSourceFileLocation.cxx", "cmSourceFileLocation.h",
        "cmSourceFileLocationKind.h",
        "cmSourceGroup.cxx", "cmSourceGroup.h",
        "cmFileSet.cxx", "cmFileSet.h",
        "cmFileSetMetadata.cxx", "cmFileSetMetadata.h",
        "cmPropertyDefinition.cxx", "cmPropertyDefinition.h",
        "cmPropertyMap.cxx", "cmPropertyMap.h",
        "cmProperty.h",
        "cmCxxModuleMapper.cxx", "cmCxxModuleMapper.h",
        "cmCxxModuleUsageEffects.cxx", "cmCxxModuleUsageEffects.h",
        "cmDyndepCollation.cxx", "cmDyndepCollation.h",
        "cmEvaluatedTargetProperty.cxx", "cmEvaluatedTargetProperty.h",
        "cmScanDepFormat.cxx", "cmScanDepFormat.h",
        "cmSyntheticTargetCache.h",
        "cmTargetLinkLibraryType.h",
        "cmTransformDepfile.cxx", "cmTransformDepfile.h",
        "cmTest.cxx", "cmTest.h",
        "cmTestGenerator.cxx", "cmTestGenerator.h",
        "cmCreateTestSourceList.cxx", "cmCreateTestSourceList.h",
        "cmStandardLevel.h", "cmStandardLevelResolver.cxx", "cmStandardLevelResolver.h",
    ]),
    ("Generator Target", "Target & Dependency Model", [
        "cmGeneratorTarget.cxx", "cmGeneratorTarget.h",
        "cmGeneratorTarget_CompatibleInterface.cxx",
        "cmGeneratorTarget_HeaderSetVerification.cxx",
        "cmGeneratorTarget_IncludeDirectories.cxx",
        "cmGeneratorTarget_Link.cxx",
        "cmGeneratorTarget_LinkDirectories.cxx",
        "cmGeneratorTarget_Options.cxx",
        "cmGeneratorTarget_Sources.cxx",
        "cmGeneratorTarget_TransitiveProperty.cxx",
        "cmGeneratorTarget_TargetPropertyEntry.cxx",
        "cmTargetTraceDependencies.cxx", "cmTargetTraceDependencies.h",
        "cmGccDepfileReader.cxx", "cmGccDepfileReader.h", "cmGccDepfileReaderTypes.h",
    ]),
    ("Link & Dependency Resolution", "Target & Dependency Model", [
        "cmComputeLinkDepends.cxx", "cmComputeLinkDepends.h",
        "cmComputeLinkInformation.cxx", "cmComputeLinkInformation.h",
        "cmDepends.cxx", "cmDepends.h",
        "cmDependsC.cxx", "cmDependsC.h",
        "cmDependsCompiler.cxx", "cmDependsCompiler.h",
        "cmDependsFortran.cxx", "cmDependsFortran.h",
        "cmDependsJava.cxx", "cmDependsJava.h",
        "cmDependsJavaParserHelper.cxx", "cmDependsJavaParserHelper.h",
        "cmDependencyProvider.h",
        "cmLinkItem.cxx", "cmLinkItem.h",
        "cmLinkItemGraphVisitor.cxx", "cmLinkItemGraphVisitor.h",
        "cmLinkLineComputer.cxx", "cmLinkLineComputer.h",
        "cmLinkLineDeviceComputer.cxx", "cmLinkLineDeviceComputer.h",
        "cmComputeTargetDepends.cxx", "cmComputeTargetDepends.h",
        "cmComputeComponentGraph.cxx", "cmComputeComponentGraph.h",
        "cmOrderDirectories.cxx", "cmOrderDirectories.h",
    ]),
    ("Runtime Dependencies", "Target & Dependency Model", [
        "cmBinUtilsLinker.cxx", "cmBinUtilsLinker.h",
        "cmBinUtilsLinuxELFGetRuntimeDependenciesTool.cxx", "cmBinUtilsLinuxELFGetRuntimeDependenciesTool.h",
        "cmBinUtilsLinuxELFLinker.cxx", "cmBinUtilsLinuxELFLinker.h",
        "cmBinUtilsLinuxELFObjdumpGetRuntimeDependenciesTool.cxx", "cmBinUtilsLinuxELFObjdumpGetRuntimeDependenciesTool.h",
        "cmBinUtilsMacOSMachOGetRuntimeDependenciesTool.cxx", "cmBinUtilsMacOSMachOGetRuntimeDependenciesTool.h",
        "cmBinUtilsMacOSMachOLinker.cxx", "cmBinUtilsMacOSMachOLinker.h",
        "cmBinUtilsMacOSMachOOToolGetRuntimeDependenciesTool.cxx", "cmBinUtilsMacOSMachOOToolGetRuntimeDependenciesTool.h",
        "cmBinUtilsWindowsPEDumpbinGetRuntimeDependenciesTool.cxx", "cmBinUtilsWindowsPEDumpbinGetRuntimeDependenciesTool.h",
        "cmBinUtilsWindowsPEGetRuntimeDependenciesTool.cxx", "cmBinUtilsWindowsPEGetRuntimeDependenciesTool.h",
        "cmBinUtilsWindowsPELinker.cxx", "cmBinUtilsWindowsPELinker.h",
        "cmBinUtilsWindowsPEObjdumpGetRuntimeDependenciesTool.cxx", "cmBinUtilsWindowsPEObjdumpGetRuntimeDependenciesTool.h",
        "cmRuntimeDependencyArchive.cxx", "cmRuntimeDependencyArchive.h",
        "cmLDConfigLDConfigTool.cxx", "cmLDConfigLDConfigTool.h",
        "cmLDConfigTool.cxx", "cmLDConfigTool.h",
        "cmELF.cxx", "cmELF.h",
        "cmMachO.cxx", "cmMachO.h",
        "cmXcFramework.cxx", "cmXcFramework.h",
        "cmInstallGetRuntimeDependenciesGenerator.cxx", "cmInstallGetRuntimeDependenciesGenerator.h",
        "cmInstallImportedRuntimeArtifactsGenerator.cxx", "cmInstallImportedRuntimeArtifactsGenerator.h",
        "cmInstallRuntimeDependencySet.cxx", "cmInstallRuntimeDependencySet.h",
        "cmInstallRuntimeDependencySetGenerator.cxx", "cmInstallRuntimeDependencySetGenerator.h",
    ]),

    # 6. Code Generation (Generators)
    ("Code Generation", "CMake Functional Decomposition", None),
    ("Generator Base", "Code Generation", [
        "cmGlobalGenerator.cxx", "cmGlobalGenerator.h",
        "cmLocalGenerator.cxx", "cmLocalGenerator.h",
        "cmGlobalCommonGenerator.cxx", "cmGlobalCommonGenerator.h",
        "cmLocalCommonGenerator.cxx", "cmLocalCommonGenerator.h",
        "cmScriptGenerator.cxx", "cmScriptGenerator.h",
        "cmMakefileProfilingData.cxx", "cmMakefileProfilingData.h",
        "cmCommonTargetGenerator.cxx", "cmCommonTargetGenerator.h",
        "cmGlobalGeneratorFactory.h",
        "cmOutputConverter.cxx", "cmOutputConverter.h",
        "cmRulePlaceholderExpander.cxx", "cmRulePlaceholderExpander.h",
        "cmPlaceholderExpander.cxx", "cmPlaceholderExpander.h",
        "cmMSVC60LinkLineComputer.cxx", "cmMSVC60LinkLineComputer.h",
        "cmGeneratorOptions.h",
        "cmOSXBundleGenerator.cxx", "cmOSXBundleGenerator.h",
        "cmExternalMakefileProjectGenerator.cxx", "cmExternalMakefileProjectGenerator.h",
        "cmGraphVizWriter.cxx", "cmGraphVizWriter.h",
    ]),
    ("Makefile Generators", "Code Generation", [
        "cmGlobalUnixMakefileGenerator3.cxx", "cmGlobalUnixMakefileGenerator3.h",
        "cmLocalUnixMakefileGenerator3.cxx", "cmLocalUnixMakefileGenerator3.h",
        "cmMakefileTargetGenerator.cxx", "cmMakefileTargetGenerator.h",
        "cmMakefileExecutableTargetGenerator.cxx", "cmMakefileExecutableTargetGenerator.h",
        "cmMakefileLibraryTargetGenerator.cxx", "cmMakefileLibraryTargetGenerator.h",
        "cmMakefileUtilityTargetGenerator.cxx", "cmMakefileUtilityTargetGenerator.h",
        "cmGlobalBorlandMakefileGenerator.cxx", "cmGlobalBorlandMakefileGenerator.h",
        "cmGlobalNMakeMakefileGenerator.cxx", "cmGlobalNMakeMakefileGenerator.h",
        "cmGlobalJOMMakefileGenerator.cxx", "cmGlobalJOMMakefileGenerator.h",
        "cmGlobalMinGWMakefileGenerator.cxx", "cmGlobalMinGWMakefileGenerator.h",
        "cmGlobalMSYSMakefileGenerator.cxx", "cmGlobalMSYSMakefileGenerator.h",
        "cmGlobalWatcomWMakeGenerator.cxx", "cmGlobalWatcomWMakeGenerator.h",
    ]),
    ("Ninja Generator", "Code Generation", [
        "cmGlobalNinjaGenerator.cxx", "cmGlobalNinjaGenerator.h",
        "cmLocalNinjaGenerator.cxx", "cmLocalNinjaGenerator.h",
        "cmNinjaTargetGenerator.cxx", "cmNinjaTargetGenerator.h",
        "cmNinjaExecutableTargetGenerator.cxx", "cmNinjaExecutableTargetGenerator.h",
        "cmNinjaLibraryTargetGenerator.cxx", "cmNinjaLibraryTargetGenerator.h",
        "cmNinjaUtilityTargetGenerator.cxx", "cmNinjaUtilityTargetGenerator.h",
        "cmNinjaNormalTargetGenerator.cxx", "cmNinjaNormalTargetGenerator.h",
        "cmNinjaTypes.h",
        "cmNinjaLinkLineComputer.cxx", "cmNinjaLinkLineComputer.h",
        "cmNinjaLinkLineDeviceComputer.cxx", "cmNinjaLinkLineDeviceComputer.h",
    ]),
    ("Visual Studio Generators", "Code Generation", [
        "cmGlobalVisualStudioGenerator.cxx", "cmGlobalVisualStudioGenerator.h",
        "cmGlobalVisualStudioVersionedGenerator.cxx", "cmGlobalVisualStudioVersionedGenerator.h",
        "cmGlobalVisualStudio7Generator.cxx", "cmGlobalVisualStudio7Generator.h",
        "cmGlobalVisualStudio8Generator.cxx", "cmGlobalVisualStudio8Generator.h",
        "cmGlobalVisualStudio10Generator.cxx", "cmGlobalVisualStudio10Generator.h",
        "cmGlobalVisualStudio11Generator.cxx", "cmGlobalVisualStudio11Generator.h",
        "cmGlobalVisualStudio12Generator.cxx", "cmGlobalVisualStudio12Generator.h",
        "cmGlobalVisualStudio14Generator.cxx", "cmGlobalVisualStudio14Generator.h",
        "cmLocalVisualStudioGenerator.cxx", "cmLocalVisualStudioGenerator.h",
        "cmLocalVisualStudio7Generator.cxx", "cmLocalVisualStudio7Generator.h",
        "cmLocalVisualStudio10Generator.cxx", "cmLocalVisualStudio10Generator.h",
        "cmVS10CLFlagTable.h", "cmVS10RCFlagTable.h",
        "cmVisualStudioGeneratorOptions.cxx", "cmVisualStudioGeneratorOptions.h",
        "cmVSSetupHelper.cxx", "cmVSSetupHelper.h",
    ]),
    ("Xcode Generator", "Code Generation", [
        "cmGlobalXCodeGenerator.cxx", "cmGlobalXCodeGenerator.h",
        "cmLocalXCodeGenerator.cxx", "cmLocalXCodeGenerator.h",
        "cmXCodeObject.cxx", "cmXCodeObject.h",
        "cmXCodeScheme.cxx", "cmXCodeScheme.h",
        "cmXCOFF.cxx", "cmXCOFF.h",
        "cmXCode21Object.cxx", "cmXCode21Object.h",
    ]),
    ("Other Generators", "Code Generation", [
        "cmGlobalGhsMultiGenerator.cxx", "cmGlobalGhsMultiGenerator.h",
        "cmLocalGhsMultiGenerator.cxx", "cmLocalGhsMultiGenerator.h",
        "cmGhsMultiTargetGenerator.cxx", "cmGhsMultiTargetGenerator.h",
        "cmGhsMultiFlagTable.h",
        "cmGlobalFastbuildGenerator.cxx", "cmGlobalFastbuildGenerator.h",
        "cmLocalFastbuildGenerator.cxx", "cmLocalFastbuildGenerator.h",
    ]),
    ("IDE Project Generators", "Code Generation", [
        "cmExtraCodeBlocksGenerator.cxx", "cmExtraCodeBlocksGenerator.h",
        "cmExtraCodeLiteGenerator.cxx", "cmExtraCodeLiteGenerator.h",
        "cmExtraEclipseCDT4Generator.cxx", "cmExtraEclipseCDT4Generator.h",
        "cmExtraKateGenerator.cxx", "cmExtraKateGenerator.h",
        "cmExtraSublimeTextGenerator.cxx", "cmExtraSublimeTextGenerator.h",
    ]),
    ("Qt Auto-generation", "Code Generation", [
        "cmQTWrapCPPCommand.cxx", "cmQTWrapCPPCommand.h",
        "cmQTWrapUICommand.cxx", "cmQTWrapUICommand.h",
        "cmQtAutoGen.cxx", "cmQtAutoGen.h",
        "cmQtAutoGenGlobalInitializer.cxx", "cmQtAutoGenGlobalInitializer.h",
        "cmQtAutoGenInitializer.cxx", "cmQtAutoGenInitializer.h",
        "cmQtAutoGenerator.cxx", "cmQtAutoGenerator.h",
        "cmQtAutoMocUic.cxx", "cmQtAutoMocUic.h",
        "cmQtAutoRcc.cxx", "cmQtAutoRcc.h",
    ]),

    # 7. Package Discovery (find_package / find_*)
    ("Package Discovery", "CMake Functional Decomposition", [
        "cmFindBase.cxx", "cmFindBase.h",
        "cmFindCommon.cxx", "cmFindCommon.h",
        "cmFindFileCommand.cxx", "cmFindFileCommand.h",
        "cmFindLibraryCommand.cxx", "cmFindLibraryCommand.h",
        "cmFindPackageCommand.cxx", "cmFindPackageCommand.h",
        "cmFindPackageStack.cxx", "cmFindPackageStack.h",
        "cmFindPathCommand.cxx", "cmFindPathCommand.h",
        "cmFindProgramCommand.cxx", "cmFindProgramCommand.h",
        "cmCMakePkgConfigCommand.cxx", "cmCMakePkgConfigCommand.h",
        "cmPackageInfoReader.cxx", "cmPackageInfoReader.h",
        "cmPkgConfigParser.cxx", "cmPkgConfigParser.h",
        "cmPkgConfigResolver.cxx", "cmPkgConfigResolver.h",
        "cmSearchPath.cxx", "cmSearchPath.h",
    ]),

    # 8. Install & Export
    ("Install & Export", "CMake Functional Decomposition", None),
    ("Install", "Install & Export", [
        "cmInstallCommand.cxx", "cmInstallCommand.h",
        "cmInstallCommandArguments.cxx", "cmInstallCommandArguments.h",
        "cmInstallDirectoryGenerator.cxx", "cmInstallDirectoryGenerator.h",
        "cmInstallDirs.cxx", "cmInstallDirs.h",
        "cmInstallExportGenerator.cxx", "cmInstallExportGenerator.h",
        "cmInstallFileSetGenerator.cxx", "cmInstallFileSetGenerator.h",
        "cmInstallFilesCommand.cxx", "cmInstallFilesCommand.h",
        "cmInstallProgramsCommand.cxx", "cmInstallProgramsCommand.h",
        "cmInstallTargetsCommand.cxx", "cmInstallTargetsCommand.h",
        "cmInstallGenerator.cxx", "cmInstallGenerator.h",
        "cmInstallTargetGenerator.cxx", "cmInstallTargetGenerator.h",
        "cmInstallScriptGenerator.cxx", "cmInstallScriptGenerator.h",
        "cmInstallSubdirectoryGenerator.cxx", "cmInstallSubdirectoryGenerator.h",
        "cmInstallCxxModuleBmiGenerator.cxx", "cmInstallCxxModuleBmiGenerator.h",
        "cmInstallCMakeConfigExportGenerator.cxx", "cmInstallCMakeConfigExportGenerator.h",
        "cmInstallAndroidMKExportGenerator.cxx", "cmInstallAndroidMKExportGenerator.h",
        "cmInstallFilesGenerator.cxx", "cmInstallFilesGenerator.h",
        "cmInstallMode.h",
        "cmInstallPackageInfoExportGenerator.cxx", "cmInstallPackageInfoExportGenerator.h",
        "cmInstallScriptHandler.cxx", "cmInstallScriptHandler.h",
        "cmInstallType.h",
        "cmInstalledFile.cxx", "cmInstalledFile.h",
        "cmFileCopier.cxx", "cmFileCopier.h",
        "cmFileInstaller.cxx", "cmFileInstaller.h",
        "cmCPackPropertiesGenerator.cxx", "cmCPackPropertiesGenerator.h",
    ]),
    ("Export", "Install & Export", [
        "cmExportCommand.cxx", "cmExportCommand.h",
        "cmExportFileGenerator.cxx", "cmExportFileGenerator.h",
        "cmExportBuildFileGenerator.cxx", "cmExportBuildFileGenerator.h",
        "cmExportInstallFileGenerator.cxx", "cmExportInstallFileGenerator.h",
        "cmExportCMakeConfigGenerator.cxx", "cmExportCMakeConfigGenerator.h",
        "cmExportBuildCMakeConfigGenerator.cxx", "cmExportBuildCMakeConfigGenerator.h",
        "cmExportInstallCMakeConfigGenerator.cxx", "cmExportInstallCMakeConfigGenerator.h",
        "cmExportAndroidMKGenerator.cxx", "cmExportAndroidMKGenerator.h",
        "cmExportBuildAndroidMKGenerator.cxx", "cmExportBuildAndroidMKGenerator.h",
        "cmExportInstallAndroidMKGenerator.cxx", "cmExportInstallAndroidMKGenerator.h",
        "cmExportTryCompileFileGenerator.cxx", "cmExportTryCompileFileGenerator.h",
        "cmExportSet.cxx", "cmExportSet.h",
        "cmExportPackageInfoGenerator.cxx", "cmExportPackageInfoGenerator.h",
        "cmExportBuildPackageInfoGenerator.cxx", "cmExportBuildPackageInfoGenerator.h",
        "cmExportInstallPackageInfoGenerator.cxx", "cmExportInstallPackageInfoGenerator.h",
        "cmImportedCxxModuleInfo.cxx", "cmImportedCxxModuleInfo.h",
    ]),

    # 9. CTest
    ("CTest", "CMake Functional Decomposition", [
        # Note: files live under Source/CTest/
        # Matched by short name — the pattern works across subdirs
        "ctest.cxx",
        "cmCTest.cxx", "cmCTest.h",
        "cmCTestBuildAndTest.cxx", "cmCTestBuildAndTest.h",
        "cmCTestBuildCommand.cxx", "cmCTestBuildCommand.h",
        "cmCTestBuildHandler.cxx", "cmCTestBuildHandler.h",
        "cmCTestCommand.cxx", "cmCTestCommand.h",
        "cmCTestConfigureCommand.cxx", "cmCTestConfigureCommand.h",
        "cmCTestCoverageCommand.cxx", "cmCTestCoverageCommand.h",
        "cmCTestCoverageHandler.cxx", "cmCTestCoverageHandler.h",
        "cmCTestGenericHandler.cxx", "cmCTestGenericHandler.h",
        "cmCTestGIT.cxx", "cmCTestGIT.h",
        "cmCTestBZR.cxx", "cmCTestBZR.h",
        "cmCTestCVS.cxx", "cmCTestCVS.h",
        "cmCTestSVN.cxx", "cmCTestSVN.h",
        "cmCTestHG.cxx", "cmCTestHG.h",
        "cmCTestP4.cxx", "cmCTestP4.h",
        "cmCTestMemCheckCommand.cxx", "cmCTestMemCheckCommand.h",
        "cmCTestMemCheckHandler.cxx", "cmCTestMemCheckHandler.h",
        "cmCTestMultiProcessHandler.cxx", "cmCTestMultiProcessHandler.h",
        "cmCTestRunTest.cxx", "cmCTestRunTest.h",
        "cmCTestTestCommand.cxx", "cmCTestTestCommand.h",
        "cmCTestTestHandler.cxx", "cmCTestTestHandler.h",
        "cmCTestSubmitCommand.cxx", "cmCTestSubmitCommand.h",
        "cmCTestSubmitHandler.cxx", "cmCTestSubmitHandler.h",
        "cmCTestStartCommand.cxx", "cmCTestStartCommand.h",
        "cmCTestUploadCommand.cxx", "cmCTestUploadCommand.h",
        "cmCTestUploadHandler.cxx", "cmCTestUploadHandler.h",
        "cmCTestScriptHandler.cxx", "cmCTestScriptHandler.h",
        "cmCTestSleepCommand.cxx", "cmCTestSleepCommand.h",
        "cmCTestUpdateCommand.cxx", "cmCTestUpdateCommand.h",
        "cmCTestUpdateHandler.cxx", "cmCTestUpdateHandler.h",
        "cmCTestDiscoverTests.cxx", "cmCTestDiscoverTests.h",
        "cmCTestBinPacker.cxx", "cmCTestBinPacker.h",
        "cmCTestResourceAllocator.cxx", "cmCTestResourceAllocator.h",
        "cmCTestResourceGroupsLexer.cxx", "cmCTestResourceGroupsLexer.h",
        "cmCTestCurl.cxx", "cmCTestCurl.h",
        "cmCTestEmptyBinaryDirectoryCommand.cxx", "cmCTestEmptyBinaryDirectoryCommand.h",
        "cmCTestGlobalVC.cxx", "cmCTestGlobalVC.h",
        "cmCTestHandlerCommand.cxx", "cmCTestHandlerCommand.h",
        "cmCTestLaunch.cxx", "cmCTestLaunch.h",
        "cmCTestLaunchReporter.cxx", "cmCTestLaunchReporter.h",
        "cmCTestReadCustomFilesCommand.cxx", "cmCTestReadCustomFilesCommand.h",
        "cmCTestResourceGroupsLexerHelper.cxx", "cmCTestResourceGroupsLexerHelper.h",
        "cmCTestResourceSpec.cxx", "cmCTestResourceSpec.h",
        "cmCTestRunScriptCommand.cxx", "cmCTestRunScriptCommand.h",
        "cmCTestTestMeasurementXMLParser.cxx", "cmCTestTestMeasurementXMLParser.h",
        "cmCTestTypes.cxx", "cmCTestTypes.h",
        "cmCTestVC.cxx", "cmCTestVC.h",
        "cmParseBlanketJSCoverage.cxx", "cmParseBlanketJSCoverage.h",
        "cmParseCacheCoverage.cxx", "cmParseCacheCoverage.h",
        "cmParseCoberturaCoverage.cxx", "cmParseCoberturaCoverage.h",
        "cmParseDelphiCoverage.cxx", "cmParseDelphiCoverage.h",
        "cmParseGTMCoverage.cxx", "cmParseGTMCoverage.h",
        "cmParseJacocoCoverage.cxx", "cmParseJacocoCoverage.h",
        "cmParseMumpsCoverage.cxx", "cmParseMumpsCoverage.h",
        "cmParsePHPCoverage.cxx", "cmParsePHPCoverage.h",
        "cmProcess.cxx", "cmProcess.h",
        "cmUVJobServerClient.cxx", "cmUVJobServerClient.h",
    ]),

    # 10. CPack
    ("CPack", "CMake Functional Decomposition", [
        "cmCPackGenerator.cxx", "cmCPackGenerator.h",
        "cmCPackGeneratorFactory.cxx", "cmCPackGeneratorFactory.h",
        "cmCPackLog.cxx", "cmCPackLog.h",
        "cmCPackComponentGroup.cxx", "cmCPackComponentGroup.h",
        "cmCPackArchiveGenerator.cxx", "cmCPackArchiveGenerator.h",
        "cmCPackDebGenerator.cxx", "cmCPackDebGenerator.h",
        "cmCPackRPMGenerator.cxx", "cmCPackRPMGenerator.h",
        "cmCPackNSISGenerator.cxx", "cmCPackNSISGenerator.h",
        "cmCPackWiXGenerator.cxx", "cmCPackWiXGenerator.h",
        "cmCPackDragNDropGenerator.cxx", "cmCPackDragNDropGenerator.h",
        "cmCPackBundleGenerator.cxx", "cmCPackBundleGenerator.h",
        "cmCPackPKGGenerator.cxx", "cmCPackPKGGenerator.h",
        "cmCPackProductBuildGenerator.cxx", "cmCPackProductBuildGenerator.h",
        "cmCPackAppImageGenerator.cxx", "cmCPackAppImageGenerator.h",
        "cmCPackCygwinBinaryGenerator.cxx", "cmCPackCygwinBinaryGenerator.h",
        "cmCPackCygwinSourceGenerator.cxx", "cmCPackCygwinSourceGenerator.h",
        "cmCPackFreeBSDGenerator.cxx", "cmCPackFreeBSDGenerator.h",
        "cmCPackIFWGenerator.cxx", "cmCPackIFWGenerator.h",
        "cmCPackIFWInstaller.cxx", "cmCPackIFWInstaller.h",
        "cmCPackIFWPackage.cxx", "cmCPackIFWPackage.h",
        "cmCPackIFWRepository.cxx", "cmCPackIFWRepository.h",
        "cmCPackIFWCommon.cxx", "cmCPackIFWCommon.h",
        "cmCPackExternalGenerator.cxx", "cmCPackExternalGenerator.h",
        "cmCPackInnoSetupGenerator.cxx", "cmCPackInnoSetupGenerator.h",
        "cmCPackNuGetGenerator.cxx", "cmCPackNuGetGenerator.h",
        "cmCPackSTGZGenerator.cxx", "cmCPackSTGZGenerator.h",
    ]),

    # 11. File API (IDE/tooling integration)
    ("File API", "CMake Functional Decomposition", [
        "cmFileAPI.cxx", "cmFileAPI.h",
        "cmFileAPICMakeFiles.cxx", "cmFileAPICMakeFiles.h",
        "cmFileAPICache.cxx", "cmFileAPICache.h",
        "cmFileAPICodemodel.cxx", "cmFileAPICodemodel.h",
        "cmFileAPICommand.cxx", "cmFileAPICommand.h",
        "cmFileAPIConfigureLog.cxx", "cmFileAPIConfigureLog.h",
        "cmFileAPIToolchains.cxx", "cmFileAPIToolchains.h",
    ]),

    # 12. Instrumentation & Diagnostics
    ("Instrumentation & Diagnostics", "CMake Functional Decomposition", [
        "cmInstrumentation.cxx", "cmInstrumentation.h",
        "cmInstrumentationCommand.cxx", "cmInstrumentationCommand.h",
        "cmInstrumentationInterrupt.cxx", "cmInstrumentationInterrupt.h",
        "cmInstrumentationQuery.cxx", "cmInstrumentationQuery.h",
        "cmMakefileProfilingData.cxx", "cmMakefileProfilingData.h",
        "cmConfigureLog.cxx", "cmConfigureLog.h",
        "cmDiagnostics.cxx", "cmDiagnostics.h",
        "cmTimestamp.cxx", "cmTimestamp.h",
    ]),

    # 13. UI Frontends
    ("UI Frontends", "CMake Functional Decomposition", None),
    ("Curses (ccmake)", "UI Frontends", [
        "ccmake.cxx",
        "cmCursesBoolWidget.cxx", "cmCursesBoolWidget.h",
        "cmCursesCacheEntryComposite.cxx", "cmCursesCacheEntryComposite.h",
        "cmCursesColor.cxx", "cmCursesColor.h",
        "cmCursesDummyWidget.cxx", "cmCursesDummyWidget.h",
        "cmCursesFilePathWidget.cxx", "cmCursesFilePathWidget.h",
        "cmCursesForm.cxx", "cmCursesForm.h",
        "cmCursesLongMessageForm.cxx", "cmCursesLongMessageForm.h",
        "cmCursesMainForm.cxx", "cmCursesMainForm.h",
        "cmCursesPathWidget.cxx", "cmCursesPathWidget.h",
        "cmCursesStringWidget.cxx", "cmCursesStringWidget.h",
        "cmCursesWidget.cxx", "cmCursesWidget.h",
        "cmCursesLabelWidget.cxx", "cmCursesLabelWidget.h",
        "cmCursesOptionsWidget.cxx", "cmCursesOptionsWidget.h",
        "cmCursesStandardIncludes.h",
    ]),
    ("Curses Form Library", "Curses (ccmake)", [
        "eti.h", "form.priv.h", "mf_common.h",
        "fld_arg.c", "fld_attr.c", "fld_current.c", "fld_def.c", "fld_dup.c",
        "fld_ftchoice.c", "fld_ftlink.c", "fld_info.c", "fld_just.c", "fld_link.c",
        "fld_max.c", "fld_move.c", "fld_newftyp.c", "fld_opts.c", "fld_pad.c",
        "fld_page.c", "fld_stat.c", "fld_type.c", "fld_user.c",
        "frm_cursor.c", "frm_data.c", "frm_def.c", "frm_driver.c", "frm_hook.c",
        "frm_opts.c", "frm_page.c", "frm_post.c", "frm_req_name.c", "frm_scale.c",
        "frm_sub.c", "frm_user.c", "frm_win.c",
        "fty_alnum.c", "fty_alpha.c", "fty_enum.c", "fty_int.c", "fty_ipv4.c",
        "fty_num.c", "fty_regex.c",
    ]),
    ("Qt GUI (cmake-gui)", "UI Frontends", [
        "CMakeGUIExec.cxx",
        "AddCacheEntry.cxx", "AddCacheEntry.h",
        "CMakeSetupDialog.cxx", "CMakeSetupDialog.h",
        "QCMake.cxx", "QCMake.h",
        "QCMakeCacheView.cxx", "QCMakeCacheView.h",
        "QCMakeWidgets.cxx", "QCMakeWidgets.h",
        "RegexExplorer.cxx", "RegexExplorer.h",
        "WarningMessagesDialog.cxx", "WarningMessagesDialog.h",
        "FirstConfigure.cxx", "FirstConfigure.h",
        "MacInstallDialog.cxx", "MacInstallDialog.h",
    ]),

    # 14. Utilities & Platform Abstraction
    ("Utilities & Platform Abstraction", "CMake Functional Decomposition", [
        "cmSystemTools.cxx", "cmSystemTools.h",
        "cmStringAlgorithms.cxx", "cmStringAlgorithms.h",
        "cmAlgorithms.h",
        "cmFileTime.cxx", "cmFileTime.h",
        "cmFileTimeCache.cxx", "cmFileTimeCache.h",
        "cmFileTimes.cxx", "cmFileTimes.h",
        "cmList.cxx", "cmList.h",
        "cmValue.cxx", "cmValue.h",
        "cmRange.h",
        "cmRST.cxx", "cmRST.h",
        "cmBase32.cxx", "cmBase32.h",
        "cmBase64.cxx", "cmBase64.h",
        "cmHexFileConverter.cxx", "cmHexFileConverter.h",
        "cmUuid.cxx", "cmUuid.h",
        "cmVersion.cxx", "cmVersion.h",
        "cmVersionConfig.h",
        "cmConsoleBuf.cxx", "cmConsoleBuf.h",
        "cmProcessOutput.cxx", "cmProcessOutput.h",
        "cmCryptoHash.cxx", "cmCryptoHash.h",
        "cmCurl.cxx", "cmCurl.h",
        "cmDocumentation.cxx", "cmDocumentation.h",
        "cmDocumentationEntry.h",
        "cmDocumentationSection.cxx", "cmDocumentationSection.h",
        "cmDocumentationFormatter.cxx", "cmDocumentationFormatter.h",
        "bindexplib.cxx", "bindexplib.h",
        "cmExpandedCommandArgument.cxx", "cmExpandedCommandArgument.h",
        "cmDebuggerAdapter.cxx", "cmDebuggerAdapter.h",
        "cmDebuggerBreakpointManager.cxx", "cmDebuggerBreakpointManager.h",
        "cmDebuggerSourceBreakpoint.cxx", "cmDebuggerSourceBreakpoint.h",
        "cmDebuggerThread.cxx", "cmDebuggerThread.h",
        "cmDebuggerThreadManager.cxx", "cmDebuggerThreadManager.h",
        "cmDebuggerVariables.cxx", "cmDebuggerVariables.h",
        "cmDebuggerVariablesHelper.cxx", "cmDebuggerVariablesHelper.h",
        "cmDebuggerVariablesManager.cxx", "cmDebuggerVariablesManager.h",
        "cmDebuggerProtocol.cxx", "cmDebuggerProtocol.h",
        "cmDebuggerExceptionManager.cxx", "cmDebuggerExceptionManager.h",
        "cmDebuggerPosixPipeConnection.cxx", "cmDebuggerPosixPipeConnection.h",
        "cmDebuggerStackFrame.cxx", "cmDebuggerStackFrame.h",
        "cmAffinity.cxx", "cmAffinity.h",
        "cmArchiveWrite.cxx", "cmArchiveWrite.h",
        "cmArgumentParser.cxx", "cmArgumentParser.h", "cmArgumentParserTypes.h",
        "cmBuildOptions.h",
        "cmCLocaleEnvironmentScope.cxx", "cmCLocaleEnvironmentScope.h",
        "cmCMakePath.cxx", "cmCMakePath.h",
        "cmConstStack.h", "cmConstStack.tcc",
        "cmDefinitions.cxx", "cmDefinitions.h",
        "cmDuration.cxx", "cmDuration.h",
        "cmDynamicLoader.cxx", "cmDynamicLoader.h",
        "cmExecutionStatus.h",
        "cmExperimental.cxx", "cmExperimental.h",
        "cmFSPermissions.cxx", "cmFSPermissions.h",
        "cmFileLock.cxx", "cmFileLock.h",
        "cmFileLockPool.cxx", "cmFileLockPool.h",
        "cmFileLockResult.cxx", "cmFileLockResult.h",
        "cmFileLockUnix.cxx",
        "cmFilePathChecksum.cxx", "cmFilePathChecksum.h",
        "cmGeneratedFileStream.cxx", "cmGeneratedFileStream.h",
        "cmGetPipes.cxx", "cmGetPipes.h",
        "cmGlobCacheEntry.h", "cmGlobVerificationManager.cxx", "cmGlobVerificationManager.h",
        "cmGraphAdjacencyList.h",
        "cmJSONHelpers.cxx", "cmJSONHelpers.h",
        "cmJSONState.cxx", "cmJSONState.h",
        "cmLinkedTree.h",
        "cmLocale.h",
        "cmMessageMetadata.h",
        "cmMessenger.cxx", "cmMessenger.h",
        "cmNewLineStyle.cxx", "cmNewLineStyle.h",
        "cmPathLabel.cxx", "cmPathLabel.h",
        "cmPathResolver.cxx", "cmPathResolver.h",
        "cmPlistParser.cxx", "cmPlistParser.h",
        "cmProcessTools.cxx", "cmProcessTools.h",
        "cmStandardLexer.h",
        "cmString.cxx", "cmString.hxx",
        "cmStringReplaceHelper.cxx", "cmStringReplaceHelper.h",
        "cmUVHandlePtr.cxx", "cmUVHandlePtr.h",
        "cmUVProcessChain.cxx", "cmUVProcessChain.h",
        "cmUVStream.h", "cmUVStreambuf.h",
        "cmUtils.hxx",
        "cmVariableWatch.cxx", "cmVariableWatch.h",
        "cmWindowsRegistry.cxx", "cmWindowsRegistry.h",
        "cmWorkerPool.cxx", "cmWorkerPool.h",
        "cmWorkingDirectory.cxx", "cmWorkingDirectory.h",
        "cmXMLParser.cxx", "cmXMLParser.h",
        "cmXMLSafe.cxx",
        "cmXMLWriter.cxx",
        "cm_codecvt.cxx", "cm_codecvt.hxx", "cm_codecvt_Encoding.hxx",
        "cm_fileno.cxx", "cm_fileno.hxx",
        "cm_get_date.c", "cm_get_date.h",
        "cm_sys_stat.h",
        "cm_utf8.c", "cm_utf8.h",
        "cmBuildDatabase.cxx", "cmBuildDatabase.h",
        "cmSarifLog.cxx", "cmSarifLog.h",
        "cmCMakeMinimumRequired.cxx", "cmCMakeMinimumRequired.h",
        "cmMessageType.h",
        "cmXMLSafe.h",
        "cmXMLWriter.h",
        "cmFLTKWrapUICommand.cxx", "cmFLTKWrapUICommand.h",
        "cmIncludeExternalMSProjectCommand.cxx", "cmIncludeExternalMSProjectCommand.h",
        "cmFileCommand_ReadMacho.cxx", "cmFileCommand_ReadMacho.h",
        "cmGeneratorExpressionContext.cxx", "cmGeneratorExpressionContext.h",
        "cmExprParserHelper.cxx", "cmExprParserHelper.h",
        "cmFortranParser.h", "cmFortranParserImpl.cxx",
        "cmGccDepfileLexerHelper.cxx",
    ]),
    ("KWSys Portability Layer", "Utilities & Platform Abstraction", [
        "Base64.c",
        "CommandLineArguments.cxx",
        "Directory.cxx",
        "DynamicLoader.cxx",
        "EncodingC.c",
        "EncodingCXX.cxx",
        "FStream.cxx",
        "Glob.cxx",
        "MD5.c",
        "ProcessUNIX.c",
        "RegularExpression.cxx",
        "Status.cxx",
        "String.c",
        "System.c",
        "SystemInformation.cxx",
        "SystemTools.cxx",
        "Terminal.c",
        "kwsysPrivate.h",
    ]),
]


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def short_name(path: str) -> str:
    return os.path.basename(path)


def build_arch_tree(db):
    """Collect file assignments and generate arch XML, then import via und."""
    root_name = "CMake Functional Decomposition"

    # Index files by short name, storing only entity IDs (not entity objects,
    # which become stale if held across iterations).
    print("Indexing files...")
    file_map = {}  # shortname → list of entity id
    for e in db.ents("file ~unknown ~unresolved"):
        file_map.setdefault(short_name(e.longname()), []).append(e.id())

    # Collect relative paths per arch node
    arch_files = {name: [] for name, _, _ in ARCH}
    assigned = set()
    for (name, _, patterns) in ARCH:
        if not patterns:
            continue
        for pat in patterns:
            for eid in file_map.get(pat, []):
                if eid not in assigned:
                    arch_files[name].append(db.ent_from_id(eid).relname())
                    assigned.add(eid)
                    print(f"  [{name}] {pat}")

    # Build children map for XML rendering
    children = {}
    for name, parent, _ in ARCH:
        children.setdefault(parent, []).append(name)

    def render(name):
        lines = [f'<arch name="{html.escape(name)}">']
        for relpath in arch_files.get(name, []):
            lines.append(f'@l./{relpath}')
        for child in children.get(name, []):
            lines.append(render(child))
        lines.append('</arch>')
        return '\n'.join(lines)

    xml = f'<!DOCTYPE arch>\n{render(root_name)}\n'

    db_path = db.name()
    db.close()

    tmp_xml = None
    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.xml', delete=False) as f:
            f.write(xml)
            tmp_xml = f.name

        # Remove existing arch (ignore errors if not present)
        subprocess.run(
            ["und", "remove", "-arch", root_name, db_path],
            capture_output=True,
        )

        result = subprocess.run(
            ["und", "import", "-arch", tmp_xml, db_path],
            capture_output=True, text=True,
        )
        if result.returncode != 0:
            print("Error importing arch:", result.stderr or result.stdout)
            sys.exit(1)
    finally:
        if tmp_xml and os.path.exists(tmp_xml):
            os.unlink(tmp_xml)

    print(f"\nDone. {len(assigned)} files assigned.")
    print(f"Architecture '{root_name}' imported into {db_path}.")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    args = sys.argv[1:]
    db_path = None
    for i, a in enumerate(args):
        if a == "-db" and i + 1 < len(args):
            db_path = args[i + 1]
        elif a.endswith(".und") or a.endswith(".undu"):
            db_path = a

    if not db_path:
        try:
            db = understand.open()
        except Exception:
            print("Usage: upython cmake_arch_build.py -db /path/to/cmake.und")
            sys.exit(1)
    else:
        db = understand.open(db_path)

    # build_arch_tree closes the db itself before calling und import
    build_arch_tree(db)


if __name__ == "__main__":
    main()

@rem ================================================================================================================
@rem Version 1.0.0
@rem ================================================================================================================
@set VERSION=1.0.0
@IF [%1]==[] ( 
	SET SCITOOLS_PATH=C:\Program Files\SciTools
	) ELSE (
	SET SCITOOLS_PATH=%~dp1
@rem %~n1
)
@rem ================================================================================================================
@set INSTALL_PATH=%~dp0
@rem ================================================================================================================
@echo We will install from '%INSTALL_PATH%' in SciTools installation located at '%SCITOOLS_PATH%'
@rem ================================================================================================================
@call und add 2> "%INSTALL_PATH%test_und.txt"
@fc "%INSTALL_PATH%und_test\test_und.txt" "%INSTALL_PATH%test_und.txt" > nul
@if errorlevel 1 GOTO UND_FAIL
@echo ... I found und, that's good ... 
@rem ================================================================================================================
@IF NOT EXIST %USERPROFILE%\AppData\Roaming\SciTools GOTO PLUGINDIR_FAIL
@echo ... I found your SciTools user configuration dir, that's good ... 
@rem ================================================================================================================
@IF NOT EXIST %USERPROFILE%\AppData\Roaming\SciTools\plugin (
@echo ... creating the plugin directory because it doesn't exist '%USERPROFILE%\AppData\Roaming\SciTools\plugin'... 
@mkdir %USERPROFILE%\AppData\Roaming\SciTools\plugin
)
@IF NOT EXIST %USERPROFILE%\AppData\Roaming\SciTools\plugin\Graph (
@echo ... creating the custom graph directory because it doesn't exist '%USERPROFILE%\AppData\Roaming\SciTools\plugin\Graph' ... 
@mkdir %USERPROFILE%\AppData\Roaming\SciTools\plugin\Graph
)
@IF NOT EXIST %USERPROFILE%\AppData\Roaming\SciTools\plugin\Codecheck (
@echo ... creating the custom graph directory because it doesn't exist '%USERPROFILE%\AppData\Roaming\SciTools\plugin\Codecheck' ... 
@mkdir %USERPROFILE%\AppData\Roaming\SciTools\plugin\Codecheck
)
@rem ================================================================================================================
@set INSTALL_PATH_DQ=%INSTALL_PATH:\=\\%
@set INSTALL_PATH_DQDQ=%INSTALL_PATH_DQ:\=\\%
@uperl -pi.bak -e "s/___D___/%INSTALL_PATH_DQDQ%/g" "%INSTALL_PATH%comparison.py"
@uperl -pi.bak -e "s/___D___/%INSTALL_PATH_DQDQ%/g" "%INSTALL_PATH%architectureparser.py"
@uperl -pi.bak -e "s/___D___/%INSTALL_PATH_DQDQ%/g" "%INSTALL_PATH%pather.py"
@uperl -pi.bak -e "s/___D___/%INSTALL_PATH_DQDQ%/g" "%INSTALL_PATH%GraphPathViewer.upy"
@uperl -pi.bak -e "s/___D___/%INSTALL_PATH_DQDQ%/g" "%INSTALL_PATH%GraphComparison.upy"
@uperl -pi.bak -e "s/___D___/%INSTALL_PATH_DQDQ%/g" "%INSTALL_PATH%ComparisonCodeCheck.upy"
del "%INSTALL_PATH%comparison.py.bak"
del "%INSTALL_PATH%architectureparser.py.bak"
del "%INSTALL_PATH%pather.py.bak"
del "%INSTALL_PATH%GraphPathViewer.upy.bak"
del "%INSTALL_PATH%GraphComparison.upy.bak"
del "%INSTALL_PATH%ComparisonCodeCheck.upy.bak"
@rem ================================================================================================================
@echo Installing GRAPH...
@copy "%INSTALL_PATH%GraphPathViewer.upy" "%USERPROFILE%\AppData\Roaming\SciTools\plugin\Graph"
@copy "%INSTALL_PATH%GraphComparison.upy" "%USERPROFILE%\AppData\Roaming\SciTools\plugin\Graph"
@echo Installing CodeCheck...
@copy "%INSTALL_PATH%ComparisonCodeCheck.upy" "%USERPROFILE%\AppData\Roaming\SciTools\plugin\Codecheck"
@rem ================================================================================================================
@echo ------------------------------------------------------------------
@echo Success!
@echo ------------------------------------------------------------------
@exit /B
:UND_FAIL
@echo ################
@echo [ERROR1]: und seems to be unavalaible. Make sure you can run 'und' from this windows.
@echo ################
@echo [INSTALL FAILED]
@echo ################
@exit /B
:PLUGINDIR_FAIL
@echo ################
@echo [ERROR1]: The Scitools configuration directory '%USERPROFILE%\AppData\Roaming\SciTools\plugin\Graph' doesn't exist. I have to stop. Install Understand first or create the directory.
@echo ################
@echo [INSTALL FAILED]
@echo ################
@exit /B
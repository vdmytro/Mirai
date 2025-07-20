@echo off
setlocal enabledelayedexpansion

set "search=Lyra"
set "replace=Mirai"

set "self=%~nx0"

for %%F in (*.*) do (
    if not "%%~aF"=="d" (
        set "filename=%%~nxF"

        rem Skip the script itself
        if /I not "!filename!"=="%self%" (
            echo !filename! | findstr /C:"%search%" >nul
            if not errorlevel 1 (
                set "newname=!filename:%search%=%replace%!"
                echo Renaming: "%%~nxF" to "!newname!"
                ren "%%~nxF" "!newname!"
            )
        )
    )
)

endlocal
pause
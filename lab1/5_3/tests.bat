@echo off
setlocal enabledelayedexpansion

set PASS=0
set FAIL=0
set EXE=fill.exe

echo Simple fill (closed contour)
%EXE% test1_input.txt test1_actual.txt
fc /b test1_actual.txt test1_expected.txt >nul 2>&1
if %errorlevel%==0 (
    echo   PASSED
    set /a PASS+=1
) else (
    echo   FAILED
    echo   --- Expected ---
    type test1_expected.txt
    echo   --- Actual ---
    type test1_actual.txt
    set /a FAIL+=1
)
echo.

echo Multiple territories
%EXE% test2_input.txt test2_actual.txt
fc /b test2_actual.txt test2_expected.txt >nul 2>&1
if %errorlevel%==0 (
    echo   PASSED
    set /a PASS+=1
) else (
    echo   FAILED
    fc test2_actual.txt test2_expected.txt | more /p /c /e +1 /l 20
    set /a FAIL+=1
)
echo.

echo Fill leaks through open contour
%EXE% test3_input.txt test3_actual.txt
fc /b test3_actual.txt test3_expected.txt >nul 2>&1
if %errorlevel%==0 (
    echo   PASSED
    set /a PASS+=1
) else (
    echo   FAILED
    set /a FAIL+=1
)
echo.

echo No start point (image must be unchanged)
%EXE% test4_input.txt test4_actual.txt
fc /b test4_actual.txt test4_expected.txt >nul 2>&1
if %errorlevel%==0 (
    echo   PASSED
    set /a PASS+=1
) else (
    echo   FAILED
    set /a FAIL+=1
)
echo.

echo Help flag (-h)
%EXE% -h >nul 2>&1
if %errorlevel%==0 (
    echo   PASSED
    set /a PASS+=1
) else (
    echo   FAILED - exit code was not 0
    set /a FAIL+=1
)
echo.

echo Invalid arguments (too many args)
%EXE% a b c > test7_actual.txt 2>&1
set ERR_CODE=%errorlevel%
findstr /c:"ERROR" test7_actual.txt >nul 2>&1
if %errorlevel%==0 (
    if %ERR_CODE%==1 (
        echo   PASSED
        set /a PASS+=1
    ) else (
        echo   FAILED - exit code was %ERR_CODE%, expected 1
        set /a FAIL+=1
    )
) else (
    echo   FAILED - no ERROR in output
    set /a FAIL+=1
)
echo.

echo Missing input file
%EXE% nonexistent_file.txt out.txt > test8_actual.txt 2>&1
set ERR_CODE=%errorlevel%
findstr /c:"ERROR" test8_actual.txt >nul 2>&1
if %errorlevel%==0 (
    if %ERR_CODE%==1 (
        echo   PASSED
        set /a PASS+=1
    ) else (
        echo   FAILED - exit code was %ERR_CODE%, expected 1
        set /a FAIL+=1
    )
) else (
    echo   FAILED - no ERROR in output
    set /a FAIL+=1
)
echo.

echo  Results: %PASS% passed, %FAIL% failed
:end
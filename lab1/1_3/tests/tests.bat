@echo off
setlocal EnableDelayedExpansion

set EXE=findtext.exe
set FAIL=0

echo
echo Running tests for %EXE%
echo

call :test_found_basic
call :test_not_found
call :test_case_sensitive
call :test_empty_file
call :test_one_line
call :test_missing_file
call :test_empty_search

echo
if %FAIL%==0 (
    echo ALL TESTS PASSED
) else (
    echo TESTS FAILED: %FAIL%
)
echo
exit /b %FAIL%

:test_found_basic
echo Test 1: Basic search
%EXE% test_small.txt "hello" > out.txt
if %errorlevel% neq 0 (
    echo FAIL: expected return 0
    set /a FAIL+=1
    goto :eof
)

findstr "^2$" out.txt >nul || (
    echo FAIL: expected line 2
    set /a FAIL+=1
)
goto :eof

:test_not_found
echo Test 2: Not found
%EXE% test_small.txt "absent" > out.txt
if %errorlevel% neq 1 (
    echo FAIL: expected return 1
    set /a FAIL+=1
    goto :eof
)

findstr "Text not found" out.txt >nul || (
    echo FAIL: expected 'Text not found'
    set /a FAIL+=1
)
goto :eof

:test_case_sensitive
echo Test 3: Case sensitivity
%EXE% test_case.txt "Hello" > out.txt
if %errorlevel% neq 0 (
    echo FAIL: expected return 0
    set /a FAIL+=1
    goto :eof
)

findstr "^1$" out.txt >nul || (
    echo FAIL: expected line 1 only
    set /a FAIL+=1
)
goto :eof

:test_empty_file
echo Test 4: Empty file
%EXE% test_empty.txt "anything" > out.txt
if %errorlevel% neq 1 (
    echo FAIL: expected return 1
    set /a FAIL+=1
)
goto :eof

:test_one_line
echo Test 5: One line file
%EXE% test_oneline.txt "world" > out.txt
if %errorlevel% neq 0 (
    echo FAIL: expected return 0
    set /a FAIL+=1
)
goto :eof

:test_missing_file
echo Test 6: Missing file
%EXE% nofile.txt "text" > out.txt
if %errorlevel% equ 0 (
    echo FAIL: expected non-zero return
    set /a FAIL+=1
)
goto :eof

:test_empty_search
echo Test 7: Empty search string
%EXE% test_small.txt "" > out.txt 2>&1
if %errorlevel% neq 3 (
    echo FAIL: expected return 3 for empty search string
    set /a FAIL+=1
) else (
    findstr /c:"Usage: findtext <file> <text>" out.txt >nul || (
        echo FAIL: expected usage message for empty search string
        set /a FAIL+=1
    )
)
goto :eof

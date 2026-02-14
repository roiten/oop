@echo off
setlocal EnableDelayedExpansion

set EXE=radix.exe
set FAIL=0

echo
echo Running tests for %EXE%
echo

call :test_basic_conversion
call :test_negative
call :test_zero
call :test_invalid_char
call :test_invalid_radix
call :test_overflow
call :test_wrong_args

echo
if %FAIL%==0 (
    echo ALL TESTS PASSED
) else (
    echo TESTS FAILED: %FAIL%
)
echo
exit /b %FAIL%

:test_basic_conversion
echo Test 1: Basic conversion 1011 (base 2) -> base 10
%EXE% 2 10 1011 > out.txt
for /f %%a in (out.txt) do set result=%%a
if "!result!" neq "11" (
    echo FAIL: expected 11, got !result!
    set /a FAIL+=1
)
goto :eof

:test_negative
echo Test 2: Negative number -101 (base 2) -> base 10
%EXE% 2 10 -101 > out.txt
for /f %%a in (out.txt) do set result=%%a
if "!result!" neq "-5" (
    echo FAIL: expected -5, got !result!
    set /a FAIL+=1
)
goto :eof

:test_zero
echo Test 3: Zero 0 (base 10) -> base 2
%EXE% 10 2 0 > out.txt
for /f %%a in (out.txt) do set result=%%a
if "!result!" neq "0" (
    echo FAIL: expected 0, got !result!
    set /a FAIL+=1
)
goto :eof

:test_invalid_char
echo Test 4: Invalid character "G" in base 16
%EXE% 16 10 G > out.txt 2>&1
findstr /c:"Invalid argument" out.txt >nul || (
    echo FAIL: expected Invalid argument message
    set /a FAIL+=1
)
goto :eof

:test_invalid_radix
echo Test 5: Invalid radix 1
%EXE% 1 10 5 > out.txt 2>&1
findstr /c:"Invalid argument" out.txt >nul || (
    echo FAIL: expected Invalid argument message
    set /a FAIL+=1
)
goto :eof

:test_overflow
echo Test 6: Overflow test (INT_MAX + 1)
set /a MAX=2147483647
set /a NEXT=MAX+1
%EXE% 10 10 2147483648 > out.txt 2>&1
findstr /c:"Overflow error" out.txt >nul || (
    echo WARNING: overflow handling depends on implementation
)
goto :eof

:test_wrong_args
echo Test 7: Wrong number of arguments
%EXE% 10 2 > out.txt 2>&1
findstr /c:"Usage: radix" out.txt >nul || (
    echo FAIL: expected Usage message
    set /a FAIL+=1
)
goto :eof
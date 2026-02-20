@echo off
setlocal EnableDelayedExpansion

set EXE=inverse.exe
set FAIL=0

echo Running tests for %EXE%

call :test_null_det


if %FAIL%==0 (
    echo ALL TESTS PASSED
) else (
    echo TESTS FAILED: %FAIL%
)
exit /b %FAIL%

:test_null_det
echo test matrix with null det
%EXE% null_det.txt > out.txt 2>&1
findstr /c:"Non-invertible" out.txt >nul || (
    echo FAIL: expected do not exist message
    set /a FAIL+=1
)
goto :eof

:test_negative
echo -101 (base 2) -> base 10
%EXE% 2 10 -101 > out.txt
for /f %%a in (out.txt) do set result=%%a
if "!result!" neq "-5" (
    echo FAIL: expected -5, got !result!
    set /a FAIL+=1
)
goto :eof

:test_zero
echo 0 (base 10) -> base 2
%EXE% 10 2 0 > out.txt
for /f %%a in (out.txt) do set result=%%a
if "!result!" neq "0" (
    echo FAIL: expected 0, got !result!
    set /a FAIL+=1
)
goto :eof

:test_invalid_char
echo invalid "G" in base 16
%EXE% 16 10 G > out.txt 2>&1
findstr /c:"Invalid argument" out.txt >nul || (
    echo FAIL: expected Invalid argument message
    set /a FAIL+=1
)
goto :eof

:test_invalid_radix
echo invalid radix 1
%EXE% 1 10 5 > out.txt 2>&1
findstr /c:"Invalid argument" out.txt >nul || (
    echo FAIL: expected Invalid argument message
    set /a FAIL+=1
)
goto :eof

:test_overflow
echo overflow test (INT_32MAX + 1)
set /a MAX=2147483647
set /a NEXT=MAX+1
%EXE% 10 10 %NEXT% > out.txt 2>&1
findstr /c:"Overflow error" out.txt >nul || (
    echo FAIL: overflow error expected
    set /a FAIL+=1
)
goto :eof

:test_wrong_args
echo wrong arguments
%EXE% 10 2 > out.txt 2>&1
findstr /c:"Usage: radix" out.txt >nul || (
    echo FAIL: expected Usage message
    set /a FAIL+=1
)
goto :eof
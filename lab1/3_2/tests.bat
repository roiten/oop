@echo off
setlocal EnableDelayedExpansion

set EXE=invert.exe
set FAIL=0

echo Running tests for %EXE%
echo ================================

:: Создаем тестовые файлы
call :create_test_files

:: Запускаем тесты
call :test_normal_matrix1
call :test_normal_matrix2
call :test_null_det
call :test_invalid_matrix_char
call :test_invalid_matrix_format_spaces
call :test_invalid_matrix_format_tabs
call :test_invalid_matrix_format_extra
call :test_invalid_matrix_empty
call :test_invalid_matrix_too_few
call :test_help
call :test_file_not_found
call :test_stdin_valid
call :test_stdin_invalid_format
call :test_stdin_invalid_value

:: Удаляем тестовые файлы
call :cleanup

echo ================================
if %FAIL%==0 (
    echo ALL TESTS PASSED
) else (
    echo TESTS FAILED: %FAIL%
)
exit /b %FAIL%

:create_test_files
:: Нормальная матрица 1 (из примера 1)
echo 1	2	3 > matrix1.txt
echo 0	1	4 >> matrix1.txt
echo 5	6	0 >> matrix1.txt

:: Нормальная матрица 2 (из примера 2)
echo 4	7	2.3 > matrix2.txt
echo 2	1	1 >> matrix2.txt
echo 3	-2	-2.31 >> matrix2.txt

:: Вырожденная матрица (из примера 3)
echo 1	2	3 > null_det.txt
echo 2	4	6 >> null_det.txt
echo 1	2	3 >> null_det.txt

:: Матрица с нечисловым значением (из примера 4)
echo 1	2	a > invalid_char.txt
echo 2	4	6 >> invalid_char.txt
echo 1	2	3 >> invalid_char.txt

:: Матрица с неправильным форматом (пробелы вместо табов)
echo 1 2 3 > format_spaces.txt
echo 0 1 4 >> format_spaces.txt
echo 5 6 0 >> format_spaces.txt

:: Матрица с неправильным форматом (лишний таб)
echo 1	2	3	 > format_extra_tab.txt
echo 0	1	4	>> format_extra_tab.txt
echo 5	6	0	>> format_extra_tab.txt

:: Матрица с неправильным форматом (не хватает чисел)
echo 1	2 > format_few.txt
echo 0	1	4 >> format_few.txt
echo 5	6	0 >> format_few.txt

:: Матрица с неправильным форматом (пустая строка)
echo 1	2	3 > format_empty.txt
echo. >> format_empty.txt
echo 5	6	0 >> format_empty.txt

:: Пустой файл
type nul > empty.txt
goto :eof

:cleanup
del matrix1.txt 2>nul
del matrix2.txt 2>nul
del null_det.txt 2>nul
del invalid_char.txt 2>nul
del format_spaces.txt 2>nul
del format_extra_tab.txt 2>nul
del format_few.txt 2>nul
del format_empty.txt 2>nul
del empty.txt 2>nul
del out.txt 2>nul
del input.txt 2>nul
goto :eof

:compare_output
set "expected=%~1"
set "actual=%~2"
echo !actual! | findstr /c:"!expected!" >nul
if errorlevel 1 (
    echo FAIL: expected "!expected!", got:
    type out.txt
    set /a FAIL+=1
) else (
    echo PASS
)
goto :eof

:test_normal_matrix1
echo.
echo Test 1: Normal matrix 1 (from example 1)
%EXE% matrix1.txt > out.txt
set expected1="-24.000	18.000	5.000"
set expected2="20.000	-15.000	-4.000"
set expected3="-5.000	4.000	1.000"

findstr /c:"-24.000" out.txt >nul && findstr /c:"18.000" out.txt >nul || (
    echo FAIL: incorrect output for matrix1
    echo Expected:
    echo -24.000  18.000   5.000
    echo 20.000   -15.000  -4.000
    echo -5.000   4.000    1.000
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_normal_matrix2
echo.
echo Test 2: Normal matrix 2 (from example 2)
%EXE% matrix2.txt > out.txt
findstr /c:"-0.009" out.txt >nul && findstr /c:"0.321" out.txt >nul || (
    echo FAIL: incorrect output for matrix2
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_null_det
echo.
echo Test 3: Null determinant matrix
%EXE% null_det.txt > out.txt 2>&1
findstr /c:"Non-invertible" out.txt >nul || (
    echo FAIL: expected "Non-invertible" message
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_invalid_matrix_char
echo.
echo Test 4: Invalid character in matrix
%EXE% invalid_char.txt > out.txt 2>&1
findstr /c:"Invalid matrix" out.txt >nul || (
    echo FAIL: expected "Invalid matrix" message
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_invalid_matrix_format_spaces
echo.
echo Test 5: Invalid format (spaces instead of tabs)
%EXE% format_spaces.txt > out.txt 2>&1
findstr /c:"Invalid matrix format" out.txt >nul || (
    echo FAIL: expected "Invalid matrix format" message for spaces
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_invalid_matrix_format_tabs
echo.
echo Test 6: Invalid format (extra tab at the end)
%EXE% format_extra_tab.txt > out.txt 2>&1
findstr /c:"Invalid matrix format" out.txt >nul || (
    echo FAIL: expected "Invalid matrix format" message for extra tab
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_invalid_matrix_format_extra
echo.
echo Test 7: Invalid format (too few numbers)
%EXE% format_few.txt > out.txt 2>&1
findstr /c:"Invalid matrix format" out.txt >nul || (
    echo FAIL: expected "Invalid matrix format" message for too few numbers
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_invalid_matrix_empty
echo.
echo Test 8: Invalid format (empty line)
%EXE% format_empty.txt > out.txt 2>&1
findstr /c:"Invalid matrix format" out.txt >nul || (
    echo FAIL: expected "Invalid matrix format" message for empty line
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_help
echo.
echo Test 9: Help option
%EXE% -h > out.txt
findstr /c:"Use this file" out.txt >nul && findstr /c:"inverse" out.txt >nul || (
    echo FAIL: help message not displayed correctly
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_file_not_found
echo.
echo Test 10: File not found
%EXE% nonexistent.txt > out.txt 2>&1
findstr /c:"Cannot open file" out.txt >nul || (
    echo FAIL: expected file not found message
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_stdin_valid
echo.
echo Test 11: Valid input from stdin
(
echo 1	2	3
echo 0	1	4
echo 5	6	0
) | %EXE% > out.txt
findstr /c:"-24.000" out.txt >nul && findstr /c:"18.000" out.txt >nul || (
    echo FAIL: stdin input failed
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_stdin_invalid_format
echo.
echo Test 12: Invalid format from stdin
(
echo 1 2 3
echo 0 1 4
echo 5 6 0
) | %EXE% > out.txt 2>&1
findstr /c:"Invalid matrix format" out.txt >nul || (
    echo FAIL: expected format error for stdin with spaces
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof

:test_stdin_invalid_value
echo.
echo Test 13: Invalid value from stdin
(
echo 1	2	a
echo 2	4	6
echo 1	2	3
) | %EXE% > out.txt 2>&1
findstr /c:"Invalid matrix" out.txt >nul || (
    echo FAIL: expected invalid value error for stdin
    echo Got:
    type out.txt
    set /a FAIL+=1
    goto :eof
)
echo PASS
goto :eof
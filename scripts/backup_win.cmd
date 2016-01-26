xcopy "C:\Documents and Settings\yurec.DRWEB\Local Settings\Application Data\Microsoft\Outlook\Outlook.pst" "D:\Backup\Outlook\" /H /R /Y

xcopy "C:\Documents and Settings\yurec.DRWEB\Application Data\Opera\Opera\profile\opera6.adr" "D:\Backup\Opera.adr" /H /R /Y

7z a -tzip -r -x!*.exe -x!*.pch -x!*.ncb -x!Debug* -x!*.obj -x!*.pdb -x!*.ilk  D:\Backup\%date:~-10%.7z "D:\Projects\drweb5\*"


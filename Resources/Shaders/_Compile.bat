
for %%i in (*.vert *.frag) do (
	"D:\Vulkan\1.2.176.1\Bin\glslc.exe" "%%~i" -o "%%~ni.spv"
	echo f | xcopy "%%~ni.spv" "..\..\bin\Debug-x64\Resources\Shaders\%%~ni.spv" /K /D /H /Y
)

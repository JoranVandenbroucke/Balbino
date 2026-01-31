import subprocess


def execute_glslang(input_file, output_file):
    """Executes glslang to compile a GLSL file into a SPIR-V binary."""
    cmd = f'.\\3rdParty\\Vulkan\\Bin\\glslang.exe -V -o {output_file} {input_file}'
    subprocess.run(cmd, shell=True, check=True)


def read_spirv_file(file_path):
    """Reads a SPIR-V binary file and returns its contents as a list of uint32_t."""
    with open(file_path, 'rb') as f:
        data = f.read()
    return list(data)


def write_cpp_file(spirv_data, output_path, variable_name, spirv_path):
    """Writes a C++ file containing the SPIR-V data as a std::vector<uint32_t>."""
    with open(output_path, 'w') as f:
        f.write('#include <vector>\n\n')
        f.write(f'const std::vector<uint8_t> {variable_name} = {{\n#if defined(__has_embed)\n#    embed "{spirv_path}"\n#else\n')
        for i, val in enumerate(spirv_data):
            if i % 8 == 0:
                f.write('    ')
            f.write(f'0x{val:02x}, ')
            if i % 16 == 15:
                f.write('\n')
        f.write('\n#endif\n};\n')


def embed_spirv_to_cpp(glsl_file, spirv_file, cpp_file, variable_name):
    """Embeds a SPIR-V file into a C++ file."""
    execute_glslang(glsl_file, spirv_file)
    spirv_data = read_spirv_file(spirv_file)
    write_cpp_file(spirv_data, cpp_file, variable_name, spirv_file)


# Usage:
embed_spirv_to_cpp('Data/fullscreen.vert', 'Embedded/SPV/fullscreen.vert.spv', 'fullscreen_vert.hpp', 'g_fullscreenVert')
embed_spirv_to_cpp('Data/fullscreen.frag', 'Embedded/SPV/fullscreen.frag.spv', 'fullscreen_frag.hpp', 'g_fullscreenFrag')
embed_spirv_to_cpp('Data/triangle.vert', 'Embedded/SPV/triangle.vert.spv', 'triangle_vert.hpp', 'g_triangleVert')
embed_spirv_to_cpp('Data/triangle.frag', 'Embedded/SPV/triangle.frag.spv', 'triangle_frag.hpp', 'g_fullscreenFrag')
embed_spirv_to_cpp('Data/ui.vert', 'Embedded/SPV/ui.vert.spv', 'ui_vert.hpp', 'g_uiVert')
embed_spirv_to_cpp('Data/ui.frag', 'Embedded/SPV/ui.frag.spv', 'ui_frag.hpp', 'g_uiFrag')
